// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = AUTHOR
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_EXCLUSIVE_TMS_H
#define TAO_EXCLUSIVE_TMS_H
#include "ace/pre.h"

#include "tao/Transport_Mux_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class TAO_Pluggable_Reply_Params;

class TAO_Export TAO_Exclusive_TMS : public TAO_Transport_Mux_Strategy
{
  // = DESCRIPTION
  //   Using this strategy only one request can be pending at a time
  //   in a connection.  This improves performance because the client
  //   does not have to demux the reply, and there is less need for
  //   synchronization.
  //   On the other hand, it does not scale well when you have
  //   multiple client threads or asynchronous messaging.
  //

public:
  TAO_Exclusive_TMS (TAO_Transport *transport);
  // Constructor.

  virtual ~TAO_Exclusive_TMS (void);
  // Destructor.

  // = Please read the documentation in the TAO_Transport_Mux_Strategy
  //   class.
  virtual CORBA::ULong request_id (void);
  virtual int bind_dispatcher (CORBA::ULong request_id,
                               TAO_Reply_Dispatcher *rh);
  virtual void unbind_dispatcher (CORBA::ULong request_id);

  virtual int dispatch_reply (TAO_Pluggable_Reply_Params &params);

  // @@ Commented for the time being, let the commented line stay for
  //    sometime - Bala
  // virtual TAO_GIOP_Message_State *get_message_state (void);
  // virtual void destroy_message_state (TAO_GIOP_Message_State *);
  virtual int idle_after_send (void);
  virtual int idle_after_reply (void);
  virtual void connection_closed (void);

protected:
  CORBA::ULong request_id_generator_;
  // Used to generate a different request_id on each call to
  // request_id().

  int has_request_;
  // If 0 then the request id and reply dispatcher below are
  // meaningless

  CORBA::ULong request_id_;
  // Request id for the current request.

  TAO_Reply_Dispatcher *rd_;
  // Reply Dispatcher corresponding to the request.

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
