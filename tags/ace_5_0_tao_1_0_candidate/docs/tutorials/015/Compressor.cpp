
// $Id$

#include "Compressor.h"
#include "ace/SOCK_Stream.h"

Compressor::Compressor( void )
        : Protocol_Task()
{
    ;
}

Compressor::~Compressor(void)
{
    ;
}

/* This is where you insert your compression code.  Most compressors
   want to work on a block of data instead of a byte-stream.
   Fortunately the message block has a block that can be compressed.
   Take a look at libz for a quick way to add compression to your
   apps
 */
int Compressor::send(ACE_Message_Block *message, ACE_Time_Value *timeout)
{
    ACE_DEBUG ((LM_INFO, "(%P|%t) Compressor::send() compressing (%s)\n", message->rd_ptr() ));

        // Create a block to hold the compressed data.  I belive libz
        // recommends a buffer about 10-20% larger than the source.
        // Other libraries/algorithms may have their own quirks.
    ACE_Message_Block * compressed = new ACE_Message_Block( message->size() );

        // Perform a bogus compression algorithm.  'CD' just tells me
        // that this is compressed data and when we "decompress" we'll 
        // look for this signature to validate the data received.
    ACE_OS::sprintf( compressed->wr_ptr(), "CD:%s", message->rd_ptr() );
    compressed->wr_ptr( strlen(compressed->wr_ptr())+1 );

        // Send the compressed data down the stream to the next module
    this->put_next( compressed );

        // We're done here.
    message->release();

    return( 0 );
}

/* And here's the decompression side.  We've written Xmit/Recv so that 
   we're guaranteed to get an entire block of compressed data.  If
   we'd used recv() in the Recv object then we might have gotten a
   partial block and that may not decompress very nicely.
 */
int Compressor::recv(ACE_Message_Block *message, ACE_Time_Value *timeout)
{
    ACE_DEBUG ((LM_INFO, "(%P|%t) Compress::recv() decompressing (%s)\n", message->rd_ptr() ));

        // Room for the decompressed data.  In the real world you
        // would probably want to send the original (uncompressed)
        // data size in the message.  You can predict the maximum
        // possible decompression size but it's cheap and easy just to 
        // send that along.  Look again at how I do exacly that
        // between Xmit and Recv.
    ACE_Message_Block * decompressed = new ACE_Message_Block( message->size() );

        // Check for our signature.  Even when you use a real
        // compression algorithm you may want to include your own
        // signature so that you can verify the block.  It pays to be
        // paranoid!
    if( ACE_OS::strncmp( message->rd_ptr(), "CD:", 3  ) )
    {
        ACE_DEBUG ((LM_INFO, "(%P|%t) Improperly encompressed data.\n" ));
        message->release();
        return(-1);
    }

        // Skip past the signature before going any further.
    message->rd_ptr( 3 );
    
        // Perform a bogus decompression algorithm.  This is where you 
        // would feed to libz or your favorite decompressor.  (It's
        // costly but you could invoke popen() on gzip!)
    ACE_OS::sprintf( decompressed->wr_ptr(), "%s", message->rd_ptr() );
    decompressed->wr_ptr( strlen(decompressed->wr_ptr())+1 );

        // Recv the decompressed data down the stream to the next module
    this->put_next( decompressed );

        // We're done here.
    message->release();

    return( 0 );
}

