// -*- C++ -*-
// $Id$

/**
 *
 * Bla bla bla
 *
 */


// TAO_IDL - Generated from
// be/be_????.cpp:??

#include "DerivedEC.h"

#include "ciao/Contexts/Context_Impl_Base_T.h"

namespace CIAO_FACET_Inherited
{
  template <typename BASE, typename EXEC>
  derived_interface_Servant_T<BASE, EXEC>::derived_interface_Servant_T (
      typename EXEC::_ptr_type exec,
      ::Components::CCMContext_ptr ctx)
    : CIAO::Servant_Interface_Base_T<BASE, EXEC>(exec, ctx)
  {
  }

  template <typename BASE, typename EXEC>
  derived_interface_Servant_T<BASE, EXEC>::~derived_interface_Servant_T (void)
  {
  }

  // All facet operations and attributes.

  template <typename BASE, typename EXEC>
  void
  derived_interface_Servant_T<BASE, EXEC>::do_derived (
    void)
  {
    ::Inherited::CCM_derived_interface_var executor =
      ::Inherited::CCM_derived_interface::_duplicate (this->executor_.in ());

    if ( ::CORBA::is_nil (executor.in ()))
      {
        throw ::CORBA::INV_OBJREF ();
      }

    executor->do_derived ();
  }
};

namespace CIAO_FACET_Inherited
{

  template <typename BASE,
            typename EXEC,
            typename BASE_BASE,
            typename EXEC_BASE>
  derived_interface_2_Servant_T<BASE, EXEC, BASE_BASE, EXEC_BASE>::derived_interface_2_Servant_T (
    typename EXEC::_ptr_type exec,
    typename EXEC_BASE::_ptr_type exec_base,
    ::Components::CCMContext_ptr ctx)
    : derived_interface_Servant_T<BASE_BASE, EXEC_BASE> (exec_base, ctx)
  {
  }

  template <typename BASE,
            typename EXEC,
            typename BASE_BASE,
            typename EXEC_BASE>
  derived_interface_2_Servant_T<BASE, EXEC, BASE_BASE, EXEC_BASE>::~derived_interface_2_Servant_T (void)
  {
  }

  // All facet operations and attributes.

  template <typename BASE,
            typename EXEC,
            typename BASE_BASE,
            typename EXEC_BASE>
  void
  derived_interface_2_Servant_T<BASE, EXEC, BASE_BASE, EXEC_BASE>::do_derived_2 (
    void)
  {
    ::Inherited::CCM_derived_2_interface_var executor =
      ::Inherited::CCM_derived_2_interface::_duplicate (this->executor_.in ());

    if ( ::CORBA::is_nil (executor.in ()))
      {
        throw ::CORBA::INV_OBJREF ();
      }

    executor->do_derived_2 ();
  }
}
