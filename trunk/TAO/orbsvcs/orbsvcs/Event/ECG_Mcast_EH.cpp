// $Id$

#include "orbsvcs/Event/ECG_Mcast_EH.h"
#include "orbsvcs/Event/ECG_UDP_Receiver.h"
#include "orbsvcs/Event_Service_Constants.h"

#include "ace/Reactor.h"

#if !defined(__ACE_INLINE__)
#include "ECG_Mcast_EH.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, ECG_Mcast_EH, "$Id$")

TAO_ECG_Mcast_EH::TAO_ECG_Mcast_EH (TAO_ECG_Dgram_Handler *recv,
                                    const ACE_TCHAR *net_if)
  : net_if_ (net_if?ACE_OS::strdup (net_if):0)
  , subscriptions_ ()
  , receiver_ (recv)
  , observer_ ()
  , auto_observer_disconnect_ ()
{
    ACE_ASSERT (this->receiver_);
}

TAO_ECG_Mcast_EH::~TAO_ECG_Mcast_EH (void)
{
  ACE_OS::free (this->net_if_);
}

void
TAO_ECG_Mcast_EH::open (RtecEventChannelAdmin::EventChannel_ptr ec
                        ACE_ENV_ARG_DECL)
{
  if (!this->receiver_)
    {
      // We are shut down.
      ACE_THROW (CORBA::INTERNAL());
    }

  if (CORBA::is_nil (ec))
    {
      ACE_ERROR ((LM_ERROR,  "TAO_ECG_Mcast_EH::open(): "
                             "nil ec argument"));
      ACE_THROW (CORBA::INTERNAL ());
    }

  // Create and activate Event Channel Observer.
  ACE_NEW (this->observer_,
           Observer (this));

  if (!this->observer_.in ())
    {
      ACE_THROW (CORBA::NO_MEMORY ());
    }

  TAO_EC_Object_Deactivator observer_deactivator;
  RtecEventChannelAdmin::Observer_var observer_ref;
  PortableServer::POA_var poa =
    this->observer_->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  activate (observer_ref,
            poa.in (),
            this->observer_.in (),
            observer_deactivator
            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventChannelAdmin::Observer_Handle handle =
    ec->append_observer (observer_ref.in ()
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->observer_->set_deactivator (observer_deactivator);
  this->auto_observer_disconnect_.set_command
    (Observer_Disconnect_Command (handle, ec));
}

int
TAO_ECG_Mcast_EH::shutdown (void)
{
  // Already shut down.
  if (!this->receiver_)
    return -1;

  // Disconnect Observer from EC.
  this->auto_observer_disconnect_.execute ();

  // Shutdown the observer.
  if (this->observer_.in ())
    {
      this->observer_->shutdown ();
      this->observer_ = 0;
    }

  // Indicates that we are in a shutdown state.
  this->receiver_ = 0;

  // Deregister from reactor, close and clean up sockets.
  size_t subscriptions_size = this->subscriptions_.size ();
  for (size_t i = 0; i != subscriptions_size; ++i)
    {
      (void) this->reactor ()->remove_handler (
                               this->subscriptions_[i].dgram->get_handle (),
                               ACE_Event_Handler::READ_MASK);
      (void) this->subscriptions_[i].dgram->close();
      delete this->subscriptions_[i].dgram;
    }
  this->subscriptions_.size (0);

  return 0;
}

int
TAO_ECG_Mcast_EH::handle_input (ACE_HANDLE fd)
{
  size_t subscriptions_size = this->subscriptions_.size ();
  for (size_t i = 0; i != subscriptions_size; ++i)
    {
      ACE_SOCK_Dgram_Mcast *socket = this->subscriptions_[i].dgram;
      if (socket->get_handle () == fd)
        {
          return this->receiver_->handle_input (*socket);
        }
    }
  return -1;
}

void
TAO_ECG_Mcast_EH::update_consumer (
         const RtecEventChannelAdmin::ConsumerQOS& sub
    ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  Address_Set multicast_addresses;

  this->compute_required_subscriptions (sub,
                                        multicast_addresses
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->delete_unwanted_subscriptions (multicast_addresses);

  this->add_new_subscriptions (multicast_addresses);
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
      if (0 < header.type && header.type < ACE_ES_EVENT_UNDEFINED)
        {
          continue;
        }
      RtecUDPAdmin::UDP_Addr addr;

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
  for (size_t i = 0; i < this->subscriptions_.size (); ++i)
    {
      ACE_INET_Addr multicast_group = this->subscriptions_[i].mcast_addr;
      if (multicast_addresses.find (multicast_group))
        {
          // Remove from the list of subscriptions to be added,
          // because we already subscribe to it...
          (void) multicast_addresses.remove (multicast_group);
          continue;
        }

      // This subscription is no longer needed - remove from reactor,
      // close and delete the socket.
      ACE_SOCK_Dgram_Mcast *socket = this->subscriptions_[i].dgram;
      (void) this->reactor ()->remove_handler (socket->get_handle (),
                                               ACE_Event_Handler::READ_MASK);
      (void) socket->close();
      delete socket;
      // Move the deleted subscription out of the <subscriptions_>
      // array by moving the last subscription in array into its place.
      this->subscriptions_[i] =
        this->subscriptions_[this->subscriptions_.size () - 1];
      this->subscriptions_.size (this->subscriptions_.size () - 1);
      --i;
    }

    return 0;
}

void
TAO_ECG_Mcast_EH::add_new_subscriptions (Address_Set& multicast_addresses)
{
  typedef ACE_Unbounded_Set_Iterator<ACE_INET_Addr> Address_Iterator;
  for (Address_Iterator k = multicast_addresses.begin ();
       k != multicast_addresses.end ();
       ++k)
    {
      Subscription new_subscription;
      new_subscription.mcast_addr = *k;
      ACE_NEW (new_subscription.dgram, ACE_SOCK_Dgram_Mcast);

      size_t subscriptions_size = this->subscriptions_.size ();
      this->subscriptions_.size (subscriptions_size + 1);
      this->subscriptions_[subscriptions_size] = new_subscription;

      ACE_SOCK_Dgram_Mcast *socket = new_subscription.dgram;
      socket->enable (ACE_NONBLOCK);
      socket->subscribe (new_subscription.mcast_addr, 1, this->net_if_);
      (void) this->reactor ()->register_handler (
                                         socket->get_handle (),
                                         this,
                                         ACE_Event_Handler::READ_MASK);
    }
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
  if (this->eh_)
    this->eh_->update_consumer (sub ACE_ENV_ARG_PARAMETER);
}

void
TAO_ECG_Mcast_EH::Observer::update_supplier (
    const RtecEventChannelAdmin::SupplierQOS&
    ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_ECG_Mcast_EH::Observer::shutdown (void)
{
  this->eh_ = 0;
  this->deactivator_.deactivate ();
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Manager<ACE_INET_Addr,ACE_SOCK_Dgram_Mcast*,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_INET_Addr, ACE_SOCK_Dgram_Mcast *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_INET_Addr, ACE_SOCK_Dgram_Mcast *>;
template class ACE_Hash<ACE_INET_Addr>;
template class ACE_Node<ACE_INET_Addr>;
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
#pragma instantiate ACE_Node<ACE_INET_Addr>
#pragma instantiate ACE_Equal_To<ACE_INET_Addr>
#pragma instantiate ACE_Unbounded_Set<ACE_INET_Addr>
#pragma instantiate ACE_Array_Base<ACE_SOCK_Dgram_Mcast *>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_INET_Addr>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_INET_Addr, ACE_SOCK_Dgram_Mcast *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_INET_Addr,ACE_SOCK_Dgram_Mcast*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_INET_Addr, ACE_SOCK_Dgram_Mcast*, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_INET_Addr,ACE_SOCK_Dgram_Mcast*,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_INET_Addr, ACE_SOCK_Dgram_Mcast*, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
