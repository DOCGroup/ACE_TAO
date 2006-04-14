// -*- C++ -*-
//
// ===================================================================
/**
 *  @file GIOP_Message_Base.h
 *
 *  $Id$
 *
 *  @author Initially Copyrighted by Sun Microsystems Inc., 1994-1995,
 *  @author modified by Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_GIOP_MESSAGE_BASE_H
#define TAO_GIOP_MESSAGE_BASE_H

#include /**/ "ace/pre.h"

#include "tao/Pluggable_Messaging.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/GIOP_Message_Generator_Parser_Impl.h"
#include "tao/GIOP_Utils.h"
#include "tao/GIOP_Message_State.h"
#include "tao/CDR.h"

class TAO_Pluggable_Reply_Params;
class TAO_Queued_Data;

/**
 * @class TAO_GIOP_Message_Base
 *
 * @brief Definitions of the GIOP specific stuff.
 *
 * This class will hold the specific details common to all the GIOP
 * versions. Some of them which are here may be shifted if things
 * start changing between versions. This class uses the
 * TAO_GIOP_Message_Reactive_Handler to read and parse messages.
 */

class TAO_Export TAO_GIOP_Message_Base : public TAO_Pluggable_Messaging
{
public:
  /// Constructor
  TAO_GIOP_Message_Base (TAO_ORB_Core *orb_core,
                         size_t input_cdr_size = ACE_CDR::DEFAULT_BUFSIZE);

  /// Dtor
  virtual ~TAO_GIOP_Message_Base (void);

  /// Initialize the underlying state object based on the @a major and
  /// @a minor revision numbers
  virtual void init (CORBA::Octet major,
                     CORBA::Octet minor);

  /// Reset the messaging the object
  virtual void reset (void);

  /// Write the RequestHeader in to the @a cdr stream. The underlying
  /// implementation of the mesaging should do the right thing.
  virtual int generate_request_header (TAO_Operation_Details &op,
                                       TAO_Target_Specification &spec,
                                       TAO_OutputCDR &cdr);

  /// Write the RequestHeader in to the @a cdr stream.
  virtual int generate_locate_request_header (
      TAO_Operation_Details &op,
      TAO_Target_Specification &spec,
      TAO_OutputCDR &cdr
    );

  /// Write the reply header
  virtual int generate_reply_header (
      TAO_OutputCDR &cdr,
      TAO_Pluggable_Reply_Params_Base &params
    );

  /// This method reads the message on the connection. Returns 0 when
  /// there is short read on the connection. Returns 1 when the full
  /// message is read and handled. Returns -1 on errors. If @a block is
  /// 1, then reply is read in a blocking manner.
  virtual int read_message (TAO_Transport *transport,
                            int block = 0,
                            ACE_Time_Value *max_wait_time = 0);


  /// Format the message. As we have not written the message length in
  /// the header, we make use of this oppurtunity to insert and format
  /// the message.
  virtual int format_message (TAO_OutputCDR &cdr);

  /// Parse the incoming messages..
  ///
  /// \return -1 There was some error parsing the GIOP header
  /// \return 0  The GIOP header was parsed correctly
  /// \return 1  There was not enough data in the message block to
  ///            parse the header
  virtual int parse_incoming_messages (ACE_Message_Block &message_block);

  /// Calculate the amount of data that is missing in the <incoming>
  /// message block.
  virtual ssize_t missing_data (ACE_Message_Block &message_block);

  /* Extract the details of the next message from the @a incoming
   * through @a qd. Returns 1 if there are more messages and returns a
   * 0 if there are no more messages in @a incoming.
   */
  virtual int extract_next_message (ACE_Message_Block &incoming,
                                    TAO_Queued_Data *&qd);

  /// Check whether the node @a qd needs consolidation from @a incoming.
  virtual int consolidate_node (TAO_Queued_Data *qd,
                                ACE_Message_Block &incoming);

  /// Get the details of the message parsed through the @a qd.
  virtual void get_message_data (TAO_Queued_Data *qd);

  /// Process the request message that we have received on the
  /// connection
  virtual int process_request_message (TAO_Transport *transport,
                                       TAO_Queued_Data *qd);


  /// Parse the reply message that we received and return the reply
  /// information through @a reply_info
  virtual int process_reply_message (
      TAO_Pluggable_Reply_Params &reply_info,
      TAO_Queued_Data *qd);

  /// Generate a reply message with the exception @a ex.
  virtual int generate_exception_reply (
      TAO_OutputCDR &cdr,
      TAO_Pluggable_Reply_Params_Base &params,
      CORBA::Exception &x
    );

  /// Header length
  virtual size_t header_length (void) const;

