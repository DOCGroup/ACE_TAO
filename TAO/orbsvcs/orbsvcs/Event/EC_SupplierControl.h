/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_SupplierControl
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
//   More details can be found in:
//   http://www.cs.wustl.edu/~coryan/EC/index.html
//
// ============================================================================

#ifndef TAO_EC_SUPPLIERCONTROL_H
#include "ace/pre.h"
#define TAO_EC_SUPPLIERCONTROL_H

#include "orbsvcs/Event/event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"

class TAO_EC_Event_Channel;
class TAO_EC_ProxyPushConsumer;

class TAO_RTEvent_Export TAO_EC_SupplierControl
{
  // = TITLE
  //   SupplierControl
  //
  // = DESCRIPTION
  //   Defines the interface for the supplier control strategy.
  //   This strategy handles misbehaving or failing suppliers.
  //
  // = MEMORY MANAGMENT
  //
  // = LOCKING
  //
  // = TODO
  //
public:
  TAO_EC_SupplierControl (void);
  // Constructor.  It does not assume ownership of the <event_channel>
  // parameter.

  virtual ~TAO_EC_SupplierControl (void);
  // destructor...

  virtual int activate (void);
  virtual int shutdown (void);
  // Activate any internal threads or timers used to poll the state of
  // the suppliers

  virtual void supplier_not_exist (TAO_EC_ProxyPushConsumer *proxy,
                                   CORBA::Environment &);
  // Invoked by helper classes when they detect that a supplier does
  // not exists (i.e. _non_existent() returns true and/or the
  // CORBA::OBJECT_NOT_EXIST exception has been raised).
};

#if defined (__ACE_INLINE__)
#include "EC_SupplierControl.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_SUPPLIERCONTROL_H */
