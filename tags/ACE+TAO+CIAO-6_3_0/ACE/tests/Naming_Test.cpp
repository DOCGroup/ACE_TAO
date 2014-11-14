
//=============================================================================
/**
 *  @file    Naming_Test.cpp
 *
 *  $Id$
 *
 *    This is a test to illustrate the Naming Services. The test
 *    does binds, rebinds, finds, and unbinds on name bindings using
 *    the local naming context.
 *
 *
 *  @author Prashant Jain <pjain@cs.wustl.edu> and Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "randomize.h"
#include "ace/Lib_Find.h"
#include "ace/SString.h"
#include "ace/Naming_Context.h"
#include "ace/Profile_Timer.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"



static char name[BUFSIZ];
static char value[BUFSIZ];
static char type[BUFSIZ];

void
initialize_array (int * array, int size)
{
  for (int n = 0; n < size; ++n)
    array[n] = n;
}

static void
print_time (ACE_Profile_Timer &timer,
            const char *test)
{
  ACE_Profile_Timer::ACE_Elapsed_Time et;
  timer.stop ();
  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("     *****  %C  *****    \n"), test));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("real time = %f secs, user time = %f secs, system time = %f secs\n"),
              et.real_time, et.user_time, et.system_time));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("time per call = %f usecs\n"),
              (et.real_time / double (ACE_NS_MAX_ENTRIES)) * 1000000));
}

static void
test_bind (ACE_Naming_Context &ns_context)
{
  int array [ACE_NS_MAX_ENTRIES];

  initialize_array (array, sizeof (array) / sizeof (array[0]));
  randomize (array, sizeof (array) / sizeof (array[0]));

  // do the binds
  for (size_t i = 0; i < ACE_NS_MAX_ENTRIES; i++)
    {
      ACE_OS::sprintf (name, "%s%d", "name", array[i]);
      ACE_NS_WString w_name (name);

      ACE_OS::sprintf (value, "%s%d", "value", array[i]);
      ACE_NS_WString w_value (value);

      ACE_OS::sprintf (type, "%s%d", "type", array [i]);
      int bind_result = ns_context.bind (w_name, w_value, type);
      ACE_TEST_ASSERT (bind_result != -1);
    }
}

static void
test_find_failure (ACE_Naming_Context &ns_context)
{
  ACE_OS::sprintf (name, "%s", "foo-bar");
  ACE_NS_WString w_name (name);
  ACE_NS_WString w_value;
  char *l_type = 0;

  // Do the finds.
  for (size_t i = 0; i < ACE_NS_MAX_ENTRIES; i++)
    {
      int resolve = ns_context.resolve (w_name, w_value, l_type);
      ACE_TEST_ASSERT (resolve == -1);
    }
}

static void
test_rebind (ACE_Naming_Context &ns_context)
{
  int array [ACE_NS_MAX_ENTRIES];

  initialize_array (array, sizeof (array) / sizeof (array[0]));
  randomize (array, sizeof (array) / sizeof (array[0]));

  // do the rebinds
  for (size_t i = 0; i < ACE_NS_MAX_ENTRIES; i++)
    {
      ACE_OS::sprintf (name, "%s%d", "name", array[i]);
      ACE_NS_WString w_name (name);

      ACE_OS::sprintf (value, "%s%d", "value", -array[i]);
      ACE_NS_WString w_value (value);

      ACE_OS::sprintf (type, "%s%d", "type", -array[i]);
      int rebind = ns_context.rebind (w_name, w_value, type);
      ACE_TEST_ASSERT (rebind != -1);
    }
}

static void
test_unbind (ACE_Naming_Context &ns_context)
{
  int array [ACE_NS_MAX_ENTRIES];

  initialize_array (array, sizeof (array) / sizeof (array[0]));
  randomize (array, sizeof (array) / sizeof (array[0]));

  // do the unbinds
  for (size_t i = 0; i < ACE_NS_MAX_ENTRIES; i++)
    {
      ACE_OS::sprintf (name, "%s%d", "name", array[i]);
      ACE_NS_WString w_name (name);
      int unbind = ns_context.unbind (w_name);
      ACE_TEST_ASSERT (unbind != -1);
    }
}

static void
test_find (ACE_Naming_Context &ns_context, int sign, int result)
{
  char temp_val[BUFSIZ];
  char temp_type[BUFSIZ];

  int array [ACE_NS_MAX_ENTRIES];

  initialize_array (array, sizeof (array) / sizeof (array[0]));
  randomize (array, sizeof (array) / sizeof (array[0]));

  // do the finds
  for (size_t i = 0; i < ACE_NS_MAX_ENTRIES; i++)
    {
      if (sign == 1)
        {
          ACE_OS::sprintf (temp_val, "%s%d", "value", array[i]);
          ACE_OS::sprintf (temp_type, "%s%d", "type", array[i]);
        }
      else
        {
          ACE_OS::sprintf (temp_val, "%s%d", "value", -array[i]);
          ACE_OS::sprintf (temp_type, "%s%d", "type", -array[i]);
        }

      ACE_OS::sprintf (name, "%s%d", "name", array[i]);

      ACE_NS_WString w_name (name);
      ACE_NS_WString w_value;
      char *type_out = 0;
      ACE_NS_WString val (temp_val);

      int const resolve_result = ns_context.resolve (w_name, w_value, type_out);
      if (resolve_result != result)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Error, resolve result not equal to resutlt (%d != %d)\n"),
                    resolve_result, result));

      char *l_value = w_value.char_rep ();

      if (l_value)
        {
          ACE_TEST_ASSERT (w_value == val);
          if (ns_context.name_options ()->debug ())
            {
              if (type_out)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("Name: %C\tValue: %C\tType: %C\n"),
                            name, l_value, type_out));
              else
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Name: %C\tValue: %C\n"),
                            name, l_value));
            }

          if (type_out)
            {
              ACE_TEST_ASSERT (ACE_OS::strcmp (type_out, temp_type) == 0);
              delete[] type_out;
            }
        }

      delete[] l_value;
    }
}


int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Naming_Test"));
  ACE_TCHAR temp_file [BUFSIZ];
  ACE_Naming_Context *ns_context = 0;
  ACE_NEW_RETURN (ns_context, ACE_Naming_Context, -1);

  ACE_Name_Options *name_options = ns_context->name_options ();

  name_options->parse_args (argc, argv);
  /*
  ** NOTE! This is an experimental value and is not magic in any way. It
  ** works for me, on one system. It's needed because in the particular
  ** case here where the underlying mmap will allocate a small area and
  ** then try to grow it, it always moves it to a new location, which
  ** totally screws things up. I once tried forcing the realloc to do
  ** MAP_FIXED but that's not a good solution since it may overwrite other
  ** mapped areas of memory, like the heap, or the C library, and get very
  ** unexpected results.    (Steve Huston, 24-August-2007)
  */
