#include "hardware.h"
#include "gpio.h"

#include "canonical_machine.h"
#include "xio.h"

struct Hal {
    Hal() {
        Motate::SysTickTimer.registerEvent(&hal_event);
    }

    void estop_machine() {
        if (cm->machine_state != MACHINE_CYCLE) {
            cm->machine_state = MACHINE_ALARM;
        } else {
            if (cm->hold_state == FEEDHOLD_OFF) {
                cm_request_feedhold(FEEDHOLD_TYPE_HOLD, FEEDHOLD_EXIT_FLUSH);
            } else if (cm->hold_state == FEEDHOLD_HOLD) {
                cm_request_cycle_start();
                xio_flush_to_command();
            }
        }
    }

    void unhome() {
        for (uint8_t i = 0; i < HOMING_AXES; i++) {
            cm->homed[i] = false;
        }
        cm->homing_state = HOMING_NOT_HOMED;
    }

    const float default_travel_min[AXES] = {
        X_TRAVEL_MIN,
        Y_TRAVEL_MIN,
        Z_TRAVEL_MIN,
        U_TRAVEL_MIN,
        V_TRAVEL_MIN,
        W_TRAVEL_MIN,
        A_TRAVEL_MIN,
        B_TRAVEL_MIN,
        C_TRAVEL_MIN
    };

    void lock_axis(uint8_t axis) {
        cm->a[axis].travel_min = DISABLE_SOFT_LIMIT - 1;
    }

    void unlock_axis(uint8_t axis) {
        cm->a[axis].travel_min = default_travel_min[axis];
    }

    Motate::SysTickEvent hal_event {[&] {
        bool feeder_front_fault;
        bool feeder_rear_fault;
        bool feeder_fault;
        bool motor_ac_fault;
        bool motor_dc_fault;
        bool power_good;
        bool motor_fault;
        bool estop_lamp;

        feeder_front_fault = !PSOC::SDLC_R_Read(1 << 9);
        feeder_rear_fault = !PSOC::SDLC_L_Read(1 << 30);
        feeder_fault = feeder_front_fault || feeder_rear_fault;

        motor_ac_fault = gpio_read_input(7);
        motor_dc_fault = gpio_read_input(8);
        power_good = gpio_read_input(9);
        motor_fault = !power_good || motor_ac_fault || motor_dc_fault;

        if (motor_fault) {
            estop_machine();
            unhome();
        }

        if (feeder_fault) {
            estop_machine();
        }

        estop_lamp = cm->machine_state == MACHINE_ALARM;
        PSOC::SDLC_R_Write(1, estop_lamp);

        /* Inhibit X/Y movement if either U or V are lowered */
        if ((cm_get_absolute_position(MODEL, AXIS_U) < -10) ||
            (cm_get_absolute_position(MODEL, AXIS_V) < -10)) {
            lock_axis(AXIS_X);
            lock_axis(AXIS_Y);
        } else {
            unlock_axis(AXIS_X);
            unlock_axis(AXIS_Y);
        }
    }, nullptr};
};

Hal Hal;
