#include "cypress.h"

void SystemInit(void) {
    __asm("cpsie i");
}
