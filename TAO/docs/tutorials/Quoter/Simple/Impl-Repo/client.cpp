#include "QuoterC.h"

#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb (CORBA::ORB_init (argc, argv));

      // The first argument names the factory; the remaining arguments name stocks.
      if (argc < 3)
        {
          std::cerr << "Usage: " << argv[0]
                    << " Factory_IOR symbol symbol..." << std::endl;
          return 1;
        }

      CORBA::Object_var factory_object (orb->string_to_object (argv[1]));
      Quoter::Stock_Factory_var factory (
        Quoter::Stock_Factory::_narrow (factory_object.in ()));

      for (int i = 2; i < argc; ++i)
        {
          try
            {
              Quoter::Stock_var stock (
                factory->get_stock (ACE_TEXT_ALWAYS_CHAR (argv[i])));
              CORBA::String_var const full_name (stock->full_name ());
              CORBA::Double const price = stock->price ();

              std::cout << "The price of a stock in \""
                        << full_name.in () << "\" is $"
                        << price << std::endl;
            }
          catch (Quoter::Invalid_Stock_Symbol const &)
            {
              std::cerr << "Invalid stock symbol <"
                        << argv[i] << ">" << std::endl;
            }
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &)
    {
      std::cerr << "CORBA exception raised!" << std::endl;
      return 1;
    }

  return 0;
}
