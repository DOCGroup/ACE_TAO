/**
 *  @file    Capabilities_Test.cpp
 *
 *    This is a test that makes sure the @c ACE_Capabililties class
 *    works correctly.
 *
 *  @author Arturo Montes <mitosys@colomsat.net.co>
 */

#include "test_config.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Capabilities.h"

static ACE_TCHAR const config[] = ACE_TEXT ("Capabilities_Test.cfg");

static int
load_config ()
{
  ACE_Capabilities caps;
  if (caps.getent (config, ACE_TEXT ("Config")) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Can't read %s\n"),
                         config),
                        1);
    }

  int b = 0;
  if (caps.getval (ACE_TEXT ("bool"), b) != 0 || b != 1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Incorrect boolean capability\n")), 1);
    }
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("bool = %d\n"),
              b));

  int n = 0;
  if (caps.getval (ACE_TEXT ("integer"), n) != 0 || n != 2)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Incorrect integer capability\n")), 1);
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("integer = %d\n"),
              n));

  ACE_TString s;
  if (caps.getval (ACE_TEXT ("string"), s) != 0 || s != ACE_TEXT ("000030"))
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Incorrect string capability\n")), 1);
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("string = %s\n"),
              s.c_str ()));

  if (caps.getval (ACE_TEXT ("escape"), s) != 0 || s != ACE_TEXT ("\033value"))
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Incorrect escaped capability\n")), 1);
    }

  if (caps.getent (config, ACE_TEXT ("Config")) != 0
      || caps.getval (ACE_TEXT ("integer"), n) != 0 || n != 2)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Reloading capabilities failed\n")), 1);
    }
  return 0;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Capabilities_Test"));

  // Create config file

  // A config file is created within the test so that the test is
  // completely self contained.

  char const file_contents[] =
    "Config|Esta entrada reservada para la configuracion,\n"
    "   bool,\n"
    "   bool,\n"
    "   integer#2,\n"
    "   integer#3,\n"
    "   string=000030,\n"
    "   string=ignored,\n"
    "   escape=\\Evalue,\n\n";

  ACE_HANDLE const fd = ACE_OS::open (config,
                                O_RDWR | O_CREAT | O_TRUNC,
                                ACE_DEFAULT_FILE_PERMS);

  if (fd == ACE_INVALID_HANDLE)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("ACE_OS::open")),
                        -1);
    }

  if (ACE_OS::write (fd, file_contents, sizeof (file_contents)) !=
      static_cast<ssize_t> (sizeof (file_contents)))
    {
      ACE_OS::unlink (config);
      ACE_OS::close (fd);
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("ACE_OS::write")),
                        -1);
    }

  if (ACE_OS::close (fd) != 0)
    {
      ACE_OS::unlink (config);
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("ACE_OS::close")),
                        -1);
    }

  int const result = load_config ();

  ACE_OS::unlink (config);

  ACE_END_TEST;
  return result;
}
