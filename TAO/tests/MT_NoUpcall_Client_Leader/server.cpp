#include "SharedIntf_i.h"
#include "worker.h"
#include "chatter.h"
#include "police.h"

#include "ace/SString.h"
#include "ace/streams.h"

int main(int argc, char* argv[])
{
  CORBA::ORB_var orb_;
  int result = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
  {
    orb_ = CORBA::ORB_init (argc, argv, "myorb-server" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    CORBA::Object_var poa_object =
      orb_->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (poa_object.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    PortableServer::POAManager_var poa_manager =
      root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
    PortableServer::POA_var poa = root_poa;

    poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    ACE_DEBUG((LM_INFO,"(%P|%t) START OF SERVER TEST\n"));
    ACE_DEBUG((LM_INFO,"(%P|%t) ORB initialized\n"));

    // Creating the servant and activating it
    //
    Test_Idl_SharedIntf_i* intf_i = new Test_Idl_SharedIntf_i(orb_.in());

    PortableServer::ServantBase_var base_var = intf_i;
    PortableServer::ObjectId_var intfId_var =
      poa->activate_object(base_var.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    CORBA::Object_var obj_var =
      poa->id_to_reference(intfId_var.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    Test_Idl::SharedIntf_var intf_var =
      Test_Idl::SharedIntf::_narrow(obj_var.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    // Creating stringified IOR of the servant and writing it to a file.
    //
    CORBA::String_var intfString_var =
      orb_->object_to_string(intf_var.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    ACE_CString ior_filename("server.ior");
    ofstream ior_filestream(ior_filename.c_str());
    ior_filestream << intfString_var.in() << endl;
    ior_filestream.close();

    ACE_DEBUG((LM_INFO,"(%P|%t) server IOR to %s\n",
      ior_filename.c_str()));

    // Running ORB in separate thread
    Worker worker (orb_.in ());
    //if (worker.activate (THR_NEW_LWP | THR_DETACHED, 1) != 0)
    if (worker.activate (THR_NEW_LWP | THR_JOINABLE, 1) != 0)
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "Cannot activate server thread(s)"), -1);

    ACE_DEBUG((LM_INFO,"(%P|%t) Await client initialization\n"));
    poll ("./client.ior");
    ACE_DEBUG((LM_INFO,"(%P|%t) Client IOR file was detected\n"));

    ACE_Mutex mutex;
    ACE_Condition<ACE_Mutex> stop_condition (mutex);

    Chatter worker2 (orb_.in (), "file://client.ior", stop_condition);
    if (worker2.activate (THR_NEW_LWP | THR_JOINABLE, 1) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "Cannot activate chatty client threads"), -1);
    }

    do {
      stop_condition.wait ();
      ACE_DEBUG((LM_INFO,"(%P|%t) So far, %d/%d requests/replies have been processed\n",
        worker2.nrequests (), worker2.nreplies ()));
    }
    while (worker2.nrequests () < 1);

    worker.thr_mgr()->wait ();

    root_poa->destroy(1, 1 ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    orb_->destroy(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    ACE_DEBUG((LM_INFO,"(%P|%t) Server Test %s\n",
      (worker2.nrequests() == worker2.nreplies())?"succeded":"failed"));
    result = (worker2.nrequests() == worker2.nreplies())? 0 : -1;
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Error: Exception caught:");
  }
  ACE_ENDTRY;

  ACE_OS::unlink ("server.ior");
  return result;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Condition<ACE_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate  ACE_Condition<ACE_Mutex>
#endif

