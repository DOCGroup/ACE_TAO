
// $Id$

#include "Crypt.h"
#include "ace/SOCK_Stream.h"

/* The expected constructor...
 */
Crypt::Crypt( void )
        : Protocol_Task()
{
}

Crypt::~Crypt(void)
{
}

/* To send the data we'll apply a signature and encryption.
 */
int Crypt::send(ACE_Message_Block *message, ACE_Time_Value *timeout)
{
    ACE_UNUSED_ARG(timeout);

    ACE_DEBUG ((LM_INFO, "(%P|%t) Crypt::send() encrypting (%s)\n", message->rd_ptr() ));

        // I suspect that some encryptors might change the data size.
        // It probably isn't safe to create a same-size destination buffer.
    ACE_Message_Block * encrypted = new ACE_Message_Block(
        message->size() +16 );

        // Perform a bogus encryption algorithm and add our safety
        // signature.  Adding the original data size is also probably
        // a good idea that I haven't encorporated here.
    ACE_OS::sprintf( encrypted->wr_ptr(), "ED:%s", message->rd_ptr() );
    encrypted->wr_ptr( strlen(encrypted->wr_ptr())+1 );

        // Send the encrypted data down the stream to the next module
    this->put_next( encrypted );

        // We're done here.
    message->release();

    return( 0 );
}

/* The upstream movement requires that we decrypt what the peer has
   given us.
*/
int Crypt::recv(ACE_Message_Block *message, ACE_Time_Value *timeout)
{
    ACE_UNUSED_ARG(timeout);

    ACE_DEBUG ((LM_INFO, "(%P|%t) Crypt::recv() decrypting (%s)\n", message->rd_ptr() ));

        // Create a destination for the decrypted data.  The same
        // block size caveat exists of course.
    ACE_Message_Block * decrypted = new ACE_Message_Block(
        message->size() +16 );

        // Check the signature as expected.
    if( ACE_OS::strncmp( message->rd_ptr(), "ED:", 3  ) )
    {
        ACE_DEBUG ((LM_INFO, "(%P|%t) Improperly encrypted data.\n" ));
        message->release();
        return(-1);
    }

        // Don't forget to skip past the signature before decrypting
        // or things will be quite exciting!
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
