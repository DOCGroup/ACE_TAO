// $Id$
#include "Logger_Service.h"
#include "Log_Macros.h"
#include "ace/Get_Opt.h"
#include "ace/CORBA_macros.h"
#include "ace/Env_Value_T.h"
#include "tao/SystemException.h"
#include "ace/Service_Config.h"
#include "ace/Arg_Shifter.h"
#include "ace/Log_Msg_Backend.h"
#include "ace/Dynamic_Service.h"

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
// Needed to set ACE_LOG_MSG::msg_ostream()
// FUZZ: disable check_for_streams_include
#  include "ace/streams.h"
#endif /* !ACE_LACKS_IOSTREAM_TOTALLY */

CIAO::Logger_Service::Logger_Service (void)
  : filename_ (ACE_TEXT("")),
    trace_ (false)
{
}

int
CIAO::Logger_Service::init (int argc, ACE_TCHAR * argv[])
{
  // Get prospective values from the environment first, those given on
  // command line can override
  ACE_Env_Value<int> log (ACE_TEXT("CIAO_LOG_LEVEL"), CIAO_debug_level);
  CIAO_debug_level = log;

  ACE_Env_Value<int> trace (ACE_TEXT("CIAO_TRACE_ENABLE"), 0);
  this->trace_ = (trace != 0);

  ACE_Env_Value<const ACE_TCHAR *> filename (ACE_TEXT("CIAO_LOG_FILE"), this->filename_.c_str ());
  this->filename_ = filename;

  ACE_Env_Value<const ACE_TCHAR *> backend (ACE_TEXT("CIAO_LOG_BACKEND"), this->backend_.c_str ());
  this->backend_ = backend;

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

  if (this->backend_.length () > 0)
    {

      ACE_Log_Msg_Backend* logger_be =
        ACE_Dynamic_Service<ACE_Log_Msg_Backend>::instance(this->backend_.c_str ());

      //        backend->open ("");

      if (logger_be == 0)
        {
          CIAO_ERROR (1,
                       (LM_EMERGENCY, CLINFO
                        "Logger_Service::init - "
                        "Unable to load backend %s\n",
                        this->backend_.c_str ()));
          return -1;
        }

      ACE_Log_Msg::msg_backend (logger_be);

      ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER);
      ACE_LOG_MSG->set_flags (ACE_Log_Msg::CUSTOM);
    }

  return 0;
}

void
CIAO::Logger_Service::parse_args (int argc, ACE_TCHAR **argv)
{
  CIAO_TRACE ("CIAO::Logger_Service::parse_args");

  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *current_arg = 0;
      if (0 != (current_arg =
                     arg_shifter.get_the_parameter
                     (ACE_TEXT ("-CIAOLogLevel"))))
        {
          CIAO_debug_level = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 == arg_shifter.cur_arg_strncasecmp
          (ACE_TEXT ("-CIAOTraceEnable")))
        {
          this->trace_ = true;

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg =
                     arg_shifter.get_the_parameter
                     (ACE_TEXT ("-CIAOLogFile"))))
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
CIAO::Logger_Service::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_CIAO_LOGGER_SERVICE);
}

ACE_STATIC_SVC_DEFINE (CIAO_LOGGER_SERVICE,
                       ACE_TEXT ("CIAO_Logger"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (CIAO_LOGGER_SERVICE),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (CIAO_Logger, CIAO_LOGGER_SERVICE)

