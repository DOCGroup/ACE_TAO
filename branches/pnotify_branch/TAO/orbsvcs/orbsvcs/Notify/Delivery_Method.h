/* -*- C++ -*- */
/**
 *  @file Delivery_Method.h
 *
 *  $Id$
 *
 *
 */

#ifndef TAO_NOTIFY_DELIVERY_METHOD_H
#define TAO_NOTIFY_DELIVERY_METHOD_H
#include "ace/pre.h"

#include "notify_export.h"
#include "Method_Request.h"
#include "Event.h"
#include "Routing_Slip.h"
#include "Types.h"
#include "Refcountable.h"

#include "ace/Message_Block.h"
#include <ace/Vector_T.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO_NOTIFY
{

/**
 * @class Delivery_Method
 *
 * @brief Base class for requests to deliver events to destinations.
 *
 */
class TAO_Notify_Export Delivery_Method : public TAO_Notify_Method_Request
{
public:
  /// Constructor
  Delivery_Method (const Delivery_Request_Ptr & delivery_request);

  /// Destructor
  virtual ~Delivery_Method ();

  /// Obtain the event.
  const TAO_Notify_Event& event (void) const;

protected:
  // derived classes can copy themselves, but
  // you cant copy-construct a Delivery_Method
  Delivery_Method (const Delivery_Method & rhs);

  Delivery_Request_Ptr delivery_request_;
};

/// Exception/surprise return safe completion notification to the Delivery Request.
class TAO_Notify_Export Delivery_Method_Complete
{
public:
  /// Construct with the Delivery Request to be notified.
  Delivery_Method_Complete (const Delivery_Request_Ptr & delivery_request);
  /// The distructor sends the notification
  ~Delivery_Method_Complete ();
  /// Abandon if notification should not be sent (unlikely!)
  void abandon ();
private:
  Delivery_Request_Ptr delivery_request_;
  bool abandoned_;
};
} //namespace TAO_NOTIFY

#if defined (__ACE_INLINE__)
#include "Delivery_Method.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NOTIFY_DELIVERY_METHOD_H */
