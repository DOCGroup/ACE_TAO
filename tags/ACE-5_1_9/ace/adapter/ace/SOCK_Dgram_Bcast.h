// $Id$

#ifndef ACE_ADAPTER_SOCK_DGRAM_BCAST_H
#define ACE_ADAPTER_SOCK_DGRAM_BCAST_H
#include "ace/pre.h"

#include "../../SOCK_Dgram_Bcast.h"

class ACE_SOCK_Dgram_Bcast_W : public ACE_SOCK_Dgram_Bcast
{
public:
  ACE_SOCK_Dgram_Bcast_W (void)
  {}

  ACE_SOCK_Dgram_Bcast_W (const ACE_Addr &local,
                          int protocol_family = PF_INET,
                          int protocol = 0,
                          int reuse_addr = 0,
                          const wchar_t *host_name = 0)
    : ACE_SOCK_Dgram_Bcast (local, protocol_family, protocol, reuse_addr, ACE_TEXT_WCHAR_TO_CHAR (host_name))
  {}

  ~ACE_SOCK_Dgram_Bcast_W (void)
  {}

  int open (const ACE_Addr &local,
            int protocol_family = PF_INET,
            int protocol = 0,
            int reuse_addr = 0,
            const wchar_t *host_name = 0)
  { return ACE_SOCK_Dgram_Bcast::open (local, protocol_family, protocol, reuse_addr, ACE_TEXT_WCHAR_TO_CHAR (host_name)); }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_SOCK_DGRAM_BCAST_H */
