// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Exclusive_TMS.h
 *
 *  $Id$
 *
 *  @author  Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_EXCLUSIVE_TMS_H
#define TAO_EXCLUSIVE_TMS_H
#include "ace/pre.h"

#include "tao/Transport_Mux_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class TAO_Pluggable_Reply_Params;

/**
 * @class TAO_Exclusive_TMS
 *
 * Using this strategy only one request can be pending at a time
 * in a connection.  This improves performance because the client
 * does not have to demux the reply, and there is less need for
 * synchronization.
 * On the other hand, it does not scale well when you have
 * multiple client threads or asynchronous messaging.
 */
class TAO_Export TAO_Exclusive_TMS : public TAO_Transport_Mux_Strategy
{

public:
  /// Constructor.
  TAO_Exclusive_TMS (TAO_Transport *transport);

  /// Destructor.
  virtual ~TAO_Exclusive_TMS (void);

  /**
   * @name The TAO_Transport_Mux_Strategy overrided methods
   *
   * Please read the documentation in the TAO_Transport_Mux_Strategy class
   * for details.
   */
  //@{
  virtual CORBA::ULong request_id (void);
  virtual int bind_dispatcher (CORBA::ULong request_id,
                               TAO_Reply_Dispatcher *rh);
  virtual int unbind_dispatcher (CORBA::ULong request_id);

  virtual int dispatch_reply (TAO_Pluggable_Reply_Params &params);

  virtual int idle_after_send (void);
  virtual int idle_after_reply (void);
  virtual void connection_closed (void);
  //@}

protected:
  /// Used to generate a different request_id on each call to
  /// request_id().
  CORBA::ULong request_id_generator_;

  /// If 0 then the request id and reply dispatcher below are
  /// meaningless
  int has_request_;

  /// Request id for the current request.
  CORBA::ULong request_id_;

  /// Reply Dispatcher corresponding to the request.
  TAO_Reply_Dispatcher *rd_;

  // @@ Commented for the time being, let the commented line stay for
  //    sometime - Bala
  // TAO_GIOP_Message_State message_state_;
  // Message state to read the incoming message.

  // @@ Having members of type TAO_GIOP* indicates that we
  // (Transport_Mux_Strategy) are aware of the underlying messaging
  // protocol. But for the present let us close our eyes till we are
  // able to iterate on a use case - Bala.
};

#include "ace/post.h"
#endif /* EXCLUSIVE_TMS_H */
