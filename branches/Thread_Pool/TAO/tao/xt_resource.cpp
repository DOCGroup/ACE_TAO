// $Id$

#include "tao/xt_resource.h"

#if defined(ACE_HAS_XT)

#if !defined (__ACE_INLINE__)
# include "tao/xt_resource.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, xt_resource, "$Id$")

XtAppContext
TAO_XT_Resource_Factory::context_ = 0;

ACE_Reactor_Impl*
TAO_XT_Resource_Factory::allocate_reactor_impl (void) const
{
  if (TAO_XT_Resource_Factory::context_ == 0)
    return 0;

  ACE_Reactor_Impl *impl = 0;

  ACE_NEW_RETURN (impl, ACE_XtReactor (TAO_XT_Resource_Factory::context_), 0);
  return impl;
}

void
TAO_XT_Resource_Factory::set_context (XtAppContext context)
{
  TAO_XT_Resource_Factory::context_ = context;
}

ACE_STATIC_SVC_DEFINE (TAO_XT_Resource_Factory,
                       ACE_TEXT ("XT_Resource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_XT_Resource_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_XT_Resource_Factory)

#endif /* ACE_HAS_XT */
