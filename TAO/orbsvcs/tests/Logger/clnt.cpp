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

#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/OS.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/LoggerC.h"
#include "clnt.h"

// constructor

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
Logger_Client::setup_record (Logger::Log_Record newrec, Logger::Log_Priority lp,
			     const char* msg)
{
  ACE_Time_Value time;
  pid_t pid;
  hostent *he;
  struct utsname h_name;
   

  char msg_data[ACE_MAXLOGMSGLEN+1];
  ACE_OS::strncpy (msg_data, msg, ACE_MAXLOGMSGLEN);
  newrec.type = lp;

  time = ACE_OS::gettimeofday();
  newrec.time = time.sec();

  pid = getpid();
  newrec.app_id = pid;

  ACE_OS::uname(&h_name);
  he = (ACE_OS::gethostbyname (h_name.nodename));
  newrec.host_addr = inet_addr(he->h_addr);

  ACE_OS::strcpy ( newrec.msg_data, msg);
}

  
// Execute client example code.

int
Logger_Client::run (void)
{
  TAO_TRY
    {
      Logger::Log_Record rec1;
      Logger::Log_Record rec2;

      setup_record (rec1, Logger::LM_DEBUG, "Logging at logger 1");
      
      this->logger_1_->log (rec1, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      
      setup_record (rec2, Logger::LM_ERROR, "Logging at logger 2");

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

  TAO_TRY
    {
      // Initialize ORB.
      this->orb_ =
        CORBA::ORB_init (argc, argv, "internet", TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          -1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "Obtained naming_context\n"));

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;
           
      CosNaming::Name factory_name(1);
      factory_name.length (1);
      factory_name[0].id = CORBA::string_dup ("logger_factory");
      CORBA::Object_var factory_ref =
        naming_context->resolve (factory_name, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      /// The following resolve() must fail with an exception.

      CosNaming::Name factory_name2(1);
      factory_name2.length (1);
      factory_name2[0].id = CORBA::string_dup ("logger_factory2");

      CORBA::Object_var factory_ref2 =
        naming_context->resolve (factory_name2, TAO_TRY_ENV);

      if (TAO_TRY_ENV.exception ())
        {
          CosNaming::NamingContext::NotFound_ptr ex;
          ex = CosNaming::NamingContext::NotFound::_narrow (TAO_TRY_ENV.exception ());
          if (ex != 0)
            TAO_TRY_ENV.print_exception ("Negative test case for name not found, succeeded\n");
          else
            TAO_TRY_ENV.print_exception ("Negative test case for name not found, FAILED!\n");
        }

      if (CORBA::is_nil (factory_ref.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "resolved to nil object"), -1);

      ACE_DEBUG ((LM_DEBUG, "Logger_Factory resolved\n"));

      Logger_Factory_var factory =
        Logger_Factory::_narrow (factory_ref.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (factory.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "narrow returned nil"), -1);

      ACE_DEBUG ((LM_DEBUG, "Logger_Factory narrowed\n"));

      CORBA::String_var str =
        this->orb_->object_to_string (factory.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "The factory IOR is <%s>\n", str.in ()));

      // Now retrieve the Logger obj ref corresponding to key1 and key2
      this->logger_1_ = factory->make_logger ("key1", TAO_TRY_ENV);
      TAO_CHECK_ENV;
      this->logger_2_ = factory->make_logger ("key2", TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "Created two loggers"));

      if (CORBA::is_nil (this->logger_1_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "nil logger1"), -1);

      if (CORBA::is_nil (this->logger_2_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "nil logger2"), -1);

      ACE_DEBUG ((LM_DEBUG, "Created two loggers\n"));

      if (this->test_nesting_)
        {
          CosNaming::Name nested(1);
          nested.length (1);
          nested[0].id = CORBA::string_dup ("my_naming_context_1");

          CosNaming::NamingContext_var nc1 =
            naming_context->bind_new_context (nested, TAO_TRY_ENV);
          TAO_CHECK_ENV;

          if (CORBA::is_nil (nc1.in ()))
            ACE_ERROR_RETURN ((LM_ERROR, "nil nested context"), -1);

          ACE_DEBUG ((LM_DEBUG, "Nested naming context created\n"));

          CosNaming::Name logger1_name (1);
          logger1_name.length (1);
          logger1_name[0].id = CORBA::string_dup ("logger_1_");
          nc1->bind (logger1_name, this->logger_1_.in (), TAO_TRY_ENV);
          TAO_CHECK_ENV;

          ACE_DEBUG ((LM_DEBUG, "Bind in nested context succeded\n"));

          nc1->unbind (logger1_name, TAO_TRY_ENV);
          TAO_CHECK_ENV;

          ACE_DEBUG ((LM_DEBUG, "Unbind in nested context succeded\n"));

          // destroy the naming context created above.
          nc1->destroy (TAO_TRY_ENV);
          TAO_CHECK_ENV;

          ACE_DEBUG ((LM_DEBUG, "destroy of nested context succeded\n"));
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
