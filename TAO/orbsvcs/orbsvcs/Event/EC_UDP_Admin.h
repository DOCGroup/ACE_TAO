/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   TAO services
//
// = FILENAME
//   EC_UDP_Admin
//
// = AUTHOR
//   Carlos O'Ryan
//
// = DESCRIPTION
//   Simple implementations of the UDP Administration service.
//
//   connects to a "remote" EC as a consumer, it also connects to the
//   <local> EC as a supplier of events, this later EC is usually
//   collocated.
//   The QoS parameters for both connections must be provided by the
//   user.
//   To avoid infinite loops of events the Gateway descreases the TTL
//   field of the events and will not deliver any events with TTL less
//   than or equal to 0.
//
// = TODO
//   The class makes an extra copy of the events, we need to
//   investigate if closer collaboration with its collocated EC could
//   be used to remove that copy.
//
// ============================================================================

#ifndef TAO_EC_UDP_ADMIN_H
#define TAO_EC_UDP_ADMIN_H

#include "orbsvcs/RtecUDPAdminS.h"
#include "orbsvcs/orbsvcs_export.h"

class TAO_ORBSVCS_Export TAO_EC_Simple_AddrServer : public POA_RtecUDPAdmin::AddrServer
{
  // = TITLE
  //   TAO Real-time Event Service; a simple UDP address server.
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
  TAO_EC_Simple_AddrServer (CORBA::UShort port);
  // Constructor

  virtual ~TAO_EC_Simple_AddrServer (void);
  // Destructor

  // = The RtecUDPAdmin::AddrServer methods
  virtual void get_addr (const RtecEventComm::EventHeader& header,
                         RtecUDPAdmin::UDP_Addr_out addr,
                         CORBA::Environment& env);

private:
  CORBA::UShort port_;
};

#endif /* TAO_EC_UDP_ADMIN_H */
