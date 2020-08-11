/*
 * motate_pin_assignments.h - pin assignments
 * For: /board/ArduinoDue
 * This file is part of the g2core project
 *
 * Copyright (c) 2013-2016 Robert Giseburt
 * Copyright (c) 2013-2016 Alden S. Hart Jr.
 *
 * This file is part of the Motate Library.
 *
 * This file ("the software") is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2 as published by the
 * Free Software Foundation. You should have received a copy of the GNU General Public
 * License, version 2 along with the software. If not, see <http://www.gnu.org/licenses/>.
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
 *
 */

#ifndef motate_pin_assignments_h
#define motate_pin_assignments_h

#include "nuvotion-pinout.h"

namespace Motate {

    /* Fake port names:
     * A,B - Board LED indicators
     * S   - Board status input pins
     * M   - Motor control (step)
     * N   - Motor control (dir)
     * L   - Left I/O board
     * R   - Right I/O board
     * H   - Head I/O board
     * C   - Chip loader I/O board */

    _MAKE_MOTATE_PIN(kLED_PinNumber, 'A', 0);
    _MAKE_MOTATE_PIN(kUSB_PinNumber, 'B', 0);

    _MAKE_MOTATE_PIN(kSocket1_StepPinNumber, 'M', 0);
    _MAKE_MOTATE_PIN(kSocket1_DirPinNumber,  'N', 0);
    _MAKE_MOTATE_PIN(kSocket2_StepPinNumber, 'M', 1);
    _MAKE_MOTATE_PIN(kSocket2_DirPinNumber,  'N', 1);
    _MAKE_MOTATE_PIN(kSocket3_StepPinNumber, 'M', 2);
    _MAKE_MOTATE_PIN(kSocket3_DirPinNumber,  'N', 2);
    _MAKE_MOTATE_PIN(kSocket4_StepPinNumber, 'M', 3);
    _MAKE_MOTATE_PIN(kSocket4_DirPinNumber,  'N', 3);
    _MAKE_MOTATE_PIN(kSocket5_StepPinNumber, 'M', 4);
    _MAKE_MOTATE_PIN(kSocket5_DirPinNumber,  'N', 4);
    _MAKE_MOTATE_PIN(kSocket6_StepPinNumber, 'M', 5);
    _MAKE_MOTATE_PIN(kSocket6_DirPinNumber,  'N', 5);

    _MAKE_MOTATE_PIN(kInput1_PinNumber, 'C', 22);
    _MAKE_MOTATE_PIN(kInput9_PinNumber, 'S',  0);
}  // namespace Motate

// We then allow each chip-type to have it's own function definitions
// that will refer to these pin assignments.
#include "motate_chip_pin_functions.h"


#ifdef MOTATE_BOARD
#define MOTATE_BOARD_PINOUT < MOTATE_BOARD-pinout.h >
#include MOTATE_BOARD_PINOUT
#else
#error Unknown board layout
#endif

#endif

// motate_pin_assignments_h
