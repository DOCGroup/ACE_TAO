/* -*- C++ -*- */
// $Id$

#ifndef ACE_ADAPTER_SOCK_DGRAM_MCAST_QOS_H
#define ACE_ADAPTER_SOCK_DGRAM_MCAST_QOS_H
#include "ace/pre.h"

#include "../../SOCK_Dgram_Mcast_QoS.h"

class ACE_SOCK_Dgram_Mcast_QoS_W : public ACE_SOCK_Dgram_Mcast_QoS 
{
public:
  ACE_SOCK_Dgram_Mcast_QoS_W (void)
  {}

  ~ACE_SOCK_Dgram_Mcast_QoS_W (void)
  {}

  int subscribe (const ACE_INET_Addr &mcast_addr,
                 const ACE_QoS_Params &qos_params,
                 int reuse_addr = 1,
                 const wchar_t *net_if = 0,
                 int protocol_family = PF_INET,
                 int protocol = 0,
                 ACE_Protocol_Info *protocolinfo = 0,
                 ACE_SOCK_GROUP g = 0,
                 u_long flags = 0,
                 ACE_QoS_Manager *qos_manager = 0,
                 ACE_QoS_Session *qos_session = 0)
  {
    return ACE_SOCK_Dgram_Mcast_QoS::subscribe (mcast_addr,
                                                qos_params,
                                                reuse_addr,
                                                ACE_TEXT_WCHAR_TO_CHAR (net_if),
                                                protocol_family,
                                                protocol,
                                                protocolinfo,
                                                g,
                                                flags,
                                                qos_manager,
                                                qos_session);
  }

};

#include "ace/post.h"
#endif /* ACE_ADAPTER_SOCK_DGRAM_MCAST_QOS_H */


