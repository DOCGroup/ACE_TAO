// $Id$

#include "orbsvcs/Event/ECG_Mcast_EH.h"

#if !defined(__ACE_INLINE__)
#include "ECG_Mcast_EH.i"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/Event/EC_Gateway_UDP.h"
#include "orbsvcs/Event_Service_Constants.h"

#include "ace/Reactor.h"
#include "ace/Unbounded_Set.h"

ACE_RCSID(Event, ECG_Mcast_EH, "$Id$")

TAO_ECG_Mcast_Socket::TAO_ECG_Mcast_Socket (ACE_SOCK_Dgram_Mcast::options opts,
                                            ACE_Lock *lock)
  : ACE_SOCK_Dgram_Mcast (opts)
  , TAO_Synch_Refcountable (lock, 1)
{
  if (!this->refcount_lock_)
    ACE_NEW_NORETURN (this->refcount_lock_, ACE_Lock_Adapter<TAO_SYNCH_MUTEX>);
  ACE_ASSERT (TAO_Synch_Refcountable::refcount () == 1);
}

TAO_ECG_Mcast_Socket::~TAO_ECG_Mcast_Socket (void)
{
}

int
TAO_ECG_Mcast_Socket::decrement (void)
{
  int count = TAO_Synch_Refcountable::decrement ();
  ACE_ASSERT (count >= 0);
  if (count == 0)
    {
      this->close ();
      delete this;
    }
  return count;
}

// ****************************************************************

TAO_ECG_Mcast_EH::TAO_ECG_Mcast_EH (TAO_ECG_UDP_Receiver *recv,
                                    const ACE_TCHAR *net_if,
                                    ACE_Lock *lock)
  : net_if_ (ACE::strnew (net_if))
  , lock_ (lock)
  , receiver_ (recv)
  , observer_ (this)
  , handle_ (0)
{
  if (!this->lock_)
    ACE_NEW_NORETURN (this->lock_,ACE_Lock_Adapter <TAO_SYNCH_MUTEX>);
}

TAO_ECG_Mcast_EH::~TAO_ECG_Mcast_EH (void)
{
  ACE::strdelete (this->net_if_);
  delete this->lock_;
}

// @@ TODO Why have a return code *and* exceptions?  Only one would do!

int
TAO_ECG_Mcast_EH::open (RtecEventChannelAdmin::EventChannel_ptr ec
                        ACE_ENV_ARG_DECL)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, -1));

  // Have we already been opened?
  if (this->handle_)
    return -1;

  // @@ TODO Think about the exception safety (or lack thereof) of
  // this code, what if the following operations fail?

  this->ec_ = RtecEventChannelAdmin::EventChannel::_duplicate (ec);

  // @@ TODO This activation should be configured by the application,
  // it is too much policy to use _this().

  RtecEventChannelAdmin::Observer_var obs =
    this->observer_._this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_TRY
    {
      this->handle_ =
        this->ec_->append_observer (obs.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }
  ACE_CATCH(CORBA::SystemException, ex)
    {
      // @@ TODO This code is tedious and error prone, plus its
      // exceptions should be ignored (no way to recover from them),
      // we should encapsulate it in a Deactivator.

      PortableServer::POA_var poa =
        this->observer_._default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      PortableServer::ObjectId_var id =
        poa->servant_to_id (&this->observer_ ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
TAO_ECG_Mcast_EH::close (ACE_ENV_SINGLE_ARG_DECL)
{
  // handle_ is the Observer_Handle.
  if (this->handle_ == 0)
    return 0;

  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, -1));

  // Double check they we're open.
  if (this->handle_ == 0)
    return 0;

  Handle_Iterator h_iter = this->handles_.begin ();
  while (h_iter != this->handles_.end ())
    {
      TAO_ECG_Mcast_Socket *socket = (*h_iter).int_id_;
      ACE_HANDLE handle = (*h_iter).ext_id_;
      if (this->reactor ()->remove_handler (handle,
                                            ACE_Event_Handler::ALL_EVENTS_MASK
                                            | ACE_Event_Handler::DONT_CALL) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("failed to remove mcast handler from ")
                    ACE_TEXT ("reactor.\n")));
      socket->decrement ();
      ++h_iter;
    }

  // Once all the sockets are closed they no longer are subscribed for anything
  this->subscriptions_.unbind_all ();
  this->handles_.unbind_all ();

  RtecEventChannelAdmin::Observer_Handle h = this->handle_;
  this->handle_ = 0;
  this->ec_->remove_observer (h ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // @@ TODO If the first operation raises an exception then the
  // second one never executes!!!
  {
    PortableServer::POA_var poa =
      this->observer_._default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    PortableServer::ObjectId_var id =
      poa->servant_to_id (&this->observer_ ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
  }

  return 0;
}

TAO_ECG_Mcast_Socket*
TAO_ECG_Mcast_EH::find (ACE_HANDLE fd)
{
  TAO_ECG_Mcast_Socket *socket = 0;
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0));
  if (this->handles_.find (fd, socket) == 0)
    socket->increment ();
  return socket;
}

