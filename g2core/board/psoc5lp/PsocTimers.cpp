
#include "PsocTimers.h"

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

} // namespace Motate

