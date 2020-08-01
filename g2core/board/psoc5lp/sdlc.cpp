#include "g2core.h"
#include "config.h"
#include <bitset>

struct sdlcIOBoard {
    virtual stat_t set(nvObj_t *nv);
    virtual stat_t get(nvObj_t *nv);
    virtual uint8_t *get_packet();
    virtual uint8_t get_packet_len();
    virtual void parse(uint8_t *data, uint16_t len);
};

template <uint8_t addr, uint8_t num_inputs, uint8_t num_outputs, uint8_t num_muxd_outputs, uint8_t num_adcs>
struct sdlcIOBoard_inst : sdlcIOBoard {
    uint8_t adcs[num_adcs];
    std::bitset<num_inputs> inputs;
    std::bitset<num_outputs> outputs;
    std::bitset<num_muxd_outputs> muxd_outputs;
    uint64_t input_data = 0;
    uint8_t mux_sel = 0;
    uint8_t pkt_count = 0;

    sdlcIOBoard_inst() {
        memset(pkt, 0, 10);
        pkt[0] = addr;
    }

    uint8_t pkt[10] __attribute__((aligned(4)));

    stat_t do_set(nvObj_t *nv) {
        if (nv->value_int < num_outputs)
            outputs.set(nv->value_int);
        else if (nv->value_int < num_muxd_outputs + num_outputs)
            muxd_outputs.set(nv->value_int - num_outputs);
        return STAT_OK;
    }

    stat_t do_clear(nvObj_t *nv) {
        if (nv->value_int < num_outputs)
            outputs.reset(nv->value_int);
        else if (nv->value_int < num_muxd_outputs + num_outputs)
            muxd_outputs.reset(nv->value_int - num_outputs);
        return STAT_OK;
    }

    stat_t do_reset(nvObj_t *nv) {
        outputs.reset();
        muxd_outputs.reset();
        return STAT_OK;
    }

    stat_t set(nvObj_t *nv) {
        const char *ptr = cfgArray[nv->index].token; 
        switch (ptr[3]) {
            case 's': return do_set(nv);
            case 'c': return do_clear(nv);
            case 'r': return do_reset(nv);
        }
        return STAT_OK;
    }

    stat_t get(nvObj_t *nv) {
        const char *ptr = cfgArray[nv->index].token; 
        switch (ptr[3]) {
            case 'e':
                return get_integer(nv, pkt_count);

            case 'i':
                return get_string(nv, inputs.to_string().c_str());

            default:
                std::string s = muxd_outputs.to_string() + outputs.to_string();
                return get_string(nv, s.c_str());
        }
    }

    uint8_t *get_packet() {
        uint32_t val;
        switch (addr) {
            case 0x10:
                mux_sel = !mux_sel;
                pkt[1] = mux_sel ? 0x87 : 0x00;
                pkt[7] = (outputs.to_ulong() >> 0) & 0xff;
                pkt[8] = (outputs.to_ulong() >> 6) & 0xff;
                pkt[9] = (outputs.to_ulong() >> 8) & 0xff;
                break;
            case 0x13:
                pkt[3] = mux_sel + 1;
                pkt[4] = (muxd_outputs.to_ullong() >> mux_sel*8) & 0xff;
                val = outputs.to_ulong();
                memcpy(&pkt[5], &val, 4); 
                mux_sel = (mux_sel + 1) % 8;
                break;
            case 0x14:
                pkt[3] = mux_sel + 1;
                pkt[4] = (muxd_outputs.to_ullong() >> mux_sel*8) & 0xff;
                val = outputs.to_ulong();
                memcpy(&pkt[5], &val, 4); 
                mux_sel = (mux_sel + 1) % 8;
                break;
            case 0x15:
                val = outputs.to_ulong();
                memcpy(&pkt[3], &val, 4); 
                break;
        }
        pkt_count++;
        return pkt;
    }

    constexpr uint8_t get_packet_len() {
        switch (addr) {
            case 0x10: return 10;
            case 0x13: return  9;
            case 0x14: return  9;
            case 0x15: return  7;
        }
    }

    void update_inputs() {
        uint8_t i;
        for (i = 0; i < num_inputs; i++) {
            (input_data >> i) & 1 ? inputs.set(i) : inputs.reset(i);
        }
    }

    void parse(uint8_t *data, uint16_t len) {
        uint64_t val = 0;
        if (data[0] != addr) return;
        switch (addr) {
            case 0x10:
                if (len != 22) return; 
                if (mux_sel) memcpy(adcs, &data[3], 2);
                else memcpy(&val, &data[17], 3);
                pkt_count--;
                break;
            case 0x13:
                if (len != 10) return; 
                memcpy(&val, &data[3], 6);
                pkt_count--;
                break;
            case 0x14:
                if (len != 10) return; 
                memcpy(&val, &data[3], 6);
                pkt_count--;
                break;
            case 0x15:
                if (len != 8) return; 
                memcpy(&val, &data[3], 4);
                pkt_count--;
                break;
        }
        if (val != input_data) {
            input_data = val;
            update_inputs();
        }
    }
};

sdlcIOBoard_inst<0x10, 24, 16,  0, 2> headIO;
sdlcIOBoard_inst<0x13, 48, 32, 64, 0> rightIO;
sdlcIOBoard_inst<0x14, 48, 32, 64, 0> leftIO;
sdlcIOBoard_inst<0x15, 32, 32,  0, 0> loaderIO;

static sdlcIOBoard& _board(nvObj_t *nv) {
    const char *ptr = cfgArray[nv->index].token; 
    
    switch (ptr[2]) {
        case 'l': return leftIO;
        case 'r': return rightIO;
        case 'h': return headIO;
        default:  return loaderIO;
    }
}

stat_t sd_get_sdlc(nvObj_t *nv) {
    return _board(nv).get(nv);
}

stat_t sd_set_sdlc(nvObj_t *nv) {
    return _board(nv).set(nv);
}

namespace PSOC {
    Motate::SysTickEvent sdlc_poll {[&] {

        static uint8_t rx_data[20] __attribute__((aligned(4)));
        static uint8_t board_sel;
        static sdlcIOBoard *board;

        if (board) {
            board->parse(rx_data, SDLC_GetRxBytes());
        }

        board_sel = (board_sel + 1) % 4;
        switch (board_sel) {
            case 0: board = &leftIO;   break;
            case 1: board = &rightIO;  break;
            case 2: board = &headIO;   break;
            case 3: board = &loaderIO; break;
        }

        SDLC_SendReceive(board->get_packet_len(), 20, board->get_packet(), rx_data);

    }, nullptr};
}
