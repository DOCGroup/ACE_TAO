#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "FooC.h"

char *ior = 0;

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

  CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv, 0, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB_init");
      return -1;
    }

  parse_args (argc, argv);

  CORBA::Object_var object = orb->string_to_object (ior, env);
  if (env.exception () != 0)
    {
      env.print_exception ("CORBA::ORB::string_to_object");
      return -1;
    }

  Foo_var foo = Foo::_narrow (object.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("Foo::_bind");
      return -1;
    }

  CORBA::Long result = foo->doit (env);
  if (env.exception () != 0)
    {
      env.print_exception ("Foo::doit");
      return -1;
    }
  
  cout << result << endl;

  CORBA::release (orb);

  return 0;
}

