/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    AVStreams.
//
// = FILENAME
//   sfp.h
//
// = AUTHOR
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
// ============================================================================

#if !defined (TAO_AV_SFP_H)
#define TAO_AV_SFP_H

#include "ace/SOCK_Dgram.h"
#include "orbsvcs/sfpC.h"

// default arguments to pass to use for the ORB
const char *TAO_SFP_ORB_ARGUMENTS = "-ORBobjrefstyle URL";

// SFP magic numbers
const char *TAO_SFP_MAGIC_NUMBER = "=SFP";
const char *TAO_SFP_FRAGMENT_MAGIC_NUMBER = "FRAG";
const char *TAO_SFP_START_MAGIC_NUMBER = "=STA";
const char *TAO_SFP_CREDIT_MAGIC_NUMBER = "=CRE";
const char *TAO_SFP_STARTREPLY_MAGIC_NUMBER = "=STR";

// SFP version 1.0
const unsigned char TAO_SFP_MAJOR_VERSION = 1;
const unsigned char TAO_SFP_MINOR_VERSION = 0;

// lengths of various SFP headers
const unsigned char TAO_SFP_FRAME_HEADER_LEN = 12;

class TAO_ORBSVCS_Export SFP_Callback
  // =TITLE
  //    Callback interface for SFP.
  //
  // =Description
  //    Application should create a callback object which they
  //    register with the SFP. The SFP implementation notifies the
  //    applicationn of any changes in the stream status like stream
  //    established, stream ended.
{
public:
  virtual int start_failed (void) = 0;
  // This is called for both active and passive start.
  
  virtual int stream_established (void) = 0;
  // This is a callback for both active and passive stream
  // establshment.
  
  virtual int receive_frame (ACE_Message_Block *frame) =0;
};

class TAO_ORBSVCS_Export SFP :public virtual ACE_Event_Handler
  // = TITLE
  //     SFP implementation on UDP.
  //
  // = Description
  //     This implements the methods to send and receive data octet
  //     streams using the Simple Flow Protocol.

{
public:
  enum State
  {
    ACTIVE_START,
    PASSIVE_START,
    TIMEDOUT_T1,
    TIMEDOUT_T2,
    REPLY_RECEIVED,
    START_RECEIVED
  };

  SFP (CORBA::ORB_ptr orb,
       ACE_Reactor* reactor,
       ACE_Time_Value timeout1,
       ACE_Time_Value timeout2,
       SFP_Callback *callback);
  // constructor.

  virtual int start_stream (const char *receiver_addr);
  // Actively start the stream by trying to connect to the UDP
  // receiver_addr in host:port format.

  virtual int start_stream (const char *local_addr,int credit_);
  // Passive start.

  virtual int send_simple_frame (ACE_Message_Block *frame);
  // sends a single frame over UDP.

  virtual int send_frame (ACE_Message_Block *frame);
  // This will send a larger frame fragmenting if necessary.

  virtual ACE_Message_Block* read_simple_frame (void);
  // receives a single frame from the network.

  virtual int end_stream (void);
  // terminates the stream.

  virtual int handle_input (ACE_HANDLE fd);
  // Callback when event happens on the dgram socket.

  virtual int handle_timeout (const ACE_Time_Value&, const void*);
  // Used for timeout for the number of tries for starting a stream.

  virtual ACE_HANDLE get_handle (void) const;
private:

  int create_cdr_buffer (char *message,
                         size_t length);
  // Helper - copies length bytes from the given message into the CDR
  // buffer. Returns 0 on success, -1 on failure

  ////  ACE_Message_Block *create_message_block (void);
  void set_cdr_length (void);
  // Helper method - copies the buffer in encoder_
  // into a new ACE_Message_Block and returns it.
  // Returns 0 on failure

  void create_local_dgram (void);
  // Create the local dgram endpoint.

  int connect_to_receiver (const char *receiver_addr);
  // Creates a connected dgram with the receiver addr.

  int send_start (void);
  // sends the start message to the receiver.

  int send_startReply (void);
  // sends the StartReply message to the receiver.

  int send_cdr_buffer (void);
  // sends the encoders cdr buffer using iovecs.

  char *magic_number_to_string (char *magic_number);
  // appends a 0 to the end of the magic number.

  int register_dgram_handler (void);
  // registers the dgram socket with the reactor.

  CORBA::ORB_ptr orb_;
  // ORB reference.

  ACE_Reactor* reactor_;
  // Used for registering the dgram handler.

  TAO_OutputCDR *encoder_;
  // Use the TAO CDR encoder to encode everything

  TAO_InputCDR *decoder_;
  // Use the TAO CDR encoder to encode everything
  
  ACE_SOCK_Dgram dgram_;
  // Connection Oriented Dgram.

  int start_tries_;
  // Number of tries to send a Start message.

  int startReply_tries_;
  // Number of tries to send a StartReply message.

  CORBA::ULong credit_;
  // Credit on the number of frames.

  ACE_Time_Value timeout1_;
  // Timeout used for Start on Sender side and also for Credit on
  // receiver side.
  
  ACE_Time_Value timeout2_;
  // Timeout used for StartReply on the receiver side and also for
  // CREDIT on the sender side.

  State state_;
  // State variable.
  // @@We can use the state pattern here.

  const char *receiver_addr_;
  // The address of the receiver to which we're connected to.
  
  ACE_INET_Addr receiver_inet_addr_;
  // INET addr of the receiver.

  SFP_Callback *callback_;
  // Application Callback Object.
  
  int sequence_num_;
  // sequence number of the packet.
};

#endif /* !defined (TAO_SFP_H) */
