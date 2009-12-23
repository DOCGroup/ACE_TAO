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
      trace_ (false)
  {
  }

  int
  Logger_Service::init (int argc, ACE_TCHAR * argv[])
  {
    // Get prospective values from the environment first, those given on
    // command line can override
    ACE_Env_Value<int> log (ACE_TEXT("DANCE_LOG_LEVEL"), DAnCE_debug_level);
    DAnCE_debug_level = log;

    ACE_Env_Value<int> trace (ACE_TEXT("DANCE_TRACE_ENABLE"), this->trace_);
    this->trace_ = trace;

    ACE_Env_Value<const ACE_TCHAR *> filename (ACE_TEXT("DANCE_LOG_FILE"), this->filename_.c_str ());
    this->filename_ = filename;

    this->parse_args (argc, argv);

    if (this->trace_)
      {
        DANCE_ENABLE_TRACE ();
      }
    else
      {
        DANCE_DISABLE_TRACE ();
      }

    return 0;
  }


  void
  Logger_Service::parse_args (int argc, ACE_TCHAR **argv)
  {
    DANCE_TRACE ("Logger_Service::parse_args");

    ACE_Get_Opt get_opts (argc,
                          argv,
                          "l:tf:",
                          0,
                          0,
                          ACE_Get_Opt::RETURN_IN_ORDER);

    get_opts.long_option (ACE_TEXT("log-level"), 'l', ACE_Get_Opt::ARG_OPTIONAL);
    get_opts.long_option (ACE_TEXT("trace"), 't', ACE_Get_Opt::ARG_OPTIONAL);
    get_opts.long_option (ACE_TEXT("log-file"), 'f', ACE_Get_Opt::ARG_OPTIONAL);

    int c;
    while ( (c = get_opts ()) != -1)
      {
        switch (c)
          {
            case 't':
              DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("Logger_Service::parse_args - ")
                            ACE_TEXT("Trace enabled\n"),
                            get_opts.opt_arg ()));
              this->trace_ = true;
              break;

            case 'l':
              DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("Logger_Service::parse_args - ")
                            ACE_TEXT("Log level set to %C\n"),
                            get_opts.opt_arg ()));
              DAnCE_debug_level = ACE_OS::atoi (get_opts.opt_arg ());
              break;

            case 'f':
              DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT("Logger_Service::parse_args - ")
                            ACE_TEXT("Log file set to %C\n"),
                            get_opts.opt_arg ()));
              this->filename_ = get_opts.opt_arg ();
              break;
          }
      }
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

