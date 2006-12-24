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
//    Suresh Kannan <kannan@uav.ae.gatech.edu> and
//    Duane Binder <duane.binder@veritas.com>

// ============================================================================

#include "ace/ARGV.h"
#include "ace/Arg_Shifter.h"
#include "ace/OS_NS_string.h"
#include "ace/Argv_Type_Converter.h"
#include "test_config.h"

ACE_RCSID (tests, ARGV_Test, "$Id$")

static void
consume_arg (int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  if (arg_shifter.is_anything_left ()) 
    arg_shifter.consume_arg (1);
  // Once we initialize an arg_shifter, we must iterate through it all!
  while ((arg_shifter.is_anything_left ())) 
    arg_shifter.ignore_arg (1);
}

static int
test_simple_argv (char *argv[])
{
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
                ACE_TEXT (" (%d) %s\n"),
                i,
                a.argv ()[i]));

  return 0;
}

static int
test_argv_type_converter (void)
{
  char *argv[20];
  argv[0] = ACE_OS_String::strdup ("one");
  argv[1] = ACE_OS_String::strdup ("two");
  argv[2] = ACE_OS_String::strdup ("three");
  argv[3] = ACE_OS_String::strdup ("four");
  argv[4] = 0;

  char *save_argv[20];
  ACE_OS_String::memcpy (save_argv, argv, sizeof (argv));

  int argc = 4;

  {
    ACE_Argv_Type_Converter ct2 (argc, argv);
  }

  {
    ACE_Argv_Type_Converter ct (argc, argv);
    ct.get_argc (); ct.get_TCHAR_argv ();
    consume_arg ( ct.get_argc (), ct.get_TCHAR_argv ());
  }
  {
    ACE_Argv_Type_Converter ct3 (argc, argv);
    ct3.get_argc (); ct3.get_ASCII_argv ();
    consume_arg ( ct3.get_argc (), ct3.get_TCHAR_argv ());
  }

  { 
    for (size_t i = 0; i < 4; i++)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT (" (%d) %s\n"),
                  i,
                  ACE_TEXT_CHAR_TO_TCHAR (argv[i])));
  }

  for (size_t i = 0; save_argv[i]; ++i) 
    ACE_OS_Memory::free (save_argv[i]);

  return 0;
}

static int
test_argv_type_converter2 (void)
{
  ACE_TCHAR *argv[20];
  argv[0] = ACE_OS_String::strdup (ACE_TEXT ("one"));
  argv[1] = ACE_OS_String::strdup (ACE_TEXT ("two"));
  argv[2] = ACE_OS_String::strdup (ACE_TEXT ("three"));
  argv[3] = ACE_OS_String::strdup (ACE_TEXT ("four"));
  argv[4] = 0;

  ACE_TCHAR *save_argv[20];
  ACE_OS_String::memcpy (save_argv, argv, sizeof (argv));

  int argc = 4;

  {
    ACE_Argv_Type_Converter ct (argc, argv);
    ct.get_argc (); ct.get_TCHAR_argv ();
    consume_arg ( ct.get_argc (), ct.get_TCHAR_argv ());
  }

  consume_arg (argc, argv);

  {
    for (size_t i = 0; i < 4; i++)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT (" (%d) %s\n"),
                  i,
                  argv[i]));
  }

  for (size_t i = 0; save_argv[i]; ++i) 
    ACE_OS_Memory::free (save_argv[i]);

  return 0;
}

static int
test_argv_quotes (void)
{
  char *argv[] = { "first",
                   "'second in single quotes'",
                   "\"third in double quotes\""
                 };
  int argc = 3;                   

  // (argc, argv)
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n*** argv ***\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("argc: %d\n"), argc));

  for (int i = 0; i < argc; ++i)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("argv[%02d] = %s\n"), i, argv[i]));
 
  // args
  ACE_ARGV args;
  for (int i = 0; i < argc; ++i) 
    args.add (argv[i]);
  args.add (ACE_TEXT ("'fourth in single quotes'"));
  args.add (ACE_TEXT ("\"fifth in double quotes\""));
  args.add (ACE_TEXT ("sixth without any quotes"));
 
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n*** args-1 ***\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("args.argc (): %d\n"), args.argc ()));

  for (int i = 0; i < args.argc (); ++i)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("args[%02d]: %s\n"), i, args[i]));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("args.argc (): %d\n"), args.argc ()));
 
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n*** args-2 ***\n")));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("args.argc (): %d\n"), args.argc ()));

  for (int i = 0; i < args.argc (); ++i)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("args.argv ()[%02d]: %s\n"),
                i, args.argv ()[i]));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("args.argc (): %d\n"), args.argc ()));
 
  return 0;
}

int
run_main (int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("ARGV_Test"));

  test_simple_argv (argv);
  test_argv_type_converter2 ();
  test_argv_type_converter ();
  test_argv_quotes ();

  ACE_END_TEST;
  return 0;
}

