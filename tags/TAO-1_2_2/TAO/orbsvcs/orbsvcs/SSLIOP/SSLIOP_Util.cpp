// -*- C++ -*-

#include "SSLIOP_Util.h"


ACE_RCSID (TAO_SSLIOP,
           SSLIOP_Util,
           "$Id$")


#include "SSLIOP_Connection_Handler.h"
#include "SSLIOP_Current.h"

#include "tao/ORB_Core.h"


int
TAO_SSLIOP_Util::setup_handler_state (TAO_ORB_Core *orb_core,
                                      TAO_IIOP_Properties *tcp_properties,
                                      TAO_SSLIOP_Connection_Handler_State &s)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Go straight to the object_ref_table in the ORB Core to avoid
      // the ORB::resolve_initial_references() mechanism's complaints
      // about the fact that the ORB isn't fully initialized yet
      // (happens on the client side).
      CORBA::Object_var obj =
        orb_core->object_ref_table ().resolve_initial_references (
          "SSLIOPCurrent"
          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      SSLIOP::Current_var current =
        SSLIOP::Current::_narrow (obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (current.in ()))
        ACE_TRY_THROW (CORBA::INV_OBJREF ());

      TAO_SSLIOP_Current_var tao_current =
        TAO_SSLIOP_Current::_narrow (current.in ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (tao_current.in ()))
        ACE_TRY_THROW (CORBA::INV_OBJREF ());


      s.tcp_properties = tcp_properties;
      s.ssliop_current = tao_current;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "Could not resolve "
                             "\"SSLIOPCurrent\" object");

      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
