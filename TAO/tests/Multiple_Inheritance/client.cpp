// $Id$
//
//===================================================================
//  = LIBRARY
//      TAO/tests/Multiple_Inheritance
//
//  = FILENAME
//     client.cpp
//
//  = DESCRIPTION
//      A client program for the Multiple Inheritance module
//
//  = AUTHOR
//     Irfan Pyarali
//
//====================================================================

#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "Multiple_InheritanceC.h"

ACE_RCSID(Multiple_Inheritance, client, "$Id$")

static char *ior = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
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

  if (ior == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant"), -1);

  // Indicates successful parsing of command line.
  return 0;
}

int
main (int argc, char **argv)
{
  CORBA::Environment env;

  // Parse the command-line arguments to get the IOR 
  parse_args (argc, argv);

  // Initialize the ORB
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB_init");
      return -1;
    }

  // Get the object reference with the IOR
  CORBA::Object_var object = orb->string_to_object (ior, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::string_to_object");
      return -1;
    }
  
  CORBA::String_var string;

  // Narrow the object reference
  A_var a = A::_narrow (object.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("A::_narrow");
      return -1;
    }

  // Narrow the object reference
  B_var b = B::_narrow (a.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("B::_narrow");
      return -1;
    }

  // Narrow the object reference
  C_var c = C::_narrow (a.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("C::_narrow");
      return -1;
    }

  // Narrow the object reference
  D_var d = D::_narrow (c.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("D::_narrow");
      return -1;
    }

  string = a->method1 (env);
  if (env.exception () != 0)
    {
      env.print_exception ("A::method1");
      return -1;
    }
  else
    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

  string = b->method1 (env);
  if (env.exception () != 0)
    {
      env.print_exception ("B::method1");
      return -1;
    }
  else
    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

  string = b->method2 (env);
  if (env.exception () != 0)
    {
      env.print_exception ("B::method2");
      return -1;
    }
  else
    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

  string = c->method1 (env);
  if (env.exception () != 0)
    {
      env.print_exception ("C::method1");
      return -1;
    }
  else
    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

  string = c->method3 (env);
  if (env.exception () != 0)
    {
      env.print_exception ("C::method3");
      return -1;
    }
  else
    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

  string = d->method1 (env);
  if (env.exception () != 0)
    {
      env.print_exception ("D::method1");
      return -1;
    }
  else
    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

  string = d->method2 (env);
  if (env.exception () != 0)
    {
      env.print_exception ("D::method2");
      return -1;
    }
  else
    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

  string = d->method3 (env);
  if (env.exception () != 0)
    {
      env.print_exception ("A::method3");
      return -1;
    }
  else
    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

  string = d->method4 (env);
  if (env.exception () != 0)
    {
      env.print_exception ("D::method4");
      return -1;
    }
  else
    ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

  return 0;
}
