// $Id$

#include "Notify_AdminProperties.h"

ACE_INLINE CORBA::Long
TAO_Notify_AdminProperties::max_queue_length (void) const
{
  return this->max_queue_length_;
}

ACE_INLINE CORBA::Long
TAO_Notify_AdminProperties::max_consumers (void) const
{
  return this->max_consumers_;
}

ACE_INLINE  CORBA::Long
TAO_Notify_AdminProperties::max_suppliers (void) const
{
  return this->max_suppliers_;
}

ACE_INLINE CORBA::Boolean
TAO_Notify_AdminProperties::reject_new_events (void) const
{
  return this->reject_new_events_;
}

ACE_INLINE TAO_Notify_Signal_Property_Long*
TAO_Notify_AdminProperties::queue_length (void)
{
  return &(this->queue_length_);
}

ACE_INLINE TAO_Notify_Property_Long*
TAO_Notify_AdminProperties::consumers (void)
{
  return &(this->consumers_);
}

ACE_INLINE TAO_Notify_Property_Long*
TAO_Notify_AdminProperties::suppliers (void)
{
  return &(this->suppliers_);
}
