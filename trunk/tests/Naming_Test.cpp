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
//    Prashant Jain
// 
// ============================================================================

#include "ace/SString.h"
#include "ace/Naming_Context.h"
#include "test_config.h"

static char name[BUFSIZ];
static char value[BUFSIZ];
static char type[BUFSIZ];

static void
bind (ACE_Naming_Context *ns_context, int result)
{
  // do the binds
  for (int i = 1; i <= ACE_NS_MAX_ENTRIES; i++) 
    {
      sprintf (name, "%s%d", "name", i);
      ACE_WString w_name (name);
      
      sprintf (value, "%s%d", "value", i);
      ACE_WString w_value (value);
      
      sprintf (type, "%s%d", "type", i);
      ACE_ASSERT (ns_context->bind (w_name, w_value, type) == result);
    }
}

static void
rebind (ACE_Naming_Context *ns_context, int result)
{
  // do the rebinds
  for (int i = 1; i <= ACE_NS_MAX_ENTRIES; i++) 
    {
      sprintf (name, "%s%d", "name", i);
      ACE_WString w_name (name);
      sprintf (value, "%s%d", "value", -i);
      ACE_WString w_value (value);
      sprintf (type, "%s%d", "type", -i);
      ACE_ASSERT (ns_context->rebind (w_name, w_value, type) == result);
    }
}

static void
unbind (ACE_Naming_Context *ns_context, int result)
{
  // do the unbinds
  for (int i = 1; i <= ACE_NS_MAX_ENTRIES; i++) 
    {
      sprintf (name, "%s%d", "name", i);
      ACE_WString w_name (name);
      ACE_ASSERT (ns_context->unbind (w_name) == result);
    }
}

static void
find (ACE_Naming_Context *ns_context, int sign, int result)
{
  char temp_val[BUFSIZ];
  char temp_type[BUFSIZ];

  // do the finds
  for (int i = 1; i <= ACE_NS_MAX_ENTRIES; i++) 
    {
      sprintf (name, "%s%d", "name", i);
      ACE_WString w_name (name);
      
      ACE_WString w_value;
      char *type_out;

      if (sign == 1)
	{
	  sprintf (temp_val, "%s%d", "value", i);
	  sprintf (temp_type, "%s%d", "type", i);
	}	  
      else
	{
	  sprintf (temp_val, "%s%d", "value", -i);
	  sprintf (temp_type, "%s%d", "type", -i);
	}

      ACE_WString val (temp_val);
      
      ACE_ASSERT (ns_context->resolve (w_name, w_value, type_out) == result);

      if (w_value.char_rep ())
	{
	  ACE_ASSERT (w_value == val);
	  ACE_DEBUG ((LM_DEBUG, "Name: %s\tValue: %s\tType: %s\n",
		      name, w_value.char_rep (), type_out));

	  if (type_out)
	    {
	      ACE_ASSERT (::strcmp (type_out, temp_type) == 0);
	      delete[] type_out;
	    }
	}
    }  
}

int
main (int argc, char *argv[])
{
  ACE_START_TEST ("Naming_Test.cpp");

  ACE_Naming_Context *ns_context;
  ACE_NEW_RETURN (ns_context, ACE_Naming_Context, -1);

  ACE_Name_Options *name_options = ns_context->name_options ();

  name_options->parse_args (argc, argv);
  name_options->database (ACE::basename (name_options->process_name (),
					 ACE_DIRECTORY_SEPARATOR_CHAR));
  
  // Remove the temporary file (if it exists)
  char temp_file [BUFSIZ];

  ::sprintf (temp_file, "%s%s%s",
             name_options->namespace_dir (),
	     ACE_DIRECTORY_SEPARATOR_STR,
	     name_options->database ());

  // Remove any existing files.  No need to check return value here
  // since we don't care if the file doesn't exist.
  ACE_OS::unlink (temp_file);  

  ACE_ASSERT (ns_context->open (ACE_Naming_Context::PROC_LOCAL) != -1);

  // Add some bindings to the database
  bind (ns_context, 0);
  rebind (ns_context, 1);
  bind (ns_context, 1);
  bind (ns_context, 1);
  rebind (ns_context, 1);

  // Remove all bindings from database
  unbind (ns_context, 0); 
  
  rebind (ns_context, 0);
  unbind (ns_context, 0); 

  // No more bindings in database so find should return -1
  find (ns_context, -1, -1);

  ACE_END_TEST;
  return 0;
}

