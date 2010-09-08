// $Id$

#include "TestC.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

#if defined (ACE_OPENVMS)
// need this to circumvent link error on OpenVMS
// has to do with interference in template instantiations
// because multiple executables are build in one directory
// reuising eachothers object files.
ACE_Time_Value dum = ACE_Time_Value::zero;
#endif

int
parse_args (int argc, ACE_TCHAR *argv[]);

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int ret_val = 0;

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var object =
        orb->resolve_initial_references ("PolicyCurrent");

      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());

      if (CORBA::is_nil (policy_current.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Nil policy current\n"));
          return 1;
        }
      CORBA::Any scope_as_any;
      scope_as_any <<= Messaging::SYNC_WITH_SERVER;

      CORBA::PolicyList policies(1); policies.length (1);
      policies[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            scope_as_any);

      policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

      policies[0]->destroy ();

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Peer_var peer =
        Test::Peer::_narrow(tmp.in ());

      if (CORBA::is_nil (peer.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Peer reference <%s>\n",
                             ior),
                            1);
        }

      while(1)
      {
        peer->noop ();
      }

      /* Warning with TRU 64 builds
       ACE_DEBUG ((LM_DEBUG, "(%P|%t) scavenger - event loop finished\n"));

      orb->destroy ();
      */
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      ret_val = 1;
    }

  return ret_val;
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
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
  // Indicates sucessful parsing of the command line
  return 0;
}
