//$Id$
#include "tao/qt_resource.h"

#if defined (ACE_HAS_QT)

#if !defined (__ACE_INLINE__)
# include "qt_resource.i"
#endif /* ! __ACE_INLINE__ */

QApplication *TAO_QtResource_Factory::qapp_ = 0 ;

ACE_Reactor_Impl *
TAO_QtResource_Factory::allocate_reactor_impl (void) const
{
  if (this->qapp_ == 0) 
    return 0;

  ACE_Reactor_Impl *impl = 0; 

  ACE_NEW_RETURN(impl, 
                 ACE_QtReactor (qapp_), 
                 0);
  return impl;
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

ACE_FACTORY_DEFINE (TAO, TAO_QtResource_Factory)

#endif /* ACE_HAS_QT */
