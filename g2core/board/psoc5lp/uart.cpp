
#include "cypress.h"
#include "board_xio.h"

namespace PSOC {

    void usb_poll() {
        static int usb_up;
        if (USBFS_GetConfiguration()) {
            if (!usb_up) {
                USBFS_CDC_Init();
                Serial.reset();
                Serial.connection_state_changed_callback(true);
                usb_up = 1;
            }
        } else {
            if (usb_up) {
                Serial.reset();
                Serial.connection_state_changed_callback(false);
                usb_up = 0;
            }
        }
    }

}
