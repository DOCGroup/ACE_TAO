//$Id$
#include "tao/tk_resource.h"

#if !defined (__ACE_INLINE__)
# include "tk_resource.i"
#endif /* ! __ACE_INLINE__ */
#include "ace/TkReactor.h"

ACE_Reactor_Impl *
TAO_TkResource_Factory::allocate_reactor_impl (void) const
{
  ACE_Reactor_Impl *impl = 0;

  ACE_NEW_RETURN(impl,
                 ACE_TkReactor (),
                 0);
  return impl;
}

ACE_STATIC_SVC_DEFINE (TAO_TkResource_Factory,
                       ACE_TEXT ("TkResource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_TkResource_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_TkReactor, TAO_TkResource_Factory)

