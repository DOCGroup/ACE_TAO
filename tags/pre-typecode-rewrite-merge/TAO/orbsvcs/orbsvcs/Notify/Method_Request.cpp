// $Id$

#include "Method_Request.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Method_Request_Queueable, "$Id$")

TAO_Notify_Method_Request_Queueable *
TAO_Notify_Method_Request_Queueable::copy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return this;
}

TAO_Notify_Method_Request_Queueable::TAO_Notify_Method_Request_Queueable()
{
}

TAO_Notify_Method_Request_Queueable::TAO_Notify_Method_Request_Queueable(const TAO_Notify_Event * event)
{
  this->init (event);
}
