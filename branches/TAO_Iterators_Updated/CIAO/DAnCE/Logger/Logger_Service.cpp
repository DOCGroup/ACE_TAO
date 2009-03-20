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
    : filename_ (""),
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

    ACE_Env_Value<const char *> filename (ACE_TEXT("DANCE_LOG_FILE"), this->filename_.c_str ());
    this->filename_ = filename;

    this->parse_args (argc, argv);

    this->set_levels ();

    return 0;
  }


  void
  Logger_Service::parse_args (int argc, ACE_TCHAR **argv)
  {
    const ACE_TCHAR *shortl = ACE_TEXT("-l");
    const ACE_TCHAR *longl = ACE_TEXT("--log-level");
    const ACE_TCHAR *tracel = ACE_TEXT("--trace");
    //    const ACE_TCHAR *traces = "-t";
    const ACE_TCHAR *lfl = ACE_TEXT("--log-file");
    const ACE_TCHAR *lfs = ACE_TEXT("-f");

    // We need to actually FIND the -l option, as the get_opt won't ignore
    // the ORB options and such.
    for (int i = 0; i < argc; ++i)
      {
        if (//ACE_OS::strncmp (argv[i], traces, 2) == 0 ||
            ACE_OS::strncmp (argv[i], tracel, 7) == 0)
          {
            this->trace_ = true;
            continue;
          }

        if (ACE_OS::strncmp (argv[i], shortl, 2) == 0 ||
            ACE_OS::strncmp (argv[i], longl, 11 ) == 0)
          {
            if ((i + 1) < argc && *argv[i + 1] != '-')
              {
                int level = ACE_OS::atoi (argv[i + 1]);

                if (level != 0)
                  this->log_level_ = level;
              }
          }

        if (ACE_OS::strncmp (argv[i], lfs, 2) == 0 ||
            ACE_OS::strncmp (argv[i], lfl, 10 ) == 0)
          {
            if ((i + 1) < argc && *argv[i + 1] != '-')
              {
                this->filename_ = ACE_TEXT_ALWAYS_CHAR (argv[i+1]);
              }
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
    DANCE_DEBUG ( (LM_TRACE, DLINFO "Logging level is set to %i\n", this->log_level_));
  }

  ACE_Log_Msg_Backend *
  Logger_Service::get_logger_backend (CORBA::ORB_ptr)
  {
    if (this->filename_ != "")
      {
        File_Logger_Backend * the_backend;
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

