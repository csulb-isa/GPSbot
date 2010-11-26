#ifndef PCCOMM_H
#define PCCOMM_H

#include "main.h"
#include ".\Resources\serial\CUart.h"

#ifdef __cplusplus
 extern "C" {
#endif

// Methods that are called 
void BuildPcPacket();
void ParsePcPacket(char*);
void TransmitStatus();

#ifdef __cplusplus
}
#endif

#endif
