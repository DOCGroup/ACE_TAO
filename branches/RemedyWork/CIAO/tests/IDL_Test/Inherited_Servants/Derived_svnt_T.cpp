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

#include "ciao/Servants/Session/Servant_Interface_Base_T.h"
#include "ciao/Contexts/Context_Impl_Base_T.h"

namespace CIAO_FACET_Inherited
{
  template <typename BASE, typename EXEC>
  interface_derived_Servant_T<BASE, EXEC>::interface_derived_Servant_T (
      ::CORBA::Object_ptr exec,
      ::Components::CCMContext_ptr ctx)
    : ::CIAO::Servant_Interface_Base_T<BASE, EXEC>(exec, ctx)
  {
  }

  template <typename BASE, typename EXEC>
  interface_derived_Servant_T<BASE, EXEC>::~interface_derived_Servant_T (void)
  {
  }

  // All facet operations and attributes.

  template <typename BASE, typename EXEC>
  void
  interface_derived_Servant_T<BASE, EXEC>::do_derived (
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