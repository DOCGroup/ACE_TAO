// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Channel_Clients_T
//
// = AUTHOR
//    Tim Harrison (harrison@cs.wustl.edu) and
//    Douglas Schmidt (schmidt@cs.wustl.edu)
//
// = DESCRIPTION
//    These classes allow applications to be consumer, suppliers, and
//    consumer suppliers, as well as being active objects.  This is
//    accomplished with adapters to prevent the use of multiple
//    inheritence (which is this root of all evil.)
//
// ============================================================================

#ifndef ACE_CHANNEL_CLIENTS_T_H
#define ACE_CHANNEL_CLIENTS_T_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventCommS.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_PushConsumer_Adapter
 *
 * @brief ACE Push Consumer Adapter
 *
 * Forwards all calls to the target_.
 */
template <class TARGET>
class ACE_PushConsumer_Adapter : public POA_RtecEventComm::PushConsumer
{
public:
  /// Forwards all calls to @a target.
  ACE_PushConsumer_Adapter (TARGET *target);

  /// Forwards to target_.
  virtual void push (const RtecEventComm::EventSet& events
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC (( CORBA::SystemException));

  /// Forwards to target_.
  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC (( CORBA::SystemException));

private:
  TARGET *target_;
};

// ************************************************************

/**
 * @class ACE_PushSupplier_Adapter
 *
 * @brief ACE Push Supplier Adapter
 *
 * Forwards all calls to disconnect_push_supplier to the target_.
 */
template <class TARGET>
class ACE_PushSupplier_Adapter : public POA_RtecEventComm::PushSupplier
{
public:
  /// Forwards all calls to <owner>.
  ACE_PushSupplier_Adapter (TARGET *target);

  /// Forwards to target_.
  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC (( CORBA::SystemException));

private:
  TARGET *target_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Channel_Clients_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Channel_Clients_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Channel_Clients_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_CHANNEL_CLIENTS_T_H */
