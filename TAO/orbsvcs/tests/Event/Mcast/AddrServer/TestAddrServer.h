// TestAddrServer.h

#ifndef TEST_ADDR_SERVER_H
#define TEST_ADDR_SERVER_H

#include <orbsvcs/RtecUDPAdminS.h>
#include <ace/INET_Addr.h>

class TestAddrServer_i : public POA_RtecUDPAdmin::AddrServer {
public:
  TestAddrServer_i (const ACE_INET_Addr& addr,
                    const ACE_INET_Addr& addr6);

  void get_addr (const RtecEventComm::EventHeader& header,
                 RtecUDPAdmin::UDP_Addr& address) override;

  void get_address (const RtecEventComm::EventHeader& header,
                    RtecUDPAdmin::UDP_Address_out addr6) override;

private:
  ACE_INET_Addr addr_;
  ACE_INET_Addr addr6_;
};

#endif
