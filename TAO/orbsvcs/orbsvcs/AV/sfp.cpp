#include "orbsvcs/AV/sfp.h"
#include "ace/ARGV.h"

// $Id$

// constructor.
SFP_Encoder::SFP_Encoder ()
  : orb_ (0),
    encoder_ (0)
{
  // We need to initialize the ORB!

  TAO_TRY
    {
      ACE_ARGV orb_args (TAO_SFP_ORB_ARGUMENTS);
      int argc = orb_args.argc ();
      this->orb_ = CORBA::ORB_init (argc,
                                    orb_args.argv (),
                                    0,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SFP_Encoder::ORB_init");
      return;
    }
  TAO_ENDTRY;
  ACE_NEW (encoder_,
           CDR ());
  
}

ACE_Message_Block *
SFP_Encoder::encode_start_message ()
{
  TAO_TRY
    {
      // construct the start message
      SFP::start_message start;

      // copy the magic number into the message
      start.magic_number = 4;
      
      // put the version number into the field
      start.major_version = TAO_SFP_MAJOR_VERSION;
      start.minor_version = TAO_SFP_MINOR_VERSION;
      
      // flags field is all zeroes
      start.flags = 0;
      
      // encode the start frame
      if (encoder_->encode (SFP::_tc_start_message,
                           &start,
                           0,
                           TAO_TRY_ENV) 
          == CORBA_TypeCode::TRAVERSE_CONTINUE)
        {
          ACE_DEBUG ((LM_DEBUG, 
                      "(%P|%t) encode of start message succeeded:"
                      "length == %d\n",
                      encoder_->length ()));

        }
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SFP_Source_Endpoint::"
                                   "start_message");
      return 0;
    }
  TAO_ENDTRY;

  // put the cdr encoded buffer into the message block
  ACE_Message_Block *message;
  
  ACE_NEW_RETURN (message, 
                  ACE_Message_Block (encoder_->length ()),
                  0);
  
  if (message->copy ((char *) encoder_->buffer (),
                     encoder_->length ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "SFP_Encoder: Could not copy"
                       " start message into ACE_Message_Block"),
                      0);

  return message;
  
}

ACE_Message_Block *
SFP_Encoder::encode_simple_frame (ACE_Message_Block *data)
{
  TAO_TRY
    {
      // construct the frame header
      SFP::frame_header frame_header;
      // copy the magic number into the message
      frame_header.magic_number.length (4);
      for (int i = 0; i < 4; i++)
        frame_header.magic_number [i] = TAO_SFP_MAGIC_NUMBER [i];
      
      // flags field is all zeroes
      frame_header.flags = 0;
      
      // put the message type 
      frame_header.message_type = 2;

      // size
      frame_header.message_size = data->length ();
      
      // encode the frame
      if (encoder_->encode (SFP::_tc_frame_header,
                           &frame_header,
                           0,
                           TAO_TRY_ENV) 
          == CORBA_TypeCode::TRAVERSE_CONTINUE)
        {
          ACE_DEBUG ((LM_DEBUG, 
                      "(%P|%t) encode of simple frame succeeded:"
                      "length == %d\n",
                      encoder_->length ()));

        }
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SFP_Encoder::"
                                   "start_message");
      return 0;
    }
  TAO_ENDTRY;

  // put the cdr encoded buffer into the message block
  ACE_Message_Block *message;
  
  ACE_NEW_RETURN (message, 
                  ACE_Message_Block (encoder_->length ()),
                  0);
  
  if (message->copy ((char *) encoder_->buffer (),
                     (size_t) encoder_->length ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "SFP_Encoder: Could not copy"
                       " header for simpleframe into "
                       "ACE_Message_Block"),
                      0);

  // set the next pointer to point to the data
  message->next (data);

  return message;
}

SFP_Encoder::~SFP_Encoder ()
{

}

SFP_Decoder::SFP_Decoder ()
{
  ACE_NEW (decoder_,
           CDR ());
  
}

int
SFP_Decoder::decode_start_message (ACE_Message_Block *message)
{

  SFP::start_message start;

  TAO_TRY
    {
      //      decoder_->setup_encapsulation (message->rd_ptr (),
      //                                     message->length ());
      decoder_->grow (message->length ());

      char *bufptr = decoder_->buffer ();

      ACE_OS::memcpy (bufptr, 
                      message->rd_ptr (), 
                      message->length ());

      decoder_->decode (SFP::_tc_start_message,
			&start,
			0,
			TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SFP_Decoder::"
                                   "decode_start_message");
      return -1;
    }
  TAO_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, 
	      "Decoded start message,"
	      "version number == %d . %d\n",
	      start.major_version,
	      start.minor_version));

  return 0;
}

SFP_Decoder::~SFP_Decoder ()
{

}
