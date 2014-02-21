
//=============================================================================
/**
 *  @file    Process_Env_Test.cpp
 *
 *  $Id$
 *
 * This program tests the limits of the Windows CreateProcess
 * environment buffer.
 *
 *  @author Chad Elliott <elliott_c@ociweb.com>
 */
//=============================================================================

#include "test_config.h"
#include "ace/Process.h"
#include "ace/SString.h"

typedef void (*setenvfn_t) (const ACE_TCHAR *name, const ACE_TCHAR *value,
                            void *ctx);

#if defined (ACE_WIN32) && !defined (ACE_USES_WCHAR) && !defined (ACE_HAS_WINCE)

void create_large_env (setenvfn_t setenv, void *ctx)
{
  static const size_t varsize = 1600;
  for (int i = 0; i < 26; i++)
    {
      char name[2] = { 'A', '\0' };
      name[0] += i;
      char value[varsize];
      ACE_OS::memset (value, 'R', varsize);
      value[varsize - 1] = '\0';
      setenv (ACE_TEXT_CHAR_TO_TCHAR (name),
              ACE_TEXT_CHAR_TO_TCHAR (value),
              ctx);
    }
}


void apo_setenv (const ACE_TCHAR *name, const ACE_TCHAR *value, void *ctx)
{
  ACE_Process_Options *apo = static_cast<ACE_Process_Options *> (ctx);
  apo->setenv (name, value);
}


void thisproc_setenv (const ACE_TCHAR *name, const ACE_TCHAR *value, void *)
{
  ACE_TString putstr (name);
  putstr += ACE_TEXT ('=');
  putstr += value;
  ACE_OS::putenv (putstr.c_str ());
}

#endif

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
  create_large_env (apo_setenv, &options);

  ACE_OS::fclose(stdout);
  ACE_Process process;
  if (process.spawn (options) != -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: This should have failed due to the large "
                  "environment buffer\n"));
      test_status = 1;
    }

  options.enable_unicode_environment ();
  if (process.spawn (options) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: This should have succeeded\n"));
      test_status = 1;
    }

  // Test inheriting a large env block with enable_unicode_environment
  ACE_Process_Options opts2 (1,
                             ACE_Process_Options::DEFAULT_COMMAND_LINE_BUF_LEN,
                             128 * 1024);
  create_large_env (thisproc_setenv, 0);
  opts2.enable_unicode_environment ();
  opts2.setenv (ACE_TEXT ("ZZ"), ACE_TEXT ("1"));
  opts2.command_line (ACE_TEXT ("cmd.exe /d /c ")
    ACE_TEXT ("\"if defined Z (exit 1) else (exit 2)\""));
  ACE_Process process2;
  if (process2.spawn (opts2) == -1)
    {
       ACE_ERROR ((LM_ERROR,
                  "ERROR: Failed to spawn process2.\n"));
      test_status = 1;
    }
  ACE_exitcode status;
  process2.wait (&status);
  if (status != 1)
    {
       ACE_ERROR ((LM_ERROR,
                  "ERROR: process2 did not inherit env var Z.\n"));
      test_status = 1;
    }

#else
  ACE_DEBUG ((LM_INFO, "This test is for Win32 without ACE_USES_WCHAR\n"));
#endif /* ACE_WIN32 && !ACE_USES_WCHAR && !ACE_HAS_WINCE */

  ACE_END_TEST;
  return test_status;
}
