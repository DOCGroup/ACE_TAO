// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//      This is a simple test to make sure the Env_Value template is
//      working correctly.
//
// = AUTHOR
//    Chris Cleeland
//
// ============================================================================

#include "test_config.h"
#include "ace/OS.h"
#include "ace/Process.h"
#include "ace/Env_Value_T.h"

int
#if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
// ACE_HAS_NONSTATIC_OBJECT_MANAGER only allows main to have two
// arguments.  And on VxWorks, which uses ACE_HAS_NONSTATIC_OBJECT_MANAGER,
// we can't use spawn because it doesn't have fork ().
main (int argc, LPTSTR [])
{
      ACE_UNUSED_ARG (argc);
      ACE_OS::putenv("TEST_VALUE_POSITIVE=10.2");
      ACE_OS::putenv("TEST_VALUE_NEGATIVE=-10.2");
#else  /* ! ACE_HAS_NONSTATIC_OBJECT_MANAGER */
main (int argc, LPTSTR [], LPTSTR envp[])
{
  if (argc == 1)
    {
      // No arguments means we're the initial test
      ACE_Process_Options options (1);
      options.setenv (envp);

      options.command_line (__TEXT ("Env_Value_Test run_as_test"));

      options.setenv(__TEXT ("TEST_VALUE_POSITIVE"), __TEXT ("%s"), 
                     __TEXT ("10.2"));
      options.setenv(__TEXT ("TEST_VALUE_NEGATIVE"), __TEXT ("%s"), 
                     __TEXT ("-10.2"));

      ACE_Process p;
      pid_t result = p.spawn (options);
      ACE_ASSERT (result != -1);
      p.wait ();
    }
  else
#endif /* ! ACE_HAS_NONSTATIC_OBJECT_MANAGER */
    {
      // In this case we're the child
      ACE_START_TEST ("Env_Value_Test");

#define TEST_THIS(type,varname,defval,expval) \
      do \
        { \
          ACE_Env_Value<type> val (varname, (defval)); \
          if (val != (expval)) \
            { \
              ACE_ERROR ((LM_ERROR, \
                          "val %d does not match expected value of %d\n", \
                          (int) (type) val, (int) (expval))); \
            } \
          ACE_ASSERT (val == (expval)); \
        } \
      while (0)

      TEST_THIS (int, "TEST_VALUE_POSITIVE", 4, 10);
      TEST_THIS (double, "TEST_VALUE_POSITIVE", -1.0, 10.2);
      TEST_THIS (long, "TEST_VALUE_POSITIVE", 0, 10);
      TEST_THIS (unsigned long, "TEST_VALUE_POSITIVE", 0, 10);
      TEST_THIS (short, "TEST_VALUE_POSITIVE", 0, 10);
      TEST_THIS (unsigned short, "TEST_VALUE_POSITIVE", 0, 10);

      TEST_THIS (int, "TEST_VALUE_NEGATIVE", 4, -10);
      TEST_THIS (double, "TEST_VALUE_NEGATIVE", -1.0, -10.2);
      TEST_THIS (long, "TEST_VALUE_NEGATIVE", 0, -10L);
      TEST_THIS (unsigned long, "TEST_VALUE_NEGATIVE", 0, (unsigned long) -10);
      TEST_THIS (short, "TEST_VALUE_NEGATIVE", 0, -10);
      TEST_THIS (unsigned short, "TEST_VALUE_NEGATIVE", 0, (unsigned short) -10);

      char* defstr = "Sarah Cleeland is Two!";
      ACE_Env_Value<char*> sval("This_Shouldnt_Be_Set_Hopefully", defstr);
      ACE_ASSERT (ACE_OS::strcmp (sval, defstr) == 0);

      ACE_END_TEST;
    }

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
  template class ACE_Env_Value<char*>;
  template class ACE_Env_Value<double>;
  template class ACE_Env_Value<int>;
  template class ACE_Env_Value<long>;
  template class ACE_Env_Value<short>;
  template class ACE_Env_Value<unsigned short>;
  template class ACE_Env_Value<unsigned long>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate  ACE_Env_Value<char*>
# pragma instantiate  ACE_Env_Value<double>
# pragma instantiate  ACE_Env_Value<int>
# pragma instantiate  ACE_Env_Value<long>
# pragma instantiate  ACE_Env_Value<short>
# pragma instantiate  ACE_Env_Value<unsigned short>
# pragma instantiate  ACE_Env_Value<unsigned long>
#endif
