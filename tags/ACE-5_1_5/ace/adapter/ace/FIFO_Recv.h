// $Id$

#ifndef ACE_ADAPTER_FIFO_RECV_H
#define ACE_ADAPTER_FIFO_RECV_H
#include "ace/pre.h"

#include "../../FIFO_Recv.h"

class ACE_FIFO_Recv_W : public ACE_FIFO_Recv
{
public:
  ACE_FIFO_Recv_W (void)
  {}

  ACE_FIFO_Recv_W (const wchar_t *rendezvous,
                   int flags = O_CREAT | O_RDONLY,
                   int perms = ACE_DEFAULT_FILE_PERMS,
                   int persistent = 1,
                   LPSECURITY_ATTRIBUTES sa = 0)
    : ACE_FIFO_Recv (ACE_TEXT_WCHAR_TO_CHAR (rendezvous), flags, perms, persistent, sa)
  {}

  int open (const wchar_t *rendezvous,
            int flags = O_CREAT | O_RDONLY,
            int perms = ACE_DEFAULT_FILE_PERMS,
            int persistent = 1,
            LPSECURITY_ATTRIBUTES sa = 0)
  { return ACE_FIFO_Recv::open (ACE_TEXT_WCHAR_TO_CHAR (rendezvous), flags, perms, persistent, sa); }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_FIFO_RECV_H */
