// -*- C++ -*-

// ============================================================================
/**
 *  @file  sfp.h
 *
 *  @author  Nagarajan Surendran <naga@cs.wustl.edu>
 */
// ============================================================================

#ifndef TAO_AV_SFP_H
#define TAO_AV_SFP_H
#include /**/ "ace/pre.h"

#include "orbsvcs/AV/AV_export.h"

#include "orbsvcs/AV/Policy.h"
#include "orbsvcs/AV/MCast.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/UDP.h"

#include "orbsvcs/sfpC.h"

#include "tao/CDR.h"

#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"


#define TAO_SFP_MAGIC_NUMBER_LEN 4
#define TAO_SFP_MESSAGE_TYPE_OFFSET 5
#define TAO_SFP_WRITEV_MAX 128

#define TAO_SFP_MAX_PACKET_SIZE ACE_MAX_DGRAM_SIZE

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_SFP_Fragment_Node
 * @brief
 */
class TAO_SFP_Fragment_Node
{
public:
  TAO_SFP_Fragment_Node (void) : data_ (0) {}
  flowProtocol::fragment fragment_info_;
  ACE_Message_Block *data_;
  friend bool operator< (const TAO_SFP_Fragment_Node& left,
                         const TAO_SFP_Fragment_Node& right);
};

/**
 * @class TAO_SFP_Fragment_Table_Entry
 * @brief
 */
class TAO_SFP_Fragment_Table_Entry
{
public:
  TAO_SFP_Fragment_Table_Entry (void)
    :last_received_ (0),
     num_fragments_ (0)
    {}
  int last_received_;
  size_t num_fragments_;
  TAO_AV_frame_info frame_info;
  ACE_Ordered_MultiSet<TAO_SFP_Fragment_Node> fragment_set_;
};

typedef ACE_Ordered_MultiSet_Iterator<TAO_SFP_Fragment_Node> FRAGMENT_SET_ITERATOR;
typedef ACE_Hash_Map_Manager<CORBA::ULong,TAO_SFP_Fragment_Table_Entry*,ACE_Null_Mutex> TAO_SFP_Fragment_Table;
typedef ACE_Hash_Map_Manager<CORBA::ULong,TAO_SFP_Fragment_Table*,ACE_Null_Mutex> TAO_SFP_Fragment_Table_Map;

/**
 * @class TAO_SFP_Frame_State
 * @brief
 */
class TAO_AV_Export TAO_SFP_Frame_State
{
public:
  TAO_SFP_Frame_State (void);
  CORBA::Boolean is_complete (void);

  int reset (void);

  TAO_InputCDR cdr;
  // This is the InputCDR that will be used to decode the message.
  flowProtocol::frameHeader frame_header_;
  flowProtocol::fragment fragment_;
  flowProtocol::frame frame_;
  CORBA::Boolean more_fragments_;
  ACE_Message_Block *frame_block_;
  // boolean flags indicating that there are more fragments.
  ACE_Message_Block static_frame_;
  TAO_SFP_Fragment_Table_Map fragment_table_map_;
};

class TAO_AV_Transport;
class TAO_AV_Core;

/**
 * @class TAO_SFP_Base
 * @brief
 */
class TAO_AV_Export TAO_SFP_Base
{
public:
  // default arguments to pass to use for the ORB
  static const char TAO_SFP_ORB_ARGUMENTS[];

