/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_ObserverStrategy
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

#ifndef TAO_EC_OBSERVERSTRATEGY_H
#define TAO_EC_OBSERVERSTRATEGY_H

#include "ace/Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/RB_Tree.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

class ACE_Lock;
class TAO_EC_Event_Channel;
class TAO_EC_ProxyPushConsumer;
class TAO_EC_ProxyPushSupplier;

class TAO_ORBSVCS_Export TAO_EC_ObserverStrategy
{
  // = TITLE
  //   The strategy to handle observers for the Event Channel
  //   subscriptions and publication.
  //
  // = DESCRIPTION
  //   The Event Channel supports Observers for the set of
  //   subscriptions and publications.
  //   This is used to implement federations of event channels,
  //   either through UDP (multicast and unicast) and/or regular CORBA
  //   calls.
  //
  //   This behavior of the EC is strategized to avoid overhead when
  //   no gateways are needed.
  //
public:
  virtual ~TAO_EC_ObserverStrategy (void);
  // Destructor

  virtual RtecEventChannelAdmin::Observer_Handle
      append_observer (RtecEventChannelAdmin::Observer_ptr,
		       CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannel::EventChannel::SYNCHRONIZATION_ERROR,
                     RtecEventChannel::EventChannel::CANT_APPEND_OBSERVER))
    = 0;
  virtual void remove_observer (
                        RtecEventChannelAdmin::Observer_Handle,
                        CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannel::EventChannel::SYNCHRONIZATION_ERROR,
                     RtecEventChannel::EventChannel::CANT_REMOVE_OBSERVER))
     = 0;
  // The basic methods to support the EC strategies.

  virtual void connected (TAO_EC_ProxyPushConsumer*,
                          CORBA::Environment&) = 0;
  virtual void disconnected (TAO_EC_ProxyPushConsumer*,
                             CORBA::Environment&) = 0;
  // Used by the EC to inform the ObserverStrategy that a Consumer has
  // connected or disconnected from it.

  virtual void connected (TAO_EC_ProxyPushSupplier*,
                          CORBA::Environment&) = 0;
  virtual void disconnected (TAO_EC_ProxyPushSupplier*,
                             CORBA::Environment&) = 0;
  // Used by the EC to inform the ObserverStrategy that a Consumer has
  // connected or disconnected from it.
};

// ****************************************************************

class TAO_EC_Null_ObserverStrategy : public TAO_EC_ObserverStrategy
{
  // = TITLE
  //   A null observer strategy.
  //
  // = DESCRIPTION
  //   This class keeps no information and simply ignores the messages
  //   from the EC.
  //
public:
  TAO_EC_Null_ObserverStrategy (void);
  // Constructor

  // = The TAO_EC_ObserverStrategy methods.
  virtual RtecEventChannelAdmin::Observer_Handle
      append_observer (RtecEventChannelAdmin::Observer_ptr,
		       CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannel::EventChannel::SYNCHRONIZATION_ERROR,
                     RtecEventChannel::EventChannel::CANT_APPEND_OBSERVER));
  virtual void remove_observer (
                        RtecEventChannelAdmin::Observer_Handle,
                        CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannel::EventChannel::SYNCHRONIZATION_ERROR,
                     RtecEventChannel::EventChannel::CANT_REMOVE_OBSERVER));
  virtual void connected (TAO_EC_ProxyPushConsumer*,
                          CORBA::Environment&);
  virtual void disconnected (TAO_EC_ProxyPushConsumer*,
                             CORBA::Environment&);
  virtual void connected (TAO_EC_ProxyPushSupplier*,
                          CORBA::Environment&);
  virtual void disconnected (TAO_EC_ProxyPushSupplier*,
                             CORBA::Environment&);
};

// ****************************************************************

