// $Id$

#include "Object.h"

ACE_INLINE CORBA::Long
TAO_NS_Object::id (void)
{
  return id_;
}

ACE_INLINE TAO_NS_Worker_Task*
TAO_NS_Object::worker_task (void)
{
  return this->worker_task_;
}

