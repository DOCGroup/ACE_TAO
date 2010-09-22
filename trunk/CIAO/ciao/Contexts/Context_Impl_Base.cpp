// $Id$

#include "Context_Impl_Base.h"
#include "ciao/Containers/Container_BaseC.h"

namespace CIAO
{
  Context_Impl_Base::Context_Impl_Base (::Components::CCMHome_ptr home,
                                        ::CIAO::Container_ptr c,
                                        const char *instance_id)
    : home_ (::Components::CCMHome::_duplicate (home)),
      container_ (::CIAO::Container::_duplicate (c)),
      ciao_instance_id_ (instance_id)
  {
  }

  Context_Impl_Base::~Context_Impl_Base (void)
  {
  }

  // Operations from ::Components::CCMContext.
#if !defined (CCM_LW)
  Components::Principal_ptr
  Context_Impl_Base::get_caller_principal (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }
#endif

  Components::CCMHome_ptr
  Context_Impl_Base::get_CCM_home (void)
  {
    return Components::CCMHome::_duplicate (this->home_.in ());
  }

#if !defined (CCM_LW)
  CORBA::Boolean
  Context_Impl_Base::get_rollback_only (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }
#endif

#if !defined (CCM_LW)
  Components::Transaction::UserTransaction_ptr
  Context_Impl_Base::get_user_transaction (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }
#endif

#if !defined (CCM_LW)
  CORBA::Boolean
  Context_Impl_Base::is_caller_in_role (const char * /* role */)
  {
    throw CORBA::NO_IMPLEMENT ();
  }
#endif

#if !defined (CCM_LW)
  void
  Context_Impl_Base::set_rollback_only (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }
#endif

  CORBA::Object_ptr
  Context_Impl_Base::resolve_service_reference(const char *service_id)
  {
    return this->container_->resolve_service_reference (service_id);
  }

  const char *
  Context_Impl_Base::_ciao_instance_id (void) const
  {
    return this->ciao_instance_id_.in ();
  }
}
