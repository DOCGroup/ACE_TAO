// $Id$

#include "EventBatch.h"

ACE_INLINE void
TAO_NS_EventBatch::batch_size (CORBA::Long batch_size)
{
  this->batch_size_ = batch_size;
}