  // SFP magic numbers
  static const char TAO_SFP_MAGIC_NUMBER[];
  static const char TAO_SFP_FRAGMENT_MAGIC_NUMBER[];
  static const char TAO_SFP_START_MAGIC_NUMBER[];
  static const char TAO_SFP_CREDIT_MAGIC_NUMBER[];
  static const char TAO_SFP_STARTREPLY_MAGIC_NUMBER[];

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
                         TAO_SFP_Frame_State &state,
                         TAO_AV_frame_info *&frame_info);

  static int read_fragment (TAO_AV_Transport *transport,
                            flowProtocol::fragment &fragment,
                            TAO_SFP_Frame_State &state,
                            TAO_AV_frame_info *&frame_info);

  static int peek_frame_header (TAO_AV_Transport *transport,
                                flowProtocol::frameHeader &header,
                                TAO_InputCDR &cdr);

  static int peek_fragment_header (TAO_AV_Transport *transport,
                                   flowProtocol::fragment &fragment,
                                   TAO_InputCDR &cdr);

  static int handle_input (TAO_AV_Transport *transport,
                           TAO_SFP_Frame_State &state,
                           TAO_AV_frame_info *&frame_info);

  static ACE_Message_Block* check_all_fragments (TAO_SFP_Fragment_Table_Entry *fragment_entry);

protected:
  static void dump_buf (char *buf,int n);
  // dumps the buffer to the screen.
};

/**
 * @class TAO_SFP_Object
 * @brief
 */
class TAO_AV_Export TAO_SFP_Object  : public TAO_AV_Protocol_Object
{
public:
  TAO_SFP_Object (TAO_AV_Callback *callback,
                  TAO_AV_Transport *transport);
  // We should add a sfp options parameter.

  virtual ~TAO_SFP_Object (void);
  // Dtor

  virtual int handle_input (void) = 0;
  virtual int send_frame (ACE_Message_Block *frame,
                          TAO_AV_frame_info *frame_info = 0);

  virtual int send_frame (const iovec *iov,
                          int iovcnt,
                          TAO_AV_frame_info *frame_info = 0);

  virtual int send_frame (const char*buf,
                          size_t len);

  virtual int destroy (void);
  virtual int set_policies (const TAO_AV_PolicyList &policies);

protected:
  ACE_Message_Block *get_fragment (ACE_Message_Block *&frame,
                                   size_t initial_len,
                                   size_t &last_mb_orig_len,
                                   size_t &last_mb_current_len);
  CORBA::ULong sequence_num_;
  CORBA::ULong source_id_;
  CORBA::Long max_credit_;
  CORBA::Long current_credit_;
  TAO_SFP_Frame_State state_;
};

/**
 * @class TAO_SFP_Producer_Object
 * @brief
 */
class TAO_AV_Export TAO_SFP_Producer_Object : public TAO_SFP_Object
{
public:
  TAO_SFP_Producer_Object (TAO_AV_Callback *callback,
                           TAO_AV_Transport *transport,
                           const char *flow_options);
  virtual int handle_input (void);
protected:
  CORBA::ULong credit_sequence_num_;
};

/**
 * @class TAO_SFP_Consumer_Object
 * @brief
 */
class TAO_AV_Export TAO_SFP_Consumer_Object : public TAO_SFP_Object
{
public:
  TAO_SFP_Consumer_Object (TAO_AV_Callback *callback,
                           TAO_AV_Transport *transport,
                           ACE_CString& flow_options);
  virtual int handle_input (void);
};

/**
 * @class TAO_AV_SFP_Factory
 * @brief
 */
class TAO_AV_Export TAO_AV_SFP_Factory : public TAO_AV_Flow_Protocol_Factory
{
public:
  TAO_AV_SFP_Factory (void);
  virtual ~TAO_AV_SFP_Factory (void);
  virtual int init (int argc, ACE_TCHAR *argv[]);
  // Initialization hook.
  virtual int match_protocol (const char *flow_string);
  virtual TAO_AV_Protocol_Object* make_protocol_object (TAO_FlowSpec_Entry *entry,
                                                        TAO_Base_StreamEndPoint *endpoint,
                                                        TAO_AV_Flow_Handler *handler,
                                                        TAO_AV_Transport *transport);
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_AV_SFP_Flow_Factory)
ACE_FACTORY_DECLARE (TAO_AV, TAO_AV_SFP_Flow_Factory)

#include /**/ "ace/post.h"
#endif /* TAO_SFP_H */
