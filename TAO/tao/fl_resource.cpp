//$Id$
#include "tao/fl_resource.h"

#if !defined (__ACE_INLINE__)
# include "fl_resource.i"
#endif /* ! __ACE_INLINE__ */
#include "ace/FlReactor.h"

ACE_Reactor_Impl *
TAO_FlResource_Factory::allocate_reactor_impl (void) const
{
  ACE_Reactor_Impl *impl = 0;

  ACE_NEW_RETURN(impl,
                 ACE_FlReactor (),
                 0);
  return impl;
}

ACE_STATIC_SVC_DEFINE (TAO_FlResource_Factory,
                       ACE_TEXT ("FlResource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_FlResource_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_FlReactor, TAO_FlResource_Factory)

