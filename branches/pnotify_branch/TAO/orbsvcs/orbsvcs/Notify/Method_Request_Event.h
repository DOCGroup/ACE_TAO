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
#include "Event.h"
#include "ProxySupplier.h"
#include "tao/Basic_Types.h"
#include "ace/CORBA_macros.h"


/**
 * @class TAO_Notify_Method_Request_Event
 *
 * @brief
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Event
{
protected:
  /// Constuctor
  TAO_Notify_Method_Request_Event (const TAO_Notify_Event *);

  /// Copy-like constructor
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
  /// Constuctor
  /// Not the event_var is passed as a separate parameter to avoid throwing
  /// exceptions from the constructor if it's necessary to copy the event.
  TAO_Notify_Method_Request_Event_Queueable (
    const TAO_Notify_Method_Request_Event & prev_request,
    const TAO_Notify_Event_var & event_var);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Event_Queueable ();

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
