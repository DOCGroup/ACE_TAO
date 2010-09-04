// -*- C++ -*-
// $Id$

#include "tao/IORManipulation/IORManip_Loader.h"
#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "testC.h"
#include "tao/TimeBaseC.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int do_shutdown = 0;

const ACE_TCHAR *name = ACE_TEXT("file://amba.ior");
TimeBase::TimeT timeout_period = 1000000;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xk:m:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = 1;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'm':
        name = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

CORBA::ULongLong
run_test (Simple_Server_ptr server);

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      // Primary server
      CORBA::Object_var object_primary =
        orb->string_to_object (ior);

      //Secondary server
      CORBA::Object_var object_secondary =
        orb->string_to_object (name);

      // Get an object reference for the ORBs IORManipultion object!
      CORBA::Object_ptr IORM =
        orb->resolve_initial_references (TAO_OBJID_IORMANIPULATION, 0);

      TAO_IOP::TAO_IOR_Manipulation_ptr iorm =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM);

      TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
      iors.length(2);
      iors [0] = object_primary;
      iors [1] = object_secondary;

      CORBA::Object_var merged = iorm->merge_iors (iors);

      CORBA::Object_var object =
        orb->resolve_initial_references ("PolicyCurrent");

      CORBA::PolicyCurrent_var policy_current =
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


      for (CORBA::ULong l = 0;
           l != policy_list.length ();
           ++l)
        {
          policy_list[l]->destroy ();
        }

      // Combined IOR stuff
      Simple_Server_var server =
        Simple_Server::_narrow (merged.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      CORBA::ULongLong freq =
        run_test (server.in ());

      if (freq != 919263)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) ERROR in the test\n")));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Shutting server down\n")));

      server->shutdown ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught an exception\n");
      return -1;
    }

  return 0;
}

CORBA::ULongLong
run_test (Simple_Server_ptr server)
{
  return server->remote_call ();
}
