// $Id$

#include "ace/Message_Block.h"
#include "orbsvcs/AV/sfp.h"

const char *buf = "Hello World\n";

main ()
{
  ACE_Message_Block *message;

  ACE_NEW_RETURN (message,
                  ACE_Message_Block (buf),
                  -1);

  SFP_Encoder encoder;

  ACE_Message_Block *start_message =
    encoder.encode_start_message ();

  for (int i = 0; i < start_message->length (); i++)
    cerr << i << (start_message->rd_ptr ())[i] << endl;

  SFP_Decoder decoder;

  decoder.decode_start_message (start_message);
  
  encoder.encode_simple_frame (message);

  return 0;
}
