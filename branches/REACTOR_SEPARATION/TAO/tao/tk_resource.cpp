//$Id$
#include "tao/tk_resource.h"

#if !defined (__ACE_INLINE__)
# include "tk_resource.i"
#endif /* ! __ACE_INLINE__ */
#include "ace/TkReactor.h"
ACE_TkReactor *
TAO_TkResource_Factory::impl_ = 0 ;

ACE_Reactor_Impl *
TAO_TkResource_Factory::allocate_reactor_impl (void) const
{
  if ( !impl_ )
  {
    ACE_NEW_RETURN(impl_,
                   ACE_TkReactor (),
                   0);
    ACE_DEBUG ((LM_DEBUG, "ACE_TkReactor created.\n"));
  }

  return impl_;
}

ACE_STATIC_SVC_DEFINE (TAO_TkResource_Factory,
                       ACE_TEXT ("TkResource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_TkResource_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_TkReactor, TAO_TkResource_Factory)

