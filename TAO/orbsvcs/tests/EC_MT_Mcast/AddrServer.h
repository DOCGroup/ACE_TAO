// -*- C++ -*-
// Reused from: $TAO_ROOT/orbsvcs/examples/RtEC/MCast

#ifndef ADDRSERVER_H
#define ADDRSERVER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecUDPAdminS.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_INET_Addr;
ACE_END_VERSIONED_NAMESPACE_DECL

/**
 * @brief Provides a fixed multicast address for event types.
 *
 * The event channel can use multiple multicast groups. This simple
 * implementation maps each event type to the same IP address and port,
 * supplied at construction.
 */
class AddrServer : public POA_RtecUDPAdmin::AddrServer
{
public:
  explicit AddrServer (ACE_INET_Addr const& addr);
  AddrServer (AddrServer const&) = delete;
  AddrServer (AddrServer&&) = delete;
  AddrServer& operator= (AddrServer const&) = delete;
  AddrServer& operator= (AddrServer&&) = delete;

  // = The RtecUDPAdmin::AddrServer methods
  void get_addr (RtecEventComm::EventHeader const& header,
                 RtecUDPAdmin::UDP_Addr_out addr) override;

  void get_address (RtecEventComm::EventHeader const& header,
                    RtecUDPAdmin::UDP_Address_out addr) override;

private:
  RtecUDPAdmin::UDP_Address addr_;
  // The address
};

#include /**/ "ace/post.h"
#endif /* ADDRSERVER_H */
