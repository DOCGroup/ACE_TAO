// -*- C++ -*-

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

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Exception.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/GIOP_Message_Generator_Parser_Impl.h"
#include "tao/GIOP_Utils.h"
#include "tao/GIOP_Message_State.h"
#include "tao/GIOP_Fragmentation_Strategy.h"
#include "tao/CDR.h"
#include "tao/Incoming_Message_Stack.h"

#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Pluggable_Reply_Params;
class TAO_Queued_Data;

/**
 * @class TAO_GIOP_Message_Base
 *
 * @brief Definitions of the GIOP specific stuff.
 *
 * This class will hold the specific details common to all the GIOP
 * versions. Some of them which are here may be shifted if things
 * start changing between versions.
 */

class TAO_Export TAO_GIOP_Message_Base
{
public:
  /// Constructor
  TAO_GIOP_Message_Base (TAO_ORB_Core *orb_core,
                         TAO_Transport * transport,
                         size_t input_cdr_size = ACE_CDR::DEFAULT_BUFSIZE);

  /// Destructor
  ~TAO_GIOP_Message_Base (void);

  /// Initialize the underlying state object based on the @a major and
  /// @a minor revision numbers
  void init (CORBA::Octet major, CORBA::Octet minor);

  /// Write the RequestHeader in to the @a cdr stream. The underlying
  /// implementation of the messaging should do the right thing.
  int generate_request_header (TAO_Operation_Details &op,
                               TAO_Target_Specification &spec,
                               TAO_OutputCDR &cdr);

  /// Write the RequestHeader in to the @a cdr stream.
  int generate_locate_request_header (
      TAO_Operation_Details &op,
      TAO_Target_Specification &spec,
      TAO_OutputCDR &cdr);

  /// Write the reply header
  int generate_reply_header (
      TAO_OutputCDR &cdr,
      TAO_Pluggable_Reply_Params_Base &params);

  int generate_fragment_header (TAO_OutputCDR & cdr, CORBA::ULong request_id);

  /// Format the message. As we have not written the message length in
  /// the header, we make use of this opportunity to insert and format
  /// the message.
  int format_message (TAO_OutputCDR &cdr, TAO_Stub* stub);

  /**
   * Parse the details of the next message from the @a incoming
   * and initializes attributes of @a qd.
   * @retval 0 If the message header could not be parsed completely,
   * @retval 1 If the message header could be parsed completely
   * @retval -1 On error.
   */
  int parse_next_message (TAO_Queued_Data &qd, size_t &mesg_length);

  /// Extract the details of the next message from the @a incoming
  /// through @a qd. Returns 0 if the message header could not be
  /// parsed completely, returns a 1 if the message header could be
  /// parsed completely and returns -1 on error.
  int extract_next_message (ACE_Message_Block &incoming, TAO_Queued_Data *&qd);

  /// Check whether the node @a qd needs consolidation from @a incoming.
  int consolidate_node (TAO_Queued_Data *qd, ACE_Message_Block &incoming);

  /// Process the request message that we have received on the
  /// connection
  int process_request_message (TAO_Transport *transport, TAO_Queued_Data *qd);

  /// Parse the reply message that we received and return the reply
  /// information through @a reply_info
  int process_reply_message (
      TAO_Pluggable_Reply_Params &reply_info,
      TAO_Queued_Data *qd);

  /// Generate a reply message with the exception @a ex.
  int generate_exception_reply (
      TAO_OutputCDR &cdr,
      TAO_Pluggable_Reply_Params_Base &params,
      const CORBA::Exception &x);

  /// Header length
  size_t header_length (void) const;

  /// The header length of a fragment
  size_t fragment_header_length (
    const TAO_GIOP_Message_Version& giop_version) const;

  TAO_OutputCDR &out_stream (void);

  /// Consolidate fragmented message with associated fragments, being
  /// stored within this class.  If reliable transport is used (like
  /// TCP) fragments are partially ordered on stack, last fragment on
  /// top. Otherwise If un-reliable transport is used (like UDP)
  /// fragments may be dis-ordered, and must be ordered before
  /// consolidation.  @return 0 on success and @a msg points to
  /// consolidated message, 1 if there are still fragments outstanding,
  /// in case of error -1 is being returned. In any case @a qd must be
  /// released by method implementation.
  int consolidate_fragmented_message (TAO_Queued_Data *qd,
                                      TAO_Queued_Data *&msg);

