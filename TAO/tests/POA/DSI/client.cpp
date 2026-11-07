//=============================================================================
/**
 *  @file     client.cpp
 *
 *    A client program for the Database IDL module
 *
 *  @author Irfan Pyarali
 */
//=============================================================================

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "DatabaseC.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_unistd.h"

#include <memory>
#include <string>

struct Client_Options
{
  std::string ior;
  ACE_TCHAR const *ior_file {};
  bool shutdown_server {};
};

struct Read_Buffer_Deleter
{
  ACE_Allocator *allocator;

  void operator() (char *data) const
  {
    this->allocator->free (data);
  }
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("xk:f:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ());
        break;

      case 'f':
        options.ior_file = get_opts.opt_arg ();
        break;

      case 'x':
        options.shutdown_server = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           "-k IOR "
                           "-f IOR file "
                           "-x [for shutting down the server] "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (options.ior.empty () && options.ior_file == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR or IOR_file for the servant\n"),
                      -1);

  return 0;
}

static int
read_IOR_from_file (ACE_TCHAR const *ior_file, std::string &ior)
{
  ACE_HANDLE const f_handle = ACE_OS::open (ior_file, 0);
  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to open %s for reading\n"),
                       ior_file),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();
  if (data == nullptr)
    {
      ACE_OS::close (f_handle);
      ACE_ERROR_RETURN ((LM_ERROR, "Unable to read ior\n"), -1);
    }

  std::unique_ptr<char, Read_Buffer_Deleter> const owned_data (
    data, Read_Buffer_Deleter {ior_buffer.alloc ()});
  ACE_OS::close (f_handle);
  ior = owned_data.get ();
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  std::string current_operation;
  try
    {
      current_operation = "CORBA::ORB_init";
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      Client_Options options {};
      if (parse_args (argc, argv, options) == -1)
        return -1;

      if (options.ior.empty ())
        {
          current_operation = "read IOR file";
          int const result = read_IOR_from_file (options.ior_file, options.ior);
          if (result != 0)
            return result;
        }

      current_operation = "CORBA::ORB::string_to_object";
      CORBA::Object_var object = orb->string_to_object (options.ior.c_str ());

      current_operation = "Database::Agent::_narrow";
      Database::Agent_var database_agent = Database::Agent::_narrow (object.in ());

      Database::NVPairSequence employee_attributes (2);
      employee_attributes.length (2);

      Database::NamedValue &first = employee_attributes[0];
      Database::NamedValue &second = employee_attributes[1];

      char const *name = "irfan";
      CORBA::Long const id = 555;

      first.name = CORBA::string_dup ("name");
      first.value <<= name;
      second.name = CORBA::string_dup ("id");
      second.value <<= id;

      current_operation = "Database::Agent::create_entry";
      Database::Entry_var entry =
        database_agent->create_entry ("irfan",
                                      "Employee",
                                      employee_attributes);

      bool duplicate_key_rejected {};
      try
        {
          database_agent->create_entry ("irfan",
                                        "Employee",
                                        employee_attributes);
        }
      catch (Database::Duplicate_Key const &)
        {
          duplicate_key_rejected = true;
        }
      if (!duplicate_key_rejected)
        ACE_ERROR_RETURN ((LM_ERROR, "Duplicate key was accepted\n"), -1);

      current_operation = "Database::Employee::_narrow";
      Database::Employee_var employee = Database::Employee::_narrow (entry.in ());

      /*
       * NOT IMPLEMENTED YET
       */
#if 0
      current_operation = "Database::Employee::id";
      employee->id (666);
#endif /* 0 */

      current_operation = "Database::Entry::find";
      entry = database_agent->find_entry ("irfan",
                                          "Employee");

      current_operation = "Database::Entry::destroy";
      database_agent->destroy_entry ("irfan",
                                     "Employee");

      current_operation = "Shutdown server";
      if (options.shutdown_server)
        database_agent->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (current_operation.c_str ());
      return -1;
    }

  return 0;
}
