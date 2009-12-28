// $Id$
#include "Logger_Service.h"
#include "Log_Macros.h"
#include "ace/Get_Opt.h"
#include "ace/CORBA_macros.h"
#include "ace/Env_Value_T.h"
#include "tao/SystemException.h"

namespace CIAO
{
  Logger_Service::Logger_Service (void)
    : filename_ (ACE_TEXT("")),
      trace_ (false)
  {
  }

  int
  Logger_Service::init (int argc, ACE_TCHAR * argv[])
  {
    // Get prospective values from the environment first, those given on
    // command line can override
    ACE_Env_Value<int> log (ACE_TEXT("CIAO_LOG_LEVEL"), CIAO_debug_level);
    CIAO_debug_level = log;

    ACE_Env_Value<int> trace (ACE_TEXT("CIAO_TRACE_ENABLE"), 0);
    this->trace_ = (trace != 0);

    ACE_Env_Value<const ACE_TCHAR *> filename (ACE_TEXT("CIAO_LOG_FILE"), this->filename_.c_str ());
    this->filename_ = filename;

    this->parse_args (argc, argv);

    if (this->trace_)
      {
        CIAO_ENABLE_TRACE ();
      }
    else
      {
        CIAO_DISABLE_TRACE ();
      }

    return 0;
  }

  void
  Logger_Service::parse_args (int argc, ACE_TCHAR **argv)
  {
    CIAO_TRACE ("Logger_Service::parse_args");

    ACE_Get_Opt get_opts (argc,
                          argv,
                          "l:tf:",
                          0,
                          0,
                          ACE_Get_Opt::RETURN_IN_ORDER);

    get_opts.long_option (ACE_TEXT("log-level"), 'l', ACE_Get_Opt::ARG_REQUIRED);
    get_opts.long_option (ACE_TEXT("trace"), 't', ACE_Get_Opt::NO_ARG);
    get_opts.long_option (ACE_TEXT("log-file"), 'f', ACE_Get_Opt::ARG_REQUIRED);

    int c;
    while ( (c = get_opts ()) != -1)
      {
        switch (c)
          {
            case 't':
              CIAO_DEBUG (9, (LM_TRACE, CLINFO ACE_TEXT("Logger_Service::parse_args - ")
                            ACE_TEXT("Trace enabled\n"),
                            get_opts.opt_arg ()));
              this->trace_ = true;
              break;

            case 'l':
              CIAO_DEBUG (9, (LM_TRACE, CLINFO ACE_TEXT("Logger_Service::parse_args - ")
                            ACE_TEXT("Log level set to %s\n"),
                            get_opts.opt_arg ()));
              CIAO_debug_level = ACE_OS::atoi (get_opts.opt_arg ());
              break;

            case 'f':
              CIAO_DEBUG (9, (LM_TRACE, CLINFO ACE_TEXT("Logger_Service::parse_args - ")
                            ACE_TEXT("Log file set to %s\n"),
                            get_opts.opt_arg ()));
              this->filename_ = get_opts.opt_arg ();
              break;
          }
      }
  }

  ACE_Log_Msg_Backend *
  Logger_Service::get_logger_backend (CORBA::ORB_ptr)
  {
    File_Logger_Backend * the_backend = 0;
    ACE_NEW_THROW_EX (the_backend,
                      File_Logger_Backend (this->filename_.c_str()),
                      CORBA::NO_MEMORY());
    return the_backend;
  }
} // CIAO

using namespace CIAO;
ACE_FACTORY_DEFINE (CIAO_Logger, Logger_Service)

