// -*- C++ -*-
#include "tao/IORManipulation/IORManip_Loader.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "testC.h"
#include "tao/TimeBaseC.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"

struct Client_Options
{
  ACE_TString primary_ior {ACE_TEXT ("file://test.ior")};
  ACE_TString secondary_ior {ACE_TEXT ("file://amba.ior")};
};

constexpr TimeBase::TimeT timeout_period {1000000};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:m:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.primary_ior = get_opts.opt_arg ();
        break;

      case 'm':
        options.secondary_ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-m <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

CORBA::ULongLong
run_test (Simple_Server_ptr const server);

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Client_Options options;
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      // Primary server
      CORBA::Object_var const object_primary =
        orb->string_to_object (options.primary_ior.c_str ());

      // Secondary server
      CORBA::Object_var const object_secondary =
        orb->string_to_object (options.secondary_ior.c_str ());

      // Get an object reference for the ORBs IORManipultion object!
      CORBA::Object_var const iorm_object =
        orb->resolve_initial_references (TAO_OBJID_IORMANIPULATION, {});

      TAO_IOP::TAO_IOR_Manipulation_var const iorm =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (iorm_object.in ());

      TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
      iors.length (2);
      iors [0] = object_primary;
      iors [1] = object_secondary;

      CORBA::Object_var const merged = iorm->merge_iors (iors);

      CORBA::Object_var const object =
        orb->resolve_initial_references ("PolicyCurrent");

      CORBA::PolicyCurrent_var const policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());

      CORBA::Any timeout_as_any;
      timeout_as_any <<= timeout_period;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);

      policy_list[0] =
        orb->create_policy (TAO::CONNECTION_TIMEOUT_POLICY_TYPE,
                            timeout_as_any);

      policy_current->set_policy_overrides (policy_list,
                                            CORBA::ADD_OVERRIDE);

      for (CORBA::ULong l {};
           l != policy_list.length ();
           ++l)
        {
          policy_list[l]->destroy ();
        }

      // Combined IOR stuff
      Simple_Server_var const server = Simple_Server::_narrow (merged.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Object reference <%s> is nil.\n"),
                             options.primary_ior.c_str ()),
                            1);
        }

      CORBA::ULongLong const freq = run_test (server.in ());

      if (freq != 919263)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) ERROR in the test\n")));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Shutting server down\n")));

      server->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught an exception\n");
      return -1;
    }

  return 0;
}

CORBA::ULongLong
run_test (Simple_Server_ptr const server)
{
  return server->remote_call ();
}
