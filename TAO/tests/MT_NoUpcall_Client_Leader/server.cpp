// $Id$

#include "SharedIntf_i.h"
#include "worker.h"
#include "chatter.h"
#include "police.h"

#include "ace/SString.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("server.ior");
int nr_threads = 1;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("t:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 't':
        nr_threads = ACE_OS::atoi(get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-t threads "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb_;
  int result = 0;

  try
  {
    ACE_DEBUG((LM_INFO,"(%P|%t) START OF SERVER TEST\n"));

    orb_ = CORBA::ORB_init (argc, argv, "myorb-server");

    if (parse_args (argc, argv) != 0)
      return 1;

    CORBA::Object_var poa_object =
      orb_->resolve_initial_references("RootPOA");

    PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (poa_object.in());

    PortableServer::POAManager_var poa_manager =
      root_poa->the_POAManager ();
    PortableServer::POA_var poa = root_poa;

    poa_manager->activate ();

    ACE_DEBUG((LM_INFO,"(%P|%t) ORB initialized\n"));

    // Creating the servant and activating it
    //
    Test_Idl_SharedIntf_i* intf_i = new Test_Idl_SharedIntf_i(orb_.in());

    PortableServer::ServantBase_var base_var = intf_i;
    PortableServer::ObjectId_var intfId_var =
      poa->activate_object(base_var.in());

    CORBA::Object_var obj_var =
      poa->id_to_reference(intfId_var.in());

    Test_Idl::SharedIntf_var intf_var =
      Test_Idl::SharedIntf::_narrow(obj_var.in());

    // Creating stringified IOR of the servant and writing it to a file.
    //
    CORBA::String_var intfString_var =
      orb_->object_to_string(intf_var.in());

    // Output the IOR to the <ior_output_file>
    FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
    if (output_file == 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                          "Cannot open output file for writing IOR: %s\n",
                          ior_output_file),
                          1);
    ACE_OS::fprintf (output_file, "%s", intfString_var.in ());
    ACE_OS::fclose (output_file);

    ACE_DEBUG((LM_INFO,"(%P|%t) server IOR to %s\n",
      ior_output_file));

    // Running ORB in separate thread
    Worker worker (orb_.in ());
    if (worker.activate (THR_NEW_LWP | THR_JOINABLE, nr_threads) != 0)
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "Cannot activate server thread(s)"), -1);

    ACE_DEBUG((LM_INFO,"(%P|%t) Await client initialization\n"));
    poll ("./client.ior");
    ACE_DEBUG((LM_INFO,"(%P|%t) Client IOR file was detected\n"));

    ACE_Mutex mutex;
    ACE_Condition<ACE_Mutex> stop_condition (mutex);
    {
      ACE_GUARD_RETURN (ACE_Mutex, guard, mutex, -1);

      Chatter worker2 (orb_.in (), ACE_TEXT("file://client.ior"), stop_condition);
      if (worker2.activate (THR_NEW_LWP | THR_JOINABLE, 1) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "Cannot activate chatty client threads"), -1);
        }

      do {
        stop_condition.wait ();
        ACE_DEBUG((LM_INFO,"(%P|%t) So far, %d/%d requests/replies have been processed\n",
                   worker2.nrequests (), worker2.nreplies ()));
      } while (worker2.nrequests () < 1);

      worker.thr_mgr()->wait ();

      root_poa->destroy(1, 1);

      orb_->destroy();

      ACE_DEBUG((LM_INFO,"(%P|%t) Server Test %C\n",
                 (worker2.nrequests() == worker2.nreplies())?"succeeded":"failed"));
      result = (worker2.nrequests() == worker2.nreplies())? 0 : -1;
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Error: Exception caught:");
  }

  ACE_OS::unlink ("server.ior");
  return result;
}

