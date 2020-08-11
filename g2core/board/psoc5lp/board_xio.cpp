#include "config.h"

#include "uart.h"
#include "sdlc.h"

void board_xio_init(void) {
    SDLC_Setup();
    SysTickTimer.registerEvent(&PSOC::usb_systick_event);
    SysTickTimer.registerEvent(&PSOC::sdlc_poll);
    SysTickTimer.registerEvent(&PSOC::status_pin_poll);
}

PSOC::UART SerialUSB;
