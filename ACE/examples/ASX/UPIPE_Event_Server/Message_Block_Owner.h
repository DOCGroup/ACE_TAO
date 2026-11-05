/* -*- C++ -*- */

#ifndef UPIPE_EVENT_SERVER_MESSAGE_BLOCK_OWNER_H
#define UPIPE_EVENT_SERVER_MESSAGE_BLOCK_OWNER_H

#include "ace/Message_Block.h"
#include <memory>

namespace UPIPE_Event_Server
{
  struct Message_Block_Release
  {
    void operator() (ACE_Message_Block *message_block) const
    {
      if (message_block != nullptr)
        message_block->release ();
    }
  };

  using Message_Block_Owner = std::unique_ptr<ACE_Message_Block, Message_Block_Release>;
}

#endif /* UPIPE_EVENT_SERVER_MESSAGE_BLOCK_OWNER_H */
