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
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Method_Request.h"

/**
 * @class TAO_Notify_Method_Request_Event
 *
 * @brief A method request for storing events.
 *
 */
class TAO_Notify_Export TAO_Notify_Method_Request_Event : public TAO_Notify_Method_Request
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Event (const TAO_Notify_Event_var& event);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Event ();

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);

  /// Obtain the event.
  const TAO_Notify_Event_var& event (void);

protected:
  /// The event.
  const TAO_Notify_Event_var event_;
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Event.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_Notify_METHOD_REQUEST_EVENT_H */
