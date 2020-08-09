#ifndef SDLC_H_ONCE
#define SDLC_H_ONCE

namespace PSOC {
    extern Motate::SysTickEvent sdlc_poll;
}

stat_t sd_get_sdlc(nvObj_t *nv);
stat_t sd_set_sdlc(nvObj_t *nv);

#endif // End of include guard: SDLC_H_ONCE

