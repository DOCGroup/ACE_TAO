//$Id$

#include "Counter_i.h"
#include <tao/RT_ORB.h>
const char *ior_output_file = "ec.ior";
// @@ Angelo, lets change the name of ior file above.

//////////////////////////////////////////////////////////////////
//
// NOTE: In this test there are different work-arounds, that were
// needed to access some functionality not available at the time of
// writing the test.
// To point out these workarounds, the following convention will
// be used:
//
//   @@ Workaround <name>
//
//   @@ Real Code <name>

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

       // @@ Workaround <RTORB access>
      RTCORBA::RTORB_var rt_orb = new TAO_RT_ORB();

      // @@ Angelo, the line above is just a temporary code, so it's
      // ok.  However, in general, 1) do not use naked operator 'new'
      // - use ACE_NEW & friends; 2) don't allocate memory where not
      // neccessary, i.e., 'TAO_RT_ORB rt_orb;' will do just fine.

      //   @@ Real Code <RTORB access>

      /*
       object = orb->resolve_initial_references ("RTORB", ACE_TRY_ENV);
       ACE_TRY_CHECK;

       RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in (),
                                                            ACE_TRY_ENV);
       ACE_TRY_CHECK;
       */

      CORBA::PolicyList policy_list;
      policy_list.length (1);

      RTCORBA::Priority priority = 256;
      policy_list[0] =
        rt_orb->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                              priority,
                                              ACE_TRY_ENV);
      // @@ Angelo, don't forget to check 'ACE_TRY_CHECK' every time
      // you pass environment to a method.

      object = orb->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // @@ Angelo, please get into habit of thorough error checking,
      // e.g., it is good to check at this point that object is not nil.

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create a child POA with a given set of
      // policies.
      // @@ Angelo, why is this POA manager set to nil?  There is much
      // more confusion with poa managers at the end of main ...
      PortableServer::POAManager_var poa_mgr =
        PortableServer::POAManager::_nil ();

      PortableServer::POA_var child_poa =
        poa->create_POA ("Child_POA",
                         poa_mgr ,
                         policy_list);


      // @@ Angelo, the lines until TODO comment are garbage.
      // Why isn't the real code here yet?

      // Create a Corba Object reference.
      int i = 0;
      object =
        child_poa->create_reference ("IDL:omg.org/CORBA/Object:1.0",
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (object))
        {
          ACE_DEBUG ((LM_DEBUG, "Unable to create a Counter Object!"));
          return 1;
        }
      Counter_var counter = Counter::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Counter_Servant servant (orb.in ());

      // TODO: Incarnate the CORBA Object with  servant.

      CORBA::String_var ior =
        orb->object_to_string (counter.in (), ACE_TRY_ENV);
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

      PortableServer::POAManager_var poa_manager =
        child_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      the_poa_mngr->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }

  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "CORBA Exception Raised.");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
