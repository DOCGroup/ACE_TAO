// $Id$

#ifndef CIAO_SERVANT_INTERFACE_BASE_T_C
#define CIAO_SERVANT_INTERFACE_BASE_T_C

#include "Facet_Servant_Base_T.h"
#include "ccm/CCM_ObjectC.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  template <typename BASE, typename EXEC, typename CONTEXT>
  Facet_Servant_Base_T<BASE, EXEC, CONTEXT>::Facet_Servant_Base_T (
      typename EXEC::_ptr_type exec,
      ::Components::CCMContext_var ctx)
    : executor_ (EXEC::_duplicate (exec)),
      ctx_ (::Components::CCMContext::_duplicate (ctx))
  {
    //TODO: pass a Object_ptr and narrow here....
//   ::Inherited::CCM_derived_interface_var tmp_var =
//       ::Inherited::CCM_derived_interface::_narrow (tmp.in());

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
    CONTEXT::_var_type sc = CONTEXT::_narrow (this->ctx_.in ());

    if (! ::CORBA::is_nil (sc.in ()))
      {
        return sc->get_CCM_object ();
      }

    throw ::CORBA::INTERNAL ();
  }
};

#endif /* CIAO_SERVANT_INTERFACE_BASE_T_C */
