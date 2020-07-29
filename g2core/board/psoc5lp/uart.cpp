#include "hardware.h"
#include "cypress.h"
#include "board_xio.h"

namespace PSOC {

    Motate::SysTickEvent usb_systick_event {[&] {
        if (USBFS_GetConfiguration()) {
            if (!Serial.usb_up) {
                USBFS_CDC_Init();
                Serial.reset();
                Serial.connection_state_changed_callback(true);
                Serial.usb_up = 1;
                USBStatusLed.set();
            }
        } else {
            if (Serial.usb_up) {
                Serial.reset();
                Serial.connection_state_changed_callback(false);
                Serial.usb_up = 0;
                USBStatusLed.clear();
            }
        }
    }, nullptr};

}
