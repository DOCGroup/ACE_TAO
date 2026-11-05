#include "ace/OS_main.h"
#include "ace/OS_Memory.h"
#include "ace/Log_Msg.h"
#include "ace/Message_Block.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <memory>

struct Message_Block_Deleter
{
  void operator() (ACE_Message_Block *message_block) const
  {
    message_block->release ();
  }
};

using Message_Block_Owner = std::unique_ptr<ACE_Message_Block, Message_Block_Deleter>;

int ACE_TMAIN (int, ACE_TCHAR **)
{
#if 0
// Just for the book...

// Listing 1 code/ch12
  ACE_Message_Block *raw_message_block {};
  ACE_NEW_RETURN (raw_message_block, ACE_Message_Block (128), -1);
  Message_Block_Owner mb {raw_message_block};

  char const *device_addr = "Dev#12";
  mb->copy (device_addr, std::strlen (device_addr) + 1);
// Listing 1
#endif /* 0 */
// Listing 2 code/ch12
  ACE_Message_Block *raw_message_block {};
  ACE_NEW_RETURN (raw_message_block, ACE_Message_Block (128), -1);
  Message_Block_Owner mb {raw_message_block};

  char const *command_sequence = "CommandSeq#14";
  int const written = std::snprintf (mb->wr_ptr (), mb->space (), "%s",
                                     command_sequence);
  if (written < 0 || static_cast<std::size_t> (written) >= mb->space ())
    return -1;
  // Move the wr_ptr() forward in the buffer by the
  // amount of data we just put in.
  mb->wr_ptr (static_cast<std::size_t> (written) + 1);
// Listing 2
// Listing 3 code/ch12
  ACE_DEBUG ((LM_DEBUG,
             ACE_TEXT ("Command Sequence --> %C\n"),
             mb->rd_ptr ()));
  mb->rd_ptr (static_cast<std::size_t> (written) + 1);
  mb.reset ();
// Listing 3
// Listing 4 code/ch12
  // Send a hangup notification to the receiver.
  ACE_NEW_RETURN
    (raw_message_block,
     ACE_Message_Block (128, ACE_Message_Block::MB_HANGUP), -1);
  mb.reset (raw_message_block);
  // Send an error notification to the receiver.
  mb->msg_type (ACE_Message_Block::MB_ERROR);
// Listing 4
  mb.reset ();

  return 0;
}
