#ifndef BYTEOPERATION_H
#define BYTEOPERATION_H

#include "crc32.h"

#include <QString>

namespace sbo
{

  static int16_t uswap16(int16_t par_int16);
  static int32_t uswap32(int32_t par_int32);
  static uint64_t uswap64(uint64_t par_int64);
  static uint32_t crc32(char *par_buf, uint32_t par_counter, uint32_t uiBegin = 0xFFFFFFFF);

  /* Get param from hex message*/
  static QString getStrParam(const QByteArray &brMessage, const T_TYPES type, const int32_t iLen, uint32_t &uiOffset);

  /* Get value to string from byte array */
  static QString strGetValueFromByte(int8_t *pData, const T_TYPES &enType, const int32_t iStringLen, int32_t &iOffset);

  /* Create new file */
  static void vWriteBinFile(QString &strName, QByteArray pByteData);

}



#endif // BYTEOPERATION_H
