// $Id$

#include "ace/OS_main.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"
#include "ace/Message_Block.h"

int ACE_TMAIN (int, ACE_TCHAR **)
{
#if 0
// Just for the book...

// Listing 1 code/ch12
  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb, ACE_Message_Block (128), -1);

  const char *deviceAddr= "Dev#12";
  mb->copy (deviceAddr, ACE_OS::strlen (deviceAddr)+1);
// Listing 1
#endif /* 0 */
// Listing 2 code/ch12
  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb, ACE_Message_Block (128), -1);

  const char *commandSeq= "CommandSeq#14";
  ACE_OS::sprintf (mb->wr_ptr (), "%s", commandSeq);
  // Move the wr_ptr() forward in the buffer by the
  // amount of data we just put in.
  mb->wr_ptr (ACE_OS::strlen (commandSeq) +1);
// Listing 2
// Listing 3 code/ch12
  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT ("Command Sequence --> %C\n"),
             mb->rd_ptr ()));
  mb->rd_ptr (ACE_OS::strlen (mb->rd_ptr ())+1);
  mb->release ();
// Listing 3
// Listing 4 code/ch12
  // Send a hangup notification to the receiver.
  ACE_NEW_RETURN
    (mb, ACE_Message_Block (128, ACE_Message_Block::MB_HANGUP), -1);
  // Send an error notification to the receiver.
  mb->msg_type (ACE_Message_Block::MB_ERROR);
// Listing 4
  mb->release ();

  return 0;
}
