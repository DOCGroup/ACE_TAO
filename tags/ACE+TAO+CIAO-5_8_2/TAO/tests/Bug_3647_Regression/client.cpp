// -*- C++ -*-
// $Id$

#include "TestC.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://middle.ior");
bool verbose = true;
long timeout = 2;

void
usage(ACE_TCHAR const *cmd,
      ACE_TCHAR const *msg)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT("usage:  %s ")
              ACE_TEXT("-v ")
              ACE_TEXT("-k <ior> ")
              ACE_TEXT("-t timeout ")
              ACE_TEXT("\n")
              ACE_TEXT("        %s\n"),
              cmd, msg));
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("vk:t:"));
  int c;
  ACE_TCHAR const *stimeout = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'v':
        verbose = true;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 't':
        stimeout = get_opts.opt_arg();
        break;

      case '?':
      default:
        usage(argv[0], ACE_TEXT("unknown argument"));
        return -1;
      }

  if (stimeout != 0)
  {
    ACE_TCHAR *end;
    long tmp = ACE_OS::strtol(stimeout, &end, 10);
    if (end == 0 || *end != '\0')
    {
      usage(argv[0], ACE_TEXT("Invalid timeout value"));
      return -1;
    }
    timeout = tmp;
  }

  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp = orb->string_to_object(ior);

      // one second in TimeT units
      TimeBase::TimeT const second = 10 * TimeBase::TimeT(1000000);

      CORBA::Any timeout_as_any;
      timeout_as_any <<= TimeBase::TimeT(timeout * second);

      TAO::Utils::PolicyList_Destroyer plist(1);
      plist.length(1);
      plist[0] =
          orb->create_policy(Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
              timeout_as_any);

      tmp = tmp->_set_policy_overrides(plist, CORBA::SET_OVERRIDE);

      Bug_3647_Regression::Middle_var middle =
          Bug_3647_Regression::Middle::_narrow(tmp.in ());

      if (CORBA::is_nil (middle.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                  "client(%P|%t) - nil "
                  "Bug_3647_Regression::Middle reference <%s>\n",
                  ior),
              1);
        }

      // Startup the tests ...
      middle->startup_test();

      ACE_DEBUG ((LM_DEBUG, "client(%P|%t) - test started up\n"));

      int const iterations = 10000;
      int const interval = iterations / 20;
      ACE_DEBUG ((LM_DEBUG, "client(%P|%t) - running pings"));
      for (int i = 0; i != iterations; ++i)
      {
        middle->ping();
        if (i % interval == 0 && i > 0)
        {
          ACE_DEBUG((LM_DEBUG, "."));
        }
      }
      ACE_DEBUG ((LM_DEBUG, "done\n"));

      middle->shutdown ();
      ACE_DEBUG ((LM_DEBUG,
              "client(%P|%t) - server shutdown request sent\n"));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG,
              "client"));
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
