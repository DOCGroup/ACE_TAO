// $Id$

// ZlibCompressor by Andrew Marlow.
// This class is used to compress data sent via ACE sockets.
// It uses the ACE_Stream ideas in chapter 9 of C++ Network Programming Vol 2.

#include "ZlibCompressor.h"
#include "ace/SOCK_Stream.h"

#include <zlib.h>

ZlibCompressor::ZlibCompressor()
    : isDeflateInitialised_(false)
{
}

ZlibCompressor::~ZlibCompressor(void)
{
    deflateEnd(&zstream_);
}

int ZlibCompressor::send(ACE_Message_Block *message, 
                         ACE_Time_Value *timeout,
                         ACE_Message_Block*& compressed)
{
    ACE_UNUSED_ARG(message);
    ACE_UNUSED_ARG(timeout);

    ACE_DEBUG ((LM_INFO, "(%P|%t) ZlibCompressor::send() compressing (%s)\n", message->rd_ptr() ));

    // Create a block to hold the compressed data.  
    // I believe libz recommends a buffer about 10-20% larger
    // than the source.
    // Other libraries/algorithms may have their own quirks.

    int err;
    int mlen = strlen(message->rd_ptr())+1;
    zstream_.next_in   = (Bytef*)message->rd_ptr();
    zstream_.avail_in  = (uInt) mlen;
    zstream_.avail_out = (uInt) 5 * message->size();
    compressed = new ACE_Message_Block(zstream_.avail_out);

    zstream_.next_out  = (Bytef*)compressed->wr_ptr();

    if (isDeflateInitialised_)
    {
        err = deflateReset(&zstream_);
        if (err != Z_OK)
        {
            return -1;
        }
    }
    else
    {
        zstream_.zalloc = (alloc_func)0;
        zstream_.zfree = (free_func)0;
        zstream_.opaque = (voidpf)0;

        err = deflateInit_(&zstream_, Z_BEST_SPEED,
                           ZLIB_VERSION, sizeof(zstream_));
        if (err != Z_OK)
        {
            return -1;
        }

        isDeflateInitialised_ = true;
    }

    // Perform the compression.

    err = deflate(&zstream_, Z_FINISH);
    if (err != Z_STREAM_END)
    {
        return (err == Z_OK) ? Z_BUF_ERROR : err;
    }

    compressed->wr_ptr(zstream_.total_out);

    return 0;
}

/* And here's the decompression side.  
   We've written Xmit/Recv so that we're guaranteed to get
   an entire block of compressed data. If we'd used recv() in
   the Recv object then we might have gotten a partial block
   and that may not decompress very nicely.
 */
int ZlibCompressor::recv(ACE_Message_Block *message, 
                         ACE_Time_Value *timeout,
                         ACE_Message_Block*& decompressed)
{
    ACE_UNUSED_ARG(message);
    ACE_UNUSED_ARG(timeout);

    ACE_DEBUG ((LM_INFO, "(%P|%t) Compress::recv() decompressing.\n" ));

    // Room for the decompressed data.  In the real world you
    // would probably want to send the original (uncompressed)
    // data size in the message.  You can predict the maximum
    // possible decompression size but it's cheap and easy 
    // just to send that along.  Look again at how I do
    // exacly that between Xmit and Recv.

    int err;

    zstream_.avail_out = 5 * message->size();
    decompressed = new ACE_Message_Block(zstream_.avail_out);

    zstream_.next_in = (Bytef*)message->rd_ptr();
    zstream_.avail_in = message->size();
    zstream_.next_out = (Bytef*)decompressed->wr_ptr();
    zstream_.zalloc = (alloc_func)0;
    zstream_.zfree = (free_func)0;

    err = inflateInit(&zstream_);
    if (err != Z_OK)
    {
        return -1;
    }

    err = inflate(&zstream_, Z_FINISH);
    if (err != Z_STREAM_END)
    {
        inflateEnd(&zstream_);
        return -1;
    }

    decompressed->wr_ptr(zstream_.total_out);

    return 0;
}
