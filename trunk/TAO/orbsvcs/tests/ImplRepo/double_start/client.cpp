// $Id$

#include "TestC.h"
#include <iostream>
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "orbsvcs/Time_Utilities.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"

int request_delay_secs = 0;
const ACE_TCHAR *ior = ACE_TEXT ("");
CORBA::ORB_var orb;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:d:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        request_delay_secs = ACE_OS::atoi(get_opts.opt_arg ());
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-d <request delay in seconds> ")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

CORBA::Object_ptr
set_timeout_policy (CORBA::Object_ptr obj, const ACE_Time_Value& to)
{
  CORBA::Object_var ret;

  try
    {
      TimeBase::TimeT timeout;
      ORBSVCS_Time::Time_Value_to_TimeT (timeout, to);
      CORBA::Any tmp;
      tmp <<= timeout;

      CORBA::PolicyList policies (1);
      policies.length (1);
      policies[0] = orb->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                                        tmp);

      ret = obj->_set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

      policies[0]->destroy ();

      if (CORBA::is_nil (ret.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Unable to set timeout policy.\n")));
          ret = CORBA::Object::_duplicate (obj);
        }
    }
  catch (CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("set_timeout_policy ()"));
      ret = CORBA::Object::_duplicate (obj);
    }

  return ret._retn ();
}

void
do_restart_test (void)
{
  CORBA::Object_var obj = orb->string_to_object (ior);
  ACE_ASSERT (!CORBA::is_nil(obj.in ()));
  obj = set_timeout_policy (obj.in (), ACE_Time_Value (5,0));
  Test_var test = Test::_narrow( obj.in () );
  ACE_ASSERT (!CORBA::is_nil(test.in ()));

  try
    {
      test->arm ();
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("client caught %C during arm\n"),
                  ex._name ()));
      return;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("client sleeping %d seconds\n"),
              request_delay_secs));
  ACE_OS::sleep (request_delay_secs);
  try
    {
      test->trigger ();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("client trigger completed\n")));
      return;
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("client caught %C during first trigger\n"),
                  ex._name ()));
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("client second trigger\n")));
  test->trigger ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    orb = CORBA::ORB_init( argc, argv );

    if (parse_args (argc, argv) != 0)
      return 1;

    do_restart_test ();
    return 0;

  }
  catch(const CORBA::Exception& ex) {
    ex._tao_print_exception (ACE_TEXT ("client:"));
  }

  return -1;
}
