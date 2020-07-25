
#include "cypress.h"
#include "board_xio.h"

namespace PSOC {

    Motate::SysTickEvent usb_systick_event {[&] {
        static int usb_up;
        if (USBFS_GetConfiguration()) {
            if (!usb_up) {
                USBFS_CDC_Init();
                Serial.reset();
                Serial.connection_state_changed_callback(true);
                usb_up = 1;
                LED_1_Write(1);
            }
        } else {
            if (usb_up) {
                Serial.reset();
                Serial.connection_state_changed_callback(false);
                usb_up = 0;
                LED_1_Write(0);
            }
        }
    }, nullptr};

}
