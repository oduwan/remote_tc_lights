// signals.h

#ifndef _SIGNALS_h
#define _SIGNALS_h

#include "settings.h"

struct COMMAND {
	uint8_t state[RECEIVER_QTY]; 
	uint8_t transactNum;
};

struct KEEPALIVE {
	uint8_t address;
	uint8_t state;
	uint8_t transactNum;
};



#endif

