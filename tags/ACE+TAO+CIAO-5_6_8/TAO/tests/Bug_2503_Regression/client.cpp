// $Id$

#include "test_i.h"
#include "common.h"
#include "ace/Get_Opt.h"

void parse_args(int argc, ACE_TCHAR * argv[]);
void test_remote_calls(CORBA::ORB_ptr orb);
void test_colocated_calls(CORBA::ORB_ptr orb);

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    ACE_DEBUG ((LM_DEBUG, "Starting client\n"));
    CORBA::ORB_var orb = initialize_orb_and_poa(argc, argv);

    parse_args(argc, argv);

    ACE_DEBUG ((LM_DEBUG, "Testing remote\n"));
    test_remote_calls(orb.in());

    ACE_DEBUG ((LM_DEBUG, "Testing colocated\n"));
    test_colocated_calls(orb.in());

    ACE_DEBUG ((LM_DEBUG, "Testing ready\n"));
  }
  catch(...)
  {
    report_exception();
    return 1;
  }

  return 0;
}

const ACE_TCHAR *ior_argument = ACE_TEXT("file://test.ior");
int niterations = 100;

void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:i:"));
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
      {
      case 'k':
        ior_argument = get_opts.opt_arg ();
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        throw "Usage: [-k ior] [-i iteration_count]";
      }
  }
}

void test_impl(
    CORBA::ORB_ptr orb,
    ACE_TCHAR const * ior,
    bool shutdown)
{
  CORBA::Object_var object = orb->string_to_object(ior);
  Test_var test = Test::_narrow(object.in());

  if(CORBA::is_nil(test.in()))
  {
    throw "Nil reference after narrow";
  }

  for(int i = 0; i != niterations; ++i)
  {
    test->sendc_the_operation(AMI_TestHandler::_nil());
  }

  ACE_Time_Value wait_for_responses_interval(1, 0);
  orb->run(wait_for_responses_interval);

  if (shutdown)
    test->shutdown ();
}

void test_remote_calls(CORBA::ORB_ptr orb)
{
  test_impl(orb, ior_argument, true);
}

void test_colocated_calls(CORBA::ORB_ptr orb)
{
  test_i servant (orb);
  CORBA::String_var ior =
    servant.create_and_activate_server();

  test_impl(orb, ACE_TEXT_CHAR_TO_TCHAR(ior.in()), false);
}
