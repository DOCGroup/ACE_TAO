// $Id$

#ifndef ACE_ADAPTER_FIFO_SEND_H
#define ACE_ADAPTER_FIFO_SEND_H
#include "ace/pre.h"

#include "../../FIFO_Send.h"

class ACE_FIFO_Send_W : public ACE_FIFO_Send
{
public:
  ACE_FIFO_Send_W (void)
  {}

  ACE_FIFO_Send_W (const wchar_t *rendezvous,
                   int flags = O_WRONLY,
                   int perms = ACE_DEFAULT_FILE_PERMS,
                   LPSECURITY_ATTRIBUTES sa = 0)
    : ACE_FIFO_Send (ACE_TEXT_WCHAR_TO_CHAR (rendezvous), flags, perms, sa)
  {}

  int open (const wchar_t *rendezvous,
            int flags = O_WRONLY,
            int perms = ACE_DEFAULT_FILE_PERMS,
            LPSECURITY_ATTRIBUTES sa = 0)
  { return ACE_FIFO_Send::open (ACE_TEXT_WCHAR_TO_CHAR (rendezvous), flags, perms, sa); }

};

#include "ace/post.h"
#endif /* ACE_ADAPTER_FIFO_SEND_H */
