// $Id$

#include "simple_test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/ORB_Core.h"
#include "tao/IORTable/IORTable.h"
#include "tao/LocalObject.h"
#include "tao/PortableServer/PortableServer.h"

CORBA::String_var testIOR;

//*************************************************************************
class Task
  : public virtual IORTable::Locator,
    public virtual CORBA::LocalObject
{
public:
  Task (CORBA::ORB_ptr orb);

  char * locate (const char *);

private:
  CORBA::ORB_var orb_;
};

char *
Task::locate (const char * id)
{
  CORBA::Object_var fwdObj = orb_->string_to_object(testIOR.in ());
  CORBA::String_var fwdString = orb_->object_to_string(fwdObj.in ());

  ACE_DEBUG ((LM_DEBUG,
              "IN LOCATOR!  Forwarding client %C to: %C\n",
              id,
              fwdString.in ()));

  return fwdString._retn();
}

Task::Task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      Simple_Test_i *test_impl = 0;
      ACE_NEW_RETURN (test_impl,
                      Simple_Test_i (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer (test_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (test_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      simple::SimpleTest_T_var theServer =
        simple::SimpleTest_T::_narrow (object.in ());

      testIOR = orb->object_to_string (theServer.in ());

      poa_manager->activate ();

      // Create task.
      Task *task = 0;
      ACE_NEW_RETURN (task,
                      Task (orb.in ()),
                      1);
      IORTable::Locator_var iorloc_transfer (task);

      // Resolve the IORTable and set the locator
      CORBA::Object_var iorTableObj =
        orb->resolve_initial_references("IORTable");

      IORTable::Table_var local_ior_table =
        IORTable::Table::_narrow (iorTableObj.in ());

      local_ior_table->set_locator (task);

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "Server - event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Server - Exception caught:");
      return 1;
    }

  return 0;
}
