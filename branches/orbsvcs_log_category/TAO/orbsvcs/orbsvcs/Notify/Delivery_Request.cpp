// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Delivery_Request.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Delivery_Request.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/Notify/Routing_Slip.h"

#include "tao/debug.h"
#include "tao/corba.h"

#include "ace/Truncate.h"

//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{

///////////////////
// Delivery_Request
Delivery_Request::Delivery_Request (const Routing_Slip_Ptr & routing_slip, size_t request_id)
  : routing_slip_ (routing_slip)
  , request_id_ (request_id)
  , delivery_type_ (0)
{
  if (DEBUG_LEVEL > 8) ORBSVCS_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Delivery_Request:: constructor\n")
      ));
}

Delivery_Request::~Delivery_Request ()
{
  if (DEBUG_LEVEL > 8) ORBSVCS_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Delivery_Request:: destructor\n")
      ));
}

// DO NOT INLINE THIS.  It uses the Routing_Slip_Ptr which cannot be declared at
// the time Delivery_Request.inl is compiled.
void
Delivery_Request::complete ()
{
  routing_slip_->delivery_request_complete (this->request_id_);
}

// DO NOT INLINE THIS.  It uses the Routing_Slip_Ptr which cannot be declared at
// the time Delivery_Request.inl is compiled.
const TAO_Notify_Event::Ptr &
Delivery_Request::event () const
{
  return this->routing_slip_->event ();
}

void
Delivery_Request::marshal (TAO_OutputCDR & cdr)
{
  if (this->delivery_type_ != 0)
  {
    cdr.write_octet (this->delivery_type_);
    CORBA::ULong dest_count =
      ACE_Utils::truncate_cast<CORBA::ULong> (this->destination_id_.size ());
    cdr.write_ulong (dest_count);
    for (size_t ndest = 0; ndest < dest_count; ++ ndest)
    {
      cdr.write_ulong (this->destination_id_ [ndest]);
    }
  }
}

unsigned long
Delivery_Request::sequence () const
{
  return this->routing_slip_->sequence ();
}


bool
Delivery_Request::should_retry () const
{
  return this->routing_slip_->should_retry ();
}

void
Delivery_Request::dispatch (
  TAO_Notify_ProxySupplier * proxy_supplier,
  bool filter)
{
  this->routing_slip_->dispatch (proxy_supplier, filter);
}

const Routing_Slip_Ptr &
Delivery_Request::routing_slip () const
{
  return this->routing_slip_;
}

} // namespace

TAO_END_VERSIONED_NAMESPACE_DECL
