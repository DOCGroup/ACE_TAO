#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"


ACE_RCSID (LoadBalancing,
           client,
           "$Id$")


const char *ior = "file://obj.ior";

int niterations = 100;
int number;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:n:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'n':
        number = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i <niterations> "
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

      CORBA::Object_var tmp =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::StockFactory_var stockfactory =
        Test::StockFactory::_narrow (tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (stockfactory.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::StockFactory reference <%s>\n",
                             ior),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, "Starting Client %d\n", number));

      for (int i = 0; i < niterations; ++i)
        {
          Test::Stock_var stock =
            stockfactory->get_stock ("RHAT");

          CORBA::String_var full_name = stock->full_name ();

          CORBA::Double price = stock->price ();

          ACE_DEBUG ((LM_DEBUG, "The price of a stock in \"%s\" is $%f\n",
                      full_name.in (),
                      price));
        }

      //stockfactory->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      //ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught in client.cpp:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
