// TestAddrServer.h

#ifndef TEST_ADDR_SERVER_H
#define TEST_ADDR_SERVER_H

#include <orbsvcs/RtecUDPAdminS.h>
#include <ace/INET_Addr.h>

class TestAddrServer_i : public POA_RtecUDPAdmin::AddrServer {
public:
  TestAddrServer_i (ACE_INET_Addr const& addr,
                    ACE_INET_Addr const& addr6);

  TestAddrServer_i (TestAddrServer_i const&) = delete;
  TestAddrServer_i (TestAddrServer_i&&) = delete;
  TestAddrServer_i& operator= (TestAddrServer_i const&) = delete;
  TestAddrServer_i& operator= (TestAddrServer_i&&) = delete;

  void get_addr (RtecEventComm::EventHeader const& header,
                 RtecUDPAdmin::UDP_Addr& address) override;

  void get_address (RtecEventComm::EventHeader const& header,
                    RtecUDPAdmin::UDP_Address_out addr6) override;

private:
  ACE_INET_Addr addr_;
  ACE_INET_Addr addr6_;
};

#endif
