#include "SharedIntf_i.h"

#include "ace/Task.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"
#include <memory>
#include <string>

std::basic_string<ACE_TCHAR> ior_output_file = ACE_TEXT ("server.ior");
std::string const upper_ior = "corbaloc::127.10.100.4:34999/bogus";
CORBA::ORB_var orb_;


class Worker : public ACE_Task_Base
{
public:
  Worker () = default;

  int svc () override
  {
    ACE_DEBUG ((LM_INFO, "(%P|%t) Running ORB in a separate thread\n"));
    try
      {
        orb_->run ();
      }
    catch (CORBA::Exception const&)
      {
      }
    return 0;
  }

  Worker (Worker const&) = delete;
  Worker (Worker&&) = delete;
  Worker& operator= (Worker const&) = delete;
  Worker& operator= (Worker&&) = delete;
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_DEBUG ((LM_INFO, "(%P|%t) SERVER START\n"));

      orb_ = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const obj = orb_->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var const mgr = poa->the_POAManager ();
      mgr->activate ();

      ACE_DEBUG ((LM_INFO, "(%P|%t) ORB initialized\n"));

      Test_Idl_SharedIntf_i * const intf_i = new Test_Idl_SharedIntf_i (orb_.in ());

      // Keep the servant pointer until ServantBase_var takes ownership below.
      PortableServer::ServantBase_var const base_var = intf_i;
      PortableServer::ObjectId_var const intfId_var =
        poa->activate_object (base_var.in ());

      CORBA::Object_var const intf_obj = poa->id_to_reference (intfId_var.in ());
      Test_Idl::SharedIntf_var const intf_var = Test_Idl::SharedIntf::_narrow (intf_obj.in ());
      CORBA::String_var const intfString_var = orb_->object_to_string (intf_var.in ());

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s\n",
                             ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", intfString_var.in ());
      }

      intf_i->set_upper (upper_ior);

      // Running ORB in separate thread
      Worker worker;
      if (worker.activate () != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) %p\n", "Cannot activate server thread(s)"),
                          -1);

      worker.thr_mgr()->wait ();
      orb_->destroy ();
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Error: Exception caught:");
    }

  ACE_OS::unlink (ior_output_file.c_str ());
  return 0;
}

