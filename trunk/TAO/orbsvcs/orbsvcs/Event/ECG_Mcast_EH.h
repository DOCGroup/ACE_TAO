/* -*- C++ -*- */
/**
 * @file ECG_Mcast_EH.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 * @author Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
 * @author and Don Hinton <dhinton@ieee.org>
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 *
 */
#ifndef TAO_ECG_MCAST_EH_H
#define TAO_ECG_MCAST_EH_H
#include "ace/pre.h"

#include "orbsvcs/Event/event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "ace/Event_Handler.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Array_Base.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/Synch.h"
#include "ace/Atomic_Op.h"
#include "tao/Synch_Refcountable.h"
//#include "tao/Utils/Servant_Var.h"

/**
 * @class TAO_ECG_Mcast_Socket
 *
 * @brief Refcounted wrapper for ACE_SOCK_Dgram_Mcast
 *
 * This class is a simple refcounted wrapper around ACE_SOCK_Dgram_Mcast that
 * makes takes care of deleting itself if the refcount drops to zero.
 */
class TAO_RTEvent_Export TAO_ECG_Mcast_Socket : 
  public ACE_SOCK_Dgram_Mcast, private TAO_Synch_Refcountable
{
public:
  /// Default Constructor
  TAO_ECG_Mcast_Socket (ACE_SOCK_Dgram_Mcast::options opts =
                          ACE_SOCK_Dgram_Mcast::DEFOPTS,
                        ACE_Lock *lock = 0);

  int increment (void);
  int decrement (void);
  int refcount (void) const;

private:
  /// We need a friend so that we have a private dtor>
  friend class ACE_SOCK_Dgram_Mcast;

  /// Destructor.  Force it on the Heap, so we can call delete this in
  /// decrement().
  ~TAO_ECG_Mcast_Socket (void);
};

class TAO_ECG_UDP_Receiver;

/**
 * @class TAO_ECG_Mcast_EH
 *
 * @brief Event Handler for UDP messages.
 *
 * This object receives callbacks from the Reactor when data is
 * available on the mcast socket, it forwards to the UDP_Receive
 * gateway which reads the events and transform it into an event.
 */
class TAO_RTEvent_Export TAO_ECG_Mcast_EH : public ACE_Event_Handler
{
public:
  /**
   * Constructor, the messages received by this EH are forwarded to
   * the <recv>.
   * It is possible to select the NIC where the multicast messages are
   * expected using <net_if>
   */
  TAO_ECG_Mcast_EH (TAO_ECG_UDP_Receiver *recv,
                    const ACE_TCHAR *net_if = 0,
                    ACE_Lock *lock = 0);

  /// Destructor
  virtual ~TAO_ECG_Mcast_EH (void);

