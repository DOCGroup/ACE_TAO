// $Id$

#include "Context_Impl_Base.h"

#if !defined (__ACE_INLINE__)
# include "Context_Impl_Base.inl"
#endif /* __ACE_INLINE__ */

#include "ciao/Containers/Container_BaseC.h"

namespace CIAO
{
  Context_Impl_Base::Context_Impl_Base (void)
  {
    // This constructor is here to keep MSVC happy and should
    // not be used at all. This constructor should be removed
    // in the future. Until then, we have the ACE_ASSERT
    // below to detect a runtime call to this constructor.
    ACE_ASSERT (0);
  }

  Context_Impl_Base::Context_Impl_Base (Components::CCMHome_ptr home,
                                        Container_ptr c,
                                        const char *instance_id)
    : home_ (Components::CCMHome::_duplicate (home)),
      container_ (Container::_duplicate (c)),
      ciao_instance_id_ (instance_id)
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
}
