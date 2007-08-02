// $Id$

#include "Context_Impl_Base.h"
#include "Container_Base.h"

namespace CIAO
{
  Context_Impl_Base::Context_Impl_Base (void)
  {
    ACE_ASSERT (0);
  }

  Context_Impl_Base::Context_Impl_Base (Components::CCMHome_ptr home,
                                        Session_Container * c)
    : home_ (Components::CCMHome::_duplicate (home)),
      container_ (c)
  {
  }

  Context_Impl_Base::~Context_Impl_Base (void)
  {
  }

  // Operations from ::Components::CCMContext.

  Components::Principal_ptr
  Context_Impl_Base::get_caller_principal (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  Components::CCMHome_ptr
  Context_Impl_Base::get_CCM_home (void)
  {
    return Components::CCMHome::_duplicate (this->home_.in ());
  }

  CORBA::Boolean
  Context_Impl_Base::get_rollback_only (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  Components::Transaction::UserTransaction_ptr
  Context_Impl_Base::get_user_transaction (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  CORBA::Boolean
  Context_Impl_Base::is_caller_in_role (const char * /* role */)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  void
  Context_Impl_Base::set_rollback_only (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  CORBA::Object_ptr
  Context_Impl_Base::resolve_service_reference(const char *)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  // CIAO-specific.

  CIAO::Session_Container *
  Context_Impl_Base::_ciao_the_Container (void) const
  {
    return this->container_;
  }

  const char *
  Context_Impl_Base::_ciao_instance_id (void) const
  {
    return this->ciao_instance_id_.in ();
  }

  void
  Context_Impl_Base::_ciao_instance_id (const char *instance_id)
  {
    this->ciao_instance_id_ = instance_id;
  }
}
