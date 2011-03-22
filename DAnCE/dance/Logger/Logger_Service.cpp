// $Id$
#include "Logger_Service.h"
#include "ace/Get_Opt.h"
#include "ace/Env_Value_T.h"
#include "ace/CORBA_macros.h"
#include "tao/SystemException.h"
#include "Log_Macros.h"
#include "ace/Service_Config.h"
#include "ace/Arg_Shifter.h"

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)

// FUZZ: disable check_for_streams_include
#  include "ace/streams.h"
#endif /* !ACE_LACKS_IOSTREAM_TOTALLY */

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

    ACE_Env_Value<int> trace (ACE_TEXT("DANCE_TRACE_ENABLE"), 0);
    this->trace_ = (trace != 0);

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

    if (this->filename_.length () > 0)
      {
#if defined (ACE_LACKS_IOSTREAM_TOTALLY)

        FILE* output_stream = ACE_OS::fopen (this->filename_.c_str (), ACE_TEXT ("a"));

        ACE_LOG_MSG->msg_ostream (output_stream, 1);

#else /* ! ACE_LACKS_IOSTREAM_TOTALLY */

        ofstream* output_stream = 0;

        //
        // note: we are allocating dynamic memory here....but
        // I assume it will persist for the life of the program
        //

        ACE_NEW_THROW_EX (output_stream,
                          ofstream (),
                          CORBA::NO_MEMORY (
                                            CORBA::SystemException::_tao_minor_code (0,
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
    DANCE_TRACE ("Logger_Service::parse_args");

    ACE_Arg_Shifter arg_shifter (argc, argv);

    while (arg_shifter.is_anything_left ())
      {
        const ACE_TCHAR *current_arg = 0;
        if (0 != (current_arg =
                  arg_shifter.get_the_parameter
                  (ACE_TEXT ("-DAnCELogLevel"))))
          {
            DAnCE_debug_level = ACE_OS::atoi (current_arg);

            arg_shifter.consume_arg ();
          }
        else if (0 == arg_shifter.cur_arg_strncasecmp
                 (ACE_TEXT ("-DAnCETraceEnable")))
          {
            this->trace_ = true;

            arg_shifter.consume_arg ();
          }
        else if (0 != (current_arg =
                       arg_shifter.get_the_parameter
                       (ACE_TEXT ("-DAnCELogFile"))))
          {
            this->filename_ = current_arg;

            arg_shifter.consume_arg ();
          }
        else
          {
            // Can't interpret this argument.  Move on to the next
            // argument.  Any arguments that don't match are ignored
            // so that the caller can still use them.
            arg_shifter.ignore_arg ();
          }
      }
  }

  int
  Logger_Service::Initializer (void)
  {
    return ACE_Service_Config::process_directive (ace_svc_desc_DANCE_LOGGER_SERVICE);
  }
} // DAnCE

ACE_STATIC_SVC_DEFINE (DANCE_LOGGER_SERVICE,
                       ACE_TEXT ("DAnCE_Logger"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (DANCE_LOGGER_SERVICE),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (DAnCE_Logger, DANCE_LOGGER_SERVICE)

