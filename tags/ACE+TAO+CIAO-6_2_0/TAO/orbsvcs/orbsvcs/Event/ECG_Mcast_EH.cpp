// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Event/ECG_Mcast_EH.h"
#include "orbsvcs/Event/ECG_UDP_Receiver.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Lifetime_Utils_T.h"
#include "ace/Unbounded_Set.h"
#include "ace/OS_NS_string.h"
#include "ace/Reactor.h"
#include "ace/os_include/os_fcntl.h"

#if !defined(__ACE_INLINE__)
#include "orbsvcs/Event/ECG_Mcast_EH.inl"
#endif /* __ACE_INLINE__ */



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ECG_Mcast_EH::TAO_ECG_Mcast_EH (TAO_ECG_Dgram_Handler *recv,
                                    const ACE_TCHAR *net_if,
                                    CORBA::ULong sz)
  : net_if_ (net_if ? ACE_OS::strdup (net_if) : 0)
  , subscriptions_ ()
  , receiver_ (recv)
  , recvbuf_size_ (sz)
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
TAO_ECG_Mcast_EH::open (RtecEventChannelAdmin::EventChannel_ptr ec)
{
  if (!this->receiver_)
    {
      // We are shut down.
      throw CORBA::INTERNAL();
    }

  if (CORBA::is_nil (ec))
    {
      ORBSVCS_ERROR ((LM_ERROR,  "TAO_ECG_Mcast_EH::open(): "
                             "nil ec argument"));
      throw CORBA::INTERNAL ();
    }

  // Create and activate Event Channel Observer.
  ACE_NEW (this->observer_,
           Observer (this));

  if (!this->observer_.in ())
    {
      throw CORBA::NO_MEMORY ();
    }

  TAO_EC_Object_Deactivator observer_deactivator;
  RtecEventChannelAdmin::Observer_var observer_ref;
  PortableServer::POA_var poa =
    this->observer_->_default_POA ();

  activate (observer_ref,
            poa.in (),
            this->observer_.in (),
            observer_deactivator);

  RtecEventChannelAdmin::Observer_Handle handle =
    ec->append_observer (observer_ref.in ());

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
  size_t const subscriptions_size = this->subscriptions_.size ();
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
  size_t const subscriptions_size = this->subscriptions_.size ();
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
         const RtecEventChannelAdmin::ConsumerQOS& sub)
{
  Address_Set multicast_addresses;

  this->compute_required_subscriptions (sub,
                                        multicast_addresses);

  this->delete_unwanted_subscriptions (multicast_addresses);

  this->add_new_subscriptions (multicast_addresses);
}

void
TAO_ECG_Mcast_EH::compute_required_subscriptions (
    const RtecEventChannelAdmin::ConsumerQOS& sub,
    Address_Set& multicast_addresses)
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

      ACE_INET_Addr inet_addr;
      try
        {
          // Grab the right mcast group for this event...
          RtecUDPAdmin::UDP_Address_var udp_addr;

          this->receiver_->get_address (header, udp_addr.out());
          switch (udp_addr->_d())
            {
            case RtecUDPAdmin::Rtec_inet:
              inet_addr.set(udp_addr->v4_addr().port,
                            udp_addr->v4_addr().ipaddr);
              break;
            case RtecUDPAdmin::Rtec_inet6:
#if defined (ACE_HAS_IPV6)
              inet_addr.set_type(PF_INET6);
#endif
              inet_addr.set_address(udp_addr->v6_addr().ipaddr,16,0);
              inet_addr.set_port_number(udp_addr->v6_addr().port);
              break;
            }
        }
      catch (const ::CORBA::BAD_OPERATION &)
        {
          // server only supports IPv4
           // Grab the right mcast group for this event...
          RtecUDPAdmin::UDP_Addr udp_addr;
          this->receiver_->get_addr (header, udp_addr);
          inet_addr.set (udp_addr.port, udp_addr.ipaddr);
        }

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

      size_t const subscriptions_size = this->subscriptions_.size ();
      this->subscriptions_.size (subscriptions_size + 1);
      this->subscriptions_[subscriptions_size] = new_subscription;

      ACE_SOCK_Dgram_Mcast *socket = new_subscription.dgram;

      if (socket->open (new_subscription.mcast_addr, this->net_if_, 1) == -1) {
        ORBSVCS_ERROR ((LM_ERROR,
                    "Error: %d - Unable to open multicast socket\n",
                    ACE_ERRNO_GET));
      }

      if ( socket->enable (ACE_NONBLOCK) != 0 ) {
        ORBSVCS_ERROR ((LM_ERROR,
                    "Error: %d - Unable to enable nonblocking on mcast_eh\n",
                    ACE_ERRNO_GET));
      }

      if (socket->join (new_subscription.mcast_addr, 1, this->net_if_) == -1) {
        ORBSVCS_ERROR ((LM_ERROR,
                    "Error: %d - Unable to join multicast group\n",
                    ACE_ERRNO_GET));
      }

      if (this->recvbuf_size_ != 0
          && (((ACE_SOCK_Dgram *)socket)->set_option(SOL_SOCKET,
                                                     SO_RCVBUF,
                                                     (void *) &this->recvbuf_size_,
                                                     sizeof (this->recvbuf_size_)) == -1)
          && errno != ENOTSUP )
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Error: %d - Unable to set mcast_eh recvbuf_size:%d\n",
                      ACE_ERRNO_GET,
                      this->recvbuf_size_));
        }
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
    const RtecEventChannelAdmin::ConsumerQOS& sub)
{
  if (this->eh_)
    this->eh_->update_consumer (sub);
}

void
TAO_ECG_Mcast_EH::Observer::update_supplier (
    const RtecEventChannelAdmin::SupplierQOS&)
{
}

void
TAO_ECG_Mcast_EH::Observer::shutdown (void)
{
  this->eh_ = 0;
  this->deactivator_.deactivate ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