  /// Discard all fragments associated to request-id encoded in
  /// cancel_request.  This operation will never be called
  /// concurrently by multiple threads nor concurrently to
  /// consolidate_fragmented_message @return -1 on failure, 0 on
  /// success, 1 no fragment on stack relating to CancelRequest.
  int discard_fragmented_message (const TAO_Queued_Data *cancel_request);

  /// Outgoing GIOP message fragmentation strategy.
  TAO_GIOP_Fragmentation_Strategy * fragmentation_strategy (void);

  /// Is the messaging object ready for processing BiDirectional
  /// request/response?
  bool is_ready_for_bidirectional (TAO_OutputCDR &msg) const;

private:
#if defined (TAO_HAS_ZIOP) && TAO_HAS_ZIOP ==1
  /// Decompresses a ZIOP message and turns it into a GIOP message
  /// When decompressed, db contains a complete new ACE_Data_Block and
  /// therefore qd its data block is also replaced.
  bool decompress (ACE_Data_Block **db, TAO_Queued_Data& qd,
                   size_t& rd_pos, size_t& wr_pos);
#endif

  /// Processes the GIOP_REQUEST messages
  int process_request (TAO_Transport *transport,
                       TAO_InputCDR &input,
                       TAO_OutputCDR &output,
                       TAO_GIOP_Message_Generator_Parser *);

  /// Processes the GIOP_LOCATE_REQUEST messages
  int process_locate_request (TAO_Transport *transport,
                              TAO_InputCDR &input,
                              TAO_OutputCDR &output,
                              TAO_GIOP_Message_Generator_Parser *);

  /// Get the parser
  TAO_GIOP_Message_Generator_Parser *get_parser (
    const TAO_GIOP_Message_Version &version) const;

  /// Print out consolidate messages
  int dump_consolidated_msg (TAO_OutputCDR &stream);

  /// Print out a debug messages..
  void dump_msg (const char *label, const u_char *ptr,
                 size_t len);

  /// Writes the GIOP header in to @a msg
  /// @note If the GIOP header happens to change in the future, we can
  /// push this method in to the generator_parser classes.
  int write_protocol_header (GIOP::MsgType t,
                             const TAO_GIOP_Message_Version& version,
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
  int send_error (TAO_Transport *transport);

  /// Close a connection, first sending GIOP::CloseConnection.
  void send_close_connection (const TAO_GIOP_Message_Version &version,
                              TAO_Transport *transport);

  /// We must send a LocateReply through @a transport, this request
  /// resulted in some kind of exception.
  int send_reply_exception (TAO_Transport *transport,
                            TAO_OutputCDR &cdr,
                            CORBA::ULong request_id,
                            IOP::ServiceContextList *svc_info,
                            CORBA::Exception *x);

  /// Write the locate reply header
  int generate_locate_reply_header (
      TAO_OutputCDR &cdr,
      TAO_Pluggable_Reply_Params_Base &params);

  /// Creates a new node for the queue with a message block in the
  /// node of size @a sz.
  TAO_Queued_Data *make_queued_data (size_t sz);

  /// Parse GIOP request-id of TAO_Queued_Data @a qd
  /// @return 0 on success, otherwise -1
  int parse_request_id (const TAO_Queued_Data *qd, CORBA::ULong &request_id) const;

  /// Parse GIOP request-id of TAO_InputCDR @a cdr.
  /// @return 0 on success, otherwise -1
  int parse_request_id (const TAO_InputCDR &cdr, CORBA::ULong &request_id) const;

  /// Set GIOP message flags in message that has been marshaled into
  /// the output CDR stream @a msg.
  /**
   * @note It is assumed that the GIOP message header is the first
   *       thing marshaled into the output CDR stream @a msg.
   */
  void set_giop_flags (TAO_OutputCDR & msg) const;

private:
  /// Cached ORB_Core pointer...
  TAO_ORB_Core *orb_core_;

  /// All the implementations of GIOP message generator and parsers
  TAO_GIOP_Message_Generator_Parser_Impl tao_giop_impl_;

  /// All Fragments being received are stored on stack in reverse
  /// order, last top
  TAO::Incoming_Message_Stack fragment_stack_;

protected:
  /**
   * @name Outgoing GIOP Fragment Related Attributes
   *
   * These attributes are only used when fragmenting outgoing GIOP
   * requests and replies.
   */
  //@{
  /// Strategy that sends data currently marshaled into this
  /// TAO_OutputCDR stream if necessary.
  auto_ptr<TAO_GIOP_Fragmentation_Strategy> fragmentation_strategy_;

  /// Buffer where the request is placed.
  TAO_OutputCDR out_stream_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_GIOP_MESSAGE_BASE_H */
