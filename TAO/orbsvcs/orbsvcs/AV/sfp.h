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
#include "orbsvcs/sfpC.h"
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"

#include "Transport.h"

#define TAO_SFP_MAGIC_NUMBER_LEN 4
#define TAO_SFP_MESSAGE_TYPE_OFFSET 5
#define TAO_SFP_WRITEV_MAX 128

//#define TAO_SFP_MAX_PACKET_SIZE ACE_MAX_DGRAM_SIZE
#define TAO_SFP_MAX_PACKET_SIZE 132

class TAO_ORBSVCS_Export TAO_SFP_Callback
{
  // =TITLE
  //    Callback interface for SFP.
  //
  // =Description
  //    Application should create a callback object which they
  //    register with the SFP. The SFP implementation notifies the
  //    applicationn of any changes in the stream status like stream
  //    established, stream ended.
public:
  virtual int start_failed (void);
  // This is called for both active and passive start.

  virtual int stream_established (void);
  // This is a callback for both active and passive stream
  // establshment.

  virtual int receive_frame (ACE_Message_Block *frame);
  // upcall to the application to receive a frame.

  virtual void end_stream (void);
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

class TAO_ORBSVCS_Export TAO_SFP_Frame_State
{
public:
  TAO_SFP_Frame_State (void);
  int is_complete (void);

  int reset (void);

  TAO_InputCDR cdr;
  // This is the InputCDR that will be used to decode the message.
  flowProtocol::frameHeader frame_header_;
  flowProtocol::fragment fragment_;
  flowProtocol::frame frame_;
  CORBA::Boolean more_fragments_;
  ACE_Message_Block *frame_block_;
  // boolean flags indicating that there are more fragments.
  ACE_Hash_Map_Manager<CORBA::ULong,TAO_SFP_Fragment_Table_Entry*,ACE_Null_Mutex> fragment_table_;
};

class TAO_AV_Transport;
class TAO_AV_Core;

class TAO_ORBSVCS_Export TAO_SFP_Base
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
  static const unsigned char TAO_SFP_MESSAGE_SIZE_OFFSET;
  static const unsigned char TAO_SFP_FRAGMENT_SIZE_OFFSET;
  static u_int frame_header_len;
  static u_int start_reply_len;
  static u_int start_len;
  static u_int credit_len;
  static u_int fragment_len;

  enum State
  {
    ACTIVE_START,
    PASSIVE_START,
    TIMEDOUT_T1,
    TIMEDOUT_T2,
    REPLY_RECEIVED,
    START_RECEIVED
  };

  TAO_SFP_Base (void);
  static CORBA::Boolean start_frame (CORBA::Octet flags,
                                     flowProtocol::MsgType type,
                                     TAO_OutputCDR &msg);

  static CORBA::Boolean write_start_message (TAO_OutputCDR &msg);
  static CORBA::Boolean write_start_reply_message (TAO_OutputCDR &msg);
  static CORBA::Boolean write_credit_message (CORBA::ULong cred_num,
                                              TAO_OutputCDR &msg);
  static CORBA::Boolean write_fragment_message (CORBA::Octet flags,
                                                CORBA::ULong fragment_number,
                                                CORBA::ULong sequence_number,
                                                CORBA::ULong source_id,
                                                TAO_OutputCDR &msg);

  static CORBA::Boolean write_frame_message (CORBA::ULong timestamp,
                                             CORBA::ULong synchSource,
                                             flowProtocol::my_seq_ulong source_ids,
                                             CORBA::ULong sequence_num,
                                             TAO_OutputCDR &msg);

  static int send_message (TAO_AV_Transport *transport,
                           TAO_OutputCDR &stream,
                           ACE_Message_Block *mb = 0);
  static int peek_message_type (TAO_AV_Transport *transport,
                                flowProtocol::MsgType &type);
  static int read_start_message (TAO_AV_Transport *transport,
                                 flowProtocol::Start &start,
                                 TAO_InputCDR &cdr);
  static int read_start_reply_message (TAO_AV_Transport *transport,
                                       flowProtocol::StartReply &start_reply,
                                       TAO_InputCDR &cdr);
  static int read_credit_message (TAO_AV_Transport *transport,
                                  flowProtocol::credit &credit,
                                  TAO_InputCDR &cdr);
  static int read_endofstream_message (TAO_AV_Transport *transport,
                                       flowProtocol::frameHeader &endofstream,
                                       TAO_InputCDR &cdr);

