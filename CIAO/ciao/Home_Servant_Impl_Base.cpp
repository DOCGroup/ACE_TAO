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
  Home_Servant_Impl_Base::get_component_def (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  CORBA::IRObject_ptr
  Home_Servant_Impl_Base::get_home_def (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

}
