// $Id$

#ifndef ZLIB_COMPRESSOR_H
#define ZLIB_COMPRESSOR_H

#include "Protocol_Task.h"

#include "CompressorBase.h"

#include <zlib.h>

class ZlibCompressor : public CompressorBase
{
public:
    ZlibCompressor();
    ~ZlibCompressor();

  // This is called when the compressor is on the
  // downstream side. We'll take the message, compress it
  // and move it along to the next module.
  int send (ACE_Message_Block *message,
            ACE_Time_Value *timeout,
            ACE_Message_Block*& compressed);

  // This one is called on the upstream side.  No surprise: we
  // decompress the data and send it on up the stream.
  int recv (ACE_Message_Block *message,
            ACE_Time_Value *timeout,
            ACE_Message_Block*& decompressed);

private:
    z_stream zstream_;
    bool isDeflateInitialised_;
};

#endif /* ZLIB_COMPRESSOR_H */
