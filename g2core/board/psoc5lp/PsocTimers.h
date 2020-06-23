/*
 utility/SamTimers.h - Library for the Motate system
 http://github.com/synthetos/motate/

 Copyright (c) 2013 - 2016 Robert Giseburt

 This file is part of the Motate Library.

 This file ("the software") is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License, version 2 as published by the
 Free Software Foundation. You should have received a copy of the GNU General Public
 License, version 2 along with the software. If not, see <http://www.gnu.org/licenses/>.

 As a special exception, you may use this file as part of a software library without
 restriction. Specifically, if other files instantiate templates or use macros or
 inline functions from this file, or you compile this file and link it with  other
 files to produce an executable, this file does not by itself cause the resulting
 executable to be covered by the GNU General Public License. This exception does not
 however invalidate any other reasons why the executable file might be covered by the
 GNU General Public License.

 THE SOFTWARE IS DISTRIBUTED IN THE HOPE THAT IT WILL BE USEFUL, BUT WITHOUT ANY
 WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
 SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef SAMTIMERS_H_ONCE
#define SAMTIMERS_H_ONCE

//#include "sam.h"
//#include "SamCommon.h"
#ifndef PWM_PTCR_TXTEN
//#include "SamDMA.h"
#endif
#include <functional> // for std::function and related
#include <type_traits> // for std::extent and std::alignment_of

/* Sam hardware has two types of timer: "Timers" and "PWMTimers"
 *
 * Timers:
 *
 * Sam hardware timers have three channels each. Each channel is actually an
 * independent timer, so we have a little nomenclature clash.
 *
 * Sam Timer != Motate::Timer!!!
 *
 * A Sam Timer CHANNEL is actually the portion that a Motate::Timer controls
 * direcly. Each SAM CHANNEL has two Motate:Timers (A and B).
 * (Actually, the Quadrature Decoder and Block Control can mix them up some,
 * but we ignore that.)
 * So, for the Sam, we have to maintain the same interface, and treat each
 * channel as an independent timer.
 *
 *
 * PWMTimers:
 *
 * For compatibility and transparency with Timers, we use the same TimerModes,
 * even through they actually use bitmaps for Timer registers.
 *
 * Timers have more modes than PWM Timers, and more interrupts.
 * We return kInvalidMode for the ones that don't map, except that we treat "Up"
 * and "UpToMatch" both as "LeftAligned," and "UpDown" and "UpDownToMatch"
 * as "CenterAligned."
 *
 * Consequently, you can use kPWMLeftAligned and kPWMCenterAligned as valid modes
 * on a Timer.
 */


namespace Motate {
#pragma mark Enums, typedefs, etc.
    /**************************************************
     *
     * Enums and typedefs
     *
     **************************************************/

#define TC_CMR_CPCTRG 0
#define TC_CMR_WAVE 0
#define TC_CMR_WAVSEL_UP 0
#define TC_CMR_WAVSEL_UP_RC 0
#define TC_CMR_WAVSEL_UPDOWN 0
#define TC_CMR_WAVSEL_UPDOWN_RC 0

    enum TimerMode {
        /* InputCapture mode (WAVE = 0) */
        kTimerInputCapture         = 0,
        /* InputCapture mode (WAVE = 0), counts up to RC */
        kTimerInputCaptureToMatch  = 0 | TC_CMR_CPCTRG,

        /* Waveform select, Up to 0xFFFFFFFF */
        kTimerUp            = TC_CMR_WAVE | TC_CMR_WAVSEL_UP,
        /* Waveform select, Up to TOP (RC) */
        kTimerUpToTop       = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC,
        /* Keep the "ToMatch" naming for compatibility */
        kTimerUpToMatch     = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC,
        /* For PWM, we'll alias kTimerUpToMatch as: */
        kPWMLeftAligned     = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC,
        /* Waveform select, Up to 0xFFFFFFFF, then Down */
        kTimerUpDown        = TC_CMR_WAVE | TC_CMR_WAVSEL_UPDOWN,
        /* Waveform select, Up to TOP (RC), then Down */
        kTimerUpDownToTop   = TC_CMR_WAVE | TC_CMR_WAVSEL_UPDOWN_RC,
        /* Keep the "ToMatch" naming for compatibility */
        kTimerUpDownToMatch = TC_CMR_WAVE | TC_CMR_WAVSEL_UPDOWN_RC,
        /* For PWM, we'll alias kTimerUpDownToMatch as: */
        kPWMCenterAligned     = kTimerUpDownToMatch,
    };

