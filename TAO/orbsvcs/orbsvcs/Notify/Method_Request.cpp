// $Id$

#include "Method_Request.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Method_Request, "$Id$")

TAO_NS_Method_Request::TAO_NS_Method_Request (TAO_NS_Event_var& event)
  :event_ (event)
{
}

TAO_NS_Method_Request::~TAO_NS_Method_Request ()
{
}
