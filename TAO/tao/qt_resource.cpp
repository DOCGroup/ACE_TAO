//$Id$
#include "tao/qt_resource.h"

#if !defined (__ACE_INLINE__)
# include "qt_resource.i"
#endif /* ! __ACE_INLINE__ */

QApplication *TAO_QtResource_Factory::qapp_ = 0 ;
ACE_QtReactor *TAO_QtResource_Factory::reactor_impl_ = 0;

ACE_Reactor_Impl *
TAO_QtResource_Factory::allocate_reactor_impl (void) const
{
  if (this->qapp_ == 0)
    return 0;
  if ( !reactor_impl_ )
  {
      ACE_NEW_RETURN(reactor_impl_,
                     ACE_QtReactor (qapp_),
                     0);
      ACE_DEBUG ( ( LM_DEBUG, "ACE_QtReactor created.\n" ) );
  }
  return reactor_impl_;
}

void
TAO_QtResource_Factory::set_context (QApplication *qapp)
{
  TAO_QtResource_Factory::qapp_ = qapp;
}

ACE_STATIC_SVC_DEFINE (TAO_QtResource_Factory,
                       ACE_TEXT ("QtResource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_QtResource_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_QtReactor, TAO_QtResource_Factory)


