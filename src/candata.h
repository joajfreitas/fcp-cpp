#ifndef __CANDATA_H__
#define __CANDATA_H__

#include <stdint.h>

typedef struct _CANdata {
	uint16_t sid;
	uint8_t dlc;
	uint16_t data[4];
} CANdata;

#endif
