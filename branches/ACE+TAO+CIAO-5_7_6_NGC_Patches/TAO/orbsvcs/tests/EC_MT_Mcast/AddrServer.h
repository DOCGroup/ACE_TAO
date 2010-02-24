// -*- C++ -*-
// $Id$
// Reused from: $TAO_ROOT/orbsvcs/examples/RtEC/MCast

#ifndef ADDRSERVER_H
#define ADDRSERVER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecUDPAdminS.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_INET_Addr;
ACE_END_VERSIONED_NAMESPACE_DECL

class AddrServer : public POA_RtecUDPAdmin::AddrServer
{
  // = TITLE
  //   A simple AddrServer
  //
  // = DESCRIPTION
  //   The EC is able to use multiple multicast groups to transmit its
  //   data, the is given control over the mapping between the Event
  //   (type,source) pair and the (ipaddr,port) pair using a
  //   AddrServer.
  //   This class implements a very simple server that simply maps the
  //   <type> component to the <ipaddr> and uses a fixed <port>,
  //   provided at initialization time.
  //
public:
  AddrServer (const ACE_INET_Addr &addr);
  // Constructor

  // = The RtecUDPAdmin::AddrServer methods
  virtual void get_addr (const RtecEventComm::EventHeader& header,
                         RtecUDPAdmin::UDP_Addr_out addr);

  virtual void get_address (const RtecEventComm::EventHeader& header,
                            RtecUDPAdmin::UDP_Address_out addr);

private:
  RtecUDPAdmin::UDP_Address addr_;
  // The address
};

#include /**/ "ace/post.h"
#endif /* ADDRSERVER_H */
