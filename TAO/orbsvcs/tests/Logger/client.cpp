// $Id$

#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/OS.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/LoggerC.h"
#include "client.h"

// Constructor
Logger_Client::Logger_Client (void)
  : test_nesting_ (0)
{
  // Do nothing
}

// Parses the command line arguments and returns an error status.
int
Logger_Client::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "dn");
  int c;
  
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'n':
        this->test_nesting_++;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n]"
                           "\n"
                           "    -d: increase debug level\n"
                           "    -n: test nesting in Naming Service\n",
			   this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

void
Logger_Client::init_record (Logger::Log_Record &newrec,
                            Logger::Log_Priority lp,
                            const char *msg)
{
  // Copy the message data into newrec.
  newrec.msg_data = CORBA::string_copy (msg);
  
  // Assign the log priority.
  newrec.type = lp;

  // Create and assign the timestamp.
  ACE_Time_Value time (ACE_OS::gettimeofday ());
  newrec.time = time.sec ();

  // Get and store the PID of the calling process.
  pid_t pid = ACE_OS::getpid ();
  newrec.app_id = pid;
  
  char name[MAXHOSTNAMELEN];
  ACE_OS::hostname (name, MAXHOSTNAMELEN);
  hostent *he = ACE_OS::gethostbyname (name);

  // Get and store the IP of the local host .
  newrec.host_addr =
    (ACE_reinterpret_cast (in_addr *,
			   he->h_addr_list[0])->s_addr);
}

void
Logger_Client::show_record (Logger::Log_Record &newrec)
{
  in_addr address;
  address.s_addr = newrec.host_addr;

  ACE_DEBUG ((LM_DEBUG,
	      " Log Priority: %d\n"
	      "         Time: %d\n"
	      "          PID: %d\n"
	      " Host Address: %s\n"
	      "      Message: %s\n",
	      newrec.type,
              newrec.time,
	      newrec.app_id,
              inet_ntoa (address),
	      newrec.msg_data.in ()));
}

CORBA::Object_var
Logger_Client::resolve_name (CosNaming::Name &name,
                             char *namestring,
			     CosNaming::NamingContext_var context,
			     CORBA::Environment &env) 
{
  name.length(1);
  name[0].id = CORBA::string_dup (namestring);

  return context->resolve (name, env); 
}

int
Logger_Client::nesting_test (CosNaming::Name &nested, 
                             char *contextstring,
			     CosNaming::NamingContext_var context,
			     CORBA::Environment &env)
{
  nested.length (1);
  nested[0].id = CORBA::string_dup (contextstring);
  
  CosNaming::NamingContext_var nc1 =
    context->bind_new_context (nested, env);
   
  if (CORBA::is_nil (nc1.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
		       "nil nested context"),
		      -1);
  ACE_DEBUG ((LM_DEBUG,
	      "Nested naming context created\n"));
  
  CosNaming::Name logger1_name (1);
  logger1_name.length (1);
  logger1_name[0].id = CORBA::string_dup ("logger_1_");
  nc1->bind (logger1_name,
	     this->logger_1_.in (),
	     env);

  ACE_DEBUG ((LM_DEBUG,
	      "Bind in nested context succeded\n"));

  nc1->unbind (logger1_name,
	       env);

  ACE_DEBUG ((LM_DEBUG,
	      "Unbind in nested context succeded\n"));

  // destroy the naming context created above.
  nc1->destroy (env);

  ACE_DEBUG ((LM_DEBUG,
	      "destroy of nested context succeded\n"));
}

// Execute client example code.

