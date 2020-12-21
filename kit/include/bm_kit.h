#ifndef _BM_KIT_H_
#define _BM_KIT_H_

#include <stdint.h>

int bm_kit_init(void);
void bm_kit_exit(void);
int64_t bm_kit_measure();

#endif //#ifndef _BM_KIT_H_