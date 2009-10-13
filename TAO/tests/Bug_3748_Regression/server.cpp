// $Id$

#include "Hello.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Task.h"
#include "ace/ARGV.h"

ACE_RCSID (Hello,
           server,
           "$Id$")

const ACE_TCHAR *ior_output_file = ACE_TEXT ("test.ior");
const ACE_TCHAR *shutdown_ior_output_file = ACE_TEXT ("shutdown_test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:s:h"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 's':
        shutdown_ior_output_file = get_opts.opt_arg ();
        break;

      case 'h':
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-s <shutdown_iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

class Server_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Server_Task (CORBA::ORB_ptr orb,
               ACE_Thread_Manager *thr_mgr)
    : ACE_Task_Base (thr_mgr)
    , orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  /// Thread entry point
  int svc (void)
  {
    try
      {
        this->orb_->run ();
      }
    catch (const CORBA::Exception&)
      {
        return -1;
      }
    return 0;
  }

private:
  /// Reference to the ORB
  CORBA::ORB_var orb_;
};

int
prepare_one_server (int argc, ACE_TCHAR *argv[],
                    ACE_TCHAR const *output_filename,
                    CORBA::ORB_out orb,
                    CORBA::ORB_ptr shutdown_orb,
                    PortableServer::POA_out root_poa,
                    Hello *&hello_impl)
{
  try
    {
      // Use output_filename as an ORB's name, just to
      // obtain a unique ORB.
      orb =
        CORBA::ORB_init (argc, argv,
                         ACE_TEXT_ALWAYS_CHAR (output_filename));

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      ACE_NEW_RETURN (hello_impl,
                      Hello (orb, shutdown_orb),
                      1);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (hello_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Hello_var hello = Test::Hello::_narrow (object.in ());

      CORBA::String_var ior = orb->object_to_string (hello.in ());

      // Output the IOR to the <output_filename>
      FILE *output_file= ACE_OS::fopen (output_filename, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s\n",
                           output_filename),
                           1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_ARGV_T<ACE_TCHAR> saved;
      for (int i = 0; i < argc; ++i)
        {
          saved.add (argv[i]);
        }
      saved.add (ACE_TEXT ("-ORBIPHopLimit"));
      saved.add (ACE_TEXT ("10000"));

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::ORB_var orb;
      CORBA::ORB_out orb_param (orb);
      PortableServer::POA_var root_poa;
      PortableServer::POA_out root_poa_param (root_poa);

      Hello *hello_impl = 0;
      if (prepare_one_server (saved.argc (), saved.argv (), ior_output_file,
          orb_param, CORBA::ORB::_nil (), root_poa_param, hello_impl))
        return 1;
      PortableServer::ServantBase_var owner_transfer(hello_impl);

      CORBA::ORB_var shutdown_orb;
      CORBA::ORB_out shutdown_orb_param (shutdown_orb);
      PortableServer::POA_var shutdown_root_poa;
      PortableServer::POA_out shutdown_root_poa_param (shutdown_root_poa);

      Hello *shutdown_hello_impl = 0;
      if (prepare_one_server (argc, argv, shutdown_ior_output_file,
                              shutdown_orb_param, orb.in (),
                              shutdown_root_poa_param,
                              shutdown_hello_impl))
        return 1;
      PortableServer::ServantBase_var shutdown_owner_transfer(shutdown_hello_impl);

      Server_Task server_task (shutdown_orb.in (),
                               ACE_Thread_Manager::instance ());
      if (server_task.activate (THR_NEW_LWP | THR_JOINABLE, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating server task\n"));
        }

      orb->run ();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));
      root_poa->destroy (1, 1);
      orb->destroy ();

      ACE_Thread_Manager::instance ()->wait ();
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) shutdown server - event loop finished\n"));
      shutdown_root_poa->destroy (1, 1);
      shutdown_orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
