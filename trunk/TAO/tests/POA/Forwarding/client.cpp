// $Id$

//===============================================================================
//
//
// = FILENAME
//     client.cpp
//
// = DESCRIPTION
//
//     This is a simple foo client implementation.  Also looks out for
//     forwarding exceptions
//
// = AUTHOR
//     Irfan Pyarali
//
//==================================================================================

#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "FooC.h"

static char *IOR = 0;
static int iterations = 6;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "k:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = get_opts.optarg;
        break;
      case 'i':
        iterations = ::atoi (get_opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           "-k IOR"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (IOR == 0)
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

  // Initialize options based on command-line arguments.
  int parse_args_result = parse_args (argc, argv);
  if (parse_args_result != 0)
    return parse_args_result;

  // Get an object reference from the argument string.
  CORBA::Object_var object = orb->string_to_object (IOR, env);

  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::string_to_object");
      return -1;
    }

  // Try to narrow the object reference to a Foo reference.
  Foo_var foo = Foo::_narrow (object.in (), env);

  if (env.exception () != 0)
    {
      env.print_exception ("Foo::_narrow");
      return -1;
    }

  CORBA::String_var original_location = orb->object_to_string (foo.in (), env);
  if (env.exception () == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "original location = %s \n", original_location.in ()));
    }
  else
    {
      env.print_exception ("ORB::object_to_string");
      return -1;
    }

  CORBA::Long result = 0;

  for (int i = 1; i <= iterations; i++)
    {

      // About half way through
      if (i == iterations / 2)
        {
          foo->forward (env);

          // If exception
          if (env.exception () != 0)
            {
              env.print_exception ("Foo::forward");
              return -1;
            }
        }
      else
        {
          // Invoke the doit() method of the foo reference.
          result = foo->doit (env);

          // If exception
          if (env.exception () != 0)
            {
              // Narrow to ForwardRequest
              PortableServer::ForwardRequest_ptr forward_request =
                PortableServer::ForwardRequest::_narrow (env.exception ());

              // If narrowing of exception succeeded
              if (forward_request != 0)
                {
                  // Set foo to new location
                  foo = Foo::_narrow (forward_request->forward_reference.in (), env);

                  // If narrowing of Foo succeeded
                  if (env.exception () == 0)
                    {
                      CORBA::String_var new_location = orb->object_to_string (foo.in (), env);
                      if (env.exception () == 0)
                        {
                          ACE_DEBUG ((LM_DEBUG, "new location = %s \n", new_location.in ()));
                          continue;
                        }
                      else
                        {
                          env.print_exception ("ORB::object_to_string");
                          return -1;
                        }
                    }
                  // If narrowing of Foo failed
                  else
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "Foo::_narrow on forwarded location failed\n"),
                                      -1);
                }

              // If narrowing of exception failed
              else
                {
                  env.print_exception ("PortableServer::ForwardRequest::_narrow");
                  return -1;
                }
            }
          else
            // Print the result of doit () method of the foo reference.
            ACE_DEBUG ((LM_DEBUG, "doit() returned %d \n", result));
        }
    }

  return 0;
}
