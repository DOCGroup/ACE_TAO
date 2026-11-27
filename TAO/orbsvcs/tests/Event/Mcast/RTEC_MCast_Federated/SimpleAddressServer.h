// SimpleAddressServer.h

#ifndef SIMPLEADDRESSSERVER_H
#define SIMPLEADDRESSSERVER_H

#include <orbsvcs/RtecUDPAdminS.h>
#include <ace/INET_Addr.h>

class SimpleAddressServer : public POA_RtecUDPAdmin::AddrServer {
public:
  explicit SimpleAddressServer (ACE_INET_Addr const &address);
  SimpleAddressServer (SimpleAddressServer const &) = delete;
  SimpleAddressServer (SimpleAddressServer &&) = delete;
  SimpleAddressServer &operator= (SimpleAddressServer const &) = delete;
  SimpleAddressServer &operator= (SimpleAddressServer &&) = delete;

  void get_addr (RtecEventComm::EventHeader const &header,
                 RtecUDPAdmin::UDP_Addr &address) override;

  void get_address (RtecEventComm::EventHeader const &header,
                    RtecUDPAdmin::UDP_Address_out addr6) override;

private:
  ACE_INET_Addr addr_;
};

#endif
