// $Id$
#include "Logger_Service.h"
#include "Log_Macros.h"
#include "ace/Get_Opt.h"
#include "ace/CORBA_macros.h"
#include "ace/Env_Value_T.h"
#include "tao/SystemException.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
// Needed to set ACE_LOG_MSG::msg_ostream()
// FUZZ: disable check_for_streams_include
#  include "ace/streams.h"
#endif /* !ACE_LACKS_IOSTREAM_TOTALLY */

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

    if (this->filename_.length () > 0)
      {
#if defined (ACE_LACKS_IOSTREAM_TOTALLY)

        FILE* output_stream = ACE_OS::fopen (this->filename_.c_str (), ACE_TEXT ("a"));

        ACE_LOG_MSG->msg_ostream (output_stream, 1);

#else /* ! ACE_LACKS_IOSTREAM_TOTALLY */

        ofstream* output_stream = 0;

        ACE_NEW_THROW_EX (output_stream,
                          ofstream (),
                          CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                              0,
                              ENOMEM),
                            CORBA::COMPLETED_NO));

        output_stream->open (ACE_TEXT_ALWAYS_CHAR (this->filename_.c_str ()),
                             ios::out | ios::app);

        if (!output_stream->bad ())
          {
            ACE_LOG_MSG->msg_ostream (output_stream, 1);
          }

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

        ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER);
        ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
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

  int
  Logger_Service::Initializer (void)
  {
    return ACE_Service_Config::process_directive (ace_svc_desc_Logger_Service);
  }
} // CIAO

using namespace CIAO;
ACE_STATIC_SVC_DEFINE (Logger_Service,
                       ACE_TEXT ("CIAO_Logger"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Logger_Service),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (CIAO_Logger, Logger_Service)

