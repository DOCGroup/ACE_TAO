#include "CRC.h"
#include "tao/corbafwd.h"
#include "ace/Log_Msg.h"

ACE_RCSID(Event,
          CRC,
          "$Id$")



namespace TAO_Event_CRC
{
  const CORBA::UShort SIZE = 256;

  // Some definitions
  CORBA::ULong lookuptable[SIZE];
  const unsigned int poly = 0xedb88320;

  void
  initialize (void)
  {
     static bool initialized = 0;
     CORBA::ULong r = 0, i = 0;

     if (initialized)
       return;
     else
       initialized = true;

     if (sizeof (r) != 4)
       {
         ACE_ERROR ((LM_ERROR,
                     "(%P|%t) CRC checking would only work for 32 bit machines \n"));

       }

     for (i = 0; i != SIZE ; ++i)
       {
         r = i;

         for (int j = 0; j != 8; ++j)
           {
             if (r & 0x01)
               {
                 r >>= 1;
                 r ^= poly;
               }
             else
               {
                 r >>= 1;
               }
           }
         lookuptable[i] = r;
       }
  }

  u_int
  compute_crc (iovec iov[], int iovcnt)
  {
    unsigned int crc = 0;

     crc = crc ^ 0xffffffffL;

     for (int cnt = 0; cnt != iovcnt; ++cnt)
       {
         long len = iov[cnt].iov_len;

         const char * p = (const char * )(iov[cnt].iov_base);

         if (p == 0)
           {
             continue;
           }

         if (len > 0)
           {
             while (len--)
               {
                 unsigned char i = (crc ^ *p) & 0xff;
                 crc = (crc >> 8) ^ lookuptable[i];
                 ++p;
               }
           }
       }

     return crc ^ 0xffffffffL;
  }

 }
