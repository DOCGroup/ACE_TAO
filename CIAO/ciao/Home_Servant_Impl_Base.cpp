// $Id$

#include "Home_Servant_Impl_Base.h"

namespace CIAO
{
  Home_Servant_Impl_Base::Home_Servant_Impl_Base (Session_Container * c)
    : container_ (c)
  {
  }

  Home_Servant_Impl_Base::~Home_Servant_Impl_Base (void)
  {
  }

  // Operations for CCMHome interface.

  CORBA::IRObject_ptr
  Home_Servant_Impl_Base::get_component_def (
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                      CORBA::IRObject::_nil ());
  }

  CORBA::IRObject_ptr
  Home_Servant_Impl_Base::get_home_def (
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                      CORBA::IRObject::_nil ());
  }

}
