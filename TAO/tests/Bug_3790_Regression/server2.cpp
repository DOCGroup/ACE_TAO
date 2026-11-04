#include "simple_testC.h"
#include "tao/ORB_Core.h"
#include "tao/IORTable/IORTable.h"
#include "tao/LocalObject.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

#include <string>

int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &test_ior,
            std::basic_string<ACE_TCHAR> &another_ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:a:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'a':
        test_ior = get_opts.opt_arg ();
        break;

      case 'k':
        another_ior = get_opts.opt_arg ();
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
  explicit Task (CORBA::ORB_ptr const orb,
                 std::basic_string<ACE_TCHAR> const &another_ior);
  Task (Task const &) = delete;
  Task (Task &&) = delete;
  Task &operator= (Task const &) = delete;
  Task &operator= (Task &&) = delete;

  char *locate (char const *) override;

private:
  CORBA::ORB_var orb_ {};
  std::basic_string<ACE_TCHAR> another_ior_;
};

char *
Task::locate (char const *)
{
  CORBA::Object_var const forward_object =
    orb_->string_to_object (ACE_TEXT_ALWAYS_CHAR (this->another_ior_.c_str ()));
  CORBA::String_var forward_string = orb_->object_to_string (forward_object.in ());

  ACE_DEBUG ((LM_DEBUG,
              "IN LOCATOR!  Forwarding client to: %C\n",
              forward_string.in ()));

  return forward_string._retn ();
}

Task::Task (CORBA::ORB_ptr const orb,
            std::basic_string<ACE_TCHAR> const &another_ior)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    another_ior_ (another_ior)
{
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv, "MY_TEST_ORB")};

      std::basic_string<ACE_TCHAR> test_ior {
        ACE_TEXT ("corbaloc:iiop:localhost:15989/SIMPLE_TEST_KEY")};
      std::basic_string<ACE_TCHAR> another_ior {ACE_TEXT ("file://test.ior")};
      if (parse_args (argc, argv, test_ior, another_ior) != 0)
        return 1;

      CORBA::Object_var const object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "rootPOA is nil!\n"));

          return -1;
        }

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      // Run ORB Event loop.
      poa_manager->activate ();

      // Create task.
      // Transfer the task pointer to the reference-counted locator owner below.
      Task *task {};
      ACE_NEW_RETURN (task,
                      Task (orb.in (), another_ior),
                      1);
      IORTable::Locator_var const owner_transfer (task);

      // Resolve the IORTable and set the locator
      CORBA::Object_var const ior_table_object =
        orb->resolve_initial_references ("IORTable");

      IORTable::Table_var const local_ior_table =
        IORTable::Table::_narrow (ior_table_object.in ());

      local_ior_table->set_locator (task);

      try
        {
          ACE_DEBUG ((LM_DEBUG,
                      "OK, now try to resolve a co-located "
                      "IORTable entry...\n"));

          CORBA::Object_var const client_object =
            orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (test_ior.c_str ()));

          simple::SimpleTest_T_var const simple_test_client =
            simple::SimpleTest_T::_narrow (client_object.in ());

          if (CORBA::is_nil (simple_test_client.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          "Simpe client is nil!\n"));
              return 1;
            }

          ACE_DEBUG ((LM_DEBUG,
                      "We narrowed the object, now let's invoke it!\n"));

          simple_test_client->shutdown ();
        }
      catch (CORBA::OBJ_ADAPTER const &objAdapter)
        {
          objAdapter._tao_print_exception ("CAUGHT OBJ_ADAPTER!!!");
        }
      catch (CORBA::OBJECT_NOT_EXIST const &objNE)
        {
          objNE._tao_print_exception ("CAUGHT OBJECT NOT EXIST!!!");
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Leaving!\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CAUGHT EXCEPTION: ");
      return -1;
    }

  return 0;
}
