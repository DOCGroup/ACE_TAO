// -*- C++ -*-

/**
 * @file ECG_Mcast_EH.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 * @author Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
 * @author Marina Spivak <marina@atdesk.com>
 * @author Don Hinton <dhinton@ieee.org>
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 *
 */
#ifndef TAO_ECG_MCAST_EH_H
#define TAO_ECG_MCAST_EH_H

#include /**/ "ace/pre.h"
#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Unbounded_Set.h"
#include "ace/Array_Base.h"
#include "ace/SOCK_Dgram_Mcast.h"

#include "orbsvcs/RtecEventChannelAdminS.h"

#include /**/ "orbsvcs/Event/event_serv_export.h"
#include "orbsvcs/Event/ECG_Adapters.h"
#include "orbsvcs/Event/EC_Lifetime_Utils.h"
#include "orbsvcs/Event/EC_Lifetime_Utils_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ECG_Mcast_EH
 *
 * @brief Event Handler for Mcast messages.
 *        NOT THREAD-SAFE.
 *
 * This object acts as an Observer to Event Channel.  It subscribes to
 * multicast groups that carry events matching the EC's subscriptions.
 * This object then receives callbacks from the Reactor when data is
 * available on the mcast sockets and alerts TAO_ECG_Dgram_Handler,
 * which reads the data, transforms it into event and pushes to the
 * Event Channel.
 */
