// $Id$

//===================================================================
//  = LIBRARY
//      TAO/tests/POA/DSI/client
//
//  = FILENAME
//     client.cpp
//
//  = DESCRIPTION
//      A client program for the Database IDL module
//
//  = AUTHOR
//     Irfan Pyarali
//
//====================================================================

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "DatabaseC.h"

ACE_RCSID(DSI, client, "$Id$")

static char *IOR = 0;
static const char *IOR_file = 0;
static int shutdown_server = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "xk:f:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = ACE_OS::strdup (get_opts.opt_arg ());
        break;

      case 'f':
        IOR_file = get_opts.opt_arg ();
        break;

      case 'x':
        shutdown_server = 1;
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

  if (IOR == 0 && IOR_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR or IOR_file for the servant\n"),
                      -1);

  // Indicates successful parsing of command line.
  return 0;
}

int
read_IOR_from_file (void)
{
  // Open the file for reading.
  ACE_HANDLE f_handle =
    ACE_OS::open (IOR_file, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for reading\n",
                       IOR_file),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to read ior\n"),
                      -1);

  IOR = ACE_OS::strdup (data);
  ior_buffer.alloc ()->free (data);

  ACE_OS::close (f_handle);

  return 0;
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  char str [255];
  // Initialize the ORB
  ACE_TRY
    {
      ACE_OS::strcpy (str,
                      "CORBA::ORB_init");
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,

                                            0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse the command-line arguments to get the location of the
      // IOR
      if (parse_args (argc, argv) == -1)
        return -1;

      if (IOR == 0)
        {
          int result = read_IOR_from_file ();
          if (result != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot read IOR from %s\n",
                               IOR_file),
                              -1);
        }

      ACE_OS::strcpy (str,
                      "CORBA::ORB::string_to_object");

      // Get the object reference with the IOR
      CORBA::Object_var object = orb->string_to_object (IOR
                                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_OS::strcpy (str,
                      "Database::Agent::_narrow");

      // Narrow the object reference to a Database::Agent
      Database::Agent_var database_agent =
        Database::Agent::_narrow (object.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Database::NVPairSequence employee_attributes (2);
      employee_attributes.length (2);

      Database::NamedValue &first =
        employee_attributes[0];
      Database::NamedValue &second =
        employee_attributes[1];

      const char *name = "irfan";
      CORBA::Long id = 555;

      first.name = CORBA::string_dup ("name");
      first.value <<= name;
      second.name = CORBA::string_dup ("id");
      second.value <<= id;

      ACE_OS::strcpy (str,
                      "Database::Agent::create_entry");

      // Create an employee
      Database::Entry_var entry =
        database_agent->create_entry ("irfan",
                                      "Employee",
                                      employee_attributes
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_OS::strcpy (str, "Database::Employee::_narrow");

      Database::Employee_var employee =
        Database::Employee::_narrow (entry.in ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /*
       *
       *  NOT IMPLEMENTED YET
       *
       *
       */

#if 0
      // Reset the id
      ACE_OS::strcpy (str, "Database::Employee::id");
      employee->id (666 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
#endif /* 0 */

      ACE_OS::strcpy (str, "Database::Entry::find");
      // Find the employee
      entry = database_agent->find_entry ("irfan",
                                          "Employee"
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_OS::strcpy (str, "Database::Entry::destroy");
      // Destroy the employee
      database_agent->destroy_entry ("irfan",
                                     "Employee"
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (shutdown_server)
        {
          database_agent->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_OS::free (IOR);

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, str);
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
