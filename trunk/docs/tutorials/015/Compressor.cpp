
// $Id$

#include "Compressor.h"
#include "ace/SOCK_Stream.h"

Compressor::Compressor( int _thr_count )
        : inherited(_thr_count)
{
}

Compressor::~Compressor(void)
{
}

int Compressor::send(ACE_Message_Block *message, ACE_Time_Value *timeout)
{
    ACE_DEBUG ((LM_INFO, "(%P|%t) Compressor::send() compressing (%s)\n", message->rd_ptr() ));

    ACE_Message_Block * compressed = new ACE_Message_Block( message->size() );

        // Perform a bogus compression algorithm
    ACE_OS::sprintf( compressed->wr_ptr(), "CD:%s", message->rd_ptr() );
    compressed->wr_ptr( strlen(compressed->wr_ptr())+1 );

        // Send the compressed data down the stream to the next module
    this->put_next( compressed );

        // We're done here.
    message->release();

    return( 0 );
}

int Compressor::recv(ACE_Message_Block *message, ACE_Time_Value *timeout)
{
    ACE_DEBUG ((LM_INFO, "(%P|%t) Compress::recv() decompressing (%s)\n", message->rd_ptr() ));

    ACE_Message_Block * decompressed = new ACE_Message_Block( message->size() );

    if( ACE_OS::strncmp( message->rd_ptr(), "CD:", 3  ) )
    {
        ACE_DEBUG ((LM_INFO, "(%P|%t) Improperly encompressed data.\n" ));
        message->release();
        return(-1);
    }

    message->rd_ptr( 3 );
    
        // Perform a bogus decompression algorithm
    ACE_OS::sprintf( decompressed->wr_ptr(), "%s", message->rd_ptr() );
    decompressed->wr_ptr( strlen(decompressed->wr_ptr())+1 );

        // Recv the decompressed data down the stream to the next module
    this->put_next( decompressed );

        // We're done here.
    message->release();

    return( 0 );
}

