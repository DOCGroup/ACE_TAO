/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_ConsumerAdmin
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
// ============================================================================

#ifndef TAO_EC_CONSUMERADMIN_H
#define TAO_EC_CONSUMERADMIN_H
#include "ace/pre.h"

#include "EC_ProxySupplier.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/ESF/ESF_Peer_Admin.h"
#include "event_export.h"

class TAO_EC_Event_Channel;
class TAO_EC_ProxyPushConsumer;

class TAO_RTEvent_Export TAO_EC_ConsumerAdmin
  : public POA_RtecEventChannelAdmin::ConsumerAdmin
  , public TAO_ESF_Peer_Admin<TAO_EC_Event_Channel,TAO_EC_ProxyPushSupplier,RtecEventChannelAdmin::ProxyPushSupplier,TAO_EC_ProxyPushConsumer>
{
  // = TITLE
  //   ConsumerAdmin
  //
  // = DESCRIPTION
  //   Implements the ConsumerAdmin interface, i.e. the factory for
  //   ProxyPushSupplier objects.
  //
  // = MEMORY MANAGMENT
  //   It does not assume ownership of the TAO_EC_Event_Channel
  //   object; but it *does* assume ownership of the
  //   TAO_EC_ProxyPushSupplier_Set object.
  //
  // = LOCKING
  //   No provisions for locking, access must be serialized
  //   externally.
  //
  // = TODO
  //
public:
  TAO_EC_ConsumerAdmin (TAO_EC_Event_Channel* event_channel);
  // constructor. If <supplier_set> is nil then it builds one using
  // the <event_channel> argument.
  // In any case it assumes ownership.

  virtual ~TAO_EC_ConsumerAdmin (void);
  // destructor...

  // = The RtecEventChannelAdmin::ConsumerAdmin methods...
  virtual RtecEventChannelAdmin::ProxyPushSupplier_ptr
      obtain_push_supplier (CORBA::Environment &)
          ACE_THROW_SPEC ((CORBA::SystemException));

  // = The PortableServer::ServantBase methods
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment& env);

private:
  PortableServer::POA_var default_POA_;
  // Store the default POA.
};

#if defined (__ACE_INLINE__)
#include "EC_ConsumerAdmin.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_CONSUMERADMIN_H */
