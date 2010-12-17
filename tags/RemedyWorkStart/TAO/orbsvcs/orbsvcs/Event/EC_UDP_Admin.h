// -*- C++ -*-

/**
 *  @file   EC_UDP_Admin.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_UDP_ADMIN_H
#define TAO_EC_UDP_ADMIN_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecUDPAdminS.h"
#include /**/ "orbsvcs/Event/event_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_EC_Simple_AddrServer
 *
 * @brief TAO Real-time Event Service; a simple UDP address server.
 *
 * The EC is able to use multiple multicast groups to transmit its
 * data, the is given control over the mapping between the Event
 * (type,source) pair and the (ipaddr,port) pair using a
 * AddrServer.
 * This class implements a very simple server that simply maps the
 * type component to the ipaddr and uses a fixed port,
 * provided at initialization time.
 */
class TAO_RTEvent_Serv_Export TAO_EC_Simple_AddrServer : public POA_RtecUDPAdmin::AddrServer
{
public:
  /// Constructor
  TAO_EC_Simple_AddrServer (CORBA::UShort port);

  /// Destructor
  virtual ~TAO_EC_Simple_AddrServer (void);

  // = The RtecUDPAdmin::AddrServer methods
  virtual void get_addr (const RtecEventComm::EventHeader& header,
                         RtecUDPAdmin::UDP_Addr_out addr);

  virtual void get_address (const RtecEventComm::EventHeader& header,
                            RtecUDPAdmin::UDP_Address_out addr);

private:
  CORBA::UShort port_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_EC_UDP_ADMIN_H */
