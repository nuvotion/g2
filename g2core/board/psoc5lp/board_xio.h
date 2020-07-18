
#ifndef board_xio_h
#define board_xio_h

#include "uart.h"

void board_hardware_init(void);  // called 1st
void board_xio_init(void);       // called later

extern PSOC::UART Serial;

#endif
