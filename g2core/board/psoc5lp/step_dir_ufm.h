/*
 * step_dir_driver.cpp - control over a Step/Direction/Enable stepper motor driver
 * This file is part of G2 project
 *
 * Copyright (c) 2016 - 2018 Alden S. Hart, Jr.
 * Copyright (c) 2016 - 2018 Robert Giseburt
 *
 * This file ("the software") is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2 as published by the
 * Free Software Foundation. You should have received a copy of the GNU General Public
 * License, version 2 along with the software.  If not, see <http://www.gnu.org/licenses/>.
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
#ifndef STEP_DIR_DRIVER_H_ONCE
#define STEP_DIR_DRIVER_H_ONCE

#include "MotatePins.h"
#include "MotateTimers.h"

#include "cypress.h"

#include "stepper.h"

using Motate::pin_number;
using Motate::OutputPin;
using Motate::PWMOutputPin;
using Motate::kStartHigh;
using Motate::kStartLow;
using Motate::kNormal;
using Motate::Timeout;

namespace PSOC {
    extern uint8_t ufm_data[MOTORS+1];
}

// Motor structures
template <pin_number step_num,  // Setup a stepper template to hold our pins
          pin_number dir_num,
          pin_number enable_num,
          pin_number ms0_num,
          pin_number ms1_num,
          pin_number ms2_num,
          pin_number vref_num>
struct StepDirStepper final : Stepper  {

    StepDirStepper(ioMode step_polarity, ioMode enable_polarity) {
        if (step_num == Motate::kSocket1_StepPinNumber) {
            I2C_UFM_SetupDMA(MOTORS+1);
            UFMEnable.set();
        }
    };

    void stepStart(int8_t steps) override {
        PSOC::ufm_data[step_num >> 1] = steps;
        if (step_num == Motate::kSocket1_StepPinNumber) {
            PSOC::ufm_data[0]++;
            I2C_UFM_WriteDMA(MOTORS+1, PSOC::ufm_data);
        }
    }

};

#endif  // STEP_DIR_DRIVER_H_ONCE
