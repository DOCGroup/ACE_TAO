// $Id$

#ifndef CIAO_SERVANT_INTERFACE_BASE_T_C
#define CIAO_SERVANT_INTERFACE_BASE_T_C

#include "Servant_Interface_Base_T.h"
#include "ccm/CCM_ObjectC.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  template <typename BASE, typename EXEC>
  Servant_Interface_Base_T<BASE, EXEC>::Servant_Interface_Base_T (
      typename EXEC::_ptr_type exec,
      ::Components::CCMContext_var ctx)
    : executor_ (EXEC::_duplicate (exec)),
      ctx_ (::Components::CCMContext::_duplicate (ctx))
  {
    //TODO: pass a Object_ptr and narrow here....
//   ::Inherited::CCM_derived_interface_var tmp_var =
//       ::Inherited::CCM_derived_interface::_narrow (tmp.in());

    CIAO_TRACE ("Servant_Interface_Base_T<BASE, EXEC>::Servant_Interface_Base_T");
  }

  template <typename BASE, typename EXEC>
  Servant_Interface_Base_T<BASE, EXEC>::~Servant_Interface_Base_T (void)
  {

  }

  template <typename BASE, typename EXEC>
  ::CORBA::Object_ptr
  Servant_Interface_Base_T<BASE, EXEC>::_get_component (void)
  {
    ::Components::SessionContext_var sc =
      ::Components::SessionContext::_narrow (this->ctx_.in ());

    if (! ::CORBA::is_nil (sc.in ()))
      {
        return sc->get_CCM_object ();
      }

    throw ::CORBA::INTERNAL ();
  }
};

#endif /* CIAO_SERVANT_INTERFACE_BASE_T_C */
