
// This little program tests my implementation of COS Naming Service.
// It uses the Logger_Factory server to create a number of logger objects.
// It then uses their object references to test functions supported by the nameserver.

// Usage: nameclient machinename
//


#include "logger.hh"
#include "CosNaming.hh"
#include <iostream.h>


int
main (int argc, char ** argv)
{

  // Bind to the appropriate servers.
  Logger_Factory_var factory;
  CosNaming::NamingContext_var nameservice;
  try 
    {
      factory = Logger_Factory::_bind (":logger_factory", argv[1]);
      nameservice = CosNaming::NamingContext::_bind (":nameserver", argv[1]);
    }
  catch (const CORBA::SystemException& se)
    {
      cout << "Unexpected exception: " << endl << &se;
      return -1;
    }

  // Create several logger objects (name specifies the level in the tree
  // where the logger will be bound)
  Logger_var l1 = factory->make_logger ("1");
  Logger_var l2 = factory->make_logger ("2");

  // Store object references received from the naming service.
  CORBA::Object_var l_obj;
  Logger_var logger;

  // Create names for registering loggers with the Naming Service.
  CosNaming::Name n1;
  n1.length (1);
  n1[0].id = CORBA::string_dup ("logger 1");
  CosNaming::Name n2;
  n2.length (1);
  n2[0].id = CORBA::string_dup ("logger 2");

  // use name service
  nameservice->bind (n1, l1);
  nameservice->bind (n2, l2);

  // test if it worked correctly
  l_obj = nameservice->resolve (n1);
  logger = Logger::_narrow (l_obj);
  logger->log ("expect logger 1");

    
  cout << "I get to the end" << endl;
  return 0;
}











