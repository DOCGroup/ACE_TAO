// $Id$

#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/OS.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/LoggerC.h"
#include "client_i.h"

ACE_RCSID(Logger, client_i, "$Id$")

// Constructor
Logger_Client::Logger_Client (void)
{
  // Do nothing
}

// Destructor
Logger_Client::~Logger_Client (void)
{
  // Do nothing
}

int
Logger_Client::init (int argc, char *argv[])
{
  this->argc_ = argc;
  this->argv_ = argv;

  TAO_TRY
    {
      // Initialize the ORB
      orb_ = CORBA::ORB_init (argc,
                              argv,
                              "internet",
                              TAO_TRY_ENV);
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "\nTrying to initialize orb\n"));

      TAO_CHECK_ENV;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "\nOrb initialized successfully\n"));

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      // Initialize the naming service
      if (this->init_naming_service (TAO_TRY_ENV) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize naming"
                           "services.\n"),
                          -1);
      // Create the logger instances
      if (this->init_loggers (TAO_TRY_ENV) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize logger"
                           "instances.\n"),
                          -1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;

}

int
Logger_Client::init_naming_service (CORBA::Environment &env)
{
  // Initialize the naming services
  if (my_name_client_.init (orb_) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to initialize "
                       "the TAO_Naming_Client. \n"),
                      -1);

  // Resolve an instance of the Logger_Factory
  CosNaming::Name factory_name (1);
  factory_name.length (1);
  factory_name[0].id = CORBA::string_dup ("logger_factory");

  CORBA::Object_var factory_ref =
    my_name_client_->resolve (factory_name,
                              env);
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "\nFactory_ref resolved\n"));

  if (CORBA::is_nil (factory_ref.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "resolved to nil object"),
                      -1);
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "\nLogger_Factory resolved\n"));

  // Narrow the factory and check the success
  factory_ =
    Logger_Factory::_narrow (factory_ref.in (),
                             env);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "\nFactory narrowed\n"));
  if (CORBA::is_nil (factory_.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "narrow returned nil"),
                      -1);
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "\nLogger_Factory narrowed\n"));

  // If debugging, get the factory's IOR
  CORBA::String_var str =
    orb_->object_to_string (factory_.in (),
                            env);
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "The factory IOR is <%s>\n",
                str.in ()));
  return 0;
}

int
Logger_Client::init_loggers (CORBA::Environment &env)
{
  // Retrieve the Logger obj ref corresponding to key1 and
  // key2.
  this->logger_1_ = factory_->make_logger ("key1",
                                           env);
  this->logger_2_ = factory_->make_logger ("key2",
                                           env);

  if (CORBA::is_nil (this->logger_1_.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "nil logger1"),
                      -1);

  if (CORBA::is_nil (this->logger_2_.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "nil logger2"),
                      -1);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "Created two loggers\n"));
  return 0;
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
                         "Praise Bob! (1)\n");

      // Setup the second log record
      this->init_record (rec2,
                         Logger::LM_ERROR,
                         "Beware The Stark Fist of Removal. (2)\n");

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

      // Change the verbosity.
      this->logger_1_->verbosity (Logger::VERBOSE_LITE, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Log the first Log_Record
      this->logger_1_->log (rec1, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Change the verbosity again.
      this->logger_2_->verbosity (Logger::VERBOSE, TAO_TRY_ENV);
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

// Parses the command line arguments and returns an error status.

int
Logger_Client::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "d");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           "\n"
                           "    -d: increase debug level\n",
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
  newrec.msg_data = CORBA::string_dup (msg);

  // Assign the log priority.
  newrec.type = lp;

  // Create and assign the timestamp.
  ACE_Time_Value time (ACE_OS::gettimeofday ());
  newrec.time = time.sec ();

  // Get and store the PID of the calling process.
  pid_t pid = ACE_OS::getpid ();
  newrec.app_id = pid;

  // Get and store the IP of the local host .
  char name[MAXHOSTNAMELEN];
  ACE_OS::hostname (name, MAXHOSTNAMELEN);
  hostent *he = ACE_OS::gethostbyname (name);
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
