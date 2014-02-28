// $Id: server.cpp

#include "TestServer.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Task.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("test.ior");

class Worker : public ACE_Task_Base
{
public:
  Worker(CORBA::ORB_ptr orb) : orb_(CORBA::ORB::_duplicate(orb))
  {
  }

  int svc(void)
  {
    orb_->run();
    return 0;
  }

private:
  CORBA::ORB_var orb_;
};

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      TestServer *test_impl = 0;
      ACE_NEW_RETURN (test_impl,
                      TestServer (),
                      1);
      PortableServer::ServantBase_var owner_transfer(test_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (test_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::TestServer_var test = Test::TestServer::_narrow (object.in ());

      CORBA::String_var ior = orb->object_to_string (test.in ());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s\n",
                           ior_output_file),
                           1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      // Run a CORBA worker thread
      Worker work (orb.in());
      work.activate (THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED, 1);
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - waiting for client to call\n"));
      ACE_OS::sleep (10);
      if (test_impl->got_callback() == false) {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - client did not make call\n"));
          return 1;
      }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - making repeated calls to client\n"));
      for (int i = 0; i < 10; i++)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - calling client attempt %d\n", i));
          test_impl->make_callback();
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - stopping client\n"));
      test_impl->shutdown_client();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - test completed\n"));

      orb->shutdown (); // shutdown our ORB

      work.wait (); // wait for the worker to finish

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
