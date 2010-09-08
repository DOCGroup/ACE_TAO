// $Id$

#include "orbsvcs/SSLIOP/SSLIOP_Util.h"
#include "orbsvcs/SSLIOP/SSLIOP_Connection_Handler.h"
#include "orbsvcs/SSLIOP/SSLIOP_Current.h"

#include "tao/ORB_Core.h"
#include "tao/debug.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::Current_ptr
TAO::SSLIOP::Util::current (TAO_ORB_Core *orb_core)
{
  try
    {
      // Go straight to the object_ref_table in the ORB Core to avoid
      // the ORB::resolve_initial_references() mechanism's complaints
      // about the fact that the ORB isn't fully initialized yet
      // (happens on the client side).
      CORBA::Object_var obj =
        orb_core->object_ref_table ().resolve_initial_reference (
          "SSLIOPCurrent");

      TAO::SSLIOP::Current_var tao_current =
        TAO::SSLIOP::Current::_narrow (obj.in ());

      if (CORBA::is_nil (tao_current.in ()))
        throw CORBA::INV_OBJREF ();

      return tao_current._retn ();
    }
  catch (const CORBA::Exception& ex)
    {
      if (TAO_debug_level > 0)
        ex._tao_print_exception (
          "Could not resolve \"SSLIOPCurrent\" object");

      return 0;
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
