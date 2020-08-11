#include "ByteOperation.h"

#include <QFile>
#include <QDebug>
#include "stdio.h"
#include "ctype.h"
// ------------------------------------------------------------------------------------------------
int16_t sbo::uswap16(int16_t par_int16)
{
#ifdef D_BIGENDIAN
  return ((((par_int16) & 0xff00) >> 8) | (((par_int16) & 0x00ff) << 8));
#else
  return par_int16;
#endif
}
// ------------------------------------------------------------------------------------------------
int32_t sbo::uswap32(int32_t par_int32)
{
#ifdef D_BIGENDIAN
    return ((((par_int32) & 0xff000000) >> 24) | (((par_int32) & 0x00ff0000) >> 8) |
            (((par_int32) & 0x0000ff00) << 8)  | (((par_int32) & 0x000000ff) << 24));
#else
  return par_int32;
#endif
}
// ------------------------------------------------------------------------------------------------
uint64_t sbo::uswap64(uint64_t par_int64)
{
    par_int64 = ((par_int64 << 8) & 0xFF00FF00FF00FF00ULL ) | ((par_int64 >> 8) & 0x00FF00FF00FF00FFULL );
    par_int64 = ((par_int64 << 16) & 0xFFFF0000FFFF0000ULL ) | ((par_int64 >> 16) & 0x0000FFFF0000FFFFULL );
    return (par_int64 << 32) | (par_int64 >> 32);
}
// ------------------------------------------------------------------------------------------------
void sbo::vWriteBinFile(QString&strName,QByteArray pByteData)
{
  QFile fileBin(strName);

  if(fileBin.open(QIODevice::WriteOnly) && pByteData != nullptr)
  {
    fileBin.write(pByteData,pByteData.size());
    fileBin.flush();
    fileBin.close();
  }
  else
  {
    qDebug() << "Error 1: Can't write hex!";
  }
}
// ------------------------------------------------------------------------------------------------
uint32_t sbo::crc32(char *par_buf, uint32_t par_counter,uint32_t uiBegin)
{
  uint32_t loc_crc32 = uiBegin;

  if(par_buf != nullptr && par_counter != 0x0)
  {
    while (par_counter--)
    {
      loc_crc32 = (loc_crc32 >> 8) ^ crc_table[(loc_crc32 ^ *par_buf++) & 0xFF];
    }
    loc_crc32 = loc_crc32 ^ 0xFFFFFFFF;
  }
  else
  {
    /* NOTHING TO DO */
  }

  return loc_crc32;
}
// ------------------------------------------------------------------------------------------------
QString sbo::getStrParam(const QByteArray &brMessage, const T_TYPES type, const int32_t iLen, uint32_t &uiOffset)
{
  QString strParam("error parse");
  QByteArray byteData;

  if(type == K_INT8)
  {
    char iData[iLen];
    if(memcpy(iData,brMessage.data()+uiOffset,cmapTypeSize.value(K_INT8)*iLen) != nullptr)
    {
        byteData.append(iData,iLen);

        int32_t intData;
        intData = (int)iData[0]+48;

        if(((int)iData[0]) <= 9 && ((int)iData[0]) >= 1)
        {
          strParam = QString(intData);
        }
        else
        {
          strParam = QString(byteData);
        }
    }
    else
    {
      // NOTHING TO DO
    }

    uiOffset += sizeof(char)*iLen;
  }
  else if(type == K_UINT8)
  {
    uint8_t iData(0);
    if(memcpy(&iData,brMessage.data()+uiOffset,sizeof(uint8_t)))
    {
      strParam = QString::number(iData);
    }
    else
    {
      // NOTHING TO DO
    }

    uiOffset += sizeof(uint8_t);
  }
  else if(type == K_INT16)
  {
    int16_t iData(-1);
    if(memcpy(&iData,brMessage.data()+uiOffset,sizeof(int16_t))!= nullptr)
    {
      strParam = QString::number(sbo::uswap16(iData));

    }
    else
    {
      // NOTHING TO DO
    }

    uiOffset+= sizeof(int16_t);
  }
  else if(type == K_UINT16)
  {
    uint16_t iData(0);
    if(memcpy(&iData,brMessage.data()+uiOffset,sizeof(uint16_t))!=nullptr)
    {
      strParam = QString::number(sbo::uswap16(iData));
    }
    else
    {
      // NOTHING TO DO
    }

    uiOffset+= sizeof(uint16_t);
  }
   else if(type == K_INT32)
   {
     int32_t iData(-1);
     if(memcpy(&iData,brMessage.data()+uiOffset,sizeof(int32_t)) != nullptr)
     {
       strParam = QString::number(sbo::uswap32(iData));
     }
     else
     {
       // NOTHING TO DO
     }

     uiOffset+= sizeof(int32_t);
   }
   else if(type == K_UINT32)
   {
     uint32_t iData(0);
     if(memcpy(&iData,brMessage.data()+uiOffset,sizeof(uint32_t))!=nullptr)
     {
       strParam = QString::number(sbo::uswap32(iData));
     }
     else
     {
       // NOTHING TO DO
     }



     uiOffset+= sizeof(uint32_t);
   }
   else if(type == K_FLOAT)
   {
     float iData(0.f);
     if(memcpy(&iData,brMessage.data()+uiOffset,sizeof(float))!=nullptr)
     {
       strParam = QString::number(sbo::uswap32(iData));
     }
     else
     {
       // NOTHING TO DO
     }
      uiOffset+= sizeof(float);
   }
   else
   {
     qDebug() << "Error 78: Error parse out data!";
   }

  return strParam;
}
// ------------------------------------------------------------------------------------------------

