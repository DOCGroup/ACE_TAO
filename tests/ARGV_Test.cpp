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
main (int, char *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("ARGV_Test"));

  // From command line.
  ACE_ARGV cl (argv);

  // My own stuff.
  ACE_ARGV my;		

  // Add to my stuff.
  my.add ("-ORBEndpoint iiop://localhost:12345");

  // Combine the two (see the ace/ARGV.h constructors documentation).
  ACE_ARGV a (cl.argv (),
              my.argv ());

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("argc = %d\n"),
              ASYS_TEXT (a.argc ())));

  // Print the contents of the combined <ACE_ARGV>.
  for (size_t i = 0; i < a.argc (); i++)
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("(%d) %s\n"),
                i,
                a.argv ()[i]));

  ACE_END_TEST;
  return 0;
}
