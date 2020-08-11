#ifndef PSOCUART_H_ONCE                                                       
#define PSOCUART_H_ONCE

#include "MotatePins.h"
#include "MotateCommon.h"

#include "g2core.h"
#include "util.h"
#include "cypress.h"

namespace PSOC {

    extern Motate::SysTickEvent usb_systick_event;

    struct UART {

        std::function<void(bool)> connection_state_changed_callback;
        std::function<void(void)> transfer_tx_done_callback;

        int usb_configured = 0;
        int usb_connected = 0;
        int cdc_open = 0;
        uint8_t rx_bytes[64];
        char *last_write = nullptr;
        char *last_read = nullptr;

        void setConnectionCallback(std::function<void(bool)> &&callback) {
            connection_state_changed_callback = std::move(callback);
        }

        bool startTXTransfer(char *buffer, const uint16_t length) {
            uint16_t num_bytes;
            uint16_t remaining = length;
            uint16_t offset = 0;

            if (!cdc_open) goto finish;

            while (remaining) {
                while (!USBFS_CDCIsReady()) {
                    if (!cdc_open) goto finish;
                }
                num_bytes = min((uint16_t) 64, remaining);

                USBFS_PutData((const uint8_t *) buffer + offset, num_bytes);
                remaining -= num_bytes;
                offset += num_bytes;
            }

            finish:
            last_read = buffer + length;
            transfer_tx_done_callback();
            return true;
        }

        void setTXTransferDoneCallback(std::function<void()> &&callback) {
            transfer_tx_done_callback = std::move(callback);
        }

        char* getTXTransferPosition() {
            return last_read;
        }

        void flush() {
        }

        bool startRXTransfer(char *&buffer, uint16_t length, char *&buffer2, uint16_t length2) {
            uint8_t first_write;
            uint16_t rx_bytes_received;

            if (!cdc_open) return false;

            if (!USBFS_DataIsReady()) return false;

            if (length + length2 < 64) return false;

            rx_bytes_received = USBFS_GetAll(rx_bytes);

            first_write = min(rx_bytes_received, length);

            if (first_write > 0) {
                memcpy(buffer, rx_bytes, first_write);
                last_write = buffer + first_write;
            }

            if (rx_bytes_received > first_write) {
                memcpy(buffer2, rx_bytes + first_write, rx_bytes_received - first_write); 
                last_write = buffer2 + rx_bytes_received - first_write;
            }

            return false;
        }

        void setRXTransferDoneCallback(std::function<void()> &&callback) {
        }

        char* getRXTransferPosition() {
            return last_write;
        }

        void flushRead() {
        }
    };
}

#endif /* end of include guard: PSOCUART_H_ONCE */
