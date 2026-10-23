/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   AddrServer.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef ADDRSERVER_H
#define ADDRSERVER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecUDPAdminS.h"

/**
 * @class AddrServer
 *
 * @brief A simple AddrServer
 *
 * The EC is able to use multiple multicast groups to transmit its
 * data, the is given control over the mapping between the Event
 * (type,source) pair and the (ipaddr,port) pair using a
 * AddrServer.
 * This class implements a very simple server that simply maps the
 * <type> component to the <ipaddr> and uses a fixed <port>,
 * provided at initialization time.
 */
class AddrServer : public POA_RtecUDPAdmin::AddrServer
{
public:
  /// Constructor
  explicit AddrServer (RtecUDPAdmin::UDP_Addr const &addr);
  AddrServer (AddrServer const &) = delete;
  AddrServer (AddrServer &&) = delete;
  AddrServer &operator= (AddrServer const &) = delete;
  AddrServer &operator= (AddrServer &&) = delete;

  // = The RtecUDPAdmin::AddrServer methods
  void get_addr (RtecEventComm::EventHeader const &header,
                 RtecUDPAdmin::UDP_Addr_out addr) override;

  void get_address (RtecEventComm::EventHeader const &header,
                    RtecUDPAdmin::UDP_Address_out addr) override;

private:
  /// The address
  RtecUDPAdmin::UDP_Addr addr_;
};

#include /**/ "ace/post.h"
#endif /* ADDRSERVER_H */
