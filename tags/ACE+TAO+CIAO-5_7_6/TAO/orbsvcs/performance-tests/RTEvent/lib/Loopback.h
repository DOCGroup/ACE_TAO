/**
 * @file Loopback.h
 *
 * $Id$
 *
 */

#ifndef TAO_RTEC_PERF_LOOPBACK_H
#define TAO_RTEC_PERF_LOOPBACK_H

#include "Federated_TestS.h"
#include "Loopback_Pair.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Loopback
 *
 * @brief Implement the Control::Loopback interface
 *
 */
class TAO_RTEC_Perf_Export Loopback
 : public virtual POA_Federated_Test::Loopback
{
public:
  /// Constructor
  Loopback (CORBA::Long experiment_id,
            CORBA::Long base_event_type,
            PortableServer::POA_ptr supplier_poa,
            PortableServer::POA_ptr consumer_poa,
            RtecEventChannelAdmin::EventChannel_ptr ec);

  //@{
  /** @name The Federated_Test::Loopback methods
   */
  virtual void disconnect (void);
  //@}

private:
  Loopback_Pair loopback_pair_;
};

#endif /* TAO_RTEC_PERF_LOOPBACK_H */
