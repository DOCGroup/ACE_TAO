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

static char *ior = 0;
static char *iorfile = 0;
static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "k:d");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        iorfile = ACE_OS::strdup (get_opts.optarg);
        break;
      case 'd':
	TAO_debug_level++;
	break; 
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           "[-k <iorfile>]"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (iorfile == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant\n"), -1);

  // Indicates successful parsing of command line.
  return 0;
}

int
main (int argc, char **argv)
{
  CORBA::Environment env;

  // Initialize the ORB
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB_init");
      return -1;
    }

  // Parse the command-line arguments to get the location of the IOR 
  if (parse_args (argc, argv) == -1)
    return -1;

    // Read the file, and get the IOR
  ACE_HANDLE input_file = ACE_OS::open (iorfile, 0);
  if (input_file == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_DEBUG,
		       "Cannot open input file for reading IOR: %s\n", 
		       iorfile),
		      -1);
  ACE_Read_Buffer ior_buffer (input_file);
  char *data = ior_buffer.read ();
  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "Unable to read ior\n"),
		      -1);
  ior = ACE_OS::strdup (data);
  ior_buffer.alloc ()-> free (data);
  ACE_OS::close (input_file);
  
  // Get the object reference with the IOR
  CORBA::Object_var object = orb->string_to_object (ior, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::string_to_object");
      return -1;
    }
  
  // Narrow the object reference to a Database::Agent
  Database::Agent_var database_agent = Database::Agent::_narrow (object.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("Database::Agent::_narrow");
      return -1;
    }

  Database::NVPairSequence employee_attributes (2);
  employee_attributes.length (2);

  Database::NamedValue &first = employee_attributes[0];    
  Database::NamedValue &second = employee_attributes[1];    
      
  char *name = "irfan";
  CORBA::Long id = 555;

  first.name = CORBA::string_dup ("name");
  first.value <<= name;
  second.name = CORBA::string_dup ("id");
  second.value <<= id;
  
  // Create an employee
  Database::Entry_var entry = database_agent->create_entry ("irfan",
                                                            "Employee",
                                                            employee_attributes,
                                                            env);
  if (env.exception () != 0)
    {
      env.print_exception ("Database::Agent::create_entry");
      return -1;
    }

  Database::Employee_var employee = Database::Employee::_narrow (entry.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("Database::Employee::_narrow");
      return -1;
    }

  /*
   * 
   *  NOT IMPLEMENTED YET 
   * 
   *
   */
   
#if 0
  // Reset the id
  employee->id (666, env);  
  if (env.exception () != 0)
    {
      env.print_exception ("Database::Employee::id");
      return -1;
    }
#endif /* 0 */

  // Destroy the employee
  database_agent->destroy_entry ("irfan",
                                 "Employee",
                                 env);
  if (env.exception () != 0)
    {
      env.print_exception ("Database::Entry::destroy");
      return -1;
    }

  return 0;
}
