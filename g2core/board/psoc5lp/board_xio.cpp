#include "config.h"

#include "uart.h"
#include "sdlc.h"

void board_xio_init(void) {
    #define USBFS_DWR_POWER_OPERATION 0x02
    USBFS_Start(0, USBFS_DWR_POWER_OPERATION);
    SysTickTimer.registerEvent(&PSOC::usb_systick_event);
    SDLC_Setup();
    SysTickTimer.registerEvent(&PSOC::sdlc_poll);
    SysTickTimer.registerEvent(&PSOC::status_pin_poll);
}

PSOC::UART Serial;