  static int read_frame (TAO_AV_Transport *transport,
                         flowProtocol::frameHeader &frame_header,
                         TAO_SFP_Frame_State &state);

  static int read_fragment (TAO_AV_Transport *transport,
                     flowProtocol::fragment &fragment,
                     TAO_SFP_Frame_State &state);

  static int peek_frame_header (TAO_AV_Transport *transport,
                                flowProtocol::frameHeader &header,
                                TAO_InputCDR &cdr);

  static int peek_fragment_header (TAO_AV_Transport *transport,
                                   flowProtocol::fragment &fragment,
                                   TAO_InputCDR &cdr);

  static int handle_input (TAO_AV_Transport *transport,
                           TAO_SFP_Frame_State &state);

  static ACE_Message_Block* check_all_fragments (TAO_SFP_Fragment_Table_Entry *fragment_entry);

protected:
  static void dump_buf (char *buf,int n);
  // dumps the buffer to the screen.
};

typedef ACE_Singleton <TAO_SFP_Base,ACE_SYNCH_MUTEX> TAO_SFP_BASE;
class TAO_ORBSVCS_Export TAO_SFP_Object
{
public:
  virtual int start (void);
  virtual int stop (void);
  TAO_SFP_Object (TAO_AV_Transport *transport = 0);
  int send_frame (ACE_Message_Block *frame);
  int end_stream (void);
  void transport (TAO_AV_Transport *transport);
protected:
  ACE_Message_Block *get_fragment (ACE_Message_Block *&frame,
                                   size_t initial_len,
                                   size_t &last_mb_orig_len,
                                   size_t &last_mb_current_len);
  TAO_AV_Transport *transport_;
  CORBA::ULong sequence_num_;
  CORBA::ULong source_id_;
  CORBA::ULong credit_;
};

class TAO_SFP_UDP_Receiver_Handler
  :public TAO_AV_UDP_Flow_Handler
{
public:
  TAO_SFP_UDP_Receiver_Handler (TAO_SFP_Callback *callback);
  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);
  virtual int start (void);
  virtual int stop  (void);
protected:
  TAO_SFP_Callback *callback_;
  TAO_SFP_Frame_State state_;
};

class TAO_SFP_UDP_Sender_Handler
  :public TAO_AV_UDP_Flow_Handler
{
public:
  TAO_SFP_UDP_Sender_Handler (TAO_SFP_Object *sfp_object);
  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);
  virtual int start (void);
  virtual int stop  (void);
protected:
  TAO_SFP_Object *sfp_object_;
};


class TAO_ORBSVCS_Export TAO_SFP_UDP_Protocol_Factory
  :public TAO_AV_Protocol_Factory
{
public:
  TAO_SFP_UDP_Protocol_Factory (void);
  ~TAO_SFP_UDP_Protocol_Factory (void);
  
  virtual int match_protocol (TAO_AV_Core::Protocol protocol);
  // verify protocol is a match.

  virtual TAO_AV_Acceptor *make_acceptor (void);
  // create an acceptor.

  virtual TAO_AV_Connector *make_connector (void);
  // create a connector.
};

class TAO_SFP_UDP_Acceptor
  :public TAO_AV_UDP_Acceptor
{
public:
  TAO_SFP_UDP_Acceptor (void);
  ~TAO_SFP_UDP_Acceptor (void);
  int make_svc_handler (TAO_AV_UDP_Flow_Handler *&handler);
};

class TAO_SFP_UDP_Connector
  :public TAO_AV_UDP_Connector
{
public:
  TAO_SFP_UDP_Connector (void);
  ~TAO_SFP_UDP_Connector (void);
  int make_svc_handler (TAO_AV_UDP_Flow_Handler *&handler);
};

#endif /* !defined (TAO_SFP_H) */
