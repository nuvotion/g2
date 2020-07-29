#include "config.h"
#include "error.h"
#include "controller.h"
#include "text_parser.h"

void hardware_init(void) {};                    // master hardware init
stat_t hardware_periodic() { return 0; };       // callback from the main loop (time sensitive)
void hw_hard_reset(void) {};
stat_t hw_flash(nvObj_t *nv) { return 0; };

stat_t hw_get_fb(nvObj_t *nv)   { return (get_float(nv, cs.fw_build)); }
stat_t hw_get_fv(nvObj_t *nv)   { return (get_float(nv, cs.fw_version)); }
stat_t hw_get_hp(nvObj_t *nv)   { return (get_string(nv, G2CORE_HARDWARE_PLATFORM)); }
stat_t hw_get_hv(nvObj_t *nv)   { return (get_string(nv, G2CORE_HARDWARE_VERSION)); }
stat_t hw_get_fbs(nvObj_t *nv)  { return (get_string(nv, G2CORE_FIRMWARE_BUILD_STRING)); }

stat_t hw_get_fbc(nvObj_t *nv) {
    nv->valuetype = TYPE_STRING;
#ifdef SETTINGS_FILE
#define settings_file_string1(s) #s
#define settings_file_string2(s) settings_file_string1(s)
    ritorno(nv_copy_string(nv, settings_file_string2(SETTINGS_FILE)));
#undef settings_file_string1
#undef settings_file_string2
#else
    ritorno(nv_copy_string(nv, "<default-settings>"));
#endif
    return (STAT_OK);
}

stat_t hw_get_id(nvObj_t *nv) {
    uint32_t id[2];
    CyGetUniqueId(id);
    char tmp[SYS_ID_LEN];
    sprintf(tmp, "%08lX", id[0] ^ id[1]);
    nv->valuetype = TYPE_STRING;
    ritorno(nv_copy_string(nv, tmp));
    return (STAT_OK);
}

#ifdef __TEXT_MODE

    static const char fmt_fb[] =  "[fb]  firmware build%18.2f\n";
    static const char fmt_fv[] =  "[fv]  firmware version%16.2f\n";
    static const char fmt_fbs[] = "[fbs] firmware build%34s\n";
    static const char fmt_fbc[] = "[fbc] firmware config%31s\n";
    static const char fmt_hp[] =  "[hp]  hardware platform%19s\n";
    static const char fmt_hv[] =  "[hv]  hardware version%27s\n";
    static const char fmt_id[] =  "[id]  g2core ID%27s\n";

    void hw_print_fb(nvObj_t *nv)  { text_print(nv, fmt_fb);}   // TYPE_FLOAT
    void hw_print_fv(nvObj_t *nv)  { text_print(nv, fmt_fv);}   // TYPE_FLOAT
    void hw_print_fbs(nvObj_t *nv) { text_print(nv, fmt_fbs);}  // TYPE_STRING
    void hw_print_fbc(nvObj_t *nv) { text_print(nv, fmt_fbc);}  // TYPE_STRING
    void hw_print_hp(nvObj_t *nv)  { text_print(nv, fmt_hp);}   // TYPE_STRING
    void hw_print_hv(nvObj_t *nv)  { text_print(nv, fmt_hv);}   // TYPE_STRING
    void hw_print_id(nvObj_t *nv)  { text_print(nv, fmt_id);}   // TYPE_STRING

#endif //__TEXT_MODE
