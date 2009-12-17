// $Id$

#include "simple_testC.h"
#include "tao/ORB_Core.h"
#include "tao/IORTable/IORTable.h"
#include "tao/LocalObject.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

const char *testIOR = "corbaloc:iiop:localhost:15989/SIMPLE_TEST_KEY";
const ACE_TCHAR *anotherIOR = ACE_TEXT ("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:a:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        testIOR = get_opts.opt_arg ();
        break;

      case 'k':
        anotherIOR = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior>"
                           "-a <cobraloc>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

//*************************************************************************
class Task
  : public virtual ACE_Task_Base,
    public virtual IORTable::Locator,
    public virtual CORBA::LocalObject
{
public:
  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb);

  int svc (void);

  char * locate (const char *);

private:
  CORBA::ORB_var orb_;
};

char *
Task::locate (const char *)
{
  CORBA::Object_var fwdObj = orb_->string_to_object(anotherIOR);
  CORBA::String_var fwdString = orb_->object_to_string(fwdObj);

  ACE_DEBUG ((LM_DEBUG,
              "IN LOCATOR!  Forwarding client to: %C\n",
              fwdString.in ()));

  return fwdString._retn();
}

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Task::svc (void)
{
  try
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "rootPOA is nil!\n"));

          return -1;
        }

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager();

      // Run ORB Event loop.
      poa_manager->activate ();

      // Resolve the IORTable and set the locator
      CORBA::Object_var iorTableObj =
        orb_->resolve_initial_references("IORTable");

      IORTable::Table_var local_ior_table =
        IORTable::Table::_narrow (iorTableObj.in ());

      local_ior_table->set_locator (this);

      try
        {
          ACE_DEBUG ((LM_DEBUG,
                      "OK, now try to resolve a co-located "
                      "IORTable entry...\n"));

          CORBA::Object_var clientObj = orb_->string_to_object (testIOR);

          simple::SimpleTest_T_var simpleTestClient =
            simple::SimpleTest_T::_narrow (clientObj.in ());

          if (CORBA::is_nil (simpleTestClient.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "Simpe client is nil!\n"));
              return 1;
            }

          ACE_DEBUG ((LM_DEBUG,
                      "We narrowed the object, now let's invoke it!\n"));

          simpleTestClient->shutdown ();
        }
      catch(CORBA::OBJ_ADAPTER &objAdapter)
        {
          objAdapter._tao_print_exception ("CAUGHT OBJ_ADAPTER!!!");
        }
      catch(CORBA::OBJECT_NOT_EXIST &objNE)
        {
          objNE._tao_print_exception ("CAUGHT OBJECT NOT EXIST!!!");
        }
      catch(CORBA::Exception &ex)
        {
          ex._tao_print_exception ("CAUGHT EXCEPTION: ");
        }
      catch(...)
        {
          ACE_ERROR ((LM_ERROR,
                      "CAUGHT UNEXPECTED EXCEPTION!\n"));
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Leaving task!\n"));
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR,
                  "Caught unexpected exception!\n"));
      return -1;
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "MY_TEST_ORB");

      if (parse_args (argc, argv) != 0)
        return 1;

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task *task = 0;
      ACE_NEW_RETURN (task,
                      Task (thread_manager, orb.in ()),
                      1);
      IORTable::Locator_var owner_transfer (task);

      // Task activation flags.
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      int result =
        task->activate (flags);

      if (result == -1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Cannot create thread with scheduling policy.  "
                      "Check svc.conf options.\n"));

          return -1;
        }

      thread_manager.wait ();
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR,
                  "Caught unexpected exception in main!\n"));
      return -1;
    }

  return 0;
}
