// $Id$

#include "ace/INet/INet_Log.h"
#include "ace/Env_Value_T.h"
#include "ace/SString.h"
#include "ace/Log_Msg.h"
#include "ace/streams.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    unsigned int INet_Log::debug_level_ = INet_Log::Initializer ();

    unsigned int INet_Log::Initializer(void)
      {
        unsigned int debug_level = 0;
        bool trace = false;
        ACE_TString filename;

        ACE_Env_Value<int> log (ACE_TEXT("INET_LOG_LEVEL"), debug_level);
        debug_level = log;

        ACE_Env_Value<int> trace_env (ACE_TEXT("INET_TRACE_ENABLE"), 0);
        trace = (trace_env != 0);

        ACE_Env_Value<const ACE_TCHAR *> filename_env (ACE_TEXT("INET_LOG_FILE"), filename.c_str ());
        filename = filename_env;

        if (trace)
          {
            INET_ENABLE_TRACE ();
          }
        else
          {
            INET_DISABLE_TRACE ();
          }

        if (filename.length () > 0)
          {
#if defined (ACE_LACKS_IOSTREAM_TOTALLY)

            FILE* output_stream = ACE_OS::fopen (filename.c_str (), ACE_TEXT ("a"));

            ACE_LOG_MSG->msg_ostream (output_stream, 1);

#else /* ! ACE_LACKS_IOSTREAM_TOTALLY */

            ofstream* output_stream = 0;

            ACE_NEW_NORETURN (output_stream,
                              ofstream ());
            if (output_stream)
              {
                output_stream->open (ACE_TEXT_ALWAYS_CHAR (filename.c_str ()),
                                    ios::out | ios::app);

                if (!output_stream->bad ())
                  {
                    ACE_LOG_MSG->msg_ostream (output_stream, 1);
                  }
              }

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

            ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER);
            ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
          }

        return debug_level;
      }

  }

ACE_END_VERSIONED_NAMESPACE_DECL
