// $Id$

#include "Multiple_Inheritance_i.h"
#include "tao/TAO.h"

ACE_RCSID(Multiple_Inheritance, server, "$Id$")

int
main (int argc, char *argv[])
{
  Multiple_Inheritance_i servant;
  TAO_ORB_Manager orb_manager;

  ACE_DEBUG ((LM_DEBUG, "\n\tMultiple Inheritance Server\n\n"));
  TAO_TRY
    {
      orb_manager.init_child_poa (argc, 
                                  argv, 
                                  "child_poa", 
                                  TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::String_var ior = orb_manager.activate_under_child_poa ("my_object", 
                                                                    &servant, 
                                                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "%s\n\n", ior.in ()));

      orb_manager.run (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}
