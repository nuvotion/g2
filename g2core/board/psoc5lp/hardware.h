/*
 * hardware.h - system hardware configuration
 * For: /board/ArduinoDue
 * THIS FILE IS HARDWARE PLATFORM SPECIFIC - ARM version
 *
 * This file is part of the g2core project
 *
 * Copyright (c) 2013 - 2018 Alden S. Hart, Jr.
 * Copyright (c) 2013 - 2018 Robert Giseburt
 *
 * This file ("the software") is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2 as published by the
 * Free Software Foundation. You should have received a copy of the GNU General Public
 * License, version 2 along with the software.  If not, see <http://www.gnu.org/licenses/> .
 *
 * As a special exception, you may use this file as part of a software library without
 * restriction. Specifically, if other files instantiate templates or use macros or
 * inline functions from this file, or you compile this file and link it with  other
 * files to produce an executable, this file does not by itself cause the resulting
 * executable to be covered by the GNU General Public License. This exception does not
 * however invalidate any other reasons why the executable file might be covered by the
 * GNU General Public License.
 *
 * THE SOFTWARE IS DISTRIBUTED IN THE HOPE THAT IT WILL BE USEFUL, BUT WITHOUT ANY
 * WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
 * SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "config.h"
#include "error.h"

#ifndef HARDWARE_H_ONCE
#define HARDWARE_H_ONCE

/*--- Hardware platform enumerations ---*/

#define G2CORE_HARDWARE_PLATFORM    "PSOC 5LP"
#define G2CORE_HARDWARE_VERSION     "Nuvotion-Yamaha"

/***** Motors & PWM channels supported by this hardware *****/
// These must be defines (not enums) so expressions like this:
//  #if (MOTORS >= 6)  will work

#define MOTORS 6                    // number of motors supported the hardware
#define PWMS 2                      // number of PWM channels supported the hardware

/*************************
 * Global System Defines *
 *************************/

#define MILLISECONDS_PER_TICK 1     // MS for system tick (systick * N)
#define SYS_ID_LEN 9                // total length including dashes and NUL

/*************************
 * Motate Setup          *
 *************************/

#include "MotatePins.h"
#include "MotateTimers.h"           // for TimerChanel<> and related...
#include "MotateServiceCall.h"      // for ServiceCall<>

using Motate::TimerChannel;
//using Motate::ServiceCall;

using Motate::pin_number;
using Motate::Pin;
using Motate::PWMOutputPin;
using Motate::OutputPin;

/************************************************************************************
 **** ARM SAM3X8E SPECIFIC HARDWARE *************************************************
 ************************************************************************************/

/**** Resource Assignment via Motate ****
 *
 * This section defines resource usage for pins, timers, PWM channels, communications
 * and other resources. Please refer to /motate/utility/SamPins.h, SamTimers.h and
 * other files for pinouts and other configuration details.
 *
 * Commenting out or #ifdef'ing out definitions below will cause the compiler to
 * drop references to these resources from the compiled code. This will reduce
 * compiled code size and runtime CPU cycles. E.g. if you are compiling for a 3 motor,
 * XYZ axis config commenting out the higher motors and axes here will remove them
 * from later code (using the motate .isNull() test).
 */

/* Interrupt usage and priority
 *
 * The following interrupts are defined w/indicated priorities
 *
 *       0      DDA_TIMER (3) for step pulse generation
 *       1      DWELL_TIMER (4) for dwell timing
 *       2      LOADER software generated interrupt (STIR / SGI)
 *       3      Serial read character interrupt
 *       4      EXEC software generated interrupt (STIR / SGI)
 *       5      Serial write character interrupt
 */

/**** Stepper DDA and dwell timer settings ****/

#define FREQUENCY_DDA           6000UL                  // Hz step frequency. Interrupts actually fire at 2x (400 KHz)
#define FREQUENCY_DWELL         1000UL
#define FREQUENCY_SGI           200000UL                // 200,000 Hz means software interrupts will fire 5 uSec after being called

/**** Motate Definitions ****/

// Timer definitions. See stepper.h and other headers for setup
typedef TimerChannel<3,0> dda_timer_type;       // stepper pulse generation in stepper.cpp
typedef TimerChannel<4,0> exec_timer_type;      // request exec timer in stepper.cpp
typedef TimerChannel<5,0> fwd_plan_timer_type;  // request exec timer in stepper.cpp

// Pin assignments
static OutputPin<Motate::kLED_PinNumber> IndicatorLed;
static OutputPin<Motate::kUSB_PinNumber> USBStatusLed;
static OutputPin<Motate::kUFM_PinNumber> UFMEnable;

/**** Motate Global Pin Allocations ****/

static OutputPin<Motate::kSpindle_EnablePinNumber> spindle_enable_pin;
static OutputPin<Motate::kSpindle_DirPinNumber> spindle_dir_pin;

// NOTE: In the v9 and the Due the flood and mist coolants are mapped to a the same pin
static OutputPin<Motate::kCoolant_EnablePinNumber> flood_enable_pin;
static OutputPin<Motate::kCoolant_EnablePinNumber> mist_enable_pin;

// Input pins are defined in gpio.cpp

/********************************
 * Function Prototypes (Common) *
 ********************************/
#include "cypress.h"

void hardware_init(void);                       // master hardware init
stat_t hardware_periodic();  // callback from the main loop (time sensitive)
void hw_hard_reset(void);
stat_t hw_flash(nvObj_t *nv);

stat_t hw_get_fb(nvObj_t *nv);
stat_t hw_get_fv(nvObj_t *nv);
stat_t hw_get_hp(nvObj_t *nv);
stat_t hw_get_hv(nvObj_t *nv);
stat_t hw_get_fbs(nvObj_t *nv);
stat_t hw_get_fbc(nvObj_t *nv);
stat_t hw_get_id(nvObj_t *nv);

#ifdef __TEXT_MODE

    void hw_print_fb(nvObj_t *nv);
    void hw_print_fv(nvObj_t *nv);
    void hw_print_fbs(nvObj_t *nv);
    void hw_print_fbc(nvObj_t *nv);
    void hw_print_hp(nvObj_t *nv);
    void hw_print_hv(nvObj_t *nv);
    void hw_print_id(nvObj_t *nv);

#else

    #define hw_print_fb tx_print_stub
    #define hw_print_fv tx_print_stub
    #define hw_print_fbs tx_print_stub
    #define hw_print_fbc tx_print_stub
    #define hw_print_hp tx_print_stub
    #define hw_print_hv tx_print_stub
    #define hw_print_id tx_print_stub

#endif // __TEXT_MODE

#endif  // end of include guard: HARDWARE_H_ONCE