class TAO_RTEvent_Serv_Export TAO_ECG_Mcast_EH :
  public ACE_Event_Handler,
  public TAO_ECG_Handler_Shutdown
{
public:

  /// Initialization and termination methods.
  //@{
  /**
   * Constructor.  Messages received by this EH will be forwarded to
   * the \a recv.  \a net_if can be used to specify NIC where multicast
   * messages are expected. \buf_sz would be used to alter the default
   * buffer size.
   *
   * See comments for receiver_ data member on why raw pointer is
   * used for the \a recv argument.
   */
  TAO_ECG_Mcast_EH (TAO_ECG_Dgram_Handler *recv,
                    const ACE_TCHAR *net_if = 0,
                    CORBA::ULong buf_sz = 0);

  /// Destructor.
  virtual ~TAO_ECG_Mcast_EH (void);

  /**
   * Register for changes in the EC subscription list.
   * When the subscription list becomes non-empty we join the proper
   * multicast groups (using Dgram_Handler to translate between event
   * types and mcast groups) and the class registers itself with the
   * reactor.
   *
   * To insure proper resource clean up, if open () is successful,
   * the user MUST call shutdown () when handler is no longer needed
   * (and its reactor still exists).
   */
  void open (RtecEventChannelAdmin::EventChannel_ptr ec);

  /// TAO_ECG_Handler_Shutdown method.
  /**
   * Remove ourselves from the event channel, unsubscribe from the
   * multicast groups, close the sockets and deregister from the
   * reactor.
   */
  virtual int shutdown (void);
  //@}

  /// Reactor callback.  Notify receiver_ that a dgram corresponding
  /// to \a fd is ready for reading.
  virtual int handle_input (ACE_HANDLE fd);

private:

  /**
   * @class Observer
   *
   * @brief Observes changes in the EC consumer subscriptions and notifies
   *        TAO_ECG_Mcast_EH  when there are changes.
   */
  class Observer :
    public virtual POA_RtecEventChannelAdmin::Observer,
    public TAO_EC_Deactivated_Object
  {
  public:
    /// Constructor.  Changes in the EC subscriptions will be reported
    /// to the \a eh.
    Observer (TAO_ECG_Mcast_EH* eh);

    /// Shut down the observer: disconnect from EC and deactivate from
    /// POA.
    void shutdown (void);

    /// Event Channel Observer methods
    //@{
    virtual void update_consumer (
        const RtecEventChannelAdmin::ConsumerQOS& sub);
    virtual void update_supplier (
        const RtecEventChannelAdmin::SupplierQOS& pub);

  private:
    /// Handler we notify of subscriptions changes.
    /*
     * Observer can keep a raw pointer to mcast handler, because the handler
     * guarantees to notify the observer (by calling shutdown ())
     * before going away.
     */
    TAO_ECG_Mcast_EH *eh_;
  };

  /// Make update_consumer () accessible to Observer.
  friend class Observer;

  /// The Observer method.  Subscribe/unsubscribe to multicast groups
  /// according to changes in consumer subscriptions.
  void update_consumer (const RtecEventChannelAdmin::ConsumerQOS& sub);


  typedef ACE_Unbounded_Set<ACE_INET_Addr> Address_Set;

  /// Helpers for updating multicast subscriptions based on changes in
  /// consumer subscriptions.
  //@{
  /// Compute the list of multicast addresses that we need to be
  /// subscribed to, in order to receive the events described in the
  /// ConsumerQOS parameter.
  /**
   * @param sub The list of event types that our event channel
   *        consumers are interested in.
   * @param multicast_addresses This method populates this list with
   *        multicast addresses that we need to be subscribed to in
   *        order to receive event types specified in /a sub.
   *
   * @throw CORBA::SystemException This method needs to perform
   *        several CORBA invocations, and it propagates any exceptions
   *        back to the caller.
   */
  void compute_required_subscriptions (
        const RtecEventChannelAdmin::ConsumerQOS& sub,
              Address_Set& multicast_addresses);

  /// Unsubscribe from any multicast addresses we are currently
  /// subscribed to that are not in the \a multicast_addresses list.
  /// Also remove from /a multicast_addresses any addresses to which we are
  /// already subscribed.
  /**
   * @param multicast_addresses List of multicast
   *        addresses we need to be subscribed to in order receive all
   *        event types in the current consumer subscriptions.
   */
  int delete_unwanted_subscriptions (
              Address_Set& multicast_addresses);

  /// Subscribe to all multicast addresses in /a multicast_addresses -
  /// we are not subscribed to them yet, but need to be.
  /**
   * @param multicast_addresses List of multicast addresses to which
   *        we need to subscribe to in order to be receiving all event
   *        types in the current consumer subscriptions.
   */
  void add_new_subscriptions (
              Address_Set& multicast_addresses);
  //@}

  /**
   * @class Observer_Disconnect_Command
   *
   * @brief Disconnects Observer from the Event Channel
   *
   * Utility class for use as a template argument to TAO_EC_Auto_Command.
   * TAO_EC_Auto_Command<Observer_Disconnect_Command> manages
   * observer connection to the Event Channel, automatically
   * disconnecting from ec in its destructor, if necessary.
   */
  class TAO_RTEvent_Serv_Export Observer_Disconnect_Command
  {
  public:
    Observer_Disconnect_Command (void);
    Observer_Disconnect_Command (RtecEventChannelAdmin::Observer_Handle handle,
                                 RtecEventChannelAdmin::EventChannel_ptr ec);

    Observer_Disconnect_Command (const Observer_Disconnect_Command &rhs);
    Observer_Disconnect_Command & operator= (const Observer_Disconnect_Command & rhs);

    void execute (void);

  private:

    RtecEventChannelAdmin::Observer_Handle handle_;
    RtecEventChannelAdmin::EventChannel_var ec_;
  };

private:
  /// The NIC used to subscribe for multicast traffic.
  ACE_TCHAR *net_if_;

  typedef struct {
    ACE_INET_Addr mcast_addr;
    ACE_SOCK_Dgram_Mcast* dgram;
  } Subscription;
  typedef ACE_Array_Base<Subscription> Subscriptions;

  /// List of multicast addresses we subscribe to and dgrams we use.
  /*
   * We use a dedicated socket for each multicast subscription.  The
   * reason: we assume the underlying software, i.e., ACE, binds each
   * socket used to receive multicast to the multicast group (mcast addr
   * + port) to avoid receiving promiscuous traffic, in which case it is
   * not possible to subscribe to more than one mcast address on the same
   * socket.
   *
   * Performance.  We use array to store subscriptions (mcast addr / dgram
   * pairs).  If performance is not adequate, we should look into
   * using a hash map, keyed on file descriptors, instead.  When there
   * are many subscriptions, handle_input() is likely to be more
   * efficient with a hash lookup than an array iteration for locating a
   * target dgram.  Difference in costs of subscripton changes between
   * hash map and array would need to be looked at as well, although
   * it is probably highly dependent on the pattern of changes.
   */
  Subscriptions subscriptions_;

  /// We callback to this object when a message arrives.
  /*
   * We can keep a raw pointer to the receiver (even though it may
   * be a refcounted object) because receiver guarantees
   * to notify us (by calling shutdown ()) before going away.
   *
   * We have to use raw pointer instead of a refcounting mechanism
   * here to avoid a circular refcounting dependency between
   * receiver and handler.
   */
  TAO_ECG_Dgram_Handler * receiver_;

  /// SOCKbuf size
  CORBA::ULong recvbuf_size_;

  /// Event Channel Observer.  Detects changes in EC consumer subscriptions.
  /// ORDER DEPENDENCY: this member should be declared before
  /// <auto_observer_disconnect_>.
  PortableServer::Servant_var<Observer> observer_;

  /// Manages connection of our observer to the Event Channel.
  /// ORDER DEPENDENCY: this member should be declared AFTER <observer_>.
  TAO_EC_Auto_Command<Observer_Disconnect_Command> auto_observer_disconnect_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "orbsvcs/Event/ECG_Mcast_EH.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_ECG_Mcast_EH_H */
