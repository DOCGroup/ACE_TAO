// $Id$

#ifndef ACE_ADAPTER_FIFO_SEND_MSG_H
#define ACE_ADAPTER_FIFO_SEND_MSG_H
#include "ace/pre.h"

#include "../../FIFO_Send_Msg.h"

class ACE_FIFO_Send_Msg_W : public ACE_FIFO_Send_Msg
{
public:
  ACE_FIFO_Send_Msg_W (void)
  {}

  ACE_FIFO_Send_Msg_W (const wchar_t *rendezvous,
                       int flags = O_WRONLY,
                       int perms = ACE_DEFAULT_FILE_PERMS,
                       LPSECURITY_ATTRIBUTES sa = 0)
    : ACE_FIFO_Send_Msg (ACE_TEXT_WCHAR_TO_CHAR (rendezvous), flags, perms, sa)
  {}

  

  int open (const wchar_t *rendezvous,
            int flags = O_WRONLY,
            int perms = ACE_DEFAULT_FILE_PERMS,
            LPSECURITY_ATTRIBUTES sa = 0)
  { return ACE_FIFO_Send_Msg::open (ACE_TEXT_WCHAR_TO_CHAR (rendezvous), flags, perms, sa); }

};


#include "ace/post.h"
#endif /* ACE_ADAPTER_FIFO_SEND_MSG_H */
