// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This simple test illustrates how to use advanced features of
//    <ACE_ARGV>.
//
// = AUTHOR
//    Suresh Kannan <kannan@uav.ae.gatech.edu>
//
// ============================================================================

#include "ace/ARGV.h"
#include "test_config.h"

ACE_RCSID(tests, ARGV_Test, "$Id$")

int
run_main (int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("ARGV_Test"));

  // From command line.
  ACE_ARGV cl (argv);

  // My own stuff.
  ACE_ARGV my;

  // Add to my stuff.
  my.add (ACE_TEXT ("-ORBEndpoint iiop://localhost:12345"));

  // Combine the two (see the ace/ARGV.h constructors documentation).
  ACE_ARGV a (cl.argv (),
              my.argv ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("argc = %d\n"),
              a.argc ()));

  // Print the contents of the combined <ACE_ARGV>.
  for (int i = 0; i < a.argc (); i++)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%d) %s\n"),
                i,
                a.argv ()[i]));

  ACE_END_TEST;
  return 0;
}
