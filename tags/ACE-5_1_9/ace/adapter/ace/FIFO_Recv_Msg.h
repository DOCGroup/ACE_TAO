// $Id$

#ifndef ACE_ADPATER_FIFO_RECV_MSG_H
#define ACE_ADPATER_FIFO_RECV_MSG_H
#include "ace/pre.h"

#include "../../FIFO_Recv_Msg.h"

class ACE_FIFO_Recv_Msg_W : public ACE_FIFO_Recv_Msg
{
public:
  ACE_FIFO_Recv_Msg_W (void)
  {}

  ACE_FIFO_Recv_Msg_W (const wchar_t *rendezvous,
                       int flags = O_CREAT | O_RDONLY,
                       int perms = ACE_DEFAULT_FILE_PERMS,
                       int persistent = 1,
                       LPSECURITY_ATTRIBUTES sa = 0)
    : ACE_FIFO_Recv_Msg (ACE_TEXT_WCHAR_TO_CHAR (rendezvous), flags, perms, persistent, sa)
  {}
  
  int open (const wchar_t *rendezvous,
            int flags = O_CREAT | O_RDONLY,
            int perms = ACE_DEFAULT_FILE_PERMS,
            int persistent = 1,
            LPSECURITY_ATTRIBUTES sa = 0)
  { return ACE_FIFO_Recv_Msg::open (ACE_TEXT_WCHAR_TO_CHAR (rendezvous), flags, perms, persistent, sa); }

};

#include "ace/post.h"
#endif /* ACE_ADPATER_FIFO_RECV_MSG_H */
