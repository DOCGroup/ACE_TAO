
// $Id$

#include "Crypt.h"
#include "ace/SOCK_Stream.h"

Crypt::Crypt( int _thr_count )
        : inherited(_thr_count)
{
}

Crypt::~Crypt(void)
{
}

int Crypt::send(ACE_Message_Block *message, ACE_Time_Value *timeout)
{
    ACE_DEBUG ((LM_INFO, "(%P|%t) Crypt::send() encrypting (%s)\n", message->rd_ptr() ));

    ACE_Message_Block * encrypted = new ACE_Message_Block( message->size() );

        // Perform a bogus encryption algorithm
    ACE_OS::sprintf( encrypted->wr_ptr(), "ED:%s", message->rd_ptr() );
    encrypted->wr_ptr( strlen(encrypted->wr_ptr())+1 );

        // Send the encrypted data down the stream to the next module
    this->put_next( encrypted );

        // We're done here.
    message->release();

    return( 0 );
}

int Crypt::recv(ACE_Message_Block *message, ACE_Time_Value *timeout)
{
    ACE_DEBUG ((LM_INFO, "(%P|%t) Crypt::recv() decrypting (%s)\n", message->rd_ptr() ));

    ACE_Message_Block * decrypted = new ACE_Message_Block( message->size() );

    if( ACE_OS::strncmp( message->rd_ptr(), "ED:", 3  ) )
    {
        ACE_DEBUG ((LM_INFO, "(%P|%t) Improperly encrypted data.\n" ));
        message->release();
        return(-1);
    }

    message->rd_ptr( 3 );
    
        // Perform a bogus decryption algorithm
    ACE_OS::sprintf( decrypted->wr_ptr(), "%s", message->rd_ptr() );
    decrypted->wr_ptr( strlen(decrypted->wr_ptr())+1 );

        // Send the decrypted data down the stream to the next module
    this->put_next( decrypted );

        // We're done here.
    message->release();

    return( 0 );
}
