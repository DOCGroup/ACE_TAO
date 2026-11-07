/* -*- C++ -*- */

#ifndef UPIPE_EVENT_SERVER_MESSAGE_BLOCK_OWNER_H
#define UPIPE_EVENT_SERVER_MESSAGE_BLOCK_OWNER_H

#include "ace/Message_Block.h"

namespace UPIPE_Event_Server
{
  using Message_Block_Owner = ACE_Message_Block_Unique_Ptr;
}

#endif /* UPIPE_EVENT_SERVER_MESSAGE_BLOCK_OWNER_H */
