// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Capabilities_Test.cpp
//
// = DESCRIPTION
//      This is a test that makes sure the <ACE_Capabililties> class
//      works correctly.
//
// = AUTHOR
//    Arturo Montes <mitosys@colomsat.net.co>
//
// ============================================================================

#include "ace/Capabilities.h"
#include "test_config.h"

ACE_RCSID(tests, Capabilities_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

static const char config[] = "Capabilities_Test.cfg";

static int
load_config (void)
{
  ACE_Capabilities caps;
  if (caps.getent (config,
                   "Config") == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Can't read %s",
                       config),
                      1);

  int b = 0;
  caps.getval ("bool", b);
  ACE_DEBUG ((LM_DEBUG,
              "bool = %d\n",
              b));

  int n = 0;
  caps.getval ("integer", n);

  ACE_DEBUG ((LM_DEBUG,
              "integer = %d\n",
              n));

  ACE_CString s;
  caps.getval ("string", s);

  ACE_DEBUG ((LM_DEBUG,
              "string = %s\n",
              s.c_str ()));
  return 0;
}

int
main (int, char *[])
{
  ACE_START_TEST (ASYS_TEXT ("Capabilities_Test"));


  // --------------------------------------------------------
  // Create config file
  // --------------------------------------------------------

  // A config file is created within the test so that the test is
  // completely self contained.

  const char file_contents[] =
    "Config|Esta entrada reservada para la configuracion,\n"
    "   bool,\n"
    "   integer#2,\n"
    "   string=000030,\n\n";

  ACE_HANDLE fd = ACE_OS::open (config, O_RDWR | O_CREAT | O_TRUNC, 0600);

  if (fd == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_OS::open"), -1);


  if (ACE_OS::write (fd, file_contents, sizeof(file_contents)) !=
      sizeof(file_contents))
    {
      ACE_OS::unlink (config);
      ACE_OS::close (fd);
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_OS::write"), -1);
    }

  if (ACE_OS::close (fd) != 0)
    {
      ACE_OS::unlink (config);
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_OS::close"), -1);
    }
  // --------------------------------------------------------


  int result = load_config ();

  ACE_OS::unlink (config);

  ACE_END_TEST;
  return result;
}
