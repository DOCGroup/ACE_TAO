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

// main () has three arguments, so we can't use
// ACE_HAS_NONSTATIC_OBJECT_MANAGER.  We don't need it, anyways.
#include "ace/inc_user_config.h"
#if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER)
# undef ACE_HAS_NONSTATIC_OBJECT_MANAGER
#endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER */

#include "test_config.h"
#include "ace/OS.h"
#include "ace/Process.h"
#include "ace/Env_Value_T.h"

int
main (int argc, char *[], char* envp[])
{
  if (argc == 1)
    {
      // No arguments means we're the initial test
      ACE_Process_Options options (1);
      options.setenv (envp);

      options.command_line ("Env_Value_Test run_as_test");

      options.setenv("TEST_VALUE_POSITIVE", "%s", "10.2");
      options.setenv("TEST_VALUE_NEGATIVE", "%s", "-10.2");

      ACE_Process p;
      ACE_ASSERT (p.spawn (options) != -1);
      p.wait ();
    }
  else
    {
      // In this case we're the child
      ACE_START_TEST ("Env_Value_Test");

#define TEST_THIS(type,varname,defval,expval) do { ACE_Env_Value<type> val(varname, (defval)); ACE_ASSERT (val == (expval)); } while (0)

      TEST_THIS (int, "TEST_VALUE_POSITIVE", 4, 10);
      TEST_THIS (double, "TEST_VALUE_POSITIVE", -1.0, 10.2);
      TEST_THIS (long, "TEST_VALUE_POSITIVE", 0, 10);
      TEST_THIS (unsigned long, "TEST_VALUE_POSITIVE", 0, 10);
      TEST_THIS (short, "TEST_VALUE_POSITIVE", 0, 10);

      TEST_THIS (int, "TEST_VALUE_NEGATIVE", 4, -10);
      TEST_THIS (double, "TEST_VALUE_NEGATIVE", -1.0, -10.2);
      TEST_THIS (long, "TEST_VALUE_NEGATIVE", 0, -10L);
      TEST_THIS (unsigned long, "TEST_VALUE_NEGATIVE", 0, 0);
      TEST_THIS (short, "TEST_VALUE_NEGATIVE", 0, -10);

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
  template class ACE_Env_Value<unsigned long>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate  ACE_Env_Value<char*>
# pragma instantiate  ACE_Env_Value<double>
# pragma instantiate  ACE_Env_Value<int>
# pragma instantiate  ACE_Env_Value<long>
# pragma instantiate  ACE_Env_Value<short>
# pragma instantiate  ACE_Env_Value<unsigned long>
#endif
