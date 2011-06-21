// -*- C++ -*-

/**
 *  @file   ESF_Proxy_RefCount_Guard.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_ESF_PROXY_REFCOUNT_GUARD_H
#define TAO_ESF_PROXY_REFCOUNT_GUARD_H

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ESF_Proxy_RefCount_Guard
 *
 * @brief Reference count based guard.
 *
 * A common idiom used on event services is to increment a
 * reference count before starting a long running operation.
 * The system can then execute the operation without any risk of
 * having the underlying object destroyed.  The advantage of using
 * a reference count is that no mutex or lock needs to be held
 * while the operation is being executed.
 * This class implements that common idiom, but it also adds hooks
 * to handle scenarios where more than one operation is performed
 * while holding the reference count.
 *
 * @todo The type of lock could be parametric
 */
template<class EVENT_CHANNEL, class PROXY>
class TAO_ESF_Proxy_RefCount_Guard
{
public:
  /// Constructor
  TAO_ESF_Proxy_RefCount_Guard (CORBA::ULong &refcount,
                                EVENT_CHANNEL *ec,
                                PROXY *proxy);

  /// Destructor
  ~TAO_ESF_Proxy_RefCount_Guard (void);

protected:
  /// The reference count, if it gets to zero then the object must be
  /// destroyed
  CORBA::ULong &refcount_;

  /// The event channel used to destroy the proxy
  EVENT_CHANNEL *event_channel_;

  /// The proxy whose lifetime is controlled by the reference count
  PROXY *proxy_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Proxy_RefCount_Guard.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/ESF/ESF_Proxy_RefCount_Guard.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Proxy_RefCount_Guard.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_PROXY_REFCOUNT_GUARD_H */
