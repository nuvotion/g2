#include "hardware.h"
#include "cypress.h"
#include "board_xio.h"
#include "gpio.h"

#define VBUS_DIN 9
#define USBFS_DWR_POWER_OPERATION   0x02
#define USBFS_LINE_CONTROL_CHANGED  0x02

namespace PSOC {

    Motate::SysTickEvent usb_systick_event {[&] {
        if (SerialUSB.usb_connected && !gpio_read_input(VBUS_DIN)) {
            /* Lost VBUS - disable everything */
            SerialUSB.usb_connected = 0;
            SerialUSB.usb_configured = 0;
            SerialUSB.cdc_open = 0;
            SerialUSB.connection_state_changed_callback(false);
            USBFS_Stop();
            USBStatusLed.clear();
        } else if (SerialUSB.usb_configured && !USBFS_GetConfiguration()) {
            /* No longer configured */
            SerialUSB.usb_configured = 0;
            SerialUSB.cdc_open = 0;
            SerialUSB.connection_state_changed_callback(false);
            USBStatusLed.clear();
        } else if (SerialUSB.cdc_open && !USBFS_GetLineControl()) {
            /* TTY closed */
            SerialUSB.cdc_open = 0;
            SerialUSB.connection_state_changed_callback(false);
            USBStatusLed.clear();
        } else if (!SerialUSB.usb_connected && gpio_read_input(VBUS_DIN)) {
            /* VBUS rising edge */
            SerialUSB.usb_connected = 1;
            USBFS_Start(0, USBFS_DWR_POWER_OPERATION);
        } else if (SerialUSB.usb_connected && !SerialUSB.usb_configured && USBFS_GetConfiguration()) {
            /* USB configured */
            SerialUSB.usb_configured = 1;
            USBFS_CDC_Init();
        } else if (SerialUSB.usb_connected && SerialUSB.usb_configured && !SerialUSB.cdc_open &&
                (USBFS_IsLineChanged() & USBFS_LINE_CONTROL_CHANGED) && USBFS_GetLineControl()) {
            /* TTY opened */
            SerialUSB.cdc_open = 1;
            SerialUSB.connection_state_changed_callback(true);
            USBStatusLed.set();
        }
    }, nullptr};

}
