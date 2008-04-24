// -*- C++ -*-

/**
 *  @file Method_Request_Event.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_METHOD_REQUEST_EVENT_H
#define TAO_Notify_METHOD_REQUEST_EVENT_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Method_Request.h"
#include "orbsvcs/Notify/Event.h"
#include "orbsvcs/Notify/ProxySupplier.h"
#include "tao/Basic_Types.h"
#include "ace/CORBA_macros.h"
#include "orbsvcs/Notify/Delivery_Request.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_Method_Request_Event
 *
 * @brief A base class for all Method Requests that are associated with events.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Event
{
public:
  /// Construct from event
  TAO_Notify_Method_Request_Event (const TAO_Notify_Event *);

  /// Construct from a delivery request
  TAO_Notify_Method_Request_Event (
    const TAO_Notify::Delivery_Request_Ptr & delivery_request);

  /// Construct from another Method Request
  /// Event is passed separately because it may be a copy of the one in request.
  TAO_Notify_Method_Request_Event (const TAO_Notify_Method_Request_Event & rhs,
    const TAO_Notify_Event * event);

public:
  /// Destructor
  virtual ~TAO_Notify_Method_Request_Event ();

  const TAO_Notify_Event * event() const;
  void complete ();
  unsigned long sequence ();
  bool should_retry ();

protected:

  /// The Event
  const TAO_Notify_Event * event_;

  /// Pointer to the routing slip's delivery request (if any)
  TAO_Notify::Delivery_Request_Ptr delivery_request_;
};

/***************************************************************/

/**
 * @class TAO_Notify_Method_Request_Event_Queueable
 *
 * @brief A method request for storing events.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Event_Queueable
  : public TAO_Notify_Method_Request_Queueable
  , public TAO_Notify_Method_Request_Event
{
public:
  /// Constructor
  /// Not the event_var is passed as a separate parameter to avoid throwing
  /// exceptions from the constructor if it's necessary to copy the event.
  TAO_Notify_Method_Request_Event_Queueable (
    const TAO_Notify_Method_Request_Event & prev_request,
    const TAO_Notify_Event::Ptr & event);

  TAO_Notify_Method_Request_Event_Queueable (
    TAO_Notify::Delivery_Request_Ptr & request);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Event_Queueable ();

  /// satisfy the pure virtual method.  Should never be called.
  virtual int execute (void);
private:
  TAO_Notify_Event::Ptr event_var_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_METHOD_REQUEST_EVENT_H */
