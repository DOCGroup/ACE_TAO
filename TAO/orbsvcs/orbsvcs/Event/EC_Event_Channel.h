/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Event_Channel
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = DESCRIPTION
//   A new implementation of the Real Time Event Services.
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_EVENT_CHANNEL_H
#define TAO_EC_EVENT_CHANNEL_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_Event_Channel : public POA_RtecEventChannelAdmin::EventChannel
{
public:
  TAO_EC_Event_Channel (TAO_EC_Factory* factory);
  // constructor

  virtual ~TAO_EC_Event_Channel (void);
  // destructor

  virtual void activate (CORBA::Environment& env);
  // Create all the objects, connect them, start the internal threads
  // (if any), etc.
  // After this call the EC can be used.

  virtual void shutdown (CORBA::Environment &env);
  // Shutdown any internal threads, cleanup all the internal
  // structures, flush all the messages, etc.

  TAO_EC_Dispatching* dispatching (void) const;
  // Access the dispatching module....

  TAO_EC_Filter_Builder* filter_builder (void) const;
  // Access the filter builder....

  TAO_EC_ConsumerAdmin* consumer_admin (void) const;
  // Access the consumer admin implementation, useful for controlling
  // the activation...

  TAO_EC_SupplierAdmin* supplier_admin (void) const;
  // Access the supplier admin implementation, useful for controlling
  // the activation...

  TAO_EC_Timer_Module* timer_module (void) const;
  // Access the timer module...

  // = The RtecEventChannelAdmin::EventChannel methods...
  virtual RtecEventChannelAdmin::ConsumerAdmin_ptr
      for_consumers (CORBA::Environment& env);
  // The default implementation is:
  //    this->consumer_admin ()->_this (env);

  virtual RtecEventChannelAdmin::SupplierAdmin_ptr
      for_suppliers (CORBA::Environment& env);
  // The default implementation is:
  //    this->supplier_admin ()->_this (env);

  virtual void destroy (CORBA::Environment &env);
  RtecEventChannelAdmin::Observer_Handle
      append_observer (RtecEventChannelAdmin::Observer_ptr,
		       CORBA::Environment &env);
  void remove_objserver (RtecEventChannelAdmin::Observer_Handle,
			 CORBA::Environment &env);
  // @@ Do we need to strategize this also????? How???

private:
  TAO_EC_Factory *factory_;
  // This is the abstract factory that creates all the objects that
  // compose an event channel, the event channel simply acts as a
  // Mediator among them.

  TAO_EC_Dispatching *dispatching_;
  // The dispatching "module"...

  TAO_EC_ConsumerAdmin *consumer_admin_;
  // The ConsumerAdmin implementation....

  TAO_EC_SupplierAdmin *supplier_admin_;
  // The SupplierAdmin implementation....

  TAO_EC_TimerModule *timer_module_;
  // The TimerModule...
};

#if defined (__ACE_INLINE__)
#include "EC_Event_Channel.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_EVENT_CHANNEL_H */
