// $Id$

#include "Delivery_Request.h"

#if ! defined (__ACE_INLINE__)
#include "Delivery_Request.inl"
#endif /* __ACE_INLINE__ */

#include "tao/debug.h"

//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

namespace TAO_Notify
{

///////////////////
// Delivery_Request
Delivery_Request::Delivery_Request (const Routing_Slip_Ptr & routing_slip, size_t request_id)
  : routing_slip_ (routing_slip)
  , request_id_ (request_id)
  , delivery_type_ (0)
{
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Delivery_Request:: constructor\n")
      ));
}

Delivery_Request::~Delivery_Request ()
{
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
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
const TAO_Notify_Event_var &
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
    size_t dest_count = this->destination_id_.size ();
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
  bool filter ACE_ENV_ARG_DECL)
{
  this->routing_slip_->dispatch (proxy_supplier, filter ACE_ENV_ARG_PARAMETER);
}
} // namespace

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
//template class ACE_Auto_Basic_Ptr<TAO_Notify::Routing_Slip>;
//template class ACE_Strong_Bound_Ptr<TAO_Notify::Routing_Slip, TAO_SYNCH_MUTEX>;
//template class ACE_Auto_Basic_Ptr<TAO_Notify::Delivery_Request>;
//template class ACE_Strong_Bound_Ptr<TAO_Notify::Delivery_Request,TAO_SYNCH_MUTEX>;
// Duplicated in Routing_Slip.cpp
//template class ACE_Vector <TAO_Notify::Delivery_Request_Ptr>;
//template class ACE_Array_Base<ACE_Strong_Bound_Ptr<TAO_Notify::Delivery_Request,TAO_SYNCH_MUTEX> >;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
//#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Notify::Routing_Slip>
//#pragma instantiate ACE_Strong_Bound_Ptr<TAO_Notify::Routing_Slip,TAO_SYNCH_MUTEX>
//#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Notify::Delivery_Request>
//#pragma instantiate ACE_Strong_Bound_Ptr<TAO_Notify::Delivery_Request,TAO_SYNCH_MUTEX>
// Duplicated in Routing_Slip.cpp
//#pragma instantiate ACE_Vector <TAO_Notify::Delivery_Request_Ptr>
//#pragma instantiate ACE_Array_Base<ACE_Strong_Bound_Ptr<TAO_Notify::Delivery_Request,TAO_SYNCH_MUTEX> >
#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

