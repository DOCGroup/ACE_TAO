//$Id$
#include "tao/fl_resource.h"

#if !defined (__ACE_INLINE__)
# include "fl_resource.i"
#endif /* ! __ACE_INLINE__ */
#include "ace/FlReactor.h"

ACE_FlReactor *TAO_FlResource_Factory::impl_ = 0;

ACE_Reactor_Impl *
TAO_FlResource_Factory::allocate_reactor_impl (void) const
{
  if ( !impl_ )
  {
    ACE_NEW_RETURN(impl_,
                   ACE_FlReactor (),
                   0);
    ACE_DEBUG ((LM_DEBUG, "ACE_FlReactor created.\n"));
  }

  return impl_;
}

ACE_STATIC_SVC_DEFINE (TAO_FlResource_Factory,
                       ACE_TEXT ("FlResource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_FlResource_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_FlReactor, TAO_FlResource_Factory)

