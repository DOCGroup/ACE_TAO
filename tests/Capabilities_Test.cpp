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

static int
load_config (void)
{
  const char *config = "Capabilities_Test.cfg";

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

  int result = load_config ();

  ACE_END_TEST;
  return result;
}
