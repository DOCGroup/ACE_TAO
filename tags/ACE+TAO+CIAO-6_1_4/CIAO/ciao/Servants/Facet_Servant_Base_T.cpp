// $Id$

#ifndef CIAO_FACET_INTERFACE_BASE_T_C
#define CIAO_FACET_INTERFACE_BASE_T_C

#include "Facet_Servant_Base_T.h"
#include "ccm/CCM_ObjectC.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  template <typename BASE, typename EXEC, typename CONTEXT>
  Facet_Servant_Base_T<BASE, EXEC, CONTEXT>::Facet_Servant_Base_T (
      typename EXEC::_ptr_type exec,
      ::Components::CCMContext_ptr ctx)
    : executor_ (EXEC::_duplicate (exec)),
      ctx_ (::Components::CCMContext::_duplicate (ctx))
  {
    CIAO_TRACE ("Facet_Servant_Base_T<BASE, EXEC, CONTEXT>::Facet_Servant_Base_T");
  }

  template <typename BASE, typename EXEC, typename CONTEXT>
  Facet_Servant_Base_T<BASE, EXEC, CONTEXT>::~Facet_Servant_Base_T (void)
  {
  }

  template <typename BASE, typename EXEC, typename CONTEXT>
  ::CORBA::Object_ptr
  Facet_Servant_Base_T<BASE, EXEC, CONTEXT>::_get_component (void)
  {
    typename CONTEXT::_var_type sc = CONTEXT::_narrow (this->ctx_.in ());

    if (::CORBA::is_nil (sc.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }

    return sc->get_CCM_object ();
  }
};

#endif /* CIAO_FACET_INTERFACE_BASE_T_C */
