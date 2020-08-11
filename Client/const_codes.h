#ifndef CONST_CODES_H
#define CONST_CODES_H

#include <stdint.h>

#define C_HOST_NAME "localhost"
#define C_PORT_NUMBER 9000
#define C_TIME_LEN 12

const uint32_t C_UNIC_LABEL = 0xBA00AB;

enum EN_RET_CODE {
  EN_DEV_NOTHING = 0,
  EN_DEV_CONNECT = 1,
  EN_DEV_DISCONECT = 2,
  EN_DEV_USER_SEND = 3,
  EN_DEV_USER_RECEIVE = 4,
};

struct ST_PROTOCOL_HEADER {
  ST_PROTOCOL_HEADER() :
    uiUnicLabel(C_UNIC_LABEL),
    uiDataSize(0),
    uiReqCode(EN_DEV_NOTHING)
  {
    for(uint32_t uiCounter(0);uiCounter < C_TIME_LEN;uiCounter++)
    {
       pTime[uiCounter] = 0x0;
    }
  }

  uint32_t uiUnicLabel;
  uint32_t uiDataSize;
  uint32_t uiReqCode;
  char pTime[C_TIME_LEN];
};


#endif // CONST_CODES_H
