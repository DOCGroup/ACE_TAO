// $Id$
//client.cpp
//

#include "QuoterC.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"
#include <iostream>

int main (int argc, char *argv [])
{

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv,
                         "" /* the ORB name, it can be anything! */,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // There must be at least one argument, the file that has to be
      // retrieved
      if (argc < 2) {
        std::cerr << "Usage: " << argv[0]
                  << "File name to be retrieved ..." << std::endl;
        return 1;
      }

      // Use the first argument to create the factory object reference,
      // in real applications we use the naming service, but let's do
      // the easy part first!

      // Get a reference to the Naming Service
      CORBA::Object_var naming_context_object =
        orb->string_to_object (argv[1], ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Narrow to get the correct reference
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_context_object.in (),
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup ("Stock_Factory");

      // Resolve the name
      CORBA::Object_var factory_object =
        naming_context->resolve (name);
      
      // Now downcast the object reference to the appropriate type
      Quoter::Stock_Factory_var factory =
        Quoter::Stock_Factory::_narrow (factory_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             argv[1]),
                            1);
        }
      
      // Now get the full name and price of the other arguments:
      for (int i = 2; i != argc; ++i)
        {
          // Get the stock object
          Quoter::Stock_var stock =
            factory->get_stock (argv[i], ACE_TRY_ENV);
          ACE_TRY_CHECK;
          
          // Get its name, put it on a _var so it is automatically
          // released!
          CORBA::String_var full_name = stock->full_name (ACE_TRY_ENV);
          ACE_TRY_CHECK;
          
          // Now get the price
          CORBA::Double price = stock->price (ACE_TRY_ENV);
          ACE_TRY_CHECK;
          
          std::cout << "The price of a stock in \""
                    << full_name.in () << "\" is $"
                    << price << std::endl;
        }
      
      // Finally destroy the ORB
      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
    }
  ACE_CATCH (Quoter::Invalid_Stock_Symbol, ex)
    {
      std::cerr << "Invalid stock symbol <"
                << ">" << std::endl;
    }
  ACE_CATCH (CORBA::SystemException, ex) {
    std::cerr << "CORBA exception raised!" << ex << std::endl;
  }
  ACE_ENDTRY;
  
  return 0;
}
