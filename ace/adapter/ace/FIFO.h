// $Id$

#ifndef ACE_ADAPTER_FIFO_H
#define ACE_ADAPTER_FIFO_H
#include "ace/pre.h"

#include "../../FIFO.h"

// @@ These probably should be moved into the other ACE_FIFO derived classes

class ACE_FIFO_W : public ACE_FIFO
{
public:
  int open (const wchar_t *rendezvous, int flags, int perms,
            LPSECURITY_ATTRIBUTES sa = 0)
  { return ACE_FIFO::open (ACE_TEXT_WCHAR_TO_CHAR (rendezvous), flags, perms, sa); }

  int get_local_addr (const wchar_t *&rendezvous) const
  {
    char *crend;
    int result = ACE_FIFO::get_local_addr (crend);

    rendezvous = ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (crend));
    delete [] crend;
    return result;
  }

protected:
  ACE_FIFO_W (void) 
  {}

  ACE_FIFO_W (const wchar_t *rendezvous, int flags, int perms,
            LPSECURITY_ATTRIBUTES sa = 0)
    : ACE_FIFO (ACE_TEXT_WCHAR_TO_CHAR (rendezvous), flags, perms, sa)
  {}

};

#include "ace/post.h"
#endif /* ACE_ADAPTER_FIFO_H */
