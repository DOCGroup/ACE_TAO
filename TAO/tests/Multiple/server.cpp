// $Id$

#include "Multiple_Impl.h"

ACE_RCSID (tests, server, "$Id$")

int main (int argc, char *argv[])
{

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Orb Initialization
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "TAO" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object;
      object = orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa = PortableServer::POA::_narrow(object.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      // Get the POAManager
      PortableServer::POAManager_var poa_manager = poa->the_POAManager(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create the servant.
      Bottom_Impl servant (orb.in ());

      // Create the delegated servant and intialize it
      // with the "real" servant.
      Delegated_Bottom_Impl delegated_servant(servant._this (),
                                              orb.in ());

      // Create the CORBA Object that is incarnated by the
      // delegated servant.
      Multiple::Bottom_var bottom = delegated_servant._this ();

      // Now we stringfy the object reference.
      CORBA::String_var ior =
        orb->object_to_string (bottom.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it

      FILE *output_file= ACE_OS::fopen ("s.ior", "w");

      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s"),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      // Activate the POAManager
      poa_manager->activate(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION, "Multiple Execution Interrupted Exception!\n");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}
