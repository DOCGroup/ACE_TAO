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

//
#ifndef TAO_PLUGGABLE_MESSAGING_H
#define TAO_PLUGGABLE_MESSAGING_H
#include "ace/pre.h"

#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/Exception.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Target_Specification;
class TAO_Pluggable_Reply_Params;
class TAO_Transport;
class TAO_Operation_Details;
class TAO_Target_Specification;
class TAO_OutputCDR;

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

  /// Write the RequestHeader in to the <cdr> stream. The underlying
  /// implementation of the mesaging should do the right thing.
  virtual int generate_request_header (
      TAO_Operation_Details &op,
      TAO_Target_Specification &spec,
      TAO_OutputCDR &cdr) = 0;

  /// Write the RequestHeader in to the <cdr> stream.
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
      TAO_Pluggable_Reply_Params &params) = 0;

  /**
   * Write the locate reply header
   * The locate reply header is a part of any messaging protocol. The
   * messaging protocol implementation would implement the way the
   * locate reply header is written.
   */
  virtual int generate_locate_reply_header (
      TAO_OutputCDR &cdr,
      TAO_Pluggable_Reply_Params &params) = 0;

  /**
   * This method reads the message on the connection. Returns 0 when
   * there is short read on the connection. Returns 1 when the full
   * message is read and handled. Returns -1 on errors. If <block> is
   * 1, then reply is read in a blocking manner. <bytes> indicates the
   * number of bytes that needs to be read from the connection.
   */
  virtual int read_message (TAO_Transport *transport,
                            int block = 0,
                            ACE_Time_Value *max_wait_time = 0) = 0;

  /// Format the message in the <cdr>. May not be needed in
  /// general.
  virtual int format_message (TAO_OutputCDR &cdr) = 0;

  /// Get the message type that was received.
  virtual TAO_Pluggable_Message_Type message_type (void) = 0;


  /// Do any initialisations that may be needed.
  virtual void init (CORBA::Octet major,
                     CORBA::Octet minor) = 0;

  /// Reset teh messaging object
  virtual void reset (int reset_flag = 1) = 0;
  // Reset the messaging object

  /// Parse the request message, make an upcall and send the reply back
  /// to the "request initiator"
  virtual int process_request_message (TAO_Transport *transport,
                                       TAO_ORB_Core *orb_core) = 0;

  /// Parse the reply message that we received and return the reply
  /// information though <reply_info>
  virtual int process_reply_message (
      TAO_Pluggable_Reply_Params &reply_info) = 0;

  /// Generate a reply message with the exception <ex>.
  virtual int generate_exception_reply (
      TAO_OutputCDR &cdr,
      TAO_Pluggable_Reply_Params &params,
      CORBA::Exception &x) = 0;

  /// Is the messaging object ready for processing BiDirectional
  /// request/response?
  virtual int is_ready_for_bidirectional (void) = 0;

  /// Are there any more messages that needs processing?
  virtual int more_messages (void);
};

#if defined (__ACE_INLINE__)
# include "tao/Pluggable_Messaging.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_PLUGGABLE_MESSAGING_H*/
