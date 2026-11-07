#include "TestC.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"
#include <string>

struct Options
{
  std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Options options;
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var const object =
        orb->resolve_initial_references ("PolicyCurrent");
      CORBA::PolicyCurrent_var const policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());

      if (CORBA::is_nil (policy_current.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Nil policy current\n"));
          return 1;
        }

      CORBA::Any scope_as_any;
      scope_as_any <<= Messaging::SYNC_WITH_SERVER;
      CORBA::PolicyList policies (1);
      policies.length (1);
      policies[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            scope_as_any);
      policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);
      policies[0]->destroy ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const tmp = orb->string_to_object (options.ior.c_str ());
      Test::Peer_var const peer = Test::Peer::_narrow (tmp.in ());

      if (CORBA::is_nil (peer.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Nil Test::Peer reference <%s>\n",
                           options.ior.c_str ()),
                          1);

      for (;;)
        peer->noop ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }
  return 0;
}
