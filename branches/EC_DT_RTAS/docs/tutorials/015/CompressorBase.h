// $Id$

#ifndef COMPRESSOR_BASE_H
#define COMPRESSOR_BASE_H

#include "Protocol_Task.h"

#include <zlib.h>

class CompressorBase
{
public:
    CompressorBase();
    virtual ~CompressorBase() = 0;

  // This is called when the compressor is on the
  // downstream side. We'll take the message, compress it
  // and move it along to the next module.
  virtual int send (ACE_Message_Block *message,
                    ACE_Time_Value *timeout,
                    ACE_Message_Block*& compressed) = 0;

  // This one is called on the upstream side.  No surprise: we
  // decompress the data and send it on up the stream.
    virtual int recv (ACE_Message_Block *message,
                      ACE_Time_Value *timeout,
                      ACE_Message_Block*& decompressed) = 0;
};

#endif /* COMPRESSOR_BASE_H */
