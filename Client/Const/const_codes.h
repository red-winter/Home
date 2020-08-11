#ifndef CONST_CODES_H
#define CONST_CODES_H

#include <stdint.h>

#define C_HOST_NAME "localhost"
#define C_PORT_NUMBER 9000
#define C_TIME_LEN 12
#define C_TIME_TO_CONNECT 4000

const uint32_t C_UNIC_LABEL = 0xBA00AB;


enum EN_REQUEST_CODE {
  EN_DEV_NOTHING = 0,
  EN_REQ_CHECK_USER = 1,
  EN_REQ_ADD_USER = 2,
  EN_REQ_ACCESS_USER = 3,
  EN_REQ_INVALID_USER = 4,
  EN_DEV_CONNECT = 5,
  EN_DEV_DISCONECT = 6,
  EN_REQ_SEND_MSG = 7,
  EN_REQ_RECEIVE_MSG = 8,
};

enum EN_MSG_LEVEL
{
  EN_MSG_LEVEL_SYSTEM = 0,
  EN_MSG_LEVEL_USERS = 1,
  EN_MSG_LEVEL_ADMIN = 2


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
