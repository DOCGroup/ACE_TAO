// $Id$

//===============================================================================
//
//
// = FILENAME
//     client.cpp
//
// = DESCRIPTION
//     This is a simple foo client implementation.
//
// = AUTHOR
//     Irfan Pyarali
//
//==================================================================================

#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "FooC.h"

static char *IOR = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = get_opts.optarg;
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
                       "Please specify the IOR for the servant"), -1);

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
  parse_args (argc, argv);

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

  // Invoke the doit() method of the foo reference.
  CORBA::Long result = foo->doit (env);

  if (env.exception () != 0)
    {
      env.print_exception ("Foo::doit");
      return -1;
    }

  // Print the result of doit () method of the foo reference.
  cout << result << endl;

  return 0;
}
