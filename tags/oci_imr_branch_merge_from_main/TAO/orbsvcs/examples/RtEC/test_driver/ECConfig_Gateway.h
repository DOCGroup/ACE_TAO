/**
 * @author Stephen Torri
 * $Id$
 */
#ifndef ECCONFIG_GATEWAY_H_
#define ECCONFIG_GATEWAY_H

#include <orbsvcs/Event/EC_Gateway_Sched.h>
#include <orbsvcs/RtecEventChannelAdminS.h>
#include <orbsvcs/RtecEventCommS.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ECConfig_Gateway : public TAO_EC_Gateway_Sched {
 public:
  ECConfig_Gateway (void);
  ~ECConfig_Gateway (void);

  void push (const RtecEventComm::EventSet &events
             ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  virtual void update_consumer (const RtecEventChannelAdmin::ConsumerQOS& sub
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void update_supplier (const RtecEventChannelAdmin::SupplierQOS& pub
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

};

#endif // ECCONFIG_GATEWAY_H_
