// -*- C++ -*-
// $Id$

#include "File_i.h"
#include "tao/debug.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_fcntl.h"

static const ACE_TCHAR *ior_output_file = ACE_TEXT("ior");
static const int nthreads = 2;


class Worker : public ACE_Task_Base
{
  // = TITLE
  //   Run a server thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run server threads
  //
public:
  Worker (CORBA::ORB_ptr orb);
  // ctor

  virtual int svc (void);
  // The thread entry point.

private:
  CORBA::ORB_var orb_;
  // The orb
};


static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:d"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'd':
        TAO_debug_level++;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "[-oior_output_file]"
                           "[-d]"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_HANDLE handle = ACE_OS::open ("big.txt",
                                    O_RDWR | O_CREAT,
                                    ACE_DEFAULT_FILE_PERMS);

  ACE_OS::lseek (handle, 1024*1024*10, SEEK_SET);
  ACE_OS::write (handle, "", 1);
  ACE_OS::close (handle);

  try
    {
      // Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      int result = parse_args (argc, argv);
      if (result != 0)
        return result;

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // Narrow the object reference to a POA reference
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList policies (5);
      policies.length (5);

      // ID Assignment Policy
      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      // Lifespan Policy
      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

      // Request Processing Policy
      policies[2] =
        root_poa->create_request_processing_policy (PortableServer::USE_DEFAULT_SERVANT);

      // Servant Retention Policy
      policies[3] =
        root_poa->create_servant_retention_policy (PortableServer::RETAIN);

      // Id Uniqueness Policy
      policies[4] =
        root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID);

      ACE_CString name = "firstPOA";
      PortableServer::POA_var first_poa =
        root_poa->create_POA (name.c_str (),
                              poa_manager.in (),
                              policies);

      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          CORBA::Policy_ptr policy = policies[i];
          policy->destroy ();
        }

      // Create a File System Implementation object in first_poa
      FileImpl::System file_system_impl (first_poa.in ());

      PortableServer::ObjectId_var file_system_oid =
        PortableServer::string_to_ObjectId ("FileSystem");

      first_poa->activate_object_with_id (file_system_oid.in (),
                                          &file_system_impl);

      CORBA::Object_var file_system =
        first_poa->id_to_reference (file_system_oid.in ());

      // Get the IOR for the "FileSystem" object
      CORBA::String_var file_system_ior =
        orb->object_to_string (file_system.in ());

      ACE_DEBUG ((LM_DEBUG,"%C\n",
                  file_system_ior.in ()));

      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          -1);
      ACE_OS::fprintf (output_file, "%s", file_system_ior.in ());
      ACE_OS::fclose (output_file);

      // set the state of the poa_manager to active i.e ready to
      // process requests
      poa_manager->activate ();

      Worker worker (orb.in ());
      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client threads\n"),
                          1);

      worker.thr_mgr ()->wait ();

      first_poa->destroy(1, 1);

      root_poa->destroy(1, 1);

      orb->destroy();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("EXCEPTION CAUGHT");
      return -1;
    }

  return 0;
}


// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker::svc (void)
{
  try
    {
      // Run the ORB for atmost 75 seconds
      ACE_Time_Value tv (75, 0);
      this->orb_->run (tv);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Worker::svc");
      return -1;
    }

  return 0;
}