int
TAO_ECG_Mcast_EH::handle_input (ACE_HANDLE fd)
{
  TAO_ECG_Mcast_Socket *socket = this->find (fd);
  if (socket)
    {
      if (this->receiver_->handle_input (*socket) == -1)
        {
          // Since we are working with UDP multicast sockets here, there aren't
          // a whole lot of errors we can get that will make us want to close
          // the close the socket, but if we return -1 to the reactor, that's
          // exactly what will happend.  Or, more precisely, the reactor will
          // remove us from it's handle set and call back our handle_close()
          // method.  Since that's not what we generally want, we just log
          // the message and always return 0.
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("handle_input() returned an error (%p)\n")));
        }

      // No matter what happened, we need to decrement the refcount that
      // find() incremented.
      socket->decrement ();
    }

  return 0;
}

void
TAO_ECG_Mcast_EH::compute_required_subscriptions (
    const RtecEventChannelAdmin::ConsumerQOS& sub,
    Address_Set& multicast_addresses
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong count = sub.dependencies.length ();
  for (CORBA::ULong i = 0; i != count; ++i)
    {
      const RtecEventComm::EventHeader& header =
                           sub.dependencies[i].event.header;
      if (0 <= header.type && header.type < ACE_ES_EVENT_UNDEFINED)
        {
          continue;
        }
      RtecUDPAdmin::UDP_Addr addr;

      // For the time being we are exception neutral, i.e., if an
      // exception is raised at this point we simply propagate it.
      // Notice that we haven't performed any changes to the state of
      // the current class, so this is the easiest approach.  The
      // alternatives are:
      // + Ignore the exception and continue with the operation: risky
      //   because we don't know if the exception indicated a
      //   temporary or permanent condition, and if it is the former
      //   we may loose an important multicast group, with no
      //   opportunity to recover it.
      // + Close the MCast Event Handler completely, too much policy
      //   for this level.
      this->receiver_->get_addr (header, addr ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_INET_Addr inet_addr (addr.port, addr.ipaddr);
      // Ignore errors, if the element is in the set we simply ignore
      // the problem...
      (void) multicast_addresses.insert (inet_addr);
    }
}

int
TAO_ECG_Mcast_EH::delete_unwanted_subscriptions (
        Address_Set& multicast_addresses)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0));

  Subscriptions_Iterator j = this->subscriptions_.begin ();
  while (j != this->subscriptions_.end ())
    {
      ACE_INET_Addr multicast_group = (*j).ext_id_;
      if (multicast_addresses.find (multicast_group))
        {
          // Remove from the list of subscriptions that should be
          // added, because it is already there...
          (void) multicast_addresses.remove (multicast_group);
          ++j;
          continue;
        }

      this->subscriptions_.unbind (multicast_group);
      TAO_ECG_Mcast_Socket *socket = (*j).int_id_;
      // Ignore errors, there is no appropriate policy to handle them
      // at this layer.
      // @@ TODO Consider if we should raise exceptions.
      ACE_HANDLE handle = socket->get_handle ();
      socket->leave (multicast_group, this->net_if_);
      if (this->reactor ()->remove_handler (handle,
                                            ACE_Event_Handler::ALL_EVENTS_MASK
                                            | ACE_Event_Handler::DONT_CALL)
          == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("failed to remove mcast handler from ")
                    ACE_TEXT ("reactor.\n")));

      this->handles_.unbind (handle);
      // We're done with it, so decrement the refcount so it can get
      // deleted (either now or when it finishes an upcall.
      socket->decrement ();

      // Increment and then remove, this is a safe way to remove the
      // element without invalidating the iterator.
      ++j;
    }
    return 0;
}