# if defined (ACE_LINUX) && defined (__x86_64__)
  name_options->base_address ((char*)0x3c00000000);
#endif
  bool unicode = false;
#if (defined (ACE_WIN32) && defined (ACE_USES_WCHAR))
  unicode = true;
#endif /* ACE_WIN32 && ACE_USES_WCHAR */
  if (unicode && name_options->use_registry () == 1)
    {
      name_options->namespace_dir (ACE_TEXT ("Software\\ACE\\Name Service"));
      name_options->database (ACE_TEXT ("Version 1"));
    }
  else
    {
      const ACE_TCHAR* pname = ACE::basename (name_options->process_name (),
                                              ACE_DIRECTORY_SEPARATOR_CHAR);
      // Allow the user to determine where the context file will be
      // located just in case the current directory is not suitable for
      // locking.  We don't just set namespace_dir () on name_options
      // because that is not sufficient to work around locking problems
      // for Tru64 when the current directory is NFS mounted from a
      // system that does not properly support locking.
      ACE_TCHAR temp_dir [MAXPATHLEN];
      if (ACE::get_temp_dir (temp_dir, MAXPATHLEN) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Temporary path too long, ")
                             ACE_TEXT ("defaulting to current directory\n")),
                             -1);
        }
      else
        {
          ACE_OS::chdir (temp_dir);
        }
      // Set the database name using the pid. mktemp isn't always available.
      ACE_OS::snprintf(temp_file, BUFSIZ,
#if !defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
                       ACE_TEXT ("%ls%d"),
#else
                       ACE_TEXT ("%s%d"),
#endif
                       pname,
                       (int)(ACE_OS::getpid ()));

      name_options->database (temp_file);
    }
  if (ns_context->open (ACE_Naming_Context::PROC_LOCAL, 1) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ns_context->open (PROC_LOCAL) %p\n"),
                         ACE_TEXT ("failed")),
                        -1);
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("time to test %d iterations using %s\n"),
              ACE_NS_MAX_ENTRIES, name_options->use_registry () ?
              ACE_TEXT ("Registry") : ACE_TEXT ("ACE")));

  ACE_Profile_Timer timer;

  timer.start ();
  // Add some bindings to the database
  test_bind (*ns_context);
  print_time (timer, "Binds");

  timer.start ();
  // Should find the entries
  test_find (*ns_context, 1, 0);
  print_time (timer, "Successful Finds");

  timer.start ();
  // Rebind with negative values
  test_rebind (*ns_context);
  print_time (timer, "Rebinds");

  timer.start ();
  // Should find the entries
  test_find (*ns_context,  -1, 0);
  print_time (timer, "Successful Finds");

  timer.start ();
  // Should not find the entries
  test_find_failure (*ns_context);
  print_time (timer, "UnSuccessful Finds");

  timer.start ();
  // Remove all bindings from database
  test_unbind (*ns_context);
  print_time (timer, "Unbinds");

  ACE_OS::sprintf (temp_file, ACE_TEXT ("%s%s%s"),
                   name_options->namespace_dir (),
                   ACE_DIRECTORY_SEPARATOR_STR,
                   name_options->database ());

  delete ns_context;

  // Remove any existing files.  No need to check return value here
  // since we don't care if the file doesn't exist.
  ACE_OS::unlink (temp_file);

  ACE_END_TEST;
  return 0;
}
