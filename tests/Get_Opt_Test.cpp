// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This program tests both the short and long option support in
//    <ACE_Get_Opt>, and demonstrates how to use it.
//
// = AUTHOR
//    Don Hinton <dhinton@dresystems.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/Get_Opt.h"
#include "ace/ARGV.h"
#include "ace/SString.h"
#include "ace/OS_NS_stdlib.h"
#include "test_config.h"



/*
 * This is the heart of the test. It sets up the optstring, instantiates
 * ACE_Get_Opt, add long options, processes them in a loop, and prints out
 * the results to the log.
 *
 * It returns 0 for success and 1 for error so we can keep track of the
 * total error count.
 */

static const ACE_TString empty_string (ACE_TEXT (""));

static int
parse_args (int test_number,
            int ordering,
            const ACE_TCHAR *test_args,
            int skip_argv = 1,
            int report_errors = 1,
            const ACE_TString &opt_prefix = empty_string)
{
  ACE_TString test;
  ACE_TString optstring (opt_prefix);

  // Test the skip_argv for the first test only.
  if (skip_argv > 0)
    {
      test = ACE_TEXT ("Test_");
      ACE_TCHAR s[20];
      test += ACE_OS::itoa (test_number, s, 10);
      test += ACE_TEXT (" ");
    }

  test += test_args;
  optstring += ACE_TEXT ("fr:o::sW;");

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT (" TEST %d   *****************************************")
              ACE_TEXT ("*******************\n"),
              test_number));
  ACE_DEBUG ((LM_INFO, "   Command line: \"%s\"\n", test.c_str ()));

  ACE_ARGV args (test.c_str ());

  ACE_Get_Opt get_opt (args.argc (),
                       args.argv (),
                       optstring.c_str (),
                       skip_argv,
                       report_errors,
                       ordering);

  // Now add the default long args.
  if (get_opt.long_option (ACE_TEXT ("flag"),
                           'f',
                           ACE_Get_Opt::NO_ARG) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("  Unable to add long option 'f'\n")), 1);

  if (get_opt.long_option (ACE_TEXT ("requires_arg"),
                           'r',
                           ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("  Unable to add long option 'r'\n")), 1);

  if (get_opt.long_option (ACE_TEXT ("optional_arg"),
                           'o',
                           ACE_Get_Opt::ARG_OPTIONAL) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("  Unable to add long option 'o'\n")), 1);

  if (get_opt.long_option (ACE_TEXT ("long_option"),
                           'l',
                           ACE_Get_Opt::ARG_OPTIONAL) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("  Unable to add long option 'l'\n")), 1);

  if (get_opt.long_option (ACE_TEXT ("long_only"),
                           -11,
                           ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("  Unable to add long option ")
                       ACE_TEXT ("\"long_only\"\n")), 1);

  if (get_opt.long_option (ACE_TEXT ("long_no_arg")) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("  Unable to add long option ")
                       ACE_TEXT ("\"long_no_arg\"\n")), 1);

  // This is the special case of providing a non-alpha numeric corresponding
  // short option.  This lets you use the corresponding short option in a
  // switch statement, even thought a meaningful short options isn't available
  // (afterall, there are only so many alpha numeric characters available).
  if (get_opt.long_option (ACE_TEXT ("non_alpha-num_short"),
                           -10,
                           ACE_Get_Opt::ARG_OPTIONAL) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("  Unable to add long option ")
                       ACE_TEXT ("\"non_alpha_short\"\n")), 1);

  // We print out the optstring here because adding long_options that
  // have corresponding short options that aren't yet present, are added.
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("      optstring: \"%s\"  skip_argv: %d\n"),
              get_opt.optstring (),  skip_argv));

  // Now, let's parse it...
  int c = 0;
  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 0:
          // Long Option.
          if (!get_opt.long_option ())
            ACE_ERROR_RETURN ((LM_ERROR, "  Long option doesn't exist.\n"), 1);

          ACE_DEBUG ((LM_INFO, "   Found long option \"%s\" %s %s\n",
                      get_opt.long_option (),
                      get_opt.opt_arg () ? ACE_TEXT ("with argument:")
                                         : ACE_TEXT (""),
                      get_opt.opt_arg () ? get_opt.opt_arg ()
                                         : ACE_TEXT ("")));
          break;
        case 'f':
          // This flag was added in both the optstring in the ctor and with
          // long_option().
        case 's':
          // This one is only short and has no long option.
          ACE_DEBUG ((LM_INFO, "   Found option flag '%s'\n",
                      get_opt.last_option ()));
          break;
        case 'r':
          // This one has a required argument, we wouldn't be here if the
          // arg were missing.  Note that we call get_opt.opt_arg () to return
          // the argument, but we could have used get_opt.opt_arg () since
          // opt_arg () is defined as "opt_arg ()".
          ACE_DEBUG ((LM_INFO,
                      "   Found option '%s' with required argument \"%s\"\n",
                      get_opt.last_option (), get_opt.opt_arg ()));
          break;
        case 'o':
          // This one has an optional argument.
        case 'l':
          // This short options was set automatically added to optstring.
          ACE_DEBUG ((LM_INFO,
                      "   Found option '%s' with optional argument \"%s\"\n",
                      get_opt.last_option (),
                      get_opt.opt_arg () ? get_opt.opt_arg ()
                                         : ACE_TEXT ("default")));
          break;
        case 1:
          // Non-option when in RETURN_IN_ORDER mode.
          ACE_DEBUG ((LM_INFO,
                      "   Found a non-option argument \"%s\" before finding "
                      "\"--\" (must be in RETURN_IN_ORDER mode).\n",
                      get_opt.opt_arg ()));
          break;
        case -10:
          // we found the short option that isn't alpha numeric.
          ACE_DEBUG ((LM_INFO,
                      "   Found option '%s' with optional argument \"%s\"\n",
                      get_opt.last_option (),
                      get_opt.opt_arg () ? get_opt.opt_arg ()
                                         : ACE_TEXT ("default")));
          break;
        case -11:
          // we found the short option that isn't alpha numeric.
          ACE_DEBUG ((LM_INFO,
                      "   Found option '%s' with argument \"%s\"\n",
                      get_opt.last_option (), get_opt.opt_arg ()));
          break;
        case ':':
          // This means an option requiring an argument didn't have one.
          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("   Option '%c' (%s) requires an argument ")
                      ACE_TEXT ("but none was supplied\n"),
                      get_opt.opt_opt (), get_opt.last_option ()));
          break;
        case '?':
          // An unrecognized option.
        default:
          // This is an error, perhaps you could handle them, but let's
          // just log it and keep going
          ACE_DEBUG ((LM_INFO,
                      "  Found an unknown option (%s) we couldn't handle.\n",
                      get_opt.last_option ()));
        }
    }

  // Print out the rest of the arguments left in the command line (if any).
  int index = 0;
  for (index = get_opt.opt_ind (); index < args.argc (); index++)
    ACE_DEBUG ((LM_INFO, "   Found non-option argument \"%s\"\n",
                args.argv ()[index]));

  // Now print them all so we can examine the permuted cmd line.
  for (index = 0; index < args.argc (); index++)
    ACE_DEBUG ((LM_INFO, "   argv[%u] \"%s\"\n",
                index, args.argv ()[index]));
  return 0;
}