int
TAO_ECG_Mcast_EH::subscribe_to_existing_socket (
        ACE_INET_Addr& multicast_group)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, -1));
  int result = 0;

  Handle_Iterator h_iter = this->handles_.begin ();
  while (h_iter != this->handles_.end ())
    {
      TAO_ECG_Mcast_Socket *socket = (*h_iter).int_id_;
      result = socket->join (multicast_group, 1, this->net_if_);
      if (result != -1)
        {
          // Add the subscription to the subscription list
          (void) this->subscriptions_.bind (multicast_group, socket);
          break;
        }
      ++h_iter;
    }
  return result;
}

void
TAO_ECG_Mcast_EH::subscribe_to_new_socket(
        ACE_INET_Addr& multicast_group)
{
  TAO_ECG_Mcast_Socket *socket;
  // Always bind the address, which means that we will only get traffic
  // that was sent to the "single" group we joined and won't get any
  // cross-talk.
  ACE_NEW (socket, TAO_ECG_Mcast_Socket(ACE_SOCK_Dgram_Mcast::OPT_BINDADDR_YES));

  socket->join (multicast_group, 1, this->net_if_);
  ACE_HANDLE handle = socket->get_handle ();

  // We only need the lock long enough to update the lists.
  ACE_MT (ACE_GUARD (ACE_Lock, ace_mon, *this->lock_));
  this->handles_.bind (handle, socket);
  // @@ TODO: we have no way to handle this failure....
  (void) this->subscriptions_.bind (multicast_group, socket);
  ACE_MT (ace_mon.release ());

  // @@ TODO: we have no way to handle this failure....
  if (this->reactor ()->register_handler (handle,
                                          this,
                                          ACE_Event_Handler::READ_MASK) == -1)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("reg handle failed\n")));
}

void
TAO_ECG_Mcast_EH::add_new_subscriptions (
    Address_Set& multicast_addresses)
{
  typedef ACE_Unbounded_Set_Iterator<ACE_INET_Addr> Address_Iterator;
  for (Address_Iterator k = multicast_addresses.begin ();
       k != multicast_addresses.end ();
       ++k)
    {
      // This is the multicast group we want to add to...
      ACE_INET_Addr multicast_group = *k;

      // Try to find a socket that has space for another multicast
      // group.
      int successful_subscription = -1;

      // It only makes sense to try subscribing to an existing
      // socket if the OS supports it.  Otherwise it will always
      // fail since we bind the address by default and you can't
      // join a multicast group that's different from the one
      // bound to the socket.
#if defined (ACE_LACKS_PERFECT_MULTICAST_FILTERING) \
    && (ACE_LACKS_PERFECT_MULTICAST_FILTERING == 0)
      successful_subscription =
        this->subscribe_to_existing_socket (multicast_group);
# endif /* ACE_LACKS_PERFECT_MULTICAST_FILTERING == 0 */

      if (successful_subscription == -1)
        {
          // We try all the sockets and all of them appear to be full,
          // we need to open a new one and add it to the Array...
          this->subscribe_to_new_socket (multicast_group);
        }
    }
}

