// $Id$

#ifndef ACE_ADAPTER_SOCK_DGRAM_MCAST_H
#define ACE_ADAPTER_SOCK_DGRAM_MCAST_H
#include "ace/pre.h"

#include "../../SOCK_Dgram_Mcast.h"

class ACE_SOCK_Dgram_Mcast_W : public ACE_SOCK_Dgram_Mcast 
{
public:
  ACE_SOCK_Dgram_Mcast_W (void)
  {}

  ~ACE_SOCK_Dgram_Mcast_W (void)
  {}

  int subscribe (const ACE_INET_Addr &mcast_addr,
                 int reuse_addr = 1,
                 const wchar_t *net_if = 0,
                 int protocol_family = PF_INET,
                 int protocol = 0)
  { 
    return ACE_SOCK_Dgram_Mcast::subscribe (mcast_addr, 
                                            reuse_addr, 
                                            ACE_TEXT_WCHAR_TO_CHAR (net_if),
                                            protocol_family,
                                            protocol);
  }

  int unsubscribe (const ACE_INET_Addr &mcast_addr,
                   const wchar_t *net_if = 0,
                   int protocol_family = PF_INET,
                   int protocol = 0)
  { 
    return ACE_SOCK_Dgram_Mcast::unsubscribe (mcast_addr, 
                                              ACE_TEXT_WCHAR_TO_CHAR (net_if),
                                              protocol_family,
                                              protocol);
  }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_SOCK_DGRAM_MCAST_H */