  /**
   * Register for changes in the EC subscription list.
   * When the subscription list becomes non-empty we join the proper
   * multicast groups (using the receiver to translate between event
   * types and mcast groups) and the class registers itself with the
   * reactor.
   */
  int open (RtecEventChannelAdmin::EventChannel_ptr ec
            ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /**
   * Remove ourselves from the event channel, unsubscribe from the
   * multicast groups, close the sockets and unsubscribe from the
   * reactor.
   */
  int close (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// Reactor callbacks
  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);

  /// The Observer methods
  void update_consumer (const RtecEventChannelAdmin::ConsumerQOS& sub
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
  void update_supplier (const RtecEventChannelAdmin::SupplierQOS& pub
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * @class Observer
   *
   * @brief Observe changes in the EC subscriptions.
   *
   * As the subscriptions on the EC change we also change the
   * mcast groups that we join.
   * We could use the TIE classes, but they don't work in all
   * compilers.
   */

  class Observer 
    : public virtual POA_RtecEventChannelAdmin::Observer
    , public virtual PortableServer::RefCountServantBase
  {
  public:
    /// We report changes in the EC subscriptions to the event
    /// handler.
    Observer (TAO_ECG_Mcast_EH* eh);

    // The Observer methods
    virtual void update_consumer (
        const RtecEventChannelAdmin::ConsumerQOS& sub
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
    virtual void update_supplier (
        const RtecEventChannelAdmin::SupplierQOS& pub
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    /// Our callback object.
    TAO_ECG_Mcast_EH *eh_;
  };

private:
  /// Find the socket mapped to a particular fd.  find() holds a lock and calls
  /// increment () on the socket, if found, and returns a pointer to it.
  TAO_ECG_Mcast_Socket *find (ACE_HANDLE fd);

  typedef ACE_Unbounded_Set<ACE_INET_Addr> Address_Set;

  /// Compute the list of multicast addresses that we need to be
  /// subscribed to, in order to receive the events described in the
  /// ConsumerQOS parameter.
  /**
   * @param sub The list of event types that our event channel
   *        consumers are interested into.
   * @param multicast_addresses Returns the list of multicast
   *        addresses that we need to subscribe to.
   * @param env Used to raise CORBA exceptions when there is no
   *        support for native C++ exceptions.
   *
   * @throw CORBA::SystemException This method needs to perform
   *        several CORBA invocations and propagates any exceptions
   *        back to the caller.
   */

  void compute_required_subscriptions (
        const RtecEventChannelAdmin::ConsumerQOS& sub,
              Address_Set& multicast_addresses
              ACE_ENV_ARG_DECL)
              ACE_THROW_SPEC ((CORBA::SystemException));

  /// Delete the list of multicast addresses that we need not
  /// subscribe to, in order to receive the events described in the
  /// ConsumerQOS parameter.
  /**
   * @param multicast_addresses Returns the list of multicast
   *        addresses that we need to subscribe to.
   */

  int delete_unwanted_subscriptions (
              Address_Set& multicast_addresses);

  /// Add the list of new multicast addresses that we need to
  /// subscribe to, in order to receive the events described in the
  /// ConsumerQOS parameter.
  /**
   * @param multicast_addresses Returns the list of multicast
   *        addresses that we need to subscribe to.
   */

  void add_new_subscriptions (
              Address_Set& multicast_addresses);

  /// Subscribe an existing socket to a multicast group.
  /**
   * @param multicast_group Returns the multicast
   *        address that we need to subscribe to.
   */
  int subscribe_to_existing_socket (ACE_INET_Addr& multicast_group);

  /// Subscribe a new socket to a multicast group.
  /**
   * @param multicast_group Returns the multicast
   *        address that we need to subscribe to.
   */
  void subscribe_to_new_socket (ACE_INET_Addr& multicast_group);

private:
  /// The NIC name used to subscribe for multicast traffic.
  ACE_TCHAR *net_if_;

  /// Define the collection used to keep the iterator
  typedef ACE_Hash_Map_Manager<ACE_INET_Addr, TAO_ECG_Mcast_Socket *, ACE_Null_Mutex>
          Subscriptions_Map;
  typedef ACE_Hash_Map_Iterator<ACE_INET_Addr, TAO_ECG_Mcast_Socket *, ACE_Null_Mutex>
          Subscriptions_Iterator;

  /// Map of multicast address to sockets, used to manage the subscription
  /// to multicast address by the update_{consumer|supplier} () methods.
  Subscriptions_Map subscriptions_;

  typedef ACE_Hash_Map_Manager<ACE_HANDLE, TAO_ECG_Mcast_Socket *, ACE_Null_Mutex>
          Handle_Map;
  typedef ACE_Hash_Map_Iterator<ACE_HANDLE, TAO_ECG_Mcast_Socket *, ACE_Null_Mutex>
          Handle_Iterator;

  /// Map of ACE_HANDLEs to sockets.  Used to find the appropriate socket in
  /// calls to handle_input ().
  Handle_Map handles_;

  /// The lock used to protect the lists.
  ACE_Lock *lock_;

  /// We callback to this object when a message arrives.
  TAO_ECG_UDP_Receiver* receiver_;

  /// This object will call us back when the subscription list
  /// changes.
  Observer observer_;

  /// Keep the handle of the observer so we can unregister later.
  RtecEventChannelAdmin::Observer_Handle handle_;

  /// The Event Channel.
  RtecEventChannelAdmin::EventChannel_var ec_;
};

#if defined(__ACE_INLINE__)
#include "ECG_Mcast_EH.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ECG_Mcast_EH_H */
