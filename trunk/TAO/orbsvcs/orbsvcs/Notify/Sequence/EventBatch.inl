// $Id$

#include "EventBatch.h"

ACE_INLINE void
TAO_NS_EventBatch::batch_size (CORBA::Long batch_size)
{
  this->batch_size_ = batch_size;
}

ACE_INLINE int
TAO_NS_EventBatch::size (void)
{
  return this->event_collection_.size ();
}
