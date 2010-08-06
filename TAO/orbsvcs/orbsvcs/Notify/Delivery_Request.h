/* -*- C++ -*- */
// $Id$

#ifndef TAO_NOTIFY_DELIVERY_REQUEST_H
#define TAO_NOTIFY_DELIVERY_REQUEST_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"
#include "orbsvcs/Notify/Topology_Object.h"
#include "orbsvcs/Notify/Event.h"
#include <ace/Vector_T.h>
#include <ace/Bound_Ptr.h>
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations of referenced classes
class TAO_Notify_EventChannelFactory;
class TAO_Notify_ProxySupplier;

namespace TAO_Notify
{

// Forward declarations of TAO_Notify classes/pointers/collections declared
// in this header
class Delivery_Request;
/// A reference-counted smart pointer to a Delivery_Request.
typedef ACE_Strong_Bound_Ptr<Delivery_Request, TAO_SYNCH_MUTEX> Delivery_Request_Ptr;

typedef ACE_Unbounded_Queue<Delivery_Request_Ptr> Delivery_Request_Queue;

// Forward declarations of TAO_Notify classes/pointers/collections referenced
// in this header

class Routing_Slip;
typedef ACE_Strong_Bound_Ptr<Routing_Slip, TAO_SYNCH_MUTEX> Routing_Slip_Ptr;

/// \brief Represents a request to deliver an event to a particular destination.
///
/// A Routing Slip contains a collection of Delivery Requests.
/// A Delivery Request is associated with a Delivery Method.  Delivery Methods
/// should be lightweight objects because they are copied, queued, and otherwise
/// passed around while they are waiting to be executed.  The Delivery Request is
/// more stable.
class TAO_Notify_Serv_Export Delivery_Request
{
public:
  /// Normal constructor
  /// \param routing_slip the routing slip that owns this Delivery Request.
  /// \param request_id an id that identifies this Delivery Request to the Routing Slip.
  Delivery_Request (const Routing_Slip_Ptr & routing_slip, size_t request_id);

  /// \brief A static "factory" method for use during restart.
  ///
  /// \param routing_slip. The routing slip to which the new Delivery Request should be attached.
  /// \param request_id The id used to identify this Delivery Request to the Routing Slip.
  /// \param ecf The EventChannelFactory responsible for reloading this Delivery Method.
  /// \param cdr A CDR stream from which the peristent information for this Delivery Requect can be retrieved.
  /// \return a pointer to the newly-allocated Delivery Request
  static Delivery_Request_Ptr create (
    const Routing_Slip_Ptr & routing_slip,
    size_t request_id,
    TAO_Notify_EventChannelFactory & ecf,
    TAO_InputCDR & cdr);

  /// a normal destructor.
  ~Delivery_Request ();

  /// \brief A method to indicate the delivery is complete.
  ///
  /// To be called by the delivery method associated with this delivery request.
  void complete ();

  /// \brief An accessor method for the event associated with the Routing Slip that owns this Delivery request.
  const TAO_Notify_Event::Ptr & event () const;

  /// \brief An accessor method for the routing slip that owns this request.
  const Routing_Slip_Ptr & routing_slip ()const;

  /// \brief Capture Delivery Type for the Delivery Method
  ///
  /// Called by the delivery method to capture information that should be persisted.
  void set_delivery_type (ACE_CDR::Octet type);

  /// \brief Capture destination ID for the Delivery Request.
  ///
  /// Called by the delivery method to capture information that should be persisted.
  void set_destination_id (IdVec & destination_id);

  /// \brief Marshal peristent information for this delivery request and its delivery methods into a CDR stream.
  ///
  /// Called during persistent event storage.
  void marshal (TAO_OutputCDR & cdr);

  /// expose routing slip method
  unsigned long sequence () const;

  /// expose routing slip method
  bool should_retry () const;

  /// expose routing slip method
  void dispatch (TAO_Notify_ProxySupplier * proxy_supplier, bool filter);

  // Meaningless, but needed by ACE_Vector on some platforms (gcc2.x LynxOS)
  bool operator == (const Delivery_Request & rhs) const;
  // Meaningless, but needed by ACE_Vector on some platforms
  bool  operator != (const Delivery_Request & rhs) const;

private:
  /// No null constructor.
  Delivery_Request ();
  /// No copy constructor.
  Delivery_Request (const Delivery_Request & rhs);
  /// no assignment operator
  Delivery_Request & operator = (const Delivery_Request & rhs);

private:
  Routing_Slip_Ptr routing_slip_;
  size_t request_id_;
  ACE_CDR::Octet delivery_type_;
  IdVec destination_id_;
};

} // namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Delivery_Request.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_NOTIFY_DELIVERY_REQUEST_H */
