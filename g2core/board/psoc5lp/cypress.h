#ifndef CYPRESS_H_ONCE
#define CYPRESS_H_ONCE

#include <stdint.h>

extern "C" {
    void LED_0_Write(uint8_t value);
    void LED_1_Write(uint8_t value);

    void CySysTickStart(void);

    typedef void (*cySysTickCallback)(void);
    cySysTickCallback CySysTickSetCallback(uint32_t number, cySysTickCallback function);

    void USBFS_Start(uint8_t device, uint8_t mode);
    uint8_t USBFS_GetConfiguration(void);
    uint8_t USBFS_CDC_Init(void);
    uint8_t USBFS_DataIsReady(void);
    uint16_t USBFS_GetAll(uint8_t *pData);
    uint8_t USBFS_CDCIsReady(void);
    void USBFS_PutData(const uint8_t *pData, uint16_t length);
}

void SystemInit(void);

#endif /* end of include guard: CYPRESS_H_ONCE */
