/* -*- C++ -*- */
// $Id$
#include "Routing_Slip.h"

namespace TAO_Notify
{

ACE_INLINE
const Routing_Slip_Ptr &
Delivery_Request::routing_slip () const
{
  return this->routing_slip_;
}

ACE_INLINE
void
Delivery_Request::set_delivery_type (ACE_CDR::Octet type)
{
  this->delivery_type_ = type;
}

ACE_INLINE
void
Delivery_Request::set_destination_id (IdVec & destination_id)
{
  this->destination_id_ = destination_id;
}

} // namespace TAO_Notify


