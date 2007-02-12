// -*- C++ -*-

/**
 *  @file   EC_ObserverStrategy.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Johnny Willemsen (jwillemsen@remedy.nl)
 *  @author Kees van Marle (kvmarle@remedy.nl)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_OBSERVERSTRATEGY_H
#define TAO_EC_OBSERVERSTRATEGY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/ESF/ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Null_Mutex.h"
#include "ace/RB_Tree.h"
#include "ace/Map_Manager.h"

#include "orbsvcs/RtecEventChannelAdminC.h"

#include /**/ "orbsvcs/Event/event_serv_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Lock;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_EC_Event_Channel_Base;
class TAO_EC_ProxyPushConsumer;
class TAO_EC_ProxyPushSupplier;

/**
 * @class TAO_EC_ObserverStrategy
 *
 * @brief The strategy to handle observers for the Event Channel subscriptions
 * and publication.
 *
 * The Event Channel supports Observers for the set of subscriptions and
 * publications. This is used to implement federations of event channels,
 * either through UDP (multicast and unicast) and/or regular CORBA calls.
 * This behavior of the EC is strategized to avoid overhead when no gateways
 * are needed.
 */
class TAO_RTEvent_Serv_Export TAO_EC_ObserverStrategy
{
public:
  /// Destructor
  virtual ~TAO_EC_ObserverStrategy (void);

  /// The basic methods to support the EC strategies.
  virtual RtecEventChannelAdmin::Observer_Handle
      append_observer (RtecEventChannelAdmin::Observer_ptr)
    = 0;
  virtual void remove_observer (
                        RtecEventChannelAdmin::Observer_Handle)
     = 0;

  /// Used by the EC to inform the ObserverStrategy that a Consumer has
  /// connected or disconnected from it.
  virtual void connected (TAO_EC_ProxyPushConsumer*) = 0;
  virtual void disconnected (TAO_EC_ProxyPushConsumer*) = 0;

  /// Used by the EC to inform the ObserverStrategy that a Supplier has
  /// connected or disconnected from it.
  virtual void connected (TAO_EC_ProxyPushSupplier*) = 0;
  virtual void disconnected (TAO_EC_ProxyPushSupplier*) = 0;
};

// ****************************************************************

/**
 * @class TAO_EC_Null_ObserverStrategy
 *
 * @brief A null observer strategy.
 *
 * This class keeps no information and simply ignores the messages
 * from the EC.
 */
class TAO_RTEvent_Serv_Export TAO_EC_Null_ObserverStrategy : public TAO_EC_ObserverStrategy
{
public:
  /// Constructor
  TAO_EC_Null_ObserverStrategy (void);

  // = The TAO_EC_ObserverStrategy methods.
  virtual RtecEventChannelAdmin::Observer_Handle
      append_observer (RtecEventChannelAdmin::Observer_ptr);
  virtual void remove_observer (
                        RtecEventChannelAdmin::Observer_Handle);
  virtual void connected (TAO_EC_ProxyPushConsumer*);
  virtual void disconnected (TAO_EC_ProxyPushConsumer*);
  virtual void connected (TAO_EC_ProxyPushSupplier*);
  virtual void disconnected (TAO_EC_ProxyPushSupplier*);
};

// ****************************************************************

/**
 * @class TAO_EC_Basic_ObserverStrategy
 *
 * @brief A simple observer strategy.
 *
 * This class simply keeps the information about the current list
 * of observers, whenever the list of consumers and/or suppliers
 * changes in queries the EC, computes the global subscription
 * and/or publication list and sends the update message to all the
 * observers.
 *
 * <H2>Memory Management</H2>
 * It assumes ownership of the @a lock, but not of the
 * Event_Channel.
 */
