// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"

const ACE_TCHAR *ior = ACE_TEXT("file://obj.ior");

int niterations = 100;
int number;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:i:"));
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object (ior);

      Test::StockFactory_var stockfactory =
        Test::StockFactory::_narrow (tmp.in ());

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

      //stockfactory->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in client.cpp:");
      return 1;
    }

  return 0;
}
