// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Naming_Test.cpp
//
// = DESCRIPTION
//      This is a test to illustrate the Naming Services. The test
//      does binds, rebinds, finds, and unbinds on name bindings using
//      the local naming context.
//
// = AUTHOR
//    Prashant Jain and Irfan Pyarali
// 
// ============================================================================

#include "ace/SString.h"
#include "ace/Naming_Context.h"
#include "ace/Profile_Timer.h"
#include "test_config.h"

static char name[BUFSIZ];
static char value[BUFSIZ];
static char type[BUFSIZ];

static void
randomize (int array[], size_t size)
{
  size_t i;

  for (i = 0; i < size; i++)
    array [i] = i;
  
  ACE_OS::srand (ACE_OS::time (0L));
 
  // Generate an array of random numbers from 0 .. size - 1.

  for (i = 0; i < size; i++)
    {
      int index = ACE_OS::rand() % size--;
      int temp = array [index];
      array [index] = array [size];
      array [size] = temp;
    }
}

static void 
print_time (ACE_Profile_Timer &timer,
	    const char *test)
{
  ACE_Profile_Timer::ACE_Elapsed_Time et;
  timer.stop ();
  timer.elapsed_time (et);
  
  ACE_DEBUG ((LM_DEBUG, "\n     *****  %s  *****     \n", test));
  ACE_DEBUG ((LM_DEBUG, "real time = %f secs, user time = %f secs, system time = %f secs\n",
	      et.real_time, et.user_time, et.system_time));
  ACE_DEBUG ((LM_DEBUG, "time per call = %f usecs\n\n", 
	      (et.real_time / double (ACE_NS_MAX_ENTRIES)) * 1000000));
}

static void
test_bind (ACE_Naming_Context &ns_context)
{
  int array [ACE_NS_MAX_ENTRIES];
  randomize (array, sizeof array / sizeof (int));

  // do the binds
  for (size_t i = 0; i < ACE_NS_MAX_ENTRIES; i++) 
    {
      sprintf (name, "%s%d", "name", array[i]);
      ACE_WString w_name (name);
      
      sprintf (value, "%s%d", "value", array[i]);
      ACE_WString w_value (value);
      
      sprintf (type, "%s%d", "type", array [i]);
      ACE_ASSERT (ns_context.bind (w_name, w_value, type) != -1);
    }
}

static void
test_find_failure (ACE_Naming_Context &ns_context)
{
  sprintf (name, "%s", "foo-bar");
  ACE_WString w_name (name);
  ACE_WString w_value;
  char *l_type = 0;
      
  // Do the finds.
  for (size_t i = 0; i < ACE_NS_MAX_ENTRIES; i++) 
    ACE_ASSERT (ns_context.resolve (w_name, w_value, l_type) == -1);
}

static void
test_rebind (ACE_Naming_Context &ns_context)
{
  int array [ACE_NS_MAX_ENTRIES];
  randomize (array, sizeof array / sizeof (int));

  // do the rebinds
  for (size_t i = 0; i < ACE_NS_MAX_ENTRIES; i++) 
    {
      sprintf (name, "%s%d", "name", array[i]);
      ACE_WString w_name (name);

      sprintf (value, "%s%d", "value", -array[i]);
      ACE_WString w_value (value);

      sprintf (type, "%s%d", "type", -array[i]);
      ACE_ASSERT (ns_context.rebind (w_name, w_value, type) != -1);
    }
}

static void
test_unbind (ACE_Naming_Context &ns_context)
{
  int array [ACE_NS_MAX_ENTRIES];
  randomize (array, sizeof array / sizeof (int));

  // do the unbinds
  for (size_t i = 0; i < ACE_NS_MAX_ENTRIES; i++) 
    {
      sprintf (name, "%s%d", "name", array[i]);
      ACE_WString w_name (name);
      ACE_ASSERT (ns_context.unbind (w_name) != -1);
    }
}

static void
test_find (ACE_Naming_Context &ns_context, int sign, int result)
{
  char temp_val[BUFSIZ];
  char temp_type[BUFSIZ];

  int array [ACE_NS_MAX_ENTRIES];
  randomize (array, sizeof array / sizeof (int));

  // do the finds
  for (size_t i = 0; i < ACE_NS_MAX_ENTRIES; i++) 
    {
      if (sign == 1)
	{
	  sprintf (temp_val, "%s%d", "value", array[i]);
	  sprintf (temp_type, "%s%d", "type", array[i]);
	}	  
      else
	{
	  sprintf (temp_val, "%s%d", "value", -array[i]);
	  sprintf (temp_type, "%s%d", "type", -array[i]);
	}

      sprintf (name, "%s%d", "name", array[i]);

      ACE_WString w_name (name);      
      ACE_WString w_value;
      char *type_out = 0;
      ACE_WString val (temp_val);
      
      ACE_ASSERT (ns_context.resolve (w_name, w_value, type_out) == result);

      char *l_value = w_value.char_rep ();

      if (l_value)
	{
	  ACE_ASSERT (w_value == val);
	  if (ns_context.name_options ()->debug ())
	    {
	      if (type_out)
		ACE_DEBUG ((LM_DEBUG, "Name: %s\tValue: %s\tType: %s\n",
			    name, l_value, type_out));
	      else
		ACE_DEBUG ((LM_DEBUG, "Name: %s\tValue: %s\n",
			    name, l_value));
	    }

	  if (type_out)
	    {
	      ACE_ASSERT (ACE_OS::strcmp (type_out, temp_type) == 0);
	      delete[] type_out;
	    }
	}

      delete[] l_value;
    }  
}

int
main (int argc, char *argv[])
{
  TCHAR temp_file [BUFSIZ];
  ACE_START_TEST ("Naming_Test");

  ACE_Naming_Context *ns_context = 0;
  ACE_NEW_RETURN (ns_context, ACE_Naming_Context, -1);

  ACE_Name_Options *name_options = ns_context->name_options ();

  name_options->parse_args (argc, argv);

#if (defined (ACE_WIN32) && defined (UNICODE))
  if (name_options->use_registry () == 1)
#else
  if (0)  
#endif /* ACE_WIN32 && UNICODE */
    {
      name_options->namespace_dir (__TEXT ("Software\\ACE\\Name Service"));
      name_options->database (__TEXT ("Version 1"));  
    }
  else
    {
      ACE_OS::strcpy (temp_file, ACE::basename (name_options->process_name (),
						ACE_DIRECTORY_SEPARATOR_CHAR));
      ACE_OS::strcat (temp_file, __TEXT ("XXXXXX"));
      
      // Set the database name using mktemp to generate a unique file name
      name_options->database (ACE_OS::mktemp (temp_file));
    }

  ACE_ASSERT (ns_context->open (ACE_Naming_Context::PROC_LOCAL, 1) != -1);

  ACE_DEBUG ((LM_DEBUG, "time to test %d iterations using %s\n", 
	      ACE_NS_MAX_ENTRIES, name_options->use_registry () ? "Registry" : "ACE"));

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

  ACE_OS::sprintf (temp_file, __TEXT ("%s%s%s"),
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
