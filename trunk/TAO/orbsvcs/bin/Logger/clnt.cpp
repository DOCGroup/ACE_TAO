// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/bin/Logger
//
// = FILENAME
//    clnt.cpp
//
// = DESCRIPTION 
//    This program tests an implementation of a logger service.  It uses the
//    Logger_Factory server to create a number of logger objects.  It then
//    uses their object references to test functions supported by the
//    logger server. 
//
// = AUTHORS
//      Sergio Flores-Gaitan
//
// ============================================================================

#include <iostream.h>
#include "loggerC.h"
#include "clnt.h"

// constructor

Logger_Client::Logger_Client (void)
  : logger_factory_key_ ("factory"),
    hostname_ ("localhost"),
    portnum_ (TAO_DEFAULT_SERVER_PORT),
    exit_later_ (0),
    factory_ (Logger_Factory::_nil ()),
    objref_ (CORBA::Object::_nil ()),
    orb_ptr_ (0),
    logger_1_ (Logger::_nil ()),
    logger_2_ (Logger::_nil ())
{
}

// Parses the command line arguments and returns an error status.

int
Logger_Client::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "d:h:p:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'h':
        hostname_ = ACE_OS::strdup (get_opts.optarg);
        break;
      case 'p':
        portnum_ = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'x':
        this->exit_later_++;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-h hostname]"
                           " [-p port]"
                           " [-x]"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Execute client example code.

int
Logger_Client::run (void)
{
  this->logger_1_->log ("Logging at logger 1", this->env_);
  this->logger_2_->log ("Logging at logger 2", this->env_);
    
  if (this->exit_later_)
    {
      // this->logger_1_->please_exit (this->env_);
      //this->logger_2_->please_exit (this->env_);
      dexc (this->env_, "server, please ACE_OS::exit");
    }
  return 0;
}

Logger_Client::~Logger_Client (void)
{
  // Free resources
  CORBA::release (this->logger_1_);
  CORBA::release (this->logger_2_);
  CORBA::release (this->factory_);
  CORBA::release (this->orb_ptr_);
}

int
Logger_Client::init (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  // Parse command line and verify parameters.
  if (this->parse_args () == -1)
    {
      return 1;
    }

  // retrieve the ORB
  this->orb_ptr_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    "logger",
                                    this->env_);

  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("ORB initialization");
      return 1;
    }

  // Retrieve a factory objref.
  this->objref_ = Logger_Factory::_bind (this->hostname_,
                                        this->portnum_,
                                        this->logger_factory_key_,
                                        this->env_);

  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("Logger_Factory::_bind");
      return 1;
    }

  if (CORBA::is_nil (this->objref_) == CORBA::B_TRUE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " _bind returned null object for key (%s), host (%s), port (%d)\n",
                       this->logger_factory_key_,
                       this->hostname_,
                       this->portnum_),
                      1);

  // Narrow the CORBA::Object reference to the stub object, checking
  // the type along the way using _is_a.  There is really no need to
  // narrow <objref> because <_bind> will return us the
  // <Logger_Factory> pointer.  However, we do it so that we can
  // explicitly test the _narrow function.
  this->factory_ = Logger_Factory::_narrow (this->objref_);

  if (this->factory_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Unable to narrow object reference to a Logger_Factory_ptr.\n"),
		      1);

  // Now retrieve the Logger obj ref corresponding to key1 and key2
  this->logger_1_ = this->factory_->make_logger ("key1", this->env_);
  this->logger_2_ = this->factory_->make_logger ("key2", this->env_);

  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("factory_->make_logger");
      return 1;
    }

  if (CORBA::is_nil (this->logger_1_))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "null logger objref returned by factory\n"),
                      1);

  if (CORBA::is_nil (this->logger_2_))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "null logger objref returned by factory\n"),
                      1);

  return 0;
}

// This function runs the test.

int
main (int argc, char **argv)
{
  int rc;
  Logger_Client logger_client;
 
  if (logger_client.init (argc, argv) != 0)
    return 1;
  else
    rc = logger_client.run ();

  cout << "....the end" << endl;
  return rc;
}




#if 0













int
main (int argc, char ** argv)
{
  CORBA::Environment env;
  // Environment variable

  CORBA::Object_ptr objref;
  // storage of the factory objref

  Logger_Factory_ptr factory;
  // factory pointer for logger

  const char *logger_factory_key = "factory";
  // Key of factory obj ref.

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
  // <LoggerFactory> pointer.  However, we do it so that we can
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
#endif
