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
    uint8_t input_idx = 0;
    uint8_t mux_sel = 0;
    uint8_t pkt_count = 0;

    sdlcIOBoard_inst() {
        memset(pkt, 0, 10);
        pkt[0] = addr;
    }

    uint8_t pkt[10] __attribute__((aligned(4)));

    void do_set(uint8_t io) {
        if (io < num_outputs)
            outputs.set(io);
        else if (io < num_muxd_outputs + num_outputs)
            muxd_outputs.set(io - num_outputs);
    }

    stat_t do_set(nvObj_t *nv) {
        do_set(nv->value_int);
        return STAT_OK;
    }

    void do_clear(uint8_t io) {
        if (io < num_outputs)
            outputs.reset(io);
        else if (io < num_muxd_outputs + num_outputs)
            muxd_outputs.reset(io - num_outputs);
    }

    stat_t do_clear(nvObj_t *nv) {
        do_clear(nv->value_int);
        return STAT_OK;
    }

    stat_t do_reset(nvObj_t *nv) {
        outputs.reset();
        muxd_outputs.reset();
        return STAT_OK;
    }

    stat_t store_idx(nvObj_t *nv) {
        input_idx = nv->value_int < num_inputs ? nv->value_int : 0;
        return STAT_OK;
    }

    stat_t set(nvObj_t *nv) {
        const char *ptr = cfgArray[nv->index].token; 
        switch (ptr[3]) {
            case 's': return do_set(nv);
            case 'c': return do_clear(nv);
            case 'r': return do_reset(nv);
            case 'i': return store_idx(nv);
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
            case 'r':
                return get_integer(nv, num_inputs ? inputs[input_idx] : 0);

            case 'a':
                return get_integer(nv, num_adcs ? adcs[0] : 0);
            case 'b':
                return get_integer(nv, num_adcs ? adcs[1] : 0);

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
                pkt[3] = (muxd_outputs.to_ullong() >> mux_sel*8) & 0xff;
                pkt[4] = mux_sel + 1;
                val = outputs.to_ulong();
                memcpy(&pkt[5], &val, 4); 
                mux_sel = (mux_sel + 1) % 8;
                break;
            case 0x14:
                pkt[3] = (muxd_outputs.to_ullong() >> mux_sel*8) & 0xff;
                pkt[4] = mux_sel + 1;
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

    void update_inputs(uint64_t val) {
        uint8_t i;
        uint32_t mask;
        for (i = 0; i < num_inputs; i++) {
            (val >> i) & 1 ? inputs.set(i) : inputs.reset(i);
        }
        mask = val ^ input_data;
        input_data = val;
        switch (addr) {
            case 0x15: return PSOC::SDLC_C_Handler(mask);
            case 0x14: return PSOC::SDLC_L_Handler(mask);
            case 0x13: return PSOC::SDLC_R_Handler(mask);
            case 0x10: return PSOC::SDLC_H_Handler(mask);
        }
    }

    void parse(uint8_t *data, uint16_t len) {
        uint64_t val = 0;
        if (data[0] != addr) return;
        switch (addr) {
            case 0x10:
                if (len != 24) return;
                if (mux_sel) {
                    memcpy(adcs, &data[3], 2);
                    val = input_data;
                } else memcpy(&val, &data[17], 3);
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
            update_inputs(val);
        }
    }
};

static sdlcIOBoard_inst<0x10, 24, 16,  0, 2> headIO;
static sdlcIOBoard_inst<0x13, 48, 32, 64, 0> rightIO;
static sdlcIOBoard_inst<0x14, 48, 32, 64, 0> leftIO;
static sdlcIOBoard_inst<0x15, 32, 32,  0, 0> loaderIO;

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
        static sdlcIOBoard *board = &leftIO;

        board->parse(rx_data, SDLC_GetRxBytes());

        board_sel = (board_sel + 1) % 4;
        switch (board_sel) {
            case 0: board = &leftIO;   break;
            case 1: board = &rightIO;  break;
            case 2: board = &headIO;   break;
            case 3: board = &loaderIO; break;
        }

        memset(rx_data, 0, 20);
        SDLC_SendReceive(board->get_packet_len(), 20, board->get_packet(), rx_data);

    }, nullptr};

    bool SDLC_C_Read(uint32_t mask) {
        return (mask & loaderIO.input_data) ? true : false;
    }

    void SDLC_C_Write(uint8_t io, bool val) {
        if (val)
            loaderIO.do_set(io);
        else
            loaderIO.do_clear(io);
    }

    bool SDLC_L_Read(uint32_t mask) {
        return (mask & leftIO.input_data) ? true : false;
    }

    void SDLC_L_Write(uint8_t io, bool val) {
        if (val)
            leftIO.do_set(io);
        else
            leftIO.do_clear(io);
    }

    bool SDLC_R_Read(uint32_t mask) {
        return (mask & rightIO.input_data) ? true : false;
    }

    void SDLC_R_Write(uint8_t io, bool val) {
        if (val)
            rightIO.do_set(io);
        else
            rightIO.do_clear(io);
    }

    bool SDLC_H_Read(uint32_t mask) {
        return (mask & headIO.input_data) ? true : false;
    }

    void SDLC_H_Write(uint8_t io, bool val) {
        if (val)
            headIO.do_set(io);
        else
            headIO.do_clear(io);
    }

}
