/* -*- C++ -*- */
// $Id$

#if !defined (TAO_AV_SFP_H)
#define TAO_AV_SFP_H

#include "orbsvcs/sfpC.h"

// default arguments to pass to use for the ORB
const char *TAO_SFP_ORB_ARGUMENTS = "-ORBobjrefstyle URL";

// SFP magic numbers
const char *TAO_SFP_MAGIC_NUMBER = "=SFP";
const char *TAO_SFP_FRAGMENT_MAGIC_NUMBER = "FRAG";
const char *TAO_SFP_START_MAGIC_NUMBER = "=STA";

// SFP version 1.0
const unsigned char TAO_SFP_MAJOR_VERSION = 1;
const unsigned char TAO_SFP_MINOR_VERSION = 0;

// lengths of various SFP headers
const unsigned char TAO_SFP_FRAME_HEADER_LEN = 12;

class SFP_Encoder
// = DESCRIPTION
//     Encodes frames for the SFP protocol
//     This class uses the CDR encoding format internally.
{
 public:
  SFP_Encoder (void);
  // Default Constructor
  
  ACE_Message_Block *encode_start_message (void);
  // returns a SFP start message. it is the users responsibility
  // to delete this message block after using it.

  ACE_Message_Block *encode_simple_frame (ACE_Message_Block *data);
  // Encodes the message_block in an SFP frame

  ACE_Message_Block *encode_start_reply_message (void);
  // returns a start-reply message. it is the users responsibility to
  // delete this message block after using it.

  ~SFP_Encoder (void);
  // Destructor

 protected:
  ACE_Message_Block *create_message_block (void);
  // Helper method - copies the buffer in encoder_
  // into a new ACE_Message_Block and returns it.
  // Returns 0 on failure

 private:

  TAO_OutputCDR *encoder_;
  // Use the TAO CDR encoder to encode everything
  
  CORBA::ORB_var orb_;
  // Need the ORB to initialize the CDR encoder
};

class SFP_Decoder
{
public:
  SFP_Decoder (void);
  // Default constructor

  int decode_start_message (ACE_Message_Block *message);
  // returns 0 if the message is a valid SFP start message
  // returns -1 otherwise.

  int decode_simple_frame (ACE_Message_Block *message);
  // decodes the message and returns the data in the message.
  // Returns the message type.
                   
  int decode_start_reply_message (ACE_Message_Block *message);
  // returns 0 if the message is a valid SFP stop message
  // returns -1 otherwise.
  
  ~SFP_Decoder (void);

protected:
  int create_cdr_buffer (char *message,
                         size_t length);
  // Helper - copies length bytes from the given message into the CDR
  // buffer. Returns 0 on success, -1 on failure

private:
  TAO_InputCDR *decoder_;
  // Use the TAO CDR encoder to encode everything
  
  CORBA::ORB_var orb_;
  // Need the ORB to initialize the CDR encoder
};

#endif /* !defined (TAO_SFP_H) */
