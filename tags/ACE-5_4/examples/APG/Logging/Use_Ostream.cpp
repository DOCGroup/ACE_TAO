// $Id$

#include "ace/Log_Msg.h"
#include "ace/streams.h"

void foo (void);

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_TRACE (ACE_TEXT ("main"));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%IHi Mom\n")));


  /* Alternatively, you can use the set_flags() method to do the same
     thing after the singleton has been created:
  */
  // Listing 2 code/ch03
  ACE_OSTREAM_TYPE *output =
    new std::ofstream ("ostream.output.test");
  ACE_LOG_MSG->msg_ostream (output, 1);
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);
  // Listing 2

  foo ();
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%IGoodnight\n")));

  return 0;
}

void foo (void)
{
  ACE_TRACE (ACE_TEXT ("foo"));

  ACE_DEBUG ((LM_INFO, ACE_TEXT ("%IHowdy Pardner\n")));
}
