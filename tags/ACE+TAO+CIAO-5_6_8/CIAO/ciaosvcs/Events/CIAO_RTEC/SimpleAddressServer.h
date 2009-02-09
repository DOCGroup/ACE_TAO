// -*- C++ -*-

//=============================================================================
/**
 *  @file SimpleAddressServer.h
 *
 *  $Id$
 *
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef SIMPLEADDRESSSERVER_H
#define SIMPLEADDRESSSERVER_H

#include <orbsvcs/RtecUDPAdminS.h>

class SimpleAddressServer : public POA_RtecUDPAdmin::AddrServer
{
public:
  SimpleAddressServer (const ACE_INET_Addr& address);

  SimpleAddressServer (const RtecUDPAdmin::UDP_Addr& addr);

  virtual void get_addr (const RtecEventComm::EventHeader& header,
                         RtecUDPAdmin::UDP_Addr& address);

  virtual void get_address (const RtecEventComm::EventHeader& header,
                            RtecUDPAdmin::UDP_Address_out address);

private:
  RtecUDPAdmin::UDP_Address address_;
};

#endif
