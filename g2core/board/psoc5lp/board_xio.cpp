#include "uart.h"

void board_xio_init(void) {
    #define USBFS_DWR_POWER_OPERATION 0x02
    USBFS_Start(0, USBFS_DWR_POWER_OPERATION);
}

PSOC::UART Serial;
