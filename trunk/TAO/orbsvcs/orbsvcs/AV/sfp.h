/* -*- C++ -*- */
// $Id$

#if !defined (TAO_AV_SFP_H)
#define TAO_AV_SFP_H

#include "ace/SOCK_CODgram.h"
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

class SFP_Encoder
// = DESCRIPTION
//     Encodes frames for the SFP protocol
//     This class uses the CDR encoding format internally.
{
 public:
  SFP_Encoder ();
  // Default Constructor
  
  ACE_Message_Block *encode_start_message ();
  // returns a SFP start message. it is the users responsibility
  // to delete this message block after using it.

  ACE_Message_Block *encode_simple_frame (ACE_Message_Block *data);
  // Encodes the message_block in an SFP frame

  //  ACE_Message_Block *encode_stop_message ();
  // returns a SFP start message. it is the users responsibility to
  // delete this message block after using it.

  ~SFP_Encoder ();

 private:

  CDR *encoder_;
  // Use the TAO CDR encoder to encode everything
  
  CORBA::ORB_var orb_;
  // Need the ORB to initialize the CDR encoder
};

/*
class SFP_Decoder
{

  SFP_Decoder ();
  // Default constructor

  int decode_start_message (ACE_Message_Block *message);
  // returns 0 if the message is a valid SFP start message
  // returns -1 otherwise.

  SFP::message_type decode_frame (ACE_Message_Block *message);
  // decodes the message and returns the data in the message.
  // Returns the message type.
                   
  int decode_stop_message (ACE_Message_Block *message);
  // returns 0 if the message is a valid SFP start message
  // returns -1 otherwise.

  ~SFP_Decoder ();

private:
  CDR decoder_;
  // Use the TAO CDR encoder to encode everything
  
  CORBA::ORB_var orb_;
  // Need the ORB to initialize the CDR encoder
};
*/

#endif /* !defined (TAO_SFP_H) */
