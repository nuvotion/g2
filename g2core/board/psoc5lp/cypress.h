#ifndef CYPRESS_H_ONCE
#define CYPRESS_H_ONCE

#include <stdint.h>

extern "C" {
    void CyGetUniqueId(uint32_t *uniqueId);

    uint8_t LED_0_Read(void);
    void LED_0_Write(uint8_t value);
    uint8_t LED_1_Read(void);
    void LED_1_Write(uint8_t value);

    uint8_t INPUT_STATUS_Read(void);

    uint8_t STEP_0_5_Read(void);
    void STEP_0_5_Write(uint8_t control);
    uint8_t DIR_0_5_Read(void);
    void DIR_0_5_Write(uint8_t control);

    void SYSTICK_TIMER_Start(void);
    void DDA_TIMER_Start(void);
    typedef void (* cyisraddress)(void);
    void SYSTICK_IRQ_StartEx(cyisraddress address);
    void DDA_IRQ_StartEx(cyisraddress address);
    void EXEC_IRQ_StartEx(cyisraddress address);
    void EXEC_IRQ_SetPending(void);
    void EXEC_IRQ_ClearPending(void);
    void FWD_PLAN_IRQ_StartEx(cyisraddress address);
    void FWD_PLAN_IRQ_SetPending(void);
    void FWD_PLAN_IRQ_ClearPending(void);

    void USBFS_Start(uint8_t device, uint8_t mode);
    uint8_t USBFS_GetConfiguration(void);
    uint8_t USBFS_CDC_Init(void);
    uint8_t USBFS_DataIsReady(void);
    uint16_t USBFS_GetAll(uint8_t *pData);
    uint8_t USBFS_CDCIsReady(void);
    void USBFS_PutData(const uint8_t *pData, uint16_t length);

    void SDLC_Setup(void);
    void SDLC_SendReceive(uint8_t tx_len, uint8_t rx_len, uint8_t *tx_data, uint8_t *rx_data);
    uint16_t SDLC_GetRxBytes(void);

    __attribute__((always_inline)) static inline void __DMB(void) {
        __asm volatile ("dmb 0xF":::"memory");
    }
}

void SystemInit(void);

#endif /* end of include guard: CYPRESS_H_ONCE */