class TAO_EC_Basic_ObserverStrategy : public TAO_EC_ObserverStrategy
{
  // = TITLE
  //   A simple observer strategy.
  //
  // = DESCRIPTION
  //   This class simply keeps the information about the current list
  //   of observers, whenever the list of consumers and/or suppliers
  //   changes in queries the EC, computes the global subscription
  //   and/or publication list and sends the update message to all the
  //   observers.
  //
  // = MEMORY MANAGMENT
  //   It assumes ownership of the <lock>, but not of the
  //   Event_Channel.
  //
public:
  TAO_EC_Basic_ObserverStrategy (TAO_EC_Event_Channel* ec,
                                 ACE_Lock* lock);
  // Constructor

  virtual ~TAO_EC_Basic_ObserverStrategy (void);
  // Destructor

  // = The TAO_EC_ObserverStrategy methods.
  virtual RtecEventChannelAdmin::Observer_Handle
      append_observer (RtecEventChannelAdmin::Observer_ptr,
		       CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannel::EventChannel::SYNCHRONIZATION_ERROR,
                     RtecEventChannel::EventChannel::CANT_APPEND_OBSERVER));
  virtual void remove_observer (
                        RtecEventChannelAdmin::Observer_Handle,
                        CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannel::EventChannel::SYNCHRONIZATION_ERROR,
                     RtecEventChannel::EventChannel::CANT_REMOVE_OBSERVER));
  virtual void connected (TAO_EC_ProxyPushConsumer*,
                          CORBA::Environment&);
  virtual void disconnected (TAO_EC_ProxyPushConsumer*,
                             CORBA::Environment&);
  virtual void connected (TAO_EC_ProxyPushSupplier*,
                          CORBA::Environment&);
  virtual void disconnected (TAO_EC_ProxyPushSupplier*,
                             CORBA::Environment&);


  struct Observer_Entry
  {
    // = TITLE
    //   The data kept for each observer.
    //
    // = DESCRIPTION
    //   The observer and its handle are kept in a simple structure.
    //   In the future this structure could contain QoS information,
    //   such as:
    //   + how often do we update the observer?
    //   + When was the last update.
    //   + Does it want to receive all changes?
    //

    Observer_Entry (void);
    Observer_Entry (RtecEventChannelAdmin::Observer_Handle h,
                    RtecEventChannelAdmin::Observer_ptr o);

    RtecEventChannelAdmin::Observer_Handle handle;
    // The handle

    RtecEventChannelAdmin::Observer_var observer;
    // The observer
  };

  struct Header_Compare
  {
    int operator () (const RtecEventComm::EventHeader& lhs,
                     const RtecEventComm::EventHeader& rhs) const;
  };

private:
  void fill_qos (RtecEventChannelAdmin::ConsumerQOS &qos,
                 CORBA::Environment &env);
  void fill_qos (RtecEventChannelAdmin::SupplierQOS &qos,
                 CORBA::Environment &env);
  // Helper functions to compute the consumer and supplier QOS.

private:
  TAO_EC_Event_Channel* event_channel_;
  // The event channel.

  ACE_Lock* lock_;
  // The lock

  typedef ACE_Map_Manager<RtecEventChannelAdmin::Observer_Handle,Observer_Entry,ACE_Null_Mutex> Observer_Map;
  typedef ACE_Map_Iterator<RtecEventChannelAdmin::Observer_Handle,Observer_Entry,ACE_Null_Mutex> Observer_Map_Iterator;

  RtecEventChannelAdmin::Observer_Handle handle_generator_;
  // The handles are generated in sequential order, but are opaque to
  // the client.

  Observer_Map observers_;
  // Keep the set of Observers

  typedef ACE_RB_Tree<RtecEventComm::EventHeader,int,Header_Compare,ACE_Null_Mutex> Headers;
  typedef ACE_RB_Tree_Iterator<RtecEventComm::EventHeader,int,Header_Compare,ACE_Null_Mutex> HeadersIterator;
};

#if defined (__ACE_INLINE__)
#include "EC_ObserverStrategy.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_OBSERVERSTRATEGY_H */
