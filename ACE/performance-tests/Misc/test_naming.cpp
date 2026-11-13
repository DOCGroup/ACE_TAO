
//=============================================================================
/**
 *  @file    test_naming.cpp
 *
 *    This is an example to do performance testing of the Naming Service
 *    using both the normal Memory Pool as well as the light Memory Pool.
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

#include <array>
#include <cstdio>
#include <cstring>
#include <memory>

#define ACE_NS_MAX_ENTRIES 4000

static std::array<char, BUFSIZ> name {};
static std::array<char, BUFSIZ> value {};
static std::array<char, BUFSIZ> type {};

struct ACE_String_Deleter
{
  void operator() (char *string) const
  {
    ACE::strdelete (string);
  }
};

using ACE_String_Unique_Ptr = std::unique_ptr<char, ACE_String_Deleter>;

static void
format_entry (std::array<char, BUFSIZ> &buffer,
              char const *prefix,
              int value)
{
  std::snprintf (buffer.data (), buffer.size (), "%s%d", prefix, value);
}

//FUZZ: disable check_for_lack_ACE_OS
void
bind (ACE_Naming_Context *const ns_context, int const result)
{
//FUZZ: enable check_for_lack_ACE_OS

  // do the binds
  for (int i = 1; i <= ACE_NS_MAX_ENTRIES; ++i)
    {
      if (i % 50 == 0)
        ACE_DEBUG ((LM_DEBUG, "."));
      format_entry (name, "name", i);
      ACE_NS_WString w_name (name.data ());

      format_entry (value, "value", i);
      ACE_NS_WString w_value (value.data ());

      format_entry (type, "type", i);
      if (ns_context->bind (w_name, w_value, type.data ()) != result) {
        ACE_ERROR ((LM_ERROR, "bind failed!"));
      }
    }
  ACE_DEBUG ((LM_DEBUG, "\n"));
}

void
rebind (ACE_Naming_Context *const ns_context, int const result)
{
  // do the rebinds
  for (int i = 1; i <= ACE_NS_MAX_ENTRIES; ++i)
    {
      format_entry (name, "name", i);
      ACE_NS_WString w_name (name.data ());
      format_entry (value, "value", -i);
      ACE_NS_WString w_value (value.data ());
      format_entry (type, "type", -i);
      if (ns_context->rebind (w_name, w_value, type.data ()) != result) {
        ACE_ERROR ((LM_ERROR, "rebind failed!"));
      }
    }
}

void
unbind (ACE_Naming_Context *const ns_context, int const result)
{
  // do the unbinds
  for (int i = 1; i <= ACE_NS_MAX_ENTRIES; ++i)
    {
      format_entry (name, "name", i);
      ACE_NS_WString w_name (name.data ());
      if (ns_context->unbind (w_name) != result) {
        ACE_ERROR ((LM_ERROR, "unbind failed!"));
      }
    }
}

void
find (ACE_Naming_Context *const ns_context,
      int const sign,
      int const result)
{
  std::array<char, BUFSIZ> temp_val {};
  std::array<char, BUFSIZ> temp_type {};

  // do the finds
  for (int i = 1; i <= ACE_NS_MAX_ENTRIES; ++i)
    {
      format_entry (name, "name", i);
      ACE_NS_WString w_name (name.data ());

      ACE_NS_WString w_value;
      char *raw_type_out {};

      if (sign == 1)
        {
          format_entry (temp_val, "value", i);
          format_entry (temp_type, "type", i);
        }
      else
        {
          format_entry (temp_val, "value", -i);
          format_entry (temp_type, "type", -i);
        }

      ACE_NS_WString val (temp_val.data ());

      int const resolve_result = ns_context->resolve (w_name,
                                                      w_value,
                                                      raw_type_out);
      ACE_String_Unique_Ptr type_out {raw_type_out};
      if (resolve_result != result) {
        ACE_ERROR ((LM_ERROR, "resolved failed!"));
      }
      ACE_ASSERT (resolve_result == result);
      ACE_UNUSED_ARG (resolve_result); // To avoid compile warning
                                       // with ACE_NDEBUG.

      ACE_String_Unique_Ptr value_out {w_value.char_rep ()};
      if (value_out)
        {
          ACE_DEBUG ((LM_DEBUG, "Name: %s\tValue: %s\tType: %s\n",
                      name.data (), value_out.get (), type_out.get ()));
          ACE_ASSERT (w_value == val);
          if (type_out)
            {
              ACE_ASSERT (std::strcmp (type_out.get (), temp_type.data ()) == 0);
            }
        }
    }
}

void
do_testing (int argc, ACE_TCHAR *argv[], bool const light)
{
  ACE_Profile_Timer timer;

  ACE_Naming_Context ns_context;
  ACE_Name_Options *const name_options = ns_context.name_options ();
  name_options->parse_args (argc, argv);

  if (!light)  // Use SYNC
    {
      name_options->database (ACE::basename (name_options->process_name (),
                                             ACE_DIRECTORY_SEPARATOR_CHAR));
      ns_context.open (ACE_Naming_Context::PROC_LOCAL);
    }
  else  // Use NO-SYNC
    {
      ACE_TCHAR const *const p = ACE::basename (
        name_options->process_name (),
        ACE_DIRECTORY_SEPARATOR_CHAR);
      std::array<ACE_TCHAR, 5 + MAXNAMELEN + 1> database_name {};
      ACE_OS::sprintf (database_name.data (), ACE_TEXT("light%s"), p);
      name_options->database (database_name.data ());
      ns_context.open (ACE_Naming_Context::PROC_LOCAL, 1);
    }

  // Add bindings to the database
  ACE_DEBUG ((LM_DEBUG, "Binding\n"));

  timer.start ();

  //FUZZ: disable check_for_lack_ACE_OS
  bind (std::addressof (ns_context), 0);
  //FUZZ: enable check_for_lack_ACE_OS

  ACE_DEBUG ((LM_DEBUG, "Unbinding\n"));
  unbind (std::addressof (ns_context), 0);
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
  do_testing (argc, argv, false);

  // Do testing with SYNC off
  ACE_DEBUG ((LM_DEBUG, "SYNC is OFF\n"));
  do_testing (argc, argv, true);
  return 0;
}
