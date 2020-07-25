
#include "PsocTimers.h"
#include "uart.h"

namespace Motate {
    /* System-wide tick counter */
    /*  Inspired by code from Atmel and Arduino.
     *  Some of which is:   Copyright (c) 2012 Arduino. All right reserved.
     *  Some of which is:   Copyright (c) 2011-2012, Atmel Corporation. All rights reserved.
     */

    Timer<SysTickTimerNum> SysTickTimer;
    //Timer<WatchDogTimerNum> WatchDogTimer;

    volatile uint32_t Timer<SysTickTimerNum>::_motateTickCount = 0;

    void SysTick_Handler() {
        SysTickTimer._increment();
        
        if (SysTickTimer.interrupt) SysTickTimer.interrupt();

        SysTickTimer._handleEvents();
    }

    void DDA_Handler() {
        __DMB();
        Motate::TimerChannel<3, 0>::interrupt();
    }
    
    void EXEC_Handler() {
        __DMB();
        Motate::TimerChannel<4, 0>::interrupt();
    }

    void FWD_PLAN_Handler() {
        __DMB();
        Motate::TimerChannel<5, 0>::interrupt();
    }

} // namespace Motate