  /// The header length of a fragment
  virtual size_t fragment_header_length (CORBA::Octet major,
                                         CORBA::Octet minor) const;

  virtual TAO_OutputCDR &out_stream (void);

protected:

  /// Processes the GIOP_REQUEST messages
  virtual int process_request (TAO_Transport *transport,
                               TAO_InputCDR &input,
                               TAO_OutputCDR &output,
                               TAO_GIOP_Message_Generator_Parser *);

  /// Processes the GIOP_LOCATE_REQUEST messages
  virtual int process_locate_request (TAO_Transport *transport,
                                      TAO_InputCDR &input,
                                      TAO_OutputCDR &output,
                                      TAO_GIOP_Message_Generator_Parser *);

  /// Set the state
  void set_state (CORBA::Octet major,
                  CORBA::Octet minor,
                  TAO_GIOP_Message_Generator_Parser *&) const;

  /// Print out a debug messages..
  void dump_msg (const char *label,
                 const u_char *ptr,
                 size_t len);

  /// Get the message type. The return value would be one of the
  /// following:
  /// TAO_PLUGGABLE_MESSAGE_REQUEST,
  /// TAO_PLUGGABLE_MESSAGE_REPLY,
  /// TAO_PLUGGABLE_MESSAGE_CLOSECONNECTION,
  /// TAO_PLUGGABLE_MESSAGE_MESSAGE_ERROR.
  TAO_Pluggable_Message_Type message_type (
                               const TAO_GIOP_Message_State &state) const;

  /// Writes the GIOP header in to @a msg
  /// @note If the GIOP header happens to change in the future, we can
  /// push this method in to the generator_parser classes.
  int write_protocol_header (TAO_GIOP_Message_Type t,
                             TAO_OutputCDR &msg);

  /// Make a GIOP_LOCATEREPLY and hand that over to the transport so
  /// that it can be sent over the connection.
  /// @note As on date 1.1 & 1.2 seem to have similar headers. Till an
  /// unmanageable difference comes let them be implemented here.
  int make_send_locate_reply (TAO_Transport *transport,
                              TAO_GIOP_Locate_Request_Header &request,
                              TAO_GIOP_Locate_Status_Msg &status,
                              TAO_OutputCDR &output,
                              TAO_GIOP_Message_Generator_Parser *);

  /// Send error messages
  int  send_error (TAO_Transport *transport);

  /// Close a connection, first sending GIOP::CloseConnection.
  void send_close_connection (const TAO_GIOP_Message_Version &version,
                              TAO_Transport *transport,
                              void *ctx);

  /// We must send a LocateReply through @a transport, this request
  /// resulted in some kind of exception.
  int send_reply_exception (TAO_Transport *transport,
                            TAO_OutputCDR &cdr,
                            CORBA::ULong request_id,
                            IOP::ServiceContextList *svc_info,
                            CORBA::Exception *x);


  /// Write the locate reply header
  virtual int generate_locate_reply_header (
      TAO_OutputCDR & /*cdr*/,
      TAO_Pluggable_Reply_Params_Base & /*params*/);

  /// Is the messaging object ready for processing BiDirectional
  /// request/response?
  virtual int is_ready_for_bidirectional (TAO_OutputCDR &msg);

  /// Creates a new node for the queue with a message block in the
  /// node of size @a sz.
  TAO_Queued_Data *make_queued_data (size_t sz);

  /// Initialize the TAO_Queued_Data from the relevant portions of
  /// a GIOP_Message_State.
  void init_queued_data (TAO_Queued_Data* qd,
                         const TAO_GIOP_Message_State& state) const;

private:
  /// Cached ORB_Core pointer...
  TAO_ORB_Core *orb_core_;

  /// The message handler object that does reading and parsing of the
  /// incoming messages
  TAO_GIOP_Message_State message_state_;

  /// All the implementations of GIOP message generator and parsers
  TAO_GIOP_Message_Generator_Parser_Impl tao_giop_impl_;

protected:
  /// Buffer used for both the output and input CDR streams, this is
  /// "safe" because we only one of the streams at a time.
  char buffer_[ACE_CDR::DEFAULT_BUFSIZE];

  /// Buffer where the request is placed.
  TAO_OutputCDR out_stream_;

  /*
   * Hook in the GIOP_Message class to add data member. This hook used in
   * speeding up the dispatch within TAO.
   */
//@@ GIOP_MESSAGE_BASE_DATA_MEMBER_ADD_HOOK

};

/*
 * Hook to specialize the Messaging implementation in TAO with either
 * GIOP or GIOP_Lite protocol.
 */
//@@ MESSAGING_SPL_EXTERN_ADD_HOOK

#include /**/ "ace/post.h"
#endif /*TAO_GIOP_MESSAGE_BASE_H*/
