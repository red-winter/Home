#ifndef CONST_USERS_H
#define CONST_USERS_H

#include <stdint.h>

#include <QString>

#define C_LEN_USER_PARAM 12

typedef std::pair<QString,uint32_t> T_HOST_CONF;

enum EN_ACCESS
{
  EN_VALID = 0,
  EN_INVALID = -1
};

enum EN_GROUP
{
  EN_GR_ADMIN = 1,
  EN_GR_OTHERS = 2
};

enum EN_SEX_USER
{
  EN_SEX_MALE,
  EN_SEX_FAMIN,
  EN_SEX_OTHERS,
};


struct USER_INFO {
  USER_INFO(QString stLogin,QString stPass,EN_SEX_USER uSex = EN_SEX_MALE):
    en8Sex(uSex),
    enGroup(EN_GR_OTHERS)
    {

    memset(strLogin,0x0,C_LEN_USER_PARAM);
    memset(strPass,0x0,C_LEN_USER_PARAM);

    memcpy(strLogin,stLogin.toUtf8().data(),C_LEN_USER_PARAM);
    memcpy(strPass,stPass.toUtf8().data(),C_LEN_USER_PARAM);
  }

  USER_INFO():
    en8Sex(EN_SEX_OTHERS){


    memset(strLogin,0x0,C_LEN_USER_PARAM);
    memset(strPass,0x0,C_LEN_USER_PARAM);
  }

  int8_t strLogin[C_LEN_USER_PARAM];
  int8_t strPass[C_LEN_USER_PARAM];
  EN_SEX_USER en8Sex;
  EN_GROUP enGroup;
};

struct USER_AUTHOR {
  USER_AUTHOR(uint32_t uiIndex,EN_ACCESS enAcs):
    uiIndentifier(uiIndex),
    enAccess(enAcs){

  }

  USER_AUTHOR():
    uiIndentifier(0),
    enAccess(EN_INVALID),
    bEntering(false){

  }

  uint32_t uiIndentifier;
  EN_ACCESS enAccess;
  bool bEntering;
};

struct USER{
  USER_AUTHOR stAutor;
  USER_INFO stUserInfo;
};

#endif // CONST_USERS_H
