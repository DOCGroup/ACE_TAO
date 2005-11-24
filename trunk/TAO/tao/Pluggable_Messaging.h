// -*- C++ -*-

//=============================================================================
/**
 *  @file     Pluggable_Messaging.h
 *
 *  $Id$
 *
 *   Interface for the TAO pluggable messaging framework.
 *
 *
 *  @author  Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_PLUGGABLE_MESSAGING_H
#define TAO_PLUGGABLE_MESSAGING_H

#include /**/ "ace/pre.h"

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Time_Value;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Exception;
}

class TAO_Target_Specification;
class TAO_Pluggable_Reply_Params;
class TAO_Pluggable_Reply_Params_Base;
class TAO_Transport;
class TAO_Operation_Details;
class TAO_Target_Specification;
class TAO_OutputCDR;
class TAO_Queued_Data;

// @@ The more I think I about this class, I feel that this class need
// not be a ABC as it is now. Instead we have these options
// (1) Parametrize this class with the "Messaging Object". Then the
//     implementations  can just redirect the request to the class
//     that it has been specialised with
// (2) Use a bridge pattern here ie. the interface exposed by this
//     class will redirect the request to the implementation which
//     would be "right messaging" object.
// Both of them feasible. If we are not going like templates much, (2)
// is seems to be a better option.
// I am going to take a closer look on this after my MS defense -
// Bala

/**
 * @class TAO_Pluggable_Messaging
 *
 * @brief Generic definitions  Messaging class.
 *
 * This interface tries to define generic methods that could be
 * different messaging protocols
 */
class TAO_Export TAO_Pluggable_Messaging
{
public:
  /// Dtor
  virtual ~TAO_Pluggable_Messaging (void);

  /// Write the RequestHeader in to the @a cdr stream. The underlying
  /// implementation of the mesaging should do the right thing.
  virtual int generate_request_header (
      TAO_Operation_Details &op,
      TAO_Target_Specification &spec,
      TAO_OutputCDR &cdr) = 0;

  /// Write the RequestHeader in to the @a cdr stream.
  virtual int generate_locate_request_header (
      TAO_Operation_Details &op,
      TAO_Target_Specification &spec,
      TAO_OutputCDR &cdr) = 0;

  /**
   * Write the reply header
   * The reply header is a part of any messaging protocol. The
   * messaging protocol implementation would implement the way the
   * reply header is written.
   */
  virtual int generate_reply_header (
      TAO_OutputCDR &cdr,
      TAO_Pluggable_Reply_Params_Base &params) = 0;

  /**
   * Write the locate reply header
   * The locate reply header is a part of any messaging protocol. The
   * messaging protocol implementation would implement the way the
   * locate reply header is written.
   */
  virtual int generate_locate_reply_header (
      TAO_OutputCDR &cdr,
      TAO_Pluggable_Reply_Params_Base &params) = 0;

  /**
   * This method reads the message on the connection. Returns 0 when
   * there is short read on the connection. Returns 1 when the full
   * message is read and handled. Returns -1 on errors. If @a block is
   * 1, then reply is read in a blocking manner.
   */
  virtual int read_message (TAO_Transport *transport,
                            int block = 0,
                            ACE_Time_Value *max_wait_time = 0) = 0;

  /// Format the message in the @a cdr. May not be needed in
  /// general.
  virtual int format_message (TAO_OutputCDR &cdr) = 0;

  /// Do any initialisations that may be needed.
  virtual void init (CORBA::Octet major,
                     CORBA::Octet minor) = 0;

  /// Parse the incoming messages..
  virtual int parse_incoming_messages (ACE_Message_Block &message_block) = 0;

  /// Calculate the amount of data that is missing in the @a incoming
  /// message block.
  virtual ssize_t missing_data (ACE_Message_Block &incoming) = 0;

  /// Get the details of the message parsed through the @a qd.
  virtual void get_message_data (TAO_Queued_Data *qd) = 0;

  /* Extract the details of the next message from the @a incoming
   * through @a qd. Returns 1 if there are more messages and returns a
   * 0 if there are no more messages in @a incoming.
  */
  virtual int extract_next_message (ACE_Message_Block &incoming,
                                    TAO_Queued_Data *&qd) = 0;

  /// Check whether the node @a qd needs consolidation from @a incoming
  virtual int consolidate_node (TAO_Queued_Data *qd,
                                ACE_Message_Block &incoming) = 0;

  /// Parse the request message, make an upcall and send the reply back
  /// to the "request initiator"
  virtual int process_request_message (TAO_Transport *transport,
                                       TAO_Queued_Data *qd) = 0;


  /// Parse the reply message that we received and return the reply
  /// information through @a reply_info
  virtual int process_reply_message (
      TAO_Pluggable_Reply_Params &reply_info,
      TAO_Queued_Data *qd) = 0;


  /// Generate a reply message with the exception @a ex.
  virtual int generate_exception_reply (
      TAO_OutputCDR &cdr,
      TAO_Pluggable_Reply_Params_Base &params,
      CORBA::Exception &ex) = 0;

  /// Is the messaging object ready for processing BiDirectional
  /// request/response?
  virtual int is_ready_for_bidirectional (TAO_OutputCDR &msg) = 0;

  /// Reset the messaging the object
  virtual void reset (void) = 0;

  /// Header length
  virtual size_t header_length (void) const = 0;

  /// Fragment header length
  virtual size_t fragment_header_length (CORBA::Octet major,
                                         CORBA::Octet minor) const = 0;

  /// Accessor for the output CDR stream
  virtual TAO_OutputCDR &out_stream (void) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /*TAO_PLUGGABLE_MESSAGING_H*/
