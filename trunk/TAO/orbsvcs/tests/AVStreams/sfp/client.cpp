// $Id$

#include "ace/Message_Block.h"
#include "orbsvcs/AV/sfp.h"

ACE_RCSID(sfp, client, "$Id$")

const char *buf = "Hello Worldasdasda sda sdasd adasdaddd\n";

main ()
{
  
  // ----------------------------------------------------------------------
  // start message

  SFP_Encoder encoder;

  ACE_Message_Block *start_message =
    encoder.encode_start_message ();

  SFP_Decoder decoder;

  decoder.decode_start_message (start_message);
  
  // ----------------------------------------------------------------------
  // start reply

  ACE_Message_Block *start_reply_message =
    encoder.encode_start_reply_message ();

  decoder.decode_start_reply_message (start_reply_message);
  

  // ----------------------------------------------------------------------
  // simple frame

  ACE_Message_Block *message;

  ACE_NEW_RETURN (message,
                  ACE_Message_Block (buf),
                  -1);

  message->length (::strlen (buf));

  ACE_Message_Block *mb = encoder.encode_simple_frame (message);

  decoder.decode_simple_frame (mb);

  return 0;
}
