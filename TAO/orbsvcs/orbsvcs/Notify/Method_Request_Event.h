/* -*- C++ -*- */
/**
 *  @file Method_Request_Event.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_METHOD_REQUEST_EVENT_H
#define TAO_Notify_METHOD_REQUEST_EVENT_H
#include /**/ "ace/pre.h"

#include "notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Method_Request.h"
#include "Method_Request_Dispatch.h" // for Request_Event_Base.  s/b moved
#include "Event.h"

/**
 * @class TAO_Notify_Method_Request_Event
 *
 * @brief A method request for storing events.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Event
  : public TAO_Notify_Method_Request
  , public TAO_Notify_Method_Request_Event_Base
{
public:
  /// Constuctor
  /// Not the event_var is passed as a separate parameter to avoid throwing
  /// exceptions from the constructor if it's necessary to copy the event.
  TAO_Notify_Method_Request_Event (
    const TAO_Notify_Method_Request_Event_Base & prev_request,
    const TAO_Notify_Event_var & event_var);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Event ();

  /// satisfy the pure virtual method.  Should never be called.
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);
private:
  TAO_Notify_Event_var event_var_;
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Event.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_METHOD_REQUEST_EVENT_H */