class TAO_RTEvent_Serv_Export TAO_EC_Basic_ObserverStrategy :
  public TAO_EC_ObserverStrategy
{
public:
  /// Constructor
  TAO_EC_Basic_ObserverStrategy (TAO_EC_Event_Channel_Base* ec,
                                 ACE_Lock* lock);

  /// Destructor
  virtual ~TAO_EC_Basic_ObserverStrategy (void);

  // = The TAO_EC_ObserverStrategy methods.
  virtual RtecEventChannelAdmin::Observer_Handle
      append_observer (RtecEventChannelAdmin::Observer_ptr);
  virtual void remove_observer (
                        RtecEventChannelAdmin::Observer_Handle);
  virtual void connected (TAO_EC_ProxyPushConsumer*);
  virtual void disconnected (TAO_EC_ProxyPushConsumer*);
  virtual void connected (TAO_EC_ProxyPushSupplier*);
  virtual void disconnected (TAO_EC_ProxyPushSupplier*);

  /**
   * @struct Observer_Entry
   *
   * @brief  The data kept for each observer.
   *
   * The observer and its handle are kept in a simple structure.
   * In the future this structure could contain QoS information,
   * such as:
   * + how often do we update the observer?
   * + When was the last update.
   * + Does it want to receive all changes?
   */
  struct Observer_Entry
  {
    Observer_Entry (void);
    Observer_Entry (RtecEventChannelAdmin::Observer_Handle h,
                    RtecEventChannelAdmin::Observer_ptr o);

    /// The handle
    RtecEventChannelAdmin::Observer_Handle handle;

    /// The observer
    RtecEventChannelAdmin::Observer_var observer;
  };

  struct Header_Compare
  {
    int operator () (const RtecEventComm::EventHeader& lhs,
                     const RtecEventComm::EventHeader& rhs) const;
  };

  typedef ACE_Map_Manager<RtecEventChannelAdmin::Observer_Handle,Observer_Entry,ACE_Null_Mutex> Observer_Map;
  typedef ACE_Map_Iterator<RtecEventChannelAdmin::Observer_Handle,Observer_Entry,ACE_Null_Mutex> Observer_Map_Iterator;


  typedef ACE_RB_Tree<RtecEventComm::EventHeader,int,Header_Compare,ACE_Null_Mutex> Headers;
  typedef ACE_RB_Tree_Iterator<RtecEventComm::EventHeader,int,Header_Compare,ACE_Null_Mutex> HeadersIterator;

protected:
  /// Helpers.
  //@{
  /// Recompute EC consumer subscriptions and send them out to all observers.
  virtual void consumer_qos_update (TAO_EC_ProxyPushSupplier *supplier);

  /// Recompute EC supplier publications and send them out to all observers.
  virtual void supplier_qos_update (TAO_EC_ProxyPushConsumer *consumer);

  /// Compute consumer QOS.
  void fill_qos (RtecEventChannelAdmin::ConsumerQOS &qos);
  /// Compute supplier QOS.
  void fill_qos (RtecEventChannelAdmin::SupplierQOS &qos);

  /// Copies all current observers into an array and passes it
  /// back to the caller through @a lst.  Returns the size of the array.
  int create_observer_list (RtecEventChannelAdmin::Observer_var *&lst);
  //@}

protected:
  /// The event channel.
  TAO_EC_Event_Channel_Base* event_channel_;

  /// The lock
  ACE_Lock* lock_;

  /// The handles are generated in sequential order, but are opaque to
  /// the client.
  RtecEventChannelAdmin::Observer_Handle handle_generator_;

  /// Keep the set of Observers
  Observer_Map observers_;
};

// ****************************************************************

/**
 * @class TAO_EC_Reactive_ObserverStrategy
 *
 * @brief A reactive observer strategy.
 *
 * This class simply keeps the information about the current list
 * of observers, whenever the list of consumers and/or suppliers
 * changes in queries the EC, computes the global subscription
 * and/or publication list and sends the update message to all the
 * observers. When an observer isn't reachable it is removed from
 * the observer list.
 *
 * <H2>Memory Management</H2>
 * It assumes ownership of the <lock>, but not of the
 * Event_Channel.
 */
class TAO_RTEvent_Serv_Export TAO_EC_Reactive_ObserverStrategy :
  public TAO_EC_Basic_ObserverStrategy
{
public:
  /// Constructor
  TAO_EC_Reactive_ObserverStrategy (TAO_EC_Event_Channel_Base* ec,
                                    ACE_Lock* lock);

  /// Destructor
  virtual ~TAO_EC_Reactive_ObserverStrategy (void);

protected:
  /// Helpers.
  //@{
  /// Recompute EC consumer subscriptions and send them out to all observers.
  virtual void consumer_qos_update (TAO_EC_ProxyPushSupplier *supplier);

  /// Recompute EC supplier publications and send them out to all observers.
  virtual void supplier_qos_update (TAO_EC_ProxyPushConsumer *consumer);

  /**
   * Copies all current observers into a map and passes it
   * back to the caller through @a map.
   * @return Returns the size of the map.
   */
  int create_observer_map (Observer_Map &map);

  /// The observer doesn't exist anymore
  void observer_not_exists (Observer_Entry& observer);
  //@}
};

// ****************************************************************

class TAO_EC_Accumulate_Supplier_Headers :
  public TAO_ESF_Worker<TAO_EC_ProxyPushSupplier>
{
public:
  /// Constructor
  TAO_EC_Accumulate_Supplier_Headers (TAO_EC_Basic_ObserverStrategy::Headers &headers);

  virtual void work (TAO_EC_ProxyPushSupplier *supplier);

private:
  TAO_EC_Basic_ObserverStrategy::Headers &headers_;
};

// ****************************************************************

class TAO_EC_Accumulate_Consumer_Headers :
  public TAO_ESF_Worker<TAO_EC_ProxyPushConsumer>
{
public:
  /// Constructor
  TAO_EC_Accumulate_Consumer_Headers (TAO_EC_Basic_ObserverStrategy::Headers &headers);

  virtual void work (TAO_EC_ProxyPushConsumer *consumer);

private:
  TAO_EC_Basic_ObserverStrategy::Headers &headers_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_ObserverStrategy.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_EC_OBSERVERSTRATEGY_H */
