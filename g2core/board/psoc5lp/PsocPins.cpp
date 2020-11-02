
#include "MotatePins.h"

using namespace Motate;

template<> _pinChangeInterrupt * PortHardware<'C'>::_firstInterrupt = nullptr;
template<> _pinChangeInterrupt * PortHardware<'L'>::_firstInterrupt = nullptr;
template<> _pinChangeInterrupt * PortHardware<'R'>::_firstInterrupt = nullptr;
template<> _pinChangeInterrupt * PortHardware<'H'>::_firstInterrupt = nullptr;
template<> _pinChangeInterrupt * PortHardware<'S'>::_firstInterrupt = nullptr;

namespace PSOC {

    void SDLC_C_Handler(uint32_t mask) {
        _pinChangeInterrupt *current = PortHardware<'C'>::_firstInterrupt;
        while (current != nullptr) {
            if ((mask & current->pc_mask) && (current->interrupt_handler)) {
                current->interrupt_handler();
            }
            current = current->next;
        }
    }

    void SDLC_L_Handler(uint32_t mask) {
        _pinChangeInterrupt *current = PortHardware<'L'>::_firstInterrupt;
        while (current != nullptr) {
            if ((mask & current->pc_mask) && (current->interrupt_handler)) {
                current->interrupt_handler();
            }
            current = current->next;
        }
    }

    void SDLC_R_Handler(uint32_t mask) {
        _pinChangeInterrupt *current = PortHardware<'R'>::_firstInterrupt;
        while (current != nullptr) {
            if ((mask & current->pc_mask) && (current->interrupt_handler)) {
                current->interrupt_handler();
            }
            current = current->next;
        }
    }

    void SDLC_H_Handler(uint32_t mask) {
        _pinChangeInterrupt *current = PortHardware<'H'>::_firstInterrupt;
        while (current != nullptr) {
            if ((mask & current->pc_mask) && (current->interrupt_handler)) {
                current->interrupt_handler();
            }
            current = current->next;
        }
    }

    Motate::SysTickEvent status_pin_poll {[&] {
        static uint32_t prev_val;
        uint32_t val, mask;
        _pinChangeInterrupt *current = PortHardware<'S'>::_firstInterrupt;

        val = INPUT_STATUS_Read();
        mask = val ^ prev_val;

        if (!mask) return;

        while (current != nullptr) {
            if ((mask & current->pc_mask) && (current->interrupt_handler)) {
                current->interrupt_handler();
            }
            current = current->next;
        }

        prev_val = val;
    }, nullptr};

}
