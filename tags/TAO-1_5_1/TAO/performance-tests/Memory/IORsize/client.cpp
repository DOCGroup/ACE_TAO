// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "TestS.h"
#include "Foo_i.h"

ACE_RCSID(Hello, client, "$Id$")

const char *ior = "file://test.ior";
static int n = 10;
int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:n:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        n = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      Foo_i *foo_i;
      ACE_NEW_RETURN (foo_i,
                      Foo_i (orb.in ()),
                      1);

      PortableServer::ServantBase_var owner_transfer(foo_i);

      Test::Foo_var foo =
        foo_i->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Memory_Growth_var mem =
        Test::Memory_Growth::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (mem.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      // Make a sequence..
      Test::PayLoad pl (50000);
      pl.length (50000);

      for (CORBA::ULong l = 0;
           l != pl.length ();
           l++)
        {
          pl[l] = foo;
        }

      // Make a few calls to the remote object
      for (int iter = 0; iter != n; iter++)
        {
          mem->send_objref (pl
                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Let us run the event loop. This way we will not exit
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