    enum TimerSyncMode {
        kTimerSyncManually = 0,
        kTimerSyncDMA      = 1
    };

    /* We're trading acronyms for verbose CamelCase. Dubious. */
    enum TimerChannelOutputOptions {
        kOutputDisconnected = 0,

        kToggleOnMatch     = 1<<0,
        kClearOnMatch      = 1<<1,
        kSetOnMatch        = 1<<2,

        kToggleOnOverflow  = 1<<3,
        kClearOnOverflow   = 1<<4,
        kSetOnOverflow     = 1<<5,


        /* Aliases for use with PWM */
        kPWMOn             = kClearOnMatch | kSetOnOverflow,
        kPWMOnInverted     = kSetOnMatch | kClearOnOverflow,
    };

    /* We use TC_CMR_EEVT_XC0 in the above to allow TIOB to be an output.
     * The defualt is for it to be the input for ExternalEvent.
     * By setting it to XC0, we allow it to be an output.
     */

    enum TimerChannelInterruptOptions {
        kInterruptsOff              = 0,
        /* Alias for "off" to make more sense
         when returned from setInterruptPending(). */
        kInterruptUnknown           = 0,

        kInterruptOnMatch          = 1<<1,
        /* Note: Interrupt on overflow could be a match C as well. */
        kInterruptOnOverflow        = 1<<3,

        /* This turns the IRQ on, but doesn't set the timer to ever trigger it. */
        kInterruptOnSoftwareTrigger = 1<<4,

        /* Set priority levels here as well: */
        kInterruptPriorityHighest   = 1<<5,
        kInterruptPriorityHigh      = 1<<6,
        kInterruptPriorityMedium    = 1<<7,
        kInterruptPriorityLow       = 1<<8,
        kInterruptPriorityLowest    = 1<<9,
    };

    enum TimerErrorCodes {
        kFrequencyUnattainable = -1,
        kInvalidMode = -2,
    };

    enum PWMTimerClockOptions {
        kPWMClockPrescalerOnly = 0,
        kPWMClockPrescaleAndDivA = 1,
        kPWMClockPrescaleAndDivB = 2,
    };

    typedef const uint8_t timer_number;

#pragma mark Timer<n>
    /**************************************************
     *
     * TIMERS: Timer<n>
     *
     **************************************************/

    template <uint8_t timerNum>
    struct Timer {
        Timer() { init(); };
        Timer(const TimerMode mode, const uint32_t freq) {
            init();
            setModeAndFrequency(mode, freq);
        };
        void init() {
        }
        int32_t setModeAndFrequency(const TimerMode mode, uint32_t freq) {
            return 0;
        }
        void setInterruptPending() {
        }
        void start() {
        }
        void stop() {
        }
        void setInterrupts(const uint32_t interrupts, const int16_t channel = -1) {
        }
        static TimerChannelInterruptOptions getInterruptCause(int16_t &channel) {
            return kInterruptsOff;
        }
    };

    template<uint8_t timerNum, uint8_t channelNum>
    struct TimerChannel : Timer<timerNum> {
        TimerChannel() : Timer<timerNum>{} {};
        TimerChannel(const TimerMode mode, const uint32_t freq) : Timer<timerNum>{mode, freq} {};

        void setDutyCycle(const float ratio) {
            Timer<timerNum>::setDutyCycleForChannel(channelNum, ratio);
        };

        float getDutyCycle() {
            return Timer<timerNum>::getDutyCycleForChannel(channelNum);
        };

        void setExactDutyCycle(const uint32_t absolute) {
            Timer<timerNum>::setExactDutyCycleForChannel(channelNum, absolute);
        };

        uint32_t getExactDutyCycle() {
            return Timer<timerNum>::getExactDutyCycleForChannel(channelNum);
        };


        void setOutputOptions(const uint32_t options) {
            Timer<timerNum>::setOutputOptions(channelNum, options);
        };

