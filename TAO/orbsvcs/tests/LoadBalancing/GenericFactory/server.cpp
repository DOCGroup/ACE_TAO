#include "LB_server.h"
#include "Basic.h"

ACE_RCSID (GenericFactory,
           server,
           "$Id$")

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      const char *loc1 = "MyLocation 1";
      const char *loc2 = "MyLocation 2";

      LB_Basic_Test basic_test (argc, argv);

      if (basic_test.start_orb_and_poa () == -1)
        return 1;

      if (basic_test.create_object_group () == -1)
        return 1;

      Basic basic_servant1 (basic_test.object_group (),
                            basic_test.load_manager (),
                            basic_test.orb (), 
                            loc1);

      Basic basic_servant2 (basic_test.object_group (),
                            basic_test.load_manager (),
                            basic_test.orb (),
                            loc2);

      if (basic_test.register_servant (&basic_servant1, loc1) == -1)
        return 1;

      if (basic_test.register_servant (&basic_servant2, loc2) == -1)
        return 1;

      basic_test.orb ()->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));
     
      basic_test.destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK; 
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "server exception");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
