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
  // new the encoder which we will use
  ACE_NEW (encoder_,
           TAO_OutputCDR ());
  
}

ACE_Message_Block *
SFP_Encoder::encode_start_message ()
{
  this->encoder_->reset ();
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
  
  return this->create_message_block ();
}

// Encodes a simple frame, i.e. just frameheader and data
ACE_Message_Block *
SFP_Encoder::encode_simple_frame (ACE_Message_Block *data)
{
  this->encoder_->reset ();
  TAO_TRY
    {
      // construct the frame header
      SFP::frame_header frame_header;
      // copy the magic number into the message
      frame_header.magic_number = 4;
      
      // flags field is all zeroes
      frame_header.flags = 0;
      
      // put the message type 
      frame_header.message_type = SFP::SIMPLEFRAME;

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
                      "(%P|%t) encode of simple frameheader succeeded:"
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

  ACE_Message_Block *mb = this->create_message_block ();

  // set the next pointer to point to the data
  mb->next (data);

  return mb;
}

ACE_Message_Block *
SFP_Encoder::encode_start_reply_message ()
{
  // Reset the internal buffer of the CDR stream
  this->encoder_->reset ();

  TAO_TRY
    {
      // construct the stop message
      SFP::start_reply start_reply_message;
      

      // flags field is all zeroes
      start_reply_message.flags = 0;
      
      // encode the start frame
      if (encoder_->encode (SFP::_tc_start_reply,
                            &start_reply_message,
                            0,
                            TAO_TRY_ENV) 
          == CORBA_TypeCode::TRAVERSE_CONTINUE)
        {
          ACE_DEBUG ((LM_DEBUG, 
                      "(%P|%t) encode of start_reply message succeeded:"
                      "length == %d\n",
                      encoder_->length ()));

        }
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SFP_Encoder::"
                                   "encode_start_reply_message");
      return 0;
    }
  TAO_ENDTRY;
  
  return this->create_message_block ();
}

SFP_Encoder::~SFP_Encoder ()
{

}

// This method copies the CDR buffer into a new
// ACE_Message_Block and returns it.
ACE_Message_Block *
SFP_Encoder::create_message_block (void)
{
  // put the cdr encoded buffer into the message block
  ACE_Message_Block *message;
  
  ACE_NEW_RETURN (message, 
                  ACE_Message_Block (encoder_->length ()),
                  0);
  
  if (message->copy ((char *) encoder_->buffer (),
                     (size_t) encoder_->length ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "SFP_Encoder: Could not copy"
                       "CDR::buffer () into "
                       "ACE_Message_Block"),
                      0);
  
  return message;
}


// ----------------------------------------------------------------------

SFP_Decoder::SFP_Decoder ()
  : decoder_ (0)
{
}

// Attempts to decode the message as an SFP start message
// returns 0 on success, -1 on failure
int
SFP_Decoder::decode_start_message (ACE_Message_Block *message)
{

  SFP::start_message start;

  TAO_TRY
    {
      this->create_cdr_buffer (message->rd_ptr (),
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

// Attempts to decode the message as an SFP simple frame.
// On success, message->rd_ptr () points to the data
// Returns 0 on success, -1 on failure
int
SFP_Decoder::decode_simple_frame (ACE_Message_Block *message)
{

  // see if the message is long enough, if not, fail
  if (message->length () < TAO_SFP_FRAME_HEADER_LEN)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) SFP_Decoder::decode_simple_frame: "
                       "Message too small to be a valid header!"),
                      -1);

  // create an empty header
  SFP::frame_header header;

  TAO_TRY
    {
      // create an internal CDR buffer
      this->create_cdr_buffer (message->rd_ptr (),
                               TAO_SFP_FRAME_HEADER_LEN);

      // decode the frame.
      decoder_->decode (SFP::_tc_frame_header,
                        &header,
                        0,
                        TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SFP_Decoder::"
                                   "decode_frame_header");
      return -1;
    }
  TAO_ENDTRY;

  ACE_DEBUG ((LM_DEBUG, 
	      "Decoded frame header, "
	      "magic number == %d "
              "message length == %d",
	      header.magic_number,
	      header.message_size));

  // Make the message read pointer point to the data
  message->rd_ptr (message->rd_ptr () + 
                   TAO_SFP_FRAME_HEADER_LEN);

  return 0;

}

// Attempts to decode the message as an SFP start message
// returns 0 on success, -1 on failure
int
SFP_Decoder::decode_start_reply_message (ACE_Message_Block *message)
{

  SFP::start_reply start_reply_message;

  TAO_TRY
    {
      this->create_cdr_buffer (message->rd_ptr (),
                               message->length ());
      
      decoder_->decode (SFP::_tc_start_reply,
                       &start_reply_message,
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
	      "Decoded start_reply message\n"));

  return 0;
}


// Copies length bytes from the given message into the
// CDR buffer. Returns 0 on success, -1 on failure
int
SFP_Decoder::create_cdr_buffer (char *message,
                                size_t length)
{
  if (this->decoder_)
    delete this->decoder_;

  ACE_NEW_RETURN (this->decoder_,
                  TAO_InputCDR (message,
                                length),
                  -1);
  
  ACE_OS::memcpy (this->decoder_->rd_ptr (), 
                  message, 
                  length);
  
  return 0;
}
  


SFP_Decoder::~SFP_Decoder ()
{
  if (this->decoder_)
    delete this->decoder_;
}
