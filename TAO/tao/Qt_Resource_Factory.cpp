//$Id$
#include "tao/Qt_Resource_Factory.h"
#include "tao/debug.h"

namespace TAO
{
  QApplication *TAO_QtResource_Factory::qapp_ = 0 ;

  ACE_Reactor_Impl *
  QtResource_Factory::create (void) const
  {
    if (this->qapp_ == 0)
      return 0;

    if (!this->reactor_impl_ )
      {
        ACE_NEW_RETURN (this->reactor_impl_,
                        ACE_QtReactor (qapp_),
                        0);
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - ACE_QtReactor created \n"));
      }

    return this->reactor_impl_;
  }

  void
  QtResource_Factory::set_context (QApplication *qapp)
  {
    TAO_QtResource_Factory::qapp_ = qapp;
  }

ACE_STATIC_SVC_DEFINE (QtResource_Factory,
                       ACE_TEXT ("QtResource_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (QtResource_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_QtReactor, TAO_QtResource_Factory)
  }
