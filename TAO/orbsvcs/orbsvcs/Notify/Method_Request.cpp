// $Id$

#include "Method_Request.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Method_Request, "$Id$")

TAO_Notify_Method_Request *
TAO_Notify_Method_Request::copy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  /// @@ TODO rename this method to on_heap
  return this;
}
