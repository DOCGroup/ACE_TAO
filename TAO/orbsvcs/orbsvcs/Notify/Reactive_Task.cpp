// $Id$

#include "Reactive_Task.h"

#if ! defined (__ACE_INLINE__)
#include "Reactive_Task.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Reactive_Task, "$Id$")

TAO_NS_Reactive_Task::TAO_NS_Reactive_Task (void)
{
}

TAO_NS_Reactive_Task::~TAO_NS_Reactive_Task ()
{
}

void
TAO_NS_Reactive_Task::exec (TAO_NS_Method_Request& method_request)
{
  method_request.call ();
}
