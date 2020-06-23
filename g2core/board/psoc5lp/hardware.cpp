#include "config.h"
#include "error.h"

void hardware_init(void) {};			// master hardware init
stat_t hardware_periodic() { return 0; };  // callback from the main loop (time sensitive)
void hw_hard_reset(void) {};
stat_t hw_flash(nvObj_t *nv) { return 0; };

stat_t hw_get_fb(nvObj_t *nv) { return 0; };
stat_t hw_get_fv(nvObj_t *nv) { return 0; };
stat_t hw_get_hp(nvObj_t *nv) { return 0; };
stat_t hw_get_hv(nvObj_t *nv) { return 0; };
stat_t hw_get_fbs(nvObj_t *nv) { return 0; };
stat_t hw_get_fbc(nvObj_t *nv) { return 0; };
stat_t hw_get_id(nvObj_t *nv) { return 0; };

void hw_print_fb(nvObj_t *nv) {};
void hw_print_fv(nvObj_t *nv) {};
void hw_print_fbs(nvObj_t *nv) {};
void hw_print_fbc(nvObj_t *nv) {};
void hw_print_hp(nvObj_t *nv) {};
void hw_print_hv(nvObj_t *nv) {};
void hw_print_id(nvObj_t *nv) {};