/*
 * Add new tests cases here.  We increment the test number and pass the
 * type of ordering we want so that each test can be tested with multiple
 * ordering values in order to demostrate the difference.
 *
 * The return value is cumulative, and serves as a failure count that is
 * returned at the end of all the tests.
 */
static int
run_test (int& test_number, int ordering)
{
  int retval = 0;

  ACE_DEBUG ((LM_INFO,
              " ##########   Running Tests with ordering = %C   ##########\n",
              ordering == 1 ? "REQUIRE_ORDER" :
                              ordering == 2 ? "PERMUTE_ARGS" :
                                              "RETURN_IN_ORDER"));

  // Basic test, but don't use the program name and don't skip any args.
  retval += parse_args
    (test_number++, ordering,
     ACE_TEXT ("-f -rreq-arg -oopt-arg -lopt-arg --long_only=lo_arg -s arg1 arg2"),
     0);

  // Same, but combining short args that don't take args
  retval += parse_args
    (test_number++, ordering,
     ACE_TEXT ("-fsrreq-arg -oopt-arg -lopt-arg --long_only=lo_arg arg1 arg2"));

  // Now we use defaults for options with optional args.
  retval += parse_args
    (test_number++, ordering,
     ACE_TEXT ("-fsrreq-arg -o -l --long_only=lo_arg arg1 arg2"));

  // Let's mix up the options and non-options an see what happens.
  retval += parse_args
    (test_number++, ordering,
     ACE_TEXT ("-fs arg1 -rreq-arg -o arg2 -l --long_only=lo_arg"));

  // Now we turn off options parsing explicitely by passing "--" in the middle.
  retval += parse_args
    (test_number++, ordering,
     ACE_TEXT ("-fs -rreq-arg -- arg1 -o arg2 -l --long_only=lo_arg"));

  // Let's try the same thing but mix up the options and non-options.
  retval += parse_args
    (test_number++, ordering,
     ACE_TEXT ("-fs arg1 arg2 -rreq-arg -- arg3 -o"));

  // One more time but with "-W".
  retval += parse_args
    (test_number++, ordering,
     ACE_TEXT ("-fs arg1 arg2 -rreq-arg -W long_option=opt-arg -- arg3 -o"));

  // Let's pass some long options.
  retval += parse_args
    (test_number++, ordering,
     ACE_TEXT ("--flag -s --requires_arg=req-arg --optional_arg --long_only lo_arg arg1 arg2"));

  // And long options without the '='.
  retval += parse_args
    (test_number++, ordering,
     ACE_TEXT ("--flag -s --requires_arg req-arg --optional_arg --long_only=lo_arg arg1 arg2"));

  // Pass "-W" to cause the next argument to be read as a long option.
  retval += parse_args
    (test_number++, ordering,
     ACE_TEXT ("-fso -W requires_arg=req-arg -Woptional_arg -l arg1 arg2"));

  // This is the special case of a non-alpha numeric short option.
  retval += parse_args
    (test_number++, ordering,
     ACE_TEXT ("-fso --non_alpha-num_short=xxx arg1 arg2"));

  // Now, let's test some error conditions (we turn off report_errors since they are
  // intentional, we don't want to break the test script)
  int report_errors = 0;

  // Bad short option.
  retval += parse_args (test_number++, ordering, ACE_TEXT ("-X"), 1, report_errors);

  // Short option with missing required arg.
  ACE_TString report_missing (ACE_TEXT (":"));
  retval += parse_args (test_number++, ordering, ACE_TEXT ("-r"), 1, report_errors, report_missing);

  // Short option with missing required arg with trailing "--".
  // This reads "--" as the arg, but should it?
  retval += parse_args (test_number++, ordering, ACE_TEXT ("-r --"), 1, report_errors);

  // Long option with missing required arg.
  retval += parse_args (test_number++, ordering, ACE_TEXT ("--long_only"), 1, report_errors);

  // Long option that doesn't take an arg has one.
  retval += parse_args (test_number++, ordering, ACE_TEXT ("--long_no_arg=bad_arg"), 1, report_errors);

  // Unknown long option.
  retval += parse_args (test_number++, ordering, ACE_TEXT ("--unknown"), 1, report_errors);

  // Ambigous long option.
  retval += parse_args (test_number++, ordering, ACE_TEXT ("--long"), 1, report_errors);

  return retval;
}

int
run_main (int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Get_Opt_Test"));
  ACE_UNUSED_ARG (argv);
  int retval = 0;
  int test_number = 0;

  // Run the tests for each type of ordering.
  retval = run_test (test_number, ACE_Get_Opt::PERMUTE_ARGS);
  retval += run_test (test_number, ACE_Get_Opt::REQUIRE_ORDER);
  retval += run_test (test_number, ACE_Get_Opt::RETURN_IN_ORDER);

  ACE_END_TEST;
  return retval;
}
