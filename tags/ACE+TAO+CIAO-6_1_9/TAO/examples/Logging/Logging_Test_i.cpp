// $Id$

#include "LoggerC.h"
#include "Logging_Test_i.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/debug.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/OS_NS_netdb.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_arpa_inet.h"
#include "ace/OS_NS_sys_time.h"

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
Logger_Client::init (int argc, ACE_TCHAR *argv[])
{
  this->argc_ = argc;
  this->argv_ = argv;

  try
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "\nTrying to initialize orb\n"));
      // Initialize the ORB
      orb_ = CORBA::ORB_init (argc,
                              argv,
                              "internet");

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "\nOrb initialized successfully\n"));

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      // Initialize the naming service
      int ret = this->init_naming_service ();
      if (ret != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize naming"
                           "services.\n"),
                          -1);
      // Create the logger instances
      ret = this->init_loggers ();
      if (ret != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize logger"
                           "instances.\n"),
                          -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("init");
      return -1;
    }

  return 0;

}

int
Logger_Client::init_naming_service (void)
{
  // Initialize the naming services
  if (my_name_client_.init (orb_.in ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to initialize "
                       "the TAO_Naming_Client.\n"),
                      -1);

  // Resolve an instance of the Logger_Factory
  CosNaming::Name factory_name (1);
  factory_name.length (1);
  factory_name[0].id = CORBA::string_dup ("Logger_Factory");

  CORBA::Object_var factory_ref =
    my_name_client_->resolve (factory_name);
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
    Logger_Factory::_narrow (factory_ref.in ());

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
    orb_->object_to_string (factory_.in ());
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "The factory IOR is <%s>\n",
                str.in ()));
  return 0;
}

int
Logger_Client::init_loggers (void)
{
  // Retrieve the Logger obj ref corresponding to key1 and
  // key2.
  try
    {
      this->logger_1_ = factory_->make_logger ("key1");

      this->logger_2_ = factory_->make_logger ("key2");

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

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\nTrying to resolve already created logger..."));
          Logger_var logger_3 = factory_->make_logger ("key1");

          if (CORBA::is_nil (logger_3.in ()))
            ACE_DEBUG ((LM_DEBUG,
                        "\nResolution failed."));
          else
            ACE_DEBUG ((LM_DEBUG,
                        "\nResolution succeeded."));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("init_loggers");
      return -1;
    }
  return 0;
}


// Execute client example code.

int
Logger_Client::run (void)
{
  try
    {
      // Create 3 Log_Records for the test
      Logger::Log_Record rec1;
      Logger::Log_Record rec2;
      Logger::Log_Record rec3;
      Logger::Log_Record rec4;
      ;
      // Setup the first log record
      this->init_record (rec1,
                         Logger::LM_DEBUG,
                         "log() test (1)\n");

      // Setup the second log record
      this->init_record (rec2,
                         Logger::LM_MAX,
                         "log() test (2)\n");

      // Setup the third log record
      this->init_record (rec3,
                         Logger::LM_INFO,
                         "logv() test (3)\n");

      // Setup the fourth log record
      this->init_record (rec4,
                         Logger::LM_EMERGENCY,
                         "log_twoway() test (4)\n");

      // If debugging, output the new log records
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\nFirst Log_Record created. Contents:\n"));
          this->show_record (rec1);

          ACE_DEBUG ((LM_DEBUG,
                      "\nSecond Log_Record created. Contents:\n"));
          this->show_record (rec2);

          ACE_DEBUG ((LM_DEBUG,
                      "\nThird log record created. Contents:\n"));
          this->show_record (rec3);

          ACE_DEBUG ((LM_DEBUG,
                      "\nFourth log record created. Contents:\n"));
          this->show_record (rec4);
        }

      // Change the verbosity.
      this->logger_1_->verbosity (Logger::VERBOSE_LITE);

      // Log the first Log_Record (VERBOSE_LITE)
      this->logger_1_->log (rec1);

      // Change the verbosity again.
      this->logger_2_->verbosity (Logger::VERBOSE);

      // Log the second Log_Record (VERBOSE)
      this->logger_2_->log (rec2);

      // Change the verbosity again
      this->logger_2_->verbosity (Logger::SILENT);

      // Log the third log record using logv() (this shows if the
      // verbosity level overrides the logger's verbosity level)
      this->logger_2_->logv (rec3, Logger::VERBOSE);

      // Change the verbosity again (so that regular log msgs can be
      // seen again)
      this->logger_2_->verbosity (Logger::VERBOSE);

      // Log the fourth record using log_twoway()
      this->logger_2_->log_twoway (rec4);
    }

  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("run");
    }
  return 0;
}

// Parses the command line arguments and returns an error status.

int
Logger_Client::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("d"));
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

  //FUZZ: disable check_for_lack_ACE_OS
  // Create and assign the timestamp.
  ACE_Time_Value time (ACE_OS::gettimeofday ());
  //FUZZ: enable check_for_lack_ACE_OS

  newrec.time = static_cast<CORBA::Long> (time.sec ());

  // Get and store the PID of the calling process.
  pid_t pid = ACE_OS::getpid ();
  newrec.app_id = pid;

  // Get and store the IP of the local host .
  char name[MAXHOSTNAMELEN];
  ACE_OS::hostname (name, MAXHOSTNAMELEN);
  hostent *he = ACE_OS::gethostbyname (name);
  newrec.host_addr =
    (reinterpret_cast<in_addr *> (he->h_addr_list[0])->s_addr);
}

void
Logger_Client::show_record (Logger::Log_Record &newrec)
{
  in_addr address;
  address.s_addr = newrec.host_addr;

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_DEBUG ((LM_DEBUG,
              " Log Priority: %d\n"
              "         Time: %d\n"
              "          PID: %ld\n"
              " Host Address: %s\n"
              "      Message: %s\n",
              newrec.type,
              newrec.time,
              newrec.app_id,
              inet_ntoa (address),
              newrec.msg_data.in ()));
  //FUZZ: enable check_for_lack_ACE_OS
}
