// ============================================================================
// $Id$

//
// = LIBRARY
//    performance_tests
// 
// = FILENAME
//    test_naming.cpp
//
// = DESCRIPTION
//      This is an example to do performance testing of the Naming Service
//      using both the normal Memory Pool as well as the light Memory Pool.
//
// = AUTHOR
//    Prashant Jain
// 
// ============================================================================

#include "ace/SString.h"
#include "ace/Naming_Context.h"
#include "ace/Profile_Timer.h"
#define ACE_NS_MAX_ENTRIES 4000

static char name[BUFSIZ];
static char value[BUFSIZ];
static char type[BUFSIZ];

void
bind (ACE_Naming_Context *ns_context, int result)
{
  // do the binds
  for (int i = 1; i <= ACE_NS_MAX_ENTRIES; i++) 
    {
      if (i % 50 == 0)
	ACE_DEBUG ((LM_DEBUG, "."));
      sprintf (name, "%s%d", "name", i);
      ACE_WString w_name (name);
      
      sprintf (value, "%s%d", "value", i);
      ACE_WString w_value (value);
      
      sprintf (type, "%s%d", "type", i);
      ACE_ASSERT (ns_context->bind (w_name, w_value, type) == result);
    }
  ACE_DEBUG ((LM_DEBUG, "\n"));
}

void
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

void
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

void
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
	  cerr << "Name: " << name << "\tValue: " << w_value.char_rep () << "\tType: " << type_out << endl;
	  if (type_out)
	    {
	      ACE_ASSERT (::strcmp (type_out, temp_type) == 0);
	      delete[] type_out;
	    }
	}
    }  
}

void do_testing (int argc, char *argv[], int light)
{
  ACE_Profile_Timer timer;

  ACE_Naming_Context ns_context;
  ACE_Name_Options *name_options = ns_context.name_options ();
  name_options->parse_args (argc, argv);

  if (light == 0)  // Use SYNC
    {
      name_options->database (ACE::basename (name_options->process_name (),
					     ACE_DIRECTORY_SEPARATOR_CHAR));
      ns_context.open (ACE_Naming_Context::PROC_LOCAL);
    }
  else  // Use NO-SYNC
    {
      name_options->database (ACE_OS::strcat ("light", ACE::basename (name_options->process_name (),
								      ACE_DIRECTORY_SEPARATOR_CHAR)));
      ns_context.open (ACE_Naming_Context::PROC_LOCAL, 1);
    }

  // Add bindings to the database
  ACE_DEBUG ((LM_DEBUG, "Binding\n"));

  timer.start ();
  bind (&ns_context, 0);

  ACE_DEBUG ((LM_DEBUG, "Unbinding\n"));
  unbind (&ns_context, 0); 
  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;

  timer.elapsed_time (et);
  ACE_DEBUG ((LM_DEBUG, "real time = %f secs, user time = %f secs, system time = %f secs\n",
	      et.real_time, et.user_time, et.system_time));
}

int
main (int argc, char *argv[])
{
  // Do testing with SYNC on
  ACE_DEBUG ((LM_DEBUG, "SYNC is ON\n"));
  do_testing (argc, argv, 0);

  // Do testing with SYNC off
  ACE_DEBUG ((LM_DEBUG, "SYNC is OFF\n"));
  do_testing (argc, argv, 1);

  return 0;
}

