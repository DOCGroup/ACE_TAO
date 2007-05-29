/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel examples
//
// = FILENAME
//   Consumer
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef ADDRSERVER_H
#define ADDRSERVER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecUDPAdminS.h"
#include "ace/INET_Addr.h"

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
  AddrServer (const ACE_INET_Addr& addr);
  // Constructor

  // = The RtecUDPAdmin::AddrServer methods
  virtual void get_addr (const RtecEventComm::EventHeader& header,
                         RtecUDPAdmin::UDP_Addr_out addr);

  virtual void get_address (const RtecEventComm::EventHeader& header,
                            RtecUDPAdmin::UDP_Address_out addr6);


private:
  ACE_INET_Addr addr_;
  // The address
};

#include /**/ "ace/post.h"
#endif /* ADDRSERVER_H */
