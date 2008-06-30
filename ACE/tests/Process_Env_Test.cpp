// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Process_Env_Test.cpp
//
// = DESCRIPTION
//      This program tests the limits of the Windows CreateProcess
//      environment buffer.
//
// = AUTHOR
//    Chad Elliott <elliott_c@ociweb.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/Process.h"

ACE_RCSID(tests,
          Process_Env_Test,
          "$Id$");

int
run_main (int, ACE_TCHAR*[])
{
  int test_status = 0;
  ACE_START_TEST (ACE_TEXT ("Process_Env_Test"));

#if defined (ACE_WIN32) && !defined (ACE_USES_WCHAR) && !defined (ACE_HAS_WINCE)
  ACE_Process_Options options (
                        0,
                        ACE_Process_Options::DEFAULT_COMMAND_LINE_BUF_LEN,
                        32 * 1024);
  options.command_line (ACE_TEXT ("attrib.exe /?"));
  static const size_t varsize = 1200;
  for(int i = 0; i < 26; i++)
    {
      char name[2] = { 'A' + i, '\0' };
      char value[varsize];
      memset(value, 'R', varsize);
      value[varsize - 1] = '\0';
      options.setenv (ACE_TEXT (name), ACE_TEXT (value));
  }


  ACE_OS::fclose(stdout);
  ACE_Process process;
  if (process.spawn (options) != -1)
    {
      ACE_ERROR ((LM_DEBUG,
                  "ERROR: This should have failed due to the large "
                  "environment buffer\n"));
      test_status = 1;
    }

  options.enable_unicode_environment ();
  if (process.spawn (options) == -1)
    {
      ACE_ERROR ((LM_DEBUG,
                  "ERROR: This should have succeeded\n"));
      test_status = 1;
    }
#else
  ACE_ERROR ((LM_INFO, "This test is for Win32 without ACE_USES_WCHAR\n"));
#endif /* ACE_WIN32 && !ACE_USES_WCHAR && !ACE_HAS_WINCE */

  ACE_END_TEST;
  return test_status;
}
