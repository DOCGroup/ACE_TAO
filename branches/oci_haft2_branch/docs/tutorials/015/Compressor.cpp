// $Id$

#include "ZlibCompressor.h"
#include "Compressor.h"
#include "ace/SOCK_Stream.h"

#include <zlib.h>

Compressor::Compressor(AlgorithmCode algorithm)
{
    switch(algorithm)
    {
    case COMPRESSION_ZLIB:
        compressor_ = new ZlibCompressor();
        break;

    default:
        compressor_ = 0;
        break;
    }
}

Compressor::~Compressor(void)
{
    delete compressor_;
}

int Compressor::send (ACE_Message_Block *message, ACE_Time_Value *timeout)
{
    if (compressor_ == 0)
        return -1;

    ACE_Message_Block* compressed = 0;
    int retval = compressor_->send(message, timeout, compressed);

    // Send the compressed data down the stream to the next module
    this->put_next( compressed );

    // We're done here.
    message->release();

    return retval;
}

int Compressor::recv (ACE_Message_Block *message, ACE_Time_Value *timeout)
{
    if (compressor_ == 0)
        return -1;

    ACE_Message_Block* decompressed = 0;
    int retval = compressor_->recv(message, timeout, decompressed);

    // Recv the decompressed data down the stream to the next module
    this->put_next( decompressed );

    // We're done here.
    message->release();

    return retval;
}
