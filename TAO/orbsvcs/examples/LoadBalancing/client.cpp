#include "TestC.h"

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdlib.h"

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://obj.ior")};
  int niterations {100};
  int number {};
};

int parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:n:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'n':
        options.number = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        options.niterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -k <ior> -i <niterations>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Client_Options options;

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object (options.ior);

      Test::StockFactory_var stockfactory =
        Test::StockFactory::_narrow (tmp.in ());

      if (CORBA::is_nil (stockfactory.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Test::StockFactory reference <%s>\n"),
                             options.ior),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting Client %d\n"), options.number));

      for (int i = 0; i < options.niterations; ++i)
        {
          Test::Stock_var stock =
            stockfactory->get_stock ("RHAT");

          CORBA::String_var full_name = stock->full_name ();

          CORBA::Double const price = stock->price ();

          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The price of a stock in \"%C\" is $%f\n"),
                      full_name.in (),
                      price));
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in client.cpp:");
      return 1;
    }

  return 0;
}
