// $Id$

#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "Protocol_Task.h"

#include "CompressorBase.h"

class Compressor : public Protocol_Task
{
public:
    enum AlgorithmCode
    {
        COMPRESSION_NONE   = 0
      , COMPRESSION_ZLIB   = 1
      , COMPRESSION_RLE    = 2 // for future expansion
      , COMPRESSION_BZIP2  = 3 // for future expansion
    };

    Compressor(AlgorithmCode algorithm);

    virtual ~Compressor (void);

protected:

    // This is called when the compressor is on the
    // downstream side. We'll take the message, compress it
    // and move it along to the next module.
    virtual int send (ACE_Message_Block *message, ACE_Time_Value *timeout);

    // This one is called on the upstream side.  No surprise: we
    // decompress the data and send it on up the stream.
    virtual int recv (ACE_Message_Block *message, ACE_Time_Value *timeout);

private:
    CompressorBase* compressor_;
};

#endif /* COMPRESSOR_H */
