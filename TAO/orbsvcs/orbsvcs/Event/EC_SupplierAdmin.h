/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_SupplierAdmin
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_SUPPLIERADMIN_H
#define TAO_EC_SUPPLIERADMIN_H
#include "ace/pre.h"

#include "EC_ProxyConsumer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Peer_Admin.h"
#include "event_export.h"

class TAO_EC_Event_Channel;
class TAO_EC_ProxyPushSupplier;

class TAO_RTEvent_Export TAO_EC_SupplierAdmin
  : public POA_RtecEventChannelAdmin::SupplierAdmin
  , public TAO_ESF_Peer_Admin<TAO_EC_Event_Channel,TAO_EC_ProxyPushConsumer,TAO_EC_ProxyPushSupplier>
{
  // = TITLE
  //   ProxyPushSupplier
  //
  // = DESCRIPTION
  //   Implement the RtecEventChannelAdmin::SupplierAdmin interface.
  //   This class is an Abstract Factory for the
  //   TAO_EC_ProxyPushConsumer.
  //
  // = MEMORY MANAGMENT
  //   It does not assume ownership of the TAO_EC_Event_Channel object
  //
public:
  TAO_EC_SupplierAdmin (TAO_EC_Event_Channel* event_channel);
  // constructor...

  virtual ~TAO_EC_SupplierAdmin (void);
  // destructor...

  // = The RtecEventChannelAdmin::SupplierAdmin methods...
  virtual RtecEventChannelAdmin::ProxyPushConsumer_ptr
      obtain_push_consumer (CORBA::Environment &)
          ACE_THROW_SPEC ((CORBA::SystemException));

  // = The PortableServer::ServantBase methods
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment& env);

private:
  PortableServer::POA_var default_POA_;
  // Store the default POA.
};

#if defined (__ACE_INLINE__)
#include "EC_SupplierAdmin.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_SUPPLIERADMIN_H */
