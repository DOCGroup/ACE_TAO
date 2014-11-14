// $Id$

// SimpleAddressServer.h

#ifndef SIMPLEADDRESSSERVER_H
#define SIMPLEADDRESSSERVER_H

#include "orbsvcs/RtecUDPAdminS.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_INET_Addr;
ACE_END_VERSIONED_NAMESPACE_DECL

class SimpleAddressServer : public POA_RtecUDPAdmin::AddrServer {
public:
  SimpleAddressServer (const ACE_INET_Addr& address);
  virtual void get_addr (const RtecEventComm::EventHeader& header,
                         RtecUDPAdmin::UDP_Addr& address);

  virtual void get_address(const RtecEventComm::EventHeader& header,
                           RtecUDPAdmin::UDP_Address& address);

private:
  RtecUDPAdmin::UDP_Address address_;
};

#endif
