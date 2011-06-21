/* -*- C++ -*- */
// $Id$

#ifndef EC_WRAPPER_H
#define EC_WRAPPER_H

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/Event/EC_Lifetime_Utils_T.h"
#include "ECMcastTests_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
class TAO_EC_Event_Channel;
TAO_END_VERSIONED_NAMESPACE_DECL

/**
 * @class EC_Wrapper
 *
 * @brief This class decorates Rtec Event Channel implementation:
 *        - destroy () also shutdowns the ORB
 *        - automatic cleanup in destructor, if necessary
 */
class ECMcastTests_Export EC_Wrapper:
  public virtual POA_RtecEventChannelAdmin::EventChannel,
  public TAO_EC_Deactivated_Object
{
public:

  /// Create a new EC_Wrapper object.
  /// (Constructor access is restricted to insure that all
  /// EC_Wrapper objects are heap-allocated.)
  static PortableServer::Servant_var<EC_Wrapper> create (void);

  /// Destructor.  Destroys the Event Channel implementation.
  virtual ~EC_Wrapper (void);

  /// Create and initialize underlying EC servant.
  int init (CORBA::ORB_ptr orb,
            PortableServer::POA_ptr poa);

  /// RtecEventChannelAdmin::Event_Channel methods.
  //@{
  virtual RtecEventChannelAdmin::ConsumerAdmin_ptr
    for_consumers (void);
  virtual RtecEventChannelAdmin::SupplierAdmin_ptr
    for_suppliers (void);

  /// Destroy the Event Channel, deactivate from POA, and shut down
  /// the ORB.
  virtual void destroy (void);

  virtual RtecEventChannelAdmin::Observer_Handle
    append_observer (RtecEventChannelAdmin::Observer_ptr observer);
  virtual void remove_observer (RtecEventChannelAdmin::Observer_Handle);
  //@}

protected:

  /// Constructor (protected).  Clients can create new
  /// EC_Wrapper objects using the static create() method.
  EC_Wrapper (void);

private:

  /// Helper - destroys Event Channel and deactivate from POA, if
  /// necessary.
  void destroy_ec (void);

  /// Event Channel implementation.
  /*
   * Once Event Channel implementation is made reference-counted, this
   * pointer should turn into a Servant_var.
   */
  TAO_EC_Event_Channel *ec_impl_;

  /// A reference to the ORB, so we can shut it down.
  CORBA::ORB_var orb_;
};

#endif /* EC_WRAPPER_H */
