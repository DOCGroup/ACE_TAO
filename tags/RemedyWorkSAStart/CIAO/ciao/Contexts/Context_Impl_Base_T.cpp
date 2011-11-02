// $Id$

#ifndef CIAO_CONTEXT_IMPL_BASE_T_C
#define CIAO_CONTEXT_IMPL_BASE_T_C

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  template <typename CONTAINER_TYPE>
  Context_Impl_Base_T<CONTAINER_TYPE>::Context_Impl_Base_T (
    ::Components::CCMHome_ptr home,
    typename CONTAINER_TYPE::_ptr_type  c,
    const char *instance_id)
    : container_ (CONTAINER_TYPE::_duplicate (c)),
      home_ (::Components::CCMHome::_duplicate (home)),
      ciao_instance_id_ (instance_id)
  {
  }

  template <typename CONTAINER_TYPE>
  Context_Impl_Base_T<CONTAINER_TYPE>::~Context_Impl_Base_T (void)
  {
  }

  // Operations from ::Components::CCMContext.
#if !defined (CCM_LW)
  template <typename CONTAINER_TYPE>
  Components::Principal_ptr
  Context_Impl_Base_T<CONTAINER_TYPE>::get_caller_principal (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }
#endif

  template <typename CONTAINER_TYPE>
  Components::CCMHome_ptr
  Context_Impl_Base_T<CONTAINER_TYPE>::get_CCM_home (void)
  {
    return Components::CCMHome::_duplicate (this->home_.in ());
  }

#if !defined (CCM_LW)
  template <typename CONTAINER_TYPE>
  CORBA::Boolean
  Context_Impl_Base_T<CONTAINER_TYPE>::get_rollback_only (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }
#endif

#if !defined (CCM_LW)
  template <typename CONTAINER_TYPE>
  Components::Transaction::UserTransaction_ptr
  Context_Impl_Base_T<CONTAINER_TYPE>::get_user_transaction (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }
#endif

#if !defined (CCM_LW)
  template <typename CONTAINER_TYPE>
  CORBA::Boolean
  Context_Impl_Base_T<CONTAINER_TYPE>::is_caller_in_role (const char * /* role */)
  {
    throw CORBA::NO_IMPLEMENT ();
  }
#endif

#if !defined (CCM_LW)
  template <typename CONTAINER_TYPE>
  void
  Context_Impl_Base_T<CONTAINER_TYPE>::set_rollback_only (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }
#endif

  template <typename CONTAINER_TYPE>
  CORBA::Object_ptr
  Context_Impl_Base_T<CONTAINER_TYPE>::resolve_service_reference(const char *service_id)
  {
    typename CONTAINER_TYPE::_var_type cnt_safe =
      CONTAINER_TYPE::_duplicate (this->container_.in ());
    if (::CORBA::is_nil (cnt_safe.in ()))
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Context_Impl::get_client_interceptor_registration - "
                     "Error: Container is nil\n"));
        throw ::CORBA::INV_OBJREF ();
      }
    return cnt_safe->resolve_service_reference (service_id);
  }

  template <typename CONTAINER_TYPE>
  const char *
  Context_Impl_Base_T<CONTAINER_TYPE>::_ciao_instance_id (void) const
  {
    return this->ciao_instance_id_.in ();
  }
}

#endif /* CIAO_CONTEXT_IMPL_BASE_T_C */

