/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Cos Event Channel
//
// = FILENAME
//   CEC_SupplierControl
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
//   More details can be found in:
//   http://www.cs.wustl.edu/~coryan/EC/index.html
//
// ============================================================================

#ifndef TAO_CEC_SUPPLIERCONTROL_H
#include "ace/pre.h"
#define TAO_CEC_SUPPLIERCONTROL_H

#include "orbsvcs/orbsvcs_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"

class TAO_CEC_EventChannel;
class TAO_CEC_ProxyPushConsumer;
class TAO_CEC_ProxyPullConsumer;

class TAO_ORBSVCS_Export TAO_CEC_SupplierControl
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
  TAO_CEC_SupplierControl (void);
  // Constructor.  It does not assume ownership of the <event_channel>
  // parameter.

  virtual ~TAO_CEC_SupplierControl (void);
  // destructor...

  virtual int activate (void);
  virtual int shutdown (void);
  // Activate any internal threads or timers used to poll the state of
  // the suppliers

  virtual void supplier_not_exist (TAO_CEC_ProxyPushConsumer *proxy,
                                   CORBA::Environment &);
  // Invoked by helper classes when they detect that a supplier does
  // not exists (i.e. _non_existent() returns true and/or the
  // CORBA::OBJECT_NOT_EXIST exception has been raised).

  virtual void supplier_not_exist (TAO_CEC_ProxyPullConsumer *proxy,
                                   CORBA::Environment &);
  // Invoked by helper classes when they detect that a supplier does
  // not exists (i.e. _non_existent() returns true and/or the
  // CORBA::OBJECT_NOT_EXIST exception has been raised).

  virtual void system_exception (TAO_CEC_ProxyPullConsumer *proxy,
                                 CORBA::SystemException &,
                                 CORBA::Environment &);
  // Some system exception was rasied while trying to push an event.
};

#if defined (__ACE_INLINE__)
#include "CEC_SupplierControl.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CEC_SUPPLIERCONTROL_H */
