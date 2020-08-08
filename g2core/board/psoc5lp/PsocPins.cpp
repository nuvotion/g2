
#include "MotatePins.h"

using namespace Motate;

template<> _pinChangeInterrupt * PortHardware<'C'>::_firstInterrupt = nullptr;

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
}
