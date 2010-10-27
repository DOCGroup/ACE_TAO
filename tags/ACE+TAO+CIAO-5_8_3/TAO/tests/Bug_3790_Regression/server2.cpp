// $Id$

#include "simple_testC.h"
#include "tao/ORB_Core.h"
#include "tao/IORTable/IORTable.h"
#include "tao/LocalObject.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

const ACE_TCHAR *testIOR = ACE_TEXT ("corbaloc:iiop:localhost:15989/SIMPLE_TEST_KEY");
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
  // Indicates successful parsing of the command line
  return 0;
}

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
Task::locate (const char *)
{
  CORBA::Object_var fwdObj = orb_->string_to_object(anotherIOR);
  CORBA::String_var fwdString = orb_->object_to_string(fwdObj.in ());

  ACE_DEBUG ((LM_DEBUG,
              "IN LOCATOR!  Forwarding client to: %C\n",
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
        CORBA::ORB_init (argc, argv, "MY_TEST_ORB");

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->resolve_initial_references("RootPOA");

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

      // Create task.
      Task *task = 0;
      ACE_NEW_RETURN (task,
                      Task (orb.in ()),
                      1);
      IORTable::Locator_var owner_transfer (task);

      // Resolve the IORTable and set the locator
      CORBA::Object_var iorTableObj =
        orb->resolve_initial_references("IORTable");

      IORTable::Table_var local_ior_table =
        IORTable::Table::_narrow (iorTableObj.in ());

      local_ior_table->set_locator (task);

      try
        {
          ACE_DEBUG ((LM_DEBUG,
                      "OK, now try to resolve a co-located "
                      "IORTable entry...\n"));

          CORBA::Object_var clientObj = orb->string_to_object (testIOR);

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
      catch(const CORBA::OBJ_ADAPTER &objAdapter)
        {
          objAdapter._tao_print_exception ("CAUGHT OBJ_ADAPTER!!!");
        }
      catch(const CORBA::OBJECT_NOT_EXIST &objNE)
        {
          objNE._tao_print_exception ("CAUGHT OBJECT NOT EXIST!!!");
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Leaving!\n"));
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("CAUGHT EXCEPTION: ");
      return -1;
    }

  return 0;
}
