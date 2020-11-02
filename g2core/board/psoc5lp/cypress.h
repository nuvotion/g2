#ifndef CYPRESS_H_ONCE
#define CYPRESS_H_ONCE

#include <stdint.h>

extern "C" {
    void CyGetUniqueId(uint32_t *uniqueId);

    uint8_t INPUT_STATUS_Read(void);

    void SYSTICK_TIMER_Start(void);
    void DDA_TIMER_Start(void);
    typedef void (* cyisraddress)(void);
    void SYSTICK_IRQ_StartEx(cyisraddress address);
    void SYSTICK_IRQ_ClearPending(void);
    void DDA_IRQ_StartEx(cyisraddress address);
    void DDA_IRQ_ClearPending(void);
    void EXEC_IRQ_StartEx(cyisraddress address);
    void EXEC_IRQ_SetPending(void);
    void EXEC_IRQ_ClearPending(void);
    void FWD_PLAN_IRQ_StartEx(cyisraddress address);
    void FWD_PLAN_IRQ_SetPending(void);
    void FWD_PLAN_IRQ_ClearPending(void);

    void USBFS_Start(uint8_t device, uint8_t mode);
    void USBFS_Stop(void);
    uint8_t USBFS_GetConfiguration(void);
    uint8_t USBFS_CDC_Init(void);
    uint8_t USBFS_DataIsReady(void);
    uint16_t USBFS_GetAll(uint8_t *pData);
    uint8_t USBFS_CDCIsReady(void);
    void USBFS_PutData(const uint8_t *pData, uint16_t length);
    uint8_t USBFS_IsLineChanged(void);
    uint16_t USBFS_GetLineControl(void);

    void SDLC_Setup(void);
    void SDLC_SendReceive(uint8_t tx_len, uint8_t rx_len, uint8_t *tx_data, uint8_t *rx_data);
    uint16_t SDLC_GetRxBytes(void);

    void I2C_UFM_SetupDMA(uint8_t max_len);
    void I2C_UFM_WriteDMA(uint8_t len, uint8_t *wr_data);

    __attribute__((always_inline)) static inline void __DMB(void) {
        __asm volatile ("dmb 0xF":::"memory");
    }
}

void SystemInit(void);

#endif /* end of include guard: CYPRESS_H_ONCE */
