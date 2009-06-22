// $Id$

#include "Swapping_Servant_Home_Impl_Base.h"

namespace CIAO
{
  Swapping_Home_Servant_Impl_Base::Swapping_Home_Servant_Impl_Base (
    Swapping_Container * c)
    : container_ (c)
  {
  }

  Swapping_Home_Servant_Impl_Base::~Swapping_Home_Servant_Impl_Base (void)
  {
  }

  // Operations for CCMHome interface.

  CORBA::IRObject_ptr
  Swapping_Home_Servant_Impl_Base::get_component_def (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  CORBA::IRObject_ptr
  Swapping_Home_Servant_Impl_Base::get_home_def (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

}
