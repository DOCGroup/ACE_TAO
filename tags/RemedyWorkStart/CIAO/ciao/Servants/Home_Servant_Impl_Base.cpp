// $Id$

#include "Home_Servant_Impl_Base.h"

namespace CIAO
{
  Home_Servant_Impl_Base::Home_Servant_Impl_Base (void)
  {
  }

  Home_Servant_Impl_Base::~Home_Servant_Impl_Base (void)
  {
  }

#if !defined (CCM_LW)
  CORBA::IRObject_ptr
  Home_Servant_Impl_Base::get_component_def (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }
#endif

#if !defined (CCM_LW)
  CORBA::IRObject_ptr
  Home_Servant_Impl_Base::get_home_def (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }
#endif

  void
  Home_Servant_Impl_Base::set_attributes (
    const Components::ConfigValues & /* descr */)
  {
  }
}

