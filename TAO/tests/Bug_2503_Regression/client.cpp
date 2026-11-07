#include "test_i.h"
#include "common.h"
#include "ace/Get_Opt.h"

void parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior, int &iterations);
void test_remote_calls (CORBA::ORB_ptr orb, ACE_TCHAR const *ior, int iterations);
void test_colocated_calls (CORBA::ORB_ptr orb, int iterations);

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_DEBUG ((LM_DEBUG, "Starting client\n"));
      CORBA::ORB_var const orb = initialize_orb_and_poa (argc, argv);
      ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");
      int iterations {100};

      parse_args (argc, argv, ior, iterations);

      ACE_DEBUG ((LM_DEBUG, "Testing remote\n"));
      test_remote_calls (orb.in (), ior, iterations);

      ACE_DEBUG ((LM_DEBUG, "Testing colocated\n"));
      test_colocated_calls (orb.in (), iterations);

      ACE_DEBUG ((LM_DEBUG, "Testing ready\n"));
    }
  catch (...)
    {
      report_exception ();
      return 1;
    }

  return 0;
}

void
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior, int &iterations)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        throw "Usage: [-k ior] [-i iteration_count]";
      }
}

void
test_impl (CORBA::ORB_ptr orb, ACE_TCHAR const *ior, int iterations, bool shutdown)
{
  CORBA::Object_var const object = orb->string_to_object (ior);
  Test_var const test = Test::_narrow (object.in ());

  if (CORBA::is_nil (test.in ()))
    throw "Nil reference after narrow";

  for (int i {}; i != iterations; ++i)
    test->sendc_the_operation (AMI_TestHandler::_nil ());

  ACE_Time_Value wait_for_responses_interval (1, 0);
  orb->run (wait_for_responses_interval);

  if (shutdown)
    test->shutdown ();
}

void
test_remote_calls (CORBA::ORB_ptr orb, ACE_TCHAR const *ior, int iterations)
{
  test_impl (orb, ior, iterations, true);
}

void
test_colocated_calls (CORBA::ORB_ptr orb, int iterations)
{
  test_i const servant (orb);
  CORBA::String_var const ior =
    servant.create_and_activate_server ();

  test_impl (orb, ACE_TEXT_CHAR_TO_TCHAR (ior.in ()), iterations, false);
}