        void startPWMOutput() {
            Timer<timerNum>::startPWMOutput(channelNum);
        };

        void stopPWMOutput() {
            Timer<timerNum>::stopPWMOutput(channelNum);
        }

        void setInterrupts(const uint32_t interrupts) {
            Timer<timerNum>::setInterrupts(interrupts, channelNum);
        };

        TimerChannelInterruptOptions getInterruptCause(int16_t &channel) {
            channel = 1;
            return Timer<timerNum>::getInterruptCause(channel);
        }

        TimerChannelInterruptOptions getInterruptCause() {
            int16_t channel = 1;
            return Timer<timerNum>::getInterruptCause(channel);
        }

        // Placeholder for user code.
        static void interrupt();
    };

#pragma mark SysTickEvent, Timer<SysTickTimerNum> SysTickTimer
    /**************************************************
     *
     * SysTickTimer and related:
     *  Timer<SysTickTimerNum> is the special Timer for Systick.
     *  SysTickTimer is the global singleton to access it.
     *  SysTickEvent is the class to use to register a new event to occur every Tick.
     *
     **************************************************/
    struct SysTickEvent {
        const std::function<void(void)> callback;
        SysTickEvent *next;
    };

    static const timer_number SysTickTimerNum = 0xFF;
    template <>
    struct Timer<SysTickTimerNum> {
        static volatile uint32_t _motateTickCount;
        SysTickEvent *firstEvent = nullptr;

        Timer() { init(); };
        Timer(const TimerMode mode, const uint32_t freq) {
            init();
        };

        void init() {
            _motateTickCount = 0;

            // Set Systick to 1ms interval, common to all SAM3 variants
            //if (SysTick_Config(SamCommon::getPeripheralClockFreq() / 1000))
            //{
                // Capture error
            //    while (true);
            //}
        };

        // Return the current value of the counter. This is a fleeting thing...
        uint32_t getValue() {
            return _motateTickCount;
        };

        void _increment() {
            _motateTickCount++;
        };

        void registerEvent(SysTickEvent *new_event) {
            if (firstEvent == nullptr) {
                firstEvent = new_event;
                return;
            }
            SysTickEvent *event = firstEvent;
            if (new_event == event) { return; }
            while (event->next != nullptr) {
                event = event->next;
                if (new_event == event) { return; }
            }
            event->next = new_event;
            new_event->next = nullptr;
        };

        void unregisterEvent(SysTickEvent *new_event) {
            if (firstEvent == new_event) {
                firstEvent = firstEvent->next;
                return;
            }
            SysTickEvent *event = firstEvent;
            while (event->next != nullptr) {
                if (event->next == new_event) {
                    event->next = event->next->next;
                    return;
                }
                event = event->next;
            }
        };

        void _handleEvents() {
            SysTickEvent *event = firstEvent;
            while (event != nullptr) {
                event->callback();
                event = event->next;
            }
        };

        // Placeholder for user code.
        static void interrupt() __attribute__ ((weak));
    };
    extern Timer<SysTickTimerNum> SysTickTimer;

#pragma mark Timeout
    /**************************************************
     *
     * Timeout: Simple non-blocking (polling) timeout class.
     *
     **************************************************/
    struct Timeout {
        uint32_t start_, delay_;
        Timeout() : start_ {0}, delay_ {0} {};

        bool isSet() {
            return (start_ > 0);
        }

        bool isPast() {
            if (!isSet()) {
                return false;
            }
            return ((SysTickTimer.getValue() - start_) > delay_);
        };

        void set(uint32_t delay) {
            start_ = SysTickTimer.getValue();
            delay_ = delay;
        };

        void clear() {
            start_ = 0;
            delay_ = 0;
        }
    };

#pragma mark delay()
    /**************************************************
     *
     * delay(): Arduino-compatible blocking-delay function
     *
     **************************************************/

    inline void delay( uint32_t microseconds )
    {
        //uint32_t doneTime = SysTickTimer.getValue() + microseconds;

        do
        {
            //__NOP();
        } while ( 1 ); //SysTickTimer.getValue() < doneTime );
    }

}

#endif /* end of include guard: SAMTIMERS_H_ONCE */
