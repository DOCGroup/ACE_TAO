/**
 * @file Loopback.h
 *
 * $Id$
 *
 */

#ifndef ECFL_LOOPBACK_H
#define ECFL_LOOPBACK_H

#include "ControlS.h"
#include "Servant_var.h"
#include "Loopback_Consumer.h"
#include "Loopback_Supplier.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ECFL_Loopback
 *
 * @brief Implement the Control::Loopback interface
 *
 */
class ECFL_Loopback
 : public virtual POA_Control::Loopback
 , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  ECFL_Loopback (void);

  /// Initialize the loopback
  void init (CORBA::Long experiment_id,
             RtecEventChannelAdmin::EventChannel_ptr ec
             TAO_ENV_ARG_DECL);

  //@{
  /** @name The Control::Loopback methods
   */
  virtual void destroy (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

private:
  /// Keep a reference to the loopback consumer
  Servant_var<ECFL_Loopback_Consumer> consumer_;

  /// Keep a reference to the loopback supplier
  Servant_var<ECFL_Loopback_Supplier> supplier_;
};

#endif /* ECFL_LOOPBACK_H */
