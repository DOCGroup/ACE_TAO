/* -*- C++ -*- */
/**
 *  @file   EC_Gateway.h
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

#ifndef TAO_EC_GATEWAY_H
#define TAO_EC_GATEWAY_H
#include /**/ "ace/pre.h"

#include /**/ "event_export.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/RtecEventCommS.h"

/**
 * @class TAO_EC_Gateway
 *
 * @brief Event Channel Gateway
 *
 * There are several ways to connect several EC together, for
 * instance:
 * + A single class can use normal IIOP and connect to one EC as
 * a supplier and to another EC as a consumer.
 * + A class connects as a consumer and transmit the events using
 * multicast, another class receives the multicast messages and
 * transform them back into a push() call.
 * This is an abstract class to represent all the different
 * strategies for EC distribution.
 *
 */
class TAO_RTEvent_Export TAO_EC_Gateway : public POA_RtecEventChannelAdmin::Observer
{
public:
  /// Default constructor.
  TAO_EC_Gateway (void);

  /// Destructor
  virtual ~TAO_EC_Gateway (void);

  /// The gateway must disconnect from all the relevant event channels,
  /// or any other communication media (such as multicast groups).
  virtual void close (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) = 0;

  /// Obtain and modify the observer handle.
  void observer_handle (RtecEventChannelAdmin::Observer_Handle h);
  RtecEventChannelAdmin::Observer_Handle observer_handle (void) const;

private:
  RtecEventChannelAdmin::Observer_Handle handle_;
};

#include /**/ "ace/post.h"
#endif /* ACE_EC_GATEWAY_H */
