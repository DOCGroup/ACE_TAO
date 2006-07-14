// $Id$

// SimpleAddressServer.h

#ifndef SIMPLEADDRESSSERVER_H
#define SIMPLEADDRESSSERVER_H

#include <orbsvcs/RtecUDPAdminS.h>

class ACE_INET_Addr;

class SimpleAddressServer : public POA_RtecUDPAdmin::AddrServer {
public:
  SimpleAddressServer (const ACE_INET_Addr& address);
  virtual void get_addr (const RtecEventComm::EventHeader& header,
                         RtecUDPAdmin::UDP_Addr& address)
    throw (CORBA::SystemException);

private:
  RtecUDPAdmin::UDP_Addr address_;
};

#endif
