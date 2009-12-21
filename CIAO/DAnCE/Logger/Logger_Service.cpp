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
                int const level = ACE_OS::atoi (argv[i + 1]);

                if (level != 0)
                  {
                    DAnCE_debug_level = level;
                  }
              }
          }

        if (ACE_OS::strncmp (argv[i], lfs, 2) == 0 ||
            ACE_OS::strncmp (argv[i], lfl, 10 ) == 0)
          {
            if ((i + 1) < argc && *argv[i + 1] != '-')
              {
                this->filename_ = argv[i+1];
              }
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

