// $Id$
// ================================================================
//
// = FILENAME
//     client.cpp
//
// = DESCRIPTION
//     This is a simple dir_service client implementation which makes calls 
//     on the Dir_Service object.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthiak@cs.wustl.edu>
//
// ================================================================

#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "Dir_ServiceS.h"
#include "Dir_ServiceC.h"


ACE_RCSID(On_Demand_Loading, client, "$Id$")

static char *IOR = 0;
static char *IOR_file = 0;
static int iterations = 1;
static int oneway = 0;
static int shutdown_server = 0;

// Obtain the arguments.

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "f:k:i:ox");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = ACE_OS::strdup (get_opts.optarg);
        break;

      case 'f':
        IOR_file = get_opts.optarg;
        break;

      case 'o':
        oneway = 1;
        break;

      case 'i':
        iterations = ::atoi (get_opts.optarg);
        break;

      case 'x':
	shutdown_server = 1;
	break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k IOR "
                           "-f IOR file "
                           "-o oneway "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (IOR == 0 && IOR_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR or IOR_file for the servant\n"), -1);

  // Indicates successful parsing of command line.
  return 0;
}

// This method obtains the IOR from a file.

int
read_IOR_from_file (void)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (IOR_file, 0);

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
  CORBA::Environment env;

  // Initialize the ORB
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB_init");
      return -1;
    }

  // Initialize options based on command-line arguments.
  int parse_args_result = parse_args (argc, argv);
  if (parse_args_result != 0)
    return parse_args_result;

  if (IOR == 0)
    {
      int result = read_IOR_from_file ();
      if (result != 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Cannot read IOR from %s\n", IOR_file), -1);
    }

  // Get an object reference from the argument string.
  CORBA::Object_var object = orb->string_to_object (IOR, env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::string_to_object");
      return -1;
    }

  // Try to narrow the object reference to a Dir_Service reference.
  Dir_Service_var my_dir_service = Dir_Service::_narrow (object.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("Dir_Service::_narrow");
      return -1;
    }

  CORBA::String_var ior =
    orb->object_to_string (my_dir_service.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::object_to_string");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\nConnecting to: %s\n\n",
              ior.in ()));

  long result = 0;
  for (int i = 0; i < iterations && env.exception () == 0; i++)
    {
      if (oneway)
        // Invoke the area_code_info() method of the foo reference.
        my_dir_service->area_codes_info (env);
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Whats the contact number for the DOC group?\n"));
          // Invoke the tele_number() method of the foo reference.
          result = my_dir_service->tele_number ("DOC Group",env);
           // Print the result of doit () method of the foo reference.
          ACE_DEBUG ((LM_DEBUG, "The telephone number is %d\n", result));
       }
       
      my_dir_service->end_note (env);
    }

  
  if (shutdown_server && env.exception () == 0)
    my_dir_service->shutdown (env);

  if (env.exception () != 0)
    {
      env.print_exception ("Dir_Service::tele_number");
      return 1;
    }
  
  ACE_OS::free (IOR);
  
  return 0;
}
