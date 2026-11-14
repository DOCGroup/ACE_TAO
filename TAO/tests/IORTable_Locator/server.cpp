#include "simple_test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/ORB_Core.h"
#include "tao/IORTable/IORTable.h"
#include "tao/LocalObject.h"
#include "tao/PortableServer/PortableServer.h"
#include <string>

//*************************************************************************
class Task
  : public virtual IORTable::Locator,
    public virtual CORBA::LocalObject
{
public:
  explicit Task (CORBA::ORB_ptr orb, std::string const &test_ior);
  Task (Task const &) = delete;
  Task (Task &&) = delete;
  Task &operator= (Task const &) = delete;
  Task &operator= (Task &&) = delete;

  char *locate (char const *) override;

private:
  CORBA::ORB_var orb_;
  std::string test_ior_;
};

char *
Task::locate (char const *id)
{
  CORBA::Object_var const fwd_object = this->orb_->string_to_object (this->test_ior_.c_str ());
  CORBA::String_var fwd_string = this->orb_->object_to_string (fwd_object.in ());

  ACE_DEBUG ((LM_DEBUG,
              "IN LOCATOR!  Forwarding client %C to: %C\n",
              id,
              fwd_string.in ()));

  return fwd_string._retn ();
}

Task::Task (CORBA::ORB_ptr orb, std::string const &test_ior)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    test_ior_ (test_ior)
{
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      Simple_Test_i *test_impl {};
      ACE_NEW_RETURN (test_impl,
                      Simple_Test_i (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer (test_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (test_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      simple::SimpleTest_T_var const the_server =
        simple::SimpleTest_T::_narrow (object.in ());

      CORBA::String_var const test_ior = orb->object_to_string (the_server.in ());

      poa_manager->activate ();

      // Create task.
      Task *task {};
      ACE_NEW_RETURN (task,
                      Task (orb.in (), test_ior.in ()),
                      1);
      IORTable::Locator_var const iorloc_transfer (task);

      // Resolve the IORTable and set the locator
      CORBA::Object_var const ior_table_object = orb->resolve_initial_references ("IORTable");

      IORTable::Table_var const local_ior_table = IORTable::Table::_narrow (ior_table_object.in ());

      local_ior_table->set_locator (iorloc_transfer.in ());

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "Server - event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Server - Exception caught:");
      return 1;
    }

  return 0;
}
