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

#include "orbsvcs/orbsvcs_export.h"
#include "ace/SOCK_Dgram.h"
#include "orbsvcs/sfpC.h"

#define MAGIC_NUMBER_LEN 5
#define MESSAGE_TYPE_OFFSET 5
#define TAO_WRITEV_MAX 128


#define SFP_MAX_PACKET_SIZE ACE_MAX_DGRAM_SIZE
//#define SFP_MAX_PACKET_SIZE 100


// various message class for SFP.

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
  // upcall to the application to receive a frame.

  virtual void end_stream (void) = 0;
  // called when the EndofStream message is received.
};

class TAO_SFP_Fragment_Node
{
public:
  TAO_SFP_Fragment_Node (void)
    :data_ (0)
    {}
  flowProtocol::fragment fragment_info_;
  ACE_Message_Block *data_;
  friend int operator< (const TAO_SFP_Fragment_Node& left,
                        const TAO_SFP_Fragment_Node& right);
};

class TAO_SFP_Fragment_Table_Entry
{
public:
  TAO_SFP_Fragment_Table_Entry (void)
    :last_received_ (0),
     num_fragments_ (0)
    {}
  int last_received_;
  size_t num_fragments_;
  ACE_Ordered_MultiSet<TAO_SFP_Fragment_Node> fragment_set_;
};

typedef ACE_Ordered_MultiSet_Iterator<TAO_SFP_Fragment_Node> FRAGMENT_SET_ITERATOR;

class TAO_ORBSVCS_Export TAO_SFP :public virtual ACE_Event_Handler
  // = TITLE
  //     SFP implementation on UDP.
  //
  // = Description
  //     This implements the methods to send and receive data octet
  //     streams using the Simple Flow Protocol.

{
public:
  // default arguments to pass to use for the ORB
  static const char *TAO_SFP_ORB_ARGUMENTS;

  // SFP magic numbers
  static const char *TAO_SFP_MAGIC_NUMBER;
  static const char *TAO_SFP_FRAGMENT_MAGIC_NUMBER;
  static const char *TAO_SFP_START_MAGIC_NUMBER;
  static const char *TAO_SFP_CREDIT_MAGIC_NUMBER;
  static const char *TAO_SFP_STARTREPLY_MAGIC_NUMBER;

  // SFP version 1.0
  static const unsigned char TAO_SFP_MAJOR_VERSION;
  static const unsigned char TAO_SFP_MINOR_VERSION;

// lengths of various SFP headers
  static const unsigned char TAO_SFP_FRAME_HEADER_LEN;
  enum State
  {
    ACTIVE_START,
    PASSIVE_START,
    TIMEDOUT_T1,
    TIMEDOUT_T2,
    REPLY_RECEIVED,
    START_RECEIVED
  };

  TAO_SFP (CORBA::ORB_ptr orb,
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

  virtual int send_frame (ACE_Message_Block *frame);
  // sends a single frame over UDP.

  virtual int end_stream (void);
  // terminates the stream.

  virtual int handle_input (ACE_HANDLE fd);
  // Callback when event happens on the dgram socket.

  virtual int handle_timeout (const ACE_Time_Value&, const void*);
  // Used for timeout for the number of tries for starting a stream.

  virtual ACE_HANDLE get_handle (void) const;
private:

  ACE_Message_Block* read_simple_frame (void);
  // receives a single frame from the network.

  int read_frame_header (flowProtocol::frameHeader &frame_header);
  // reads the frame header from the peek buffer in the datagram.

  ACE_Message_Block* read_fragment (void);
  // reads a fragment from the wire.

  void create_local_dgram (void);
  // Create the local dgram endpoint.

  int connect_to_receiver (const char *receiver_addr);
  // Creates a connected dgram with the receiver addr.

  int send_start (void);
  // sends the start message to the receiver.

  int send_startReply (void);
  // sends the StartReply message to the receiver.

  int send_cdr_buffer (TAO_OutputCDR &cdr,
                       ACE_Message_Block *mb);
  // sends the cdr buffer using iovecs.

  int register_dgram_handler (void);
  // registers the dgram socket with the reactor.

  ACE_Message_Block *check_all_fragments (TAO_SFP_Fragment_Table_Entry *fragment_entry);
  // checks if all the fragments for this entry has been received and returns the 
  // head of the chain of message blocks for that frame.

  void dump_buf (char *buf,int n);
  // dumps the buffer to the screen.

  CORBA::ORB_ptr orb_;
  // ORB reference.

  ACE_Reactor* reactor_;
  // Used for registering the dgram handler.

  TAO_OutputCDR output_cdr_;
  // Use the TAO CDR decoder to decode everything

  //  TAO_InputCDR input_cdr_;
  // Use the TAO CDR encoder to encode everything
  
  ACE_SOCK_Dgram dgram_;
  // Connection Oriented Dgram.

  int start_tries_;
  // Number of tries to send a Start message.

  int startReply_tries_;
  // Number of tries to send a StartReply message.

  ACE_Time_Value timeout1_;
  // Timeout used for Start on Sender side and also for Credit on
  // receiver side.
  
  ACE_Time_Value timeout2_;
  // Timeout used for StartReply on the receiver side and also for
  // CREDIT on the sender side.

  State state_;
  // State variable.

  const char *receiver_addr_;
  // The address of the receiver to which we're connected to.
  
  ACE_INET_Addr receiver_inet_addr_;
  // INET addr of the receiver.

  SFP_Callback *callback_;
  // Application Callback Object.
  
  int sequence_num_;
  // sequence number of the packet.

  flowProtocol::frameHeader frame_header_;
  ssize_t frame_header_len_;
  // frame header to be sent with all frames.
  // length of the frame header.

  flowProtocol::fragment fragment_;
  ssize_t fragment_len_;
  // fragment header for each fragment.

  flowProtocol::Start start_;
  ssize_t start_len_;
  // Start message and its length.

  flowProtocol::StartReply start_reply_;
  ssize_t start_reply_len_;
  // StartReply message and its length.
  
  flowProtocol::credit credit_;
  ssize_t credit_len_;
  CORBA::ULong credit_num_;
  // Credit message and its  length.

  char magic_number_[MAGIC_NUMBER_LEN];
  // used for peeking the magic_number.
  const size_t magic_number_len_;
  u_int more_fragments_;
  // boolean flags indicating that there are more fragments.
  ACE_Hash_Map_Manager<CORBA::ULong,TAO_SFP_Fragment_Table_Entry*,ACE_Null_Mutex> fragment_table_;
  // chain of fragments of the current frame.
};

#endif /* !defined (TAO_SFP_H) */
