
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  $Id$
 *
 *  Implementation of the server running the UDP object.
 *
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#include "ace/Get_Opt.h"
#include "ace/Thread_Manager.h"

#include "UDP_i.h"

// The following include file forces DIOP to be linked into the
// executable and initialized for static builds.
#include "tao/Strategies/advanced_resource.h"




const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");
int orb_threads = 1; // Number of threads running inside the ORB.

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'i':
        orb_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-i <ORB threads> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

static ACE_THR_FUNC_RETURN svc (void *arg)
{
  CORBA::ORB_ptr orb = (CORBA::ORB_ptr)arg;

  try
    {
      orb->run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception: orb->run");
    }
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "PerformanceServer");

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Install a persistent POA in order to achieve a persistent IOR
      // for our object.
      //*
      CORBA::PolicyList policies;
      policies.length (2);
      policies[0] =
        root_poa->create_lifespan_policy(PortableServer::PERSISTENT);

      policies[1] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      PortableServer::POA_var persistent_poa =
        root_poa->create_POA("persistent",
                             poa_manager.in (),
                             policies);

      policies[0]->destroy ();

      policies[1]->destroy ();

      if (parse_args (argc, argv) != 0)
        return 1;

      UDP_i udp_i;

      udp_i.orb (orb.in ());

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("UDP_Object");

      persistent_poa->activate_object_with_id (id.in (),
                                               &udp_i);

      CORBA::Object_var obj =
        persistent_poa->id_to_reference (id.in ());

      UDP_var udp_var = UDP::_narrow (obj.in ());

      if (CORBA::is_nil (udp_var.in ()))
        ACE_DEBUG ((LM_DEBUG,
                    "Failed to narrow correct object reference.\n"));

      CORBA::String_var ior =
        orb->object_to_string (udp_var.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      poa_manager->activate ();

      if (orb_threads > 1)
        ACE_Thread_Manager::instance ()->spawn_n (orb_threads-1,
                                                  svc,
                                                  orb.in ());

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "ORB finished\n"));

      root_poa->destroy (1, // ethernalize objects
                         0  // wait for completion
                         );

      orb->destroy ();

      ACE_Thread_Manager::instance ()->wait ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
