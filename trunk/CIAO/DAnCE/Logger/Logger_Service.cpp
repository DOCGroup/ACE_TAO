// $Id$
#include "Logger_Service.h"
#include "ace/Get_Opt.h"
#include "ace/Env_Value_T.h"
#include "ace/CORBA_macros.h"
#include "tao/SystemException.h"
#include "Log_Macros.h"

namespace DAnCE
  {
  Logger_Service::Logger_Service (void)
    : filename_ (ACE_TEXT("")),
      trace_ (false),
      log_level_ (5)
  {
  }

  int
  Logger_Service::init (int argc, ACE_TCHAR * argv[])
  {
    // Get prospective values from the environment first, those given on
    // command line can override
    ACE_Env_Value<int> log (ACE_TEXT("DANCE_LOG_LEVEL"), this->log_level_);

    this->log_level_ = log;

    ACE_Env_Value<int> trace (ACE_TEXT("DANCE_TRACE_ENABLE"), this->trace_);
    this->trace_ = trace;

    ACE_Env_Value<const ACE_TCHAR *> filename (ACE_TEXT("DANCE_LOG_FILE"), this->filename_.c_str ());
    this->filename_ = filename;

    this->parse_args (argc, argv);

    this->set_levels ();

    return 0;
  }


  void
  Logger_Service::parse_args (int argc, ACE_TCHAR **argv)
  {
    DANCE_TRACE ("DAnCE_Logger::parse_args");

    ACE_Get_Opt get_opts (argc,
                          argv,
                          "l:tf:",
                          0,
                          0,
                          ACE_Get_Opt::RETURN_IN_ORDER);

    get_opts.long_option (ACE_TEXT("log-level"), 'l', ACE_Get_Opt::ARG_OPTIONAL);
    get_opts.long_option (ACE_TEXT("trace"), 't', ACE_Get_Opt::ARG_REQUIRED);
    get_opts.long_option (ACE_TEXT("log-file"), 'f', ACE_Get_Opt::ARG_OPTIONAL);

    char c;
    int level = 0;
    while ( (c = get_opts ()) != -1)
      {
        switch (c)
          {
          case 'l':
            DANCE_DEBUG ((LM_TRACE, DLINFO ACE_TEXT ("DAnCE_Logger::parse_args -  ")
                          ACE_TEXT ("Log level is %s\n"),
                          get_opts.opt_arg ()));
            level = ACE_OS::atoi (get_opts.opt_arg ());
            if (level != 0)
              this->log_level_ = level;
            break;
          case 't':
            DANCE_DEBUG ((LM_TRACE, DLINFO ACE_TEXT ("DAnCE_Logger::parse_args -  ")
                          ACE_TEXT ("Trace enabled\n")));
            this->trace_ = true;
            break;

          case 'f':
            DANCE_DEBUG ((LM_TRACE, DLINFO ACE_TEXT ("DAnCE_Logger::parse_args -  ")
                          ACE_TEXT ("Log file becomes %s\n"),
                          get_opts.opt_arg ()));
            this->filename_ = get_opts.opt_arg ();
            break;
          case 'h':
            //case '?': // Display help for use of the server.
            //default:
            DANCE_ERROR ((LM_ERROR,
                          "Log service options:  %C\n"
                          "\t-l|--log-level <level>\t\tLog level [1-10]\n"
                          "\t-t|--trace\t\tEnable trace. Log level is set to 10\n"
                          "\t-f|--log-file <file name> \t\tLog file\n"
                          "\t-h|--help\t\t\tThis help message\n"
                          ,argv [0]));
            break;
          }
      }
  }

  void
  Logger_Service::set_levels (void)
  {
    if (this->trace_)
      {
        DANCE_ENABLE_TRACE ();
        this->log_level_ = 10;
      }
    else
      {
        DANCE_DISABLE_TRACE ();
      }

    u_long new_mask = 0;

    if (this->log_level_ >= 9)
      {
        new_mask |= LM_TRACE;
      }
    if (this->log_level_ >= 8)
      {
        new_mask |= LM_DEBUG;
      }
    if (this->log_level_ >= 7)
      {
        new_mask |= LM_INFO;
      }
    if (this->log_level_ >= 6)
      {
        new_mask |= LM_NOTICE;
      }
    if (this->log_level_ >= 5)
      {
        new_mask |= LM_WARNING;
      }
    if (this->log_level_ >= 4)
      {
        new_mask |= LM_ERROR;
      }
    if (this->log_level_ >= 3)
      {
        new_mask |= LM_CRITICAL;
      }
    if (this->log_level_ >= 2)
      {
        new_mask |= LM_ALERT;
      }
    if (this->log_level_ >= 1)
      {
        new_mask |= LM_EMERGENCY;
      }

    ACE_Log_Msg::instance()->priority_mask(new_mask, ACE_Log_Msg::PROCESS);
    DANCE_DEBUG ( (LM_TRACE, DLINFO ACE_TEXT("Logging level is set to %i\n"), this->log_level_));
  }

  ACE_Log_Msg_Backend *
  Logger_Service::get_logger_backend (CORBA::ORB_ptr)
  {
    if (this->filename_ != ACE_TEXT(""))
      {
        File_Logger_Backend * the_backend = 0;
        ACE_NEW_THROW_EX (the_backend,
                          File_Logger_Backend (this->filename_.c_str()),
                          CORBA::NO_MEMORY());
        return the_backend;
      }
    return 0;
  }

} // DAnCE

using namespace DAnCE;
ACE_FACTORY_DEFINE (DAnCE_Logger, Logger_Service)

