// $Id$

#include "Context_Impl_Base.h"

namespace CIAO
{
  Context_Impl_Base::Context_Impl_Base (void)
  {
    // Should not be called.
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
  Context_Impl_Base::get_caller_principal (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                      Components::Principal::_nil ());
  }

  Components::CCMHome_ptr
  Context_Impl_Base::get_CCM_home (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return Components::CCMHome::_duplicate (this->home_.in ());
  }

  CORBA::Boolean
  Context_Impl_Base::get_rollback_only (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::IllegalState))
  {
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), false);
  }

  Components::Transaction::UserTransaction_ptr
  Context_Impl_Base::get_user_transaction (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::IllegalState))
  {
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                      Components::Transaction::UserTransaction::_nil ());
  }

  CORBA::Boolean
  Context_Impl_Base::is_caller_in_role (
      const char * /* role */
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), false);
  }

  void
  Context_Impl_Base::set_rollback_only (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::IllegalState))
  {
    ACE_THROW (CORBA::NO_IMPLEMENT ());
  }

  // CIAO-specific.

  CIAO::Session_Container *
  Context_Impl_Base::_ciao_the_Container (void) const
  {
    return this->container_;
  }
}
