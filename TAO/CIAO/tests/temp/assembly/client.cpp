// $Id$

#include "controlC.h"
#include "Client_init.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Hello, client, "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
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

      if (parse_args (argc, argv) != 0)
        return 1;

      CIAO::Client_init (orb.in ());

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ATest::controller_var ctrlr =
        ATest::controller::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (ctrlr.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      ::Components::Deployment::AssemblyFactory_var fac
          = ctrlr->get_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ::Components::Cookie_var ck1 = fac->create_assembly ("test1"
                                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ::Components::Cookie_var ck2 = fac->create_assembly ("test2"
                                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ::Components::Deployment::Assembly_var asb1
          = fac->lookup (ck1.in ()
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ::Components::Deployment::Assembly_var asb2
          = fac->lookup (ck2.in ()
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      asb1->build (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      asb1->tear_down (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      fac->destroy (ck1.in ()
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      asb2->build (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // This should tear down assembly2 first.
      fac->destroy (ck2.in ()
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ctrlr->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
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
