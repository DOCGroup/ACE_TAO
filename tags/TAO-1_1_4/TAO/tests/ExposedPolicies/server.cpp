//$Id$

#include "Counter_i.h"

const char *ior_output_file = "ec.ior";

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // ORB Initialization.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var object;

      object = orb->resolve_initial_references ("RootPoa", ACE_TRY_ENV);
      ACE_CHECK;

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // @@ Remove after poa interface is supported.
      TAO_POA *tao_poa = orb->orb_core ()->root_poa ();

      PortableServer::POAManager_var poa_manager
        = poa->the_POAManager(ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // @@ Angelo, activate when you are ready to receive requests.
      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      RTCORBA::Priority priority = 256;
      object = tao_poa->create_reference_with_priority ("IDL:Counter:1.0", priority);

      // @@ Angelo, please create child poa with RT policies, and
      // register two objects under it...
      // Also, please include all the test code in the file, and just
      // comment out what's not working at the moment.

      Counter_var counter = Counter::_narrow (object.in ());
      CounterServant servant;

      counter = servant._this ();
      // Angelo, use activate_object_with_priority instead of the
      // things you are doing above.  Then you can use
      // poa->id_to_reference or something like that to obtain the
      // object reference for your servant.

      CORBA::String_var ior = orb->object_to_string (counter.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      orb->run ();
      // @@ Angelo, check if this method needs ACE_TRY_ENV argument.
    }

  ACE_CATCHANY
    {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "CORBA Exception Raised.");
    return 1;
  }
  ACE_ENDTRY;

  return 0;
}
