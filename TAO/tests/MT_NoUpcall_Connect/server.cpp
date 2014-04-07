// $Id$

#include "SharedIntf_i.h"

#include "ace/Task.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("server.ior");
const char *upper_ior = "corbaloc::127.10.100.4:34999/bogus";
CORBA::ORB_var orb_;


class Worker : public ACE_Task_Base
{
public:
  int svc (void)
  {
    ACE_DEBUG((LM_INFO,"(%P|%t) Running ORB in a separate thread\n"));
    try
      {
        orb_->run ();
      }
    catch (const CORBA::Exception&)
      {
      }
    return 0;
  }

};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;

  try
    {
      ACE_DEBUG((LM_INFO,"(%P|%t) SERVER START\n"));

      orb_ = CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj = orb_->resolve_initial_references("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow (obj.in());
      PortableServer::POAManager_var mgr = poa->the_POAManager ();
      mgr->activate ();

      ACE_DEBUG((LM_INFO,"(%P|%t) ORB initialized\n"));

      Test_Idl_SharedIntf_i* intf_i = new Test_Idl_SharedIntf_i(orb_.in());

      PortableServer::ServantBase_var base_var = intf_i;
      PortableServer::ObjectId_var intfId_var =
        poa->activate_object(base_var.in());

      obj = poa->id_to_reference(intfId_var.in());
      Test_Idl::SharedIntf_var intf_var = Test_Idl::SharedIntf::_narrow(obj.in());
      CORBA::String_var intfString_var = orb_->object_to_string(intf_var.in());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s\n",
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", intfString_var.in ());
      ACE_OS::fclose (output_file);

      intf_i->set_upper (upper_ior);

      // Running ORB in separate thread
      Worker worker;
      if (worker.activate () != 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) %p\n", "Cannot activate server thread(s)"),
                        -1);

      worker.thr_mgr()->wait ();
      orb_->destroy();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Error: Exception caught:");
    }

  ACE_OS::unlink (ior_output_file);
  return result;
}

