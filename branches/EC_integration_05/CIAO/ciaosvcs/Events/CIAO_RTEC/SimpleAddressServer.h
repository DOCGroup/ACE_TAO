// SimpleAddressServer.h

#ifndef SIMPLEADDRESSSERVER_H
#define SIMPLEADDRESSSERVER_H

#include <orbsvcs/RtecUDPAdminS.h>

class ACE_INET_Addr;

class SimpleAddressServer : public POA_RtecUDPAdmin::AddrServer {
public:
  SimpleAddressServer (const ACE_INET_Addr& address);

  SimpleAddressServer (const RtecUDPAdmin::UDP_Addr& addr);

  virtual void get_addr (const RtecEventComm::EventHeader& header,
                         RtecUDPAdmin::UDP_Addr& address)
    throw (CORBA::SystemException);


/*
  virtual void get_ip_address (const RtecEventComm::EventHeader& header,
                               RtecUDPAdmin::UDP_IP_Address_out address
                               ACE_ENV_ARG_DECL)
    throw (CORBA::SystemException);
*/
private:
  RtecUDPAdmin::UDP_Addr address_;
};

#endif
