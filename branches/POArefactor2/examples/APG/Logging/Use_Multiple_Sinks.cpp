// $Id$

#include "ace/Log_Msg.h"
#include "ace/streams.h"
#if (defined (_MSC_VER) && (_MSC_VER >= 1200) && (_MSC_VER < 1300))
#include <fstream>
#endif
int ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  // Output to default destination (stderr)
  ACE_LOG_MSG->open (argv[0]);

  ACE_TRACE ("main");

  ACE_OSTREAM_TYPE *output =
        (ACE_OSTREAM_TYPE *) new std::ofstream ("ostream.output.test");

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%IThis will go to STDERR\n")));

  ACE_LOG_MSG->open
    (argv[0], ACE_Log_Msg::SYSLOG, ACE_TEXT ("syslogTest"));
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::STDERR);
  ACE_DEBUG
    ((LM_DEBUG, ACE_TEXT ("%IThis goes to STDERR & syslog\n")));

  ACE_LOG_MSG->msg_ostream (output, 0);
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%IThis will go to STDERR, ")
              ACE_TEXT ("syslog & an ostream\n")));

  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::OSTREAM);
  delete output;

  return 0;
}
