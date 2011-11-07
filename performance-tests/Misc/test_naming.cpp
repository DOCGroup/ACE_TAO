
//=============================================================================
/**
 *  @file    test_naming.cpp
 *
 *  $Id$
 *
 *    This is an example to do performance testing of the Naming Service
 *    using both the normal Memory Pool as well as the light Memory Pool.
 *
 *
 *  @author Prashant Jain
 */
//=============================================================================


#include "ace/OS_main.h"
#include "ace/ACE.h"
#include "ace/SString.h"
#include "ace/Naming_Context.h"
#include "ace/Profile_Timer.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"

#define ACE_NS_MAX_ENTRIES 4000

static char name[BUFSIZ];
static char value[BUFSIZ];
static char type[BUFSIZ];

//FUZZ: disable check_for_lack_ACE_OS
void
bind (ACE_Naming_Context *ns_context, int result)
{
//FUZZ: enable check_for_lack_ACE_OS

  // do the binds
  for (int i = 1; i <= ACE_NS_MAX_ENTRIES; i++)
    {
      if (i % 50 == 0)
        ACE_DEBUG ((LM_DEBUG, "."));
      ACE_OS::sprintf (name, "%s%d", "name", i);
      ACE_NS_WString w_name (name);

      ACE_OS::sprintf (value, "%s%d", "value", i);
      ACE_NS_WString w_value (value);

      ACE_OS::sprintf (type, "%s%d", "type", i);
      if (ns_context->bind (w_name, w_value, type) != result) {
        ACE_ERROR ((LM_ERROR, "bind failed!"));
      }
    }
  ACE_DEBUG ((LM_DEBUG, "\n"));
}

void
rebind (ACE_Naming_Context *ns_context, int result)
{
  // do the rebinds
  for (int i = 1; i <= ACE_NS_MAX_ENTRIES; i++)
    {
      ACE_OS::sprintf (name, "%s%d", "name", i);
      ACE_NS_WString w_name (name);
      ACE_OS::sprintf (value, "%s%d", "value", -i);
      ACE_NS_WString w_value (value);
      ACE_OS::sprintf (type, "%s%d", "type", -i);
      if (ns_context->rebind (w_name, w_value, type) != result) {
        ACE_ERROR ((LM_ERROR, "rebind failed!"));
      }
    }
}

void
unbind (ACE_Naming_Context *ns_context, int result)
{
  // do the unbinds
  for (int i = 1; i <= ACE_NS_MAX_ENTRIES; i++)
    {
      ACE_OS::sprintf (name, "%s%d", "name", i);
      ACE_NS_WString w_name (name);
      if (ns_context->unbind (w_name) != result) {
        ACE_ERROR ((LM_ERROR, "unbind failed!"));
      }
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
      ACE_OS::sprintf (name, "%s%d", "name", i);
      ACE_NS_WString w_name (name);

      ACE_NS_WString w_value;
      char *type_out;

      if (sign == 1)
        {
          ACE_OS::sprintf (temp_val, "%s%d", "value", i);
          ACE_OS::sprintf (temp_type, "%s%d", "type", i);
        }
      else
        {
          ACE_OS::sprintf (temp_val, "%s%d", "value", -i);
          ACE_OS::sprintf (temp_type, "%s%d", "type", -i);
        }

      ACE_NS_WString val (temp_val);

      int resolve_result = ns_context->resolve (w_name, w_value, type_out);
      if (resolve_result != result) {
        ACE_ERROR ((LM_ERROR, "resolved failed!"));
      }
      ACE_ASSERT (resolve_result == result);
      ACE_UNUSED_ARG (resolve_result); // To avoid compile warning
                                       // with ACE_NDEBUG.

      if (w_value.char_rep ())
        {
          ACE_DEBUG ((LM_DEBUG, "Name: %s\tValue: %s\tType: %s\n",
                      name, w_value.char_rep (), type_out));
          ACE_ASSERT (w_value == val);
          if (type_out)
            {
              ACE_ASSERT (ACE_OS::strcmp (type_out, temp_type) == 0);
              delete[] type_out;
            }
        }
    }
}

void do_testing (int argc, ACE_TCHAR *argv[], int light)
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
      const ACE_TCHAR *p = ACE::basename (name_options->process_name (),
                                          ACE_DIRECTORY_SEPARATOR_CHAR);
      ACE_TCHAR s[5 /* strlen ("light") */ + MAXNAMELEN + 1];
      ACE_OS::sprintf (s, ACE_TEXT("light%s"), p);
      name_options->database (s);
      ns_context.open (ACE_Naming_Context::PROC_LOCAL, 1);
    }

  // Add bindings to the database
  ACE_DEBUG ((LM_DEBUG, "Binding\n"));

  timer.start ();

  //FUZZ: disable check_for_lack_ACE_OS
  bind (&ns_context, 0);
  //FUZZ: enable check_for_lack_ACE_OS

  ACE_DEBUG ((LM_DEBUG, "Unbinding\n"));
  unbind (&ns_context, 0);
  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;

  timer.elapsed_time (et);
  ACE_DEBUG ((LM_DEBUG, "real time = %f secs, user time = %f secs, system time = %f secs\n",
              et.real_time, et.user_time, et.system_time));
}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Do testing with SYNC on
  ACE_DEBUG ((LM_DEBUG, "SYNC is ON\n"));
  do_testing (argc, argv, 0);

  // Do testing with SYNC off
  ACE_DEBUG ((LM_DEBUG, "SYNC is OFF\n"));
  do_testing (argc, argv, 1);
  return 0;
}