int
Logger_Client::run (void)
{
  TAO_TRY
    {
      // Create 2 Log_Records for the test
      Logger::Log_Record rec1;
      Logger::Log_Record rec2;

      // Setup the first log record
      this->init_record (rec1,
                         Logger::LM_DEBUG,
                         "Logging at logger 1\n");
      
      // Setup the second log record
      this->init_record (rec2,
                         Logger::LM_ERROR,
                         "Logging at logger 2\n");

      // If debugging, output the new log records
      if (TAO_debug_level > 0)
	{
	  ACE_DEBUG ((LM_DEBUG,
                      "First Log_Record created. Contents:\n"));
	  this->show_record (rec1);

	  ACE_DEBUG ((LM_DEBUG,
                      "Second Log_Record created. Contents:\n"));
	  this->show_record (rec2);
	}
      
      // @@ Matt, I recommend that you make a couple of calls to
      // log(), using different verbosity levels each time.  Log the
      // first Log_Record, i.e.,
      // this->logger_1_->verbosity (foo1);

      this->logger_1_->log (rec1, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Log the second Log_Record.   
      this->logger_2_->log (rec2, TAO_TRY_ENV);
      TAO_CHECK_ENV;

    }
  TAO_CATCHANY
    {      
      TAO_TRY_ENV.print_exception ("run");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

Logger_Client::~Logger_Client (void)
{
  // Do nothing
}

int
Logger_Client::init (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  // @@ Matt, this method is rather long.  Can you please break it
  // into several smaller (private) methods?

  TAO_TRY
    {
      // Initialize ORB.
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    "internet",
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // @@ Somewhere around here is where we want to initialize the
      // new TAO_Naming_Client.

      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          -1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Obtained naming_context\n"));
      
      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;
           
      CosNaming::Name factory_name (1);
      CORBA::Object_var factory_ref =
	this->resolve_name (factory_name,
                            "logger_factory",
			    naming_context,
                            TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // The following code must fail with an exception.
      CosNaming::Name factory_name2 (1);
      CORBA::Object_var factory_ref2 =
	this->resolve_name (factory_name2,
                            "logger_factory2",
			    naming_context,
                            TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      if (TAO_TRY_ENV.exception () && TAO_debug_level > 0)
        {
          CosNaming::NamingContext::NotFound_ptr ex =
	    CosNaming::NamingContext::NotFound::_narrow (TAO_TRY_ENV.exception ());
          if (ex != 0)
            TAO_TRY_ENV.print_exception
	      ("Negative test case for name not found, succeeded\n");
          else
	    TAO_TRY_ENV.print_exception
	      ("Negative test case for name not found, FAILED!\n");
	  TAO_TRY_ENV.clear ();
        }

      if (CORBA::is_nil (factory_ref.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "resolved to nil object"),
                          -1);
      
      if (TAO_debug_level > 0)
	  ACE_DEBUG ((LM_DEBUG,
		      "\nLogger_Factory resolved\n"));
      
      Logger_Factory_var factory =
        Logger_Factory::_narrow (factory_ref.in (),
                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (factory.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "narrow returned nil"),
                          -1);
      if (TAO_debug_level > 0)
	  ACE_DEBUG ((LM_DEBUG,
		      "\nLogger_Factory narrowed\n"));
      
      CORBA::String_var str =
        this->orb_->object_to_string (factory.in (),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (TAO_debug_level > 0)
	ACE_DEBUG ((LM_DEBUG,
                    "The factory IOR is <%s>\n",
                    str.in ()));

      // Now retrieve the Logger obj ref corresponding to key1 and
      // key2.
      this->logger_1_ = factory->make_logger ("key1",
                                              TAO_TRY_ENV);
      TAO_CHECK_ENV;
      this->logger_2_ = factory->make_logger ("key2",
                                              TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (TAO_debug_level > 0)
	{      
	      
      if (CORBA::is_nil (this->logger_1_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "nil logger1"),
                          -1);
      
      if (CORBA::is_nil (this->logger_2_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "nil logger2"),
                          -1);

      ACE_DEBUG ((LM_DEBUG,
                  "Created two loggers\n"));
	}
      
      if (this->test_nesting_)
        {
	  CosNaming::Name nested (1);
	  this->nesting_test (nested,
                              "my_naming_context_1",
                              naming_context,
                              TAO_TRY_ENV);
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// This function runs the test.

int
main (int argc, char **argv)
{
  Logger_Client logger_client;

  if (logger_client.init (argc, argv) != 0)
    return 1;

  return logger_client.run ();
}