void
TAO_ECG_Mcast_EH::update_consumer (
    const RtecEventChannelAdmin::ConsumerQOS& sub
    ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Make sure we are open for business.
  if (!this->handle_)
    return;

  // 1) Figure out the list of multicast groups that we need to
  //    subscribe to

  Address_Set multicast_addresses;

  this->compute_required_subscriptions (sub,
                                        multicast_addresses
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // 2) To conserve OS and network resources we first unsubscribe from
  //    the multicast groups no longer wanted.  This is done by
  //    iterating over the current set of multicast groups and
  //    removing those not present in the "multicast_addresses" set.

  this->delete_unwanted_subscriptions (multicast_addresses);

  // 3) After the loop above the "multicast_addresses" set contains
  //    only the new subscriptions... go ahead and add them to the
  //    sockets, opening sockets if needed.

  this->add_new_subscriptions (multicast_addresses);
}

void
TAO_ECG_Mcast_EH::update_supplier (const RtecEventChannelAdmin::SupplierQOS&
                                   ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do nothing
}

// ****************************************************************

TAO_ECG_Mcast_EH::Observer::Observer (TAO_ECG_Mcast_EH* eh)
  :  eh_ (eh)
{
}

void
TAO_ECG_Mcast_EH::Observer::update_consumer (
    const RtecEventChannelAdmin::ConsumerQOS& sub
    ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->eh_->update_consumer (sub ACE_ENV_ARG_PARAMETER);
}

void
TAO_ECG_Mcast_EH::Observer::update_supplier (
    const RtecEventChannelAdmin::SupplierQOS& pub
    ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->eh_->update_supplier (pub ACE_ENV_ARG_PARAMETER);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Manager<ACE_INET_Addr, TAO_ECG_Mcast_Socket*, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_INET_Addr, TAO_ECG_Mcast_Socket *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_INET_Addr, TAO_ECG_Mcast_Socket *>;
template class ACE_Hash<ACE_INET_Addr>;
template class ACE_Node<ACE_INET_Addr>;
template class ACE_Equal_To<ACE_INET_Addr>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_INET_Addr, TAO_ECG_Mcast_Socket *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_INET_Addr, TAO_ECG_Mcast_Socket *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_INET_Addr, TAO_ECG_Mcast_Socket *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_INET_Addr, TAO_ECG_Mcast_Socket *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_INET_Addr, TAO_ECG_Mcast_Socket *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Manager<ACE_HANDLE, TAO_ECG_Mcast_Socket *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_HANDLE, TAO_ECG_Mcast_Socket *, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_HANDLE, TAO_ECG_Mcast_Socket * >;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_HANDLE, TAO_ECG_Mcast_Socket *, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_HANDLE, TAO_ECG_Mcast_Socket *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_HANDLE, TAO_ECG_Mcast_Socket *, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_HANDLE, TAO_ECG_Mcast_Socket *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_HANDLE, TAO_ECG_Mcast_Socket*, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>;

template class ACE_Unbounded_Set<ACE_INET_Addr>;
template class ACE_Unbounded_Set_Iterator<ACE_INET_Addr>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Manager<ACE_INET_Addr, TAO_ECG_Mcast_Socket *,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_INET_Addr, TAO_ECG_Mcast_Socket *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_INET_Addr, TAO_ECG_Mcast_Socket *>
#pragma instantiate ACE_Hash<ACE_INET_Addr>
#pragma instantiate ACE_Node<ACE_INET_Addr>
#pragma instantiate ACE_Equal_To<ACE_INET_Addr>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_INET_Addr, TAO_ECG_Mcast_Socket *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_INET_Addr, TAO_ECG_Mcast_Socket *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_INET_Addr, TAO_ECG_Mcast_Socket *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_INET_Addr, TAO_ECG_Mcast_Socket *,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_INET_Addr, TAO_ECG_Mcast_Socket *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Manager<ACE_HANDLE, TAO_ECG_Mcast_Socket *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_HANDLE, TAO_ECG_Mcast_Socket *, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_HANDLE, TAO_ECG_Mcast_Socket * >
#pragma instantiate ACE_Hash<ACE_HANDLE>
#pragma instantiate ACE_Node<ACE_HANDLE>
#pragma instantiate ACE_Equal_To<ACE_HANDLE>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_HANDLE, TAO_ECG_Mcast_Socket *, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_HANDLE, TAO_ECG_Mcast_Socket *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_HANDLE, TAO_Mcast_Socket*, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_HANDLE, TAO_ECG_Mcast_Socket *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_HANDLE, TAO_ECG_Mcast_Socket *, ACE_Hash<ACE_HANDLE>, ACE_Equal_To<ACE_HANDLE>, ACE_Null_Mutex>

#pragma instantiate ACE_Unbounded_Set<ACE_INET_Addr>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_INET_Addr>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
