/* -*- C++ -*- */
/**
 *  @file   EC_ObserverStrategy.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_OBSERVERSTRATEGY_H
#define TAO_EC_OBSERVERSTRATEGY_H
#include "ace/pre.h"

#include "orbsvcs/ESF/ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/RB_Tree.h"
#include "ace/Map_Manager.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "event_export.h"

class ACE_Lock;
class TAO_EC_Event_Channel_Base;
class TAO_EC_ProxyPushConsumer;
class TAO_EC_ProxyPushSupplier;

/**
 * @class TAO_EC_ObserverStrategy
 *
 * @brief The strategy to handle observers for the Event Channel
 * subscriptions and publication.
 *
 * The Event Channel supports Observers for the set of
 * subscriptions and publications.
 * This is used to implement federations of event channels,
 * either through UDP (multicast and unicast) and/or regular CORBA
 * calls.
 * This behavior of the EC is strategized to avoid overhead when
 * no gateways are needed.
 */
class TAO_RTEvent_Export TAO_EC_ObserverStrategy
{
public:
  /// Destructor
  virtual ~TAO_EC_ObserverStrategy (void);

  /// The basic methods to support the EC strategies.
  virtual RtecEventChannelAdmin::Observer_Handle
      append_observer (RtecEventChannelAdmin::Observer_ptr
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER))
    = 0;
  virtual void remove_observer (
                        RtecEventChannelAdmin::Observer_Handle
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER))
     = 0;

  /// Used by the EC to inform the ObserverStrategy that a Consumer has
  /// connected or disconnected from it.
  virtual void connected (TAO_EC_ProxyPushConsumer*
                          ACE_ENV_ARG_DECL_NOT_USED) = 0;
  virtual void disconnected (TAO_EC_ProxyPushConsumer*
                             ACE_ENV_ARG_DECL_NOT_USED) = 0;

  /// Used by the EC to inform the ObserverStrategy that a Consumer has
  /// connected or disconnected from it.
  virtual void connected (TAO_EC_ProxyPushSupplier*
                          ACE_ENV_ARG_DECL_NOT_USED) = 0;
  virtual void disconnected (TAO_EC_ProxyPushSupplier*
                             ACE_ENV_ARG_DECL_NOT_USED) = 0;
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
class TAO_RTEvent_Export TAO_EC_Null_ObserverStrategy : public TAO_EC_ObserverStrategy
{
public:
  /// Constructor
  TAO_EC_Null_ObserverStrategy (void);

  // = The TAO_EC_ObserverStrategy methods.
  virtual RtecEventChannelAdmin::Observer_Handle
      append_observer (RtecEventChannelAdmin::Observer_ptr
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER));
  virtual void remove_observer (
                        RtecEventChannelAdmin::Observer_Handle
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER));
  virtual void connected (TAO_EC_ProxyPushConsumer*
                          ACE_ENV_ARG_DECL_NOT_USED);
  virtual void disconnected (TAO_EC_ProxyPushConsumer*
                             ACE_ENV_ARG_DECL_NOT_USED);
  virtual void connected (TAO_EC_ProxyPushSupplier*
                          ACE_ENV_ARG_DECL_NOT_USED);
  virtual void disconnected (TAO_EC_ProxyPushSupplier*
                             ACE_ENV_ARG_DECL_NOT_USED);
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
 * It assumes ownership of the <lock>, but not of the
 * Event_Channel.
 */
class TAO_RTEvent_Export TAO_EC_Basic_ObserverStrategy :
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
      append_observer (RtecEventChannelAdmin::Observer_ptr
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER));
  virtual void remove_observer (
                        RtecEventChannelAdmin::Observer_Handle
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER));
  virtual void connected (TAO_EC_ProxyPushConsumer*
                          ACE_ENV_ARG_DECL_NOT_USED);
  virtual void disconnected (TAO_EC_ProxyPushConsumer*
                             ACE_ENV_ARG_DECL_NOT_USED);
  virtual void connected (TAO_EC_ProxyPushSupplier*
                          ACE_ENV_ARG_DECL_NOT_USED);
  virtual void disconnected (TAO_EC_ProxyPushSupplier*
                             ACE_ENV_ARG_DECL_NOT_USED);

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
    // The ACE_INLINE macros here are to keep g++ 2.7.X happy,
    // otherwise it thinks they are used as inline functions before
    // beign used as such.... Apparently in the template code for the
    // Hash_Map_Manager.
    ACE_INLINE Observer_Entry (void);
    ACE_INLINE Observer_Entry (RtecEventChannelAdmin::Observer_Handle h,
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
  void consumer_qos_update (TAO_EC_ProxyPushSupplier *supplier
                            ACE_ENV_ARG_DECL);

  /// Recompute EC supplier publications and send them out to all observers.
  void supplier_qos_update (TAO_EC_ProxyPushConsumer *consumer
                            ACE_ENV_ARG_DECL);

  /// Compute consumer QOS.
  void fill_qos (RtecEventChannelAdmin::ConsumerQOS &qos
                 ACE_ENV_ARG_DECL);
  /// Compute supplier QOS.
  void fill_qos (RtecEventChannelAdmin::SupplierQOS &qos
                 ACE_ENV_ARG_DECL);

  /// Copies all current observers into an array and passes it
  /// back to the caller through <lst>.  Returns the size of the array.
  int create_observer_list (RtecEventChannelAdmin::Observer_var *&lst
                            ACE_ENV_ARG_DECL);
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

class TAO_EC_Accumulate_Supplier_Headers :
  public TAO_ESF_Worker<TAO_EC_ProxyPushSupplier>
{
public:
  /// Constructor
  TAO_EC_Accumulate_Supplier_Headers (TAO_EC_Basic_ObserverStrategy::Headers &headers);

  virtual void work (TAO_EC_ProxyPushSupplier *supplier
                     ACE_ENV_ARG_DECL);

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

  virtual void work (TAO_EC_ProxyPushConsumer *consumer
                     ACE_ENV_ARG_DECL);

private:
  TAO_EC_Basic_ObserverStrategy::Headers &headers_;
};



#if defined (__ACE_INLINE__)
#include "EC_ObserverStrategy.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_OBSERVERSTRATEGY_H */
