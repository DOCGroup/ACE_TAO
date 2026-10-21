// SimpleAddressServer.h

#ifndef SIMPLEADDRESSSERVER_H
#define SIMPLEADDRESSSERVER_H

#include "orbsvcs/RtecUDPAdminS.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_INET_Addr;
ACE_END_VERSIONED_NAMESPACE_DECL

class SimpleAddressServer : public POA_RtecUDPAdmin::AddrServer {
public:
  SimpleAddressServer (ACE_INET_Addr const & address);
  void get_addr (RtecEventComm::EventHeader const & header,
                 RtecUDPAdmin::UDP_Addr & address) override;

  void get_address (RtecEventComm::EventHeader const & header,
                    RtecUDPAdmin::UDP_Address & address) override;

private:
  RtecUDPAdmin::UDP_Address address_;
};

#endif
