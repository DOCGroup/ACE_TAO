// $Id$

#include "orbsvcs/Event/ECG_Mcast_EH.h"
#include "orbsvcs/Event/EC_Gateway_UDP.h"
#include "orbsvcs/Event_Service_Constants.h"

#include "ace/Reactor.h"

#if !defined(__ACE_INLINE__)
#include "ECG_Mcast_EH.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, ECG_UDP_EH, "$Id$")

TAO_ECG_Mcast_EH::TAO_ECG_Mcast_EH (TAO_ECG_UDP_Receiver *recv,
                                    const ACE_TCHAR *net_if)
  : net_if_ (net_if?ACE_OS::strdup (net_if):0)
  , receiver_ (recv)
  , observer_ (this)
{
}

TAO_ECG_Mcast_EH::~TAO_ECG_Mcast_EH (void)
{
  ACE_OS::free (this->net_if_);
}

// @@ TODO Why have a return code *and* exceptions?  Only one would do!

int
TAO_ECG_Mcast_EH::open (RtecEventChannelAdmin::EventChannel_ptr ec
                        TAO_ENV_ARG_DECL)
{
  // @@ TODO Think about the exception safety (or lack thereof) of
  // this code, what if the following operations fail?

  this->ec_ = RtecEventChannelAdmin::EventChannel::_duplicate (ec);

  // @@ TODO This activation should be configured by the application,
  // it is too much policy to use _this().
  // @@ TODO Using a full instance instead of a pointer makes memory
  // management complex.  The observer_ object should derive from
  // PortableServer::RefCountServantBase and use the POA to control
  // its lifetime.

  RtecEventChannelAdmin::Observer_var obs =
    this->observer_._this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_TRY
    {
      this->handle_ =
        this->ec_->append_observer (obs.in () TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }
  ACE_CATCH(CORBA::SystemException, ex)
    {
      // @@ TODO This code is tedious and error prone, plus its
      // exceptions should be ignored (no way to recover from them),
      // we should encapsulate it in a Deactivator.

      PortableServer::POA_var poa =
        this->observer_._default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      PortableServer::ObjectId_var id =
        poa->servant_to_id (&this->observer_ TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      poa->deactivate_object (id.in () TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
TAO_ECG_Mcast_EH::close (TAO_ENV_SINGLE_ARG_DECL)
{
  if (this->handle_ == 0)
    return 0;

  RtecEventChannelAdmin::Observer_Handle h = this->handle_;
  this->handle_ = 0;
  this->ec_->remove_observer (h TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // @@ TODO If the first operation raises an exception then the
  // second one never executes!!!
  {
    PortableServer::POA_var poa =
      this->observer_._default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    PortableServer::ObjectId_var id =
      poa->servant_to_id (&this->observer_ TAO_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    poa->deactivate_object (id.in () TAO_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
  }

  // Ignore the result, the handler could have been removed by a call
  // to update_consumer() or something similar.
  (void) this->reactor ()->remove_handler (this,
                                           ACE_Event_Handler::READ_MASK);

  size_t sockets_size = this->sockets_.size ();
  for (size_t i = 0; i != sockets_size; ++i)
    {
      // Ignore any errors that we may have when closing the socket,
      // there is nothing we can do about them at this point....
      (void) this->sockets_[i]->close();
      delete this->sockets_[i];
    }
  this->sockets_.size (0);

  // Once all the sockets are closed they no longer are subscribed for
  // anything
  this->subscriptions_.unbind_all ();

  return 0;
}

int
TAO_ECG_Mcast_EH::handle_input (ACE_HANDLE fd)
{
  // @@ TODO How expensive is this loop?  Should we use a more
  //    efficient data structure than an Array?  For example a RB tree
  //    or a hash map?
  size_t sockets_size = this->sockets_.size ();
  for (size_t i = 0; i != sockets_size; ++i)
    {
      ACE_SOCK_Dgram_Mcast *socket = this->sockets_[i];
      if (socket->get_handle () == fd)
        {
          return this->receiver_->handle_input (*socket);
        }
    }
  return -1;
}

void
TAO_ECG_Mcast_EH::compute_required_subscriptions (
    const RtecEventChannelAdmin::ConsumerQOS& sub,
    Address_Set& multicast_addresses
    TAO_ENV_ARG_DECL)
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
      this->receiver_->get_addr (header, addr TAO_ENV_ARG_PARAMETER);
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

      char buf[256];
      multicast_group.addr_to_string (buf, sizeof buf, 1);
      ACE_SOCK_Dgram_Mcast *socket = (*j).int_id_;
      // Ignore errors, there is no appropriate policy to handle them
      // at this layer.
      // @@ TODO Consider if we should raise exceptions.
      socket->unsubscribe (multicast_group, this->net_if_);
      // Increment and then remove, this is a safe way to remove the
      // element without invalidating the iterator.
      ++j;
      this->subscriptions_.unbind (multicast_group);
    }
    return 0;
}

int
TAO_ECG_Mcast_EH::subscribe_to_existing_socket (
        ACE_INET_Addr& multicast_group)
{
  size_t sockets_size = this->sockets_.size ();
  int result = -1;
  for (size_t i = 0; i != sockets_size; ++i)
    {
      ACE_SOCK_Dgram_Mcast *socket = this->sockets_[i];
      result =
        socket->subscribe (multicast_group, 1, this->net_if_);
      if (result == 0)
        {
          // Add the subscription to the subscription list
          (void) this->subscriptions_.bind (multicast_group, socket);
          break;
        }
      /* assert(result == -1) */
      if (errno == ENOBUFS || errno == ETOOMANYREFS)
        {
          // The socket is full, try with the next one...
          continue;
        }
      // @@ TODO: There was an error, but the problem is not with
      // too many subscriptions... what we need to do is close the
      // socket?  Or skip the subscription?
    }
  return result;
}

void
TAO_ECG_Mcast_EH::subscribe_to_new_socket(
        ACE_INET_Addr& multicast_group)
{
  ACE_SOCK_Dgram_Mcast *socket;
  ACE_NEW (socket, ACE_SOCK_Dgram_Mcast);
  size_t sockets_size = this->sockets_.size ();

  if (this->sockets_.max_size () == sockets_size)
    {
      this->sockets_.max_size (sockets_size + 1);
      this->sockets_.size (sockets_size + 1);
    }
  this->sockets_[sockets_size] = socket;
  socket->subscribe (multicast_group, 1, this->net_if_);

  // @@ TODO: we have no way to handle this failure....
  (void) this->subscriptions_.bind (multicast_group, socket);

  // @@ TODO: we have no way to handle this failure....
  (void) this->reactor ()->register_handler (socket->get_handle (),
                                             this,
                                             ACE_Event_Handler::READ_MASK);
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

      char buf[256];
      multicast_group.addr_to_string (buf, sizeof buf, 1);

      // Try to find a socket that has space for another multicast
      // group.
      int successful_subscription =
        this->subscribe_to_existing_socket (multicast_group);

      if (successful_subscription == -1)
        {
          // We try all the sockets and all of them appear to be full,
          // we need to open a new one and add it to the Array...
          this->subscribe_to_new_socket(multicast_group);
        }
    }
}

void
TAO_ECG_Mcast_EH::update_consumer (
    const RtecEventChannelAdmin::ConsumerQOS& sub
    TAO_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ TODO This function turned out to be too long, we need to break
  //    it up in pieces, maybe 3 or 4 private member functions....

  // 1) Figure out the list of multicast groups that we need to
  //    subscribe to

  Address_Set multicast_addresses;

  this->compute_required_subscriptions (sub, multicast_addresses TAO_ENV_ARG_PARAMETER);
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
                                   TAO_ENV_ARG_DECL_NOT_USED)
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
    TAO_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->eh_->update_consumer (sub TAO_ENV_ARG_PARAMETER);
}

void
TAO_ECG_Mcast_EH::Observer::update_supplier (
    const RtecEventChannelAdmin::SupplierQOS& pub
    TAO_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->eh_->update_supplier (pub TAO_ENV_ARG_PARAMETER);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Manager<ACE_INET_Addr,ACE_SOCK_Dgram_Mcast*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_INET_Addr, ACE_SOCK_Dgram_Mcast *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_INET_Addr, ACE_SOCK_Dgram_Mcast *>;
template class ACE_Hash<ACE_INET_Addr>;
template class ACE_Equal_To<ACE_INET_Addr>;
template class ACE_Unbounded_Set<ACE_INET_Addr>;
template class ACE_Array_Base<ACE_SOCK_Dgram_Mcast *>;
template class ACE_Unbounded_Set_Iterator<ACE_INET_Addr>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_INET_Addr, ACE_SOCK_Dgram_Mcast*, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_INET_Addr,ACE_SOCK_Dgram_Mcast*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_INET_Addr, ACE_SOCK_Dgram_Mcast*, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_INET_Addr,ACE_SOCK_Dgram_Mcast*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_INET_Addr, ACE_SOCK_Dgram_Mcast*, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Manager<ACE_INET_Addr,ACE_SOCK_Dgram_Mcast*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_INET_Addr, ACE_SOCK_Dgram_Mcast *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_INET_Addr, ACE_SOCK_Dgram_Mcast *>
#pragma instantiate ACE_Hash<ACE_INET_Addr>
#pragma instantiate ACE_Equal_To<ACE_INET_Addr>
#pragma instantiate ACE_Unbounded_Set<ACE_INET_Addr>
#pragma instantiate ACE_Array_Base<ACE_SOCK_Dgram_Mcast *>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_INET_Addr>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_INET_Addr, ACE_SOCK_Dgram_Mcast *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_INET_Addr,ACE_SOCK_Dgram_Mcast*,ACE_Null_Mutex>;
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_INET_Addr, ACE_SOCK_Dgram_Mcast*, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_INET_Addr,ACE_SOCK_Dgram_Mcast*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_INET_Addr, ACE_SOCK_Dgram_Mcast*, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
