
// This little program tests my implementation of COS Naming Service.
// It uses the Logger_Factory server to create a number of logger objects.
// It then uses their object references to test functions supported by the nameserver.

// Usage: nameclient machinename
//

#include "loggerC.h"
#include <iostream.h>

int
main (int argc, char ** argv)
{
  Logger_Factory_ptr factory;
  // factory pointer for logger

  const char *logger_factory_key = "factory";
  // Key of factory obj ref.

  CORBA::Environment env;
  // Environment variable

  CORBA::Object_ptr objref;
  // storage of the factory objref

  char * hostname = "macarena";
  CORBA::ULong portnum = 10026;

  // retrieve the ORB
  CORBA::ORB_init (argc,
                   argv,
                   "logger",
                   env);

  if (env.exception () != 0)
    {
      env.print_exception ("ORB initialization");
      return 1;
    }

  // Retrieve a factory objref.
  objref = Logger_Factory::_bind (hostname,
                                  portnum,
                                  logger_factory_key,
                                  env);

  if (env.exception () != 0)
    {
      env.print_exception ("Logger_Factory::_bind");
      return 1;
    }

  if (CORBA::is_nil (objref) == CORBA::B_TRUE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " _bind returned null object for key (%s), host (%s), port (%d)\n",
		       logger_factory_key,
                       hostname,
                       portnum),
                       1);

  // Narrow the CORBA::Object reference to the stub object, checking
  // the type along the way using _is_a.  There is really no need to
  // narrow <objref> because <_bind> will return us the
  // <Cubit_Factory> pointer.  However, we do it so that we can
  // explicitly test the _narrow function.
  factory = Logger_Factory::_narrow (objref);

  if (factory == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Unable to narrow object reference to a Logger_Factory_ptr.\n"),
		      1);

  // Create several logger objects (name specifies the level in the tree
  // where the logger will be bound)
  Logger_var l1 = factory->make_logger ("key1", env);
  Logger_var l2 = factory->make_logger ("key2", env);

  l1->log ("Logging at logger 1", env);
  l2->log ("Logging at logger 2", env);
    
  cout << "....the end" << endl;
  return 0;
}











