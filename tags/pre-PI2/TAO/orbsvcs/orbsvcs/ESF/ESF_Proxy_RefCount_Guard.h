/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Event Service Framework
//
// = FILENAME
//   ESF_Proxy_RefCount_Guard
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_ESF_PROXY_REFCOUNT_GUARD_H
#define TAO_ESF_PROXY_REFCOUNT_GUARD_H

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class EVENT_CHANNEL, class PROXY>
class TAO_ESF_Proxy_RefCount_Guard
{
  // = TITLE
  //   Reference count based guard.
  //
  // = DESCRIPTION
  //   A common idiom used on event services is to increment a
  //   reference count before starting a long running operation.
  //   The system can then execute the operation without any risk of
  //   having the underlying object destroyed.  The advantage of using
  //   a reference count is that no mutex or lock needs to be held
  //   while the operation is beign executed.
  //   This class implements that common idiom, but it also adds hooks
  //   to handle scenarios where more than one operation is performed
  //   while holding the reference count.
  // 
  // = TODO
  //   @@ The type of lock could be parametric
  //
public:
  TAO_ESF_Proxy_RefCount_Guard (CORBA::ULong &refcount,
                                EVENT_CHANNEL *ec,
                                PROXY *proxy);
  // Constructor

  ~TAO_ESF_Proxy_RefCount_Guard (void);
  // Destructor

protected:
  CORBA::ULong &refcount_;
  // The reference count, if it gets to zero then the object must be
  // destroyed

  EVENT_CHANNEL *event_channel_;
  // The event channel used to destroy the proxy

  PROXY *proxy_;
  // The proxy whose lifetime is controlled by the reference count
};

#if defined (__ACE_INLINE__)
#include "ESF_Proxy_RefCount_Guard.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ESF_Proxy_RefCount_Guard.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Proxy_RefCount_Guard.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_PROXY_REFCOUNT_GUARD_H */
