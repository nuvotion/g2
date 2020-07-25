#ifndef CYPRESS_H_ONCE
#define CYPRESS_H_ONCE

#include <stdint.h>

extern "C" {
    void LED_0_Write(uint8_t value);
    void LED_1_Write(uint8_t value);

    void SYSTICK_TIMER_Start(void);
    void DDA_TIMER_Start(void);
    typedef void (* cyisraddress)(void);
    void SYSTICK_IRQ_StartEx(cyisraddress address);
    void DDA_IRQ_StartEx(cyisraddress address);
    void EXEC_IRQ_StartEx(cyisraddress address);
    void EXEC_IRQ_SetPending(void);
    void FWD_PLAN_IRQ_StartEx(cyisraddress address);
    void FWD_PLAN_IRQ_SetPending(void);

    void USBFS_Start(uint8_t device, uint8_t mode);
    uint8_t USBFS_GetConfiguration(void);
    uint8_t USBFS_CDC_Init(void);
    uint8_t USBFS_DataIsReady(void);
    uint16_t USBFS_GetAll(uint8_t *pData);
    uint8_t USBFS_CDCIsReady(void);
    void USBFS_PutData(const uint8_t *pData, uint16_t length);

    __attribute__((always_inline)) static inline void __DMB(void) {
        __asm volatile ("dmb 0xF":::"memory");
    }
}

void SystemInit(void);

#endif /* end of include guard: CYPRESS_H_ONCE */
