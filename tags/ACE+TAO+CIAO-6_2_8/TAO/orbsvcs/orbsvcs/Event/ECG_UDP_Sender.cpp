/**
 * @file ECG_UDP_Sender.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 *
 */

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Event/ECG_UDP_Sender.h"
#include "orbsvcs/Event_Utilities.h"
#include "tao/CDR.h"

#if !defined(__ACE_INLINE__)
#include "orbsvcs/Event/ECG_UDP_Sender.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ECG_UDP_Sender::~TAO_ECG_UDP_Sender (void)
{
}

void
TAO_ECG_UDP_Sender::init (RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
                          RtecUDPAdmin::AddrServer_ptr addr_server,
                          TAO_ECG_Refcounted_Endpoint endpoint_rptr)
{
  if (CORBA::is_nil (lcl_ec))
    {
      ORBSVCS_ERROR ((LM_ERROR, "TAO_ECG_UDP_Sender::init(): "
                            "<lcl_ec> argument is nil."));
      throw CORBA::INTERNAL ();
    }

  if (CORBA::is_nil (addr_server))
    {
      ORBSVCS_ERROR ((LM_ERROR, "TAO_ECG_UDP_Sender::init(): "
                            "address server argument is nil."));
      throw CORBA::INTERNAL ();
    }

  this->cdr_sender_.init (endpoint_rptr);

  this->lcl_ec_ =
    RtecEventChannelAdmin::EventChannel::_duplicate (lcl_ec);

  this->addr_server_ =
    RtecUDPAdmin::AddrServer::_duplicate (addr_server);
}

void
TAO_ECG_UDP_Sender::connect (const RtecEventChannelAdmin::ConsumerQOS& sub)
{
  if (CORBA::is_nil (this->lcl_ec_.in ()))
    {
      //FUZZ: disable check_for_lack_ACE_OS
      ORBSVCS_ERROR ((LM_ERROR, "Error initializing TAO_ECG_UDP_Sender: "
                            "init() has not been called before connect()."));
      //FUZZ: enable check_for_lack_ACE_OS

      throw CORBA::INTERNAL ();
    }

  if (sub.dependencies.length () == 0)
    {
      ORBSVCS_ERROR ((LM_ERROR, "TAO_ECG_UDP_Sender::connect(): "
                            "0-length subscriptions argument."));
      throw CORBA::INTERNAL ();
    }

  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    {
      this->new_connect (sub);
    }
  else
    {
      this->reconnect (sub);
    }
}

void
TAO_ECG_UDP_Sender::new_connect (const RtecEventChannelAdmin::ConsumerQOS& sub)
{
  // Activate with poa.
  RtecEventComm::PushConsumer_var consumer_ref;
  PortableServer::POA_var poa = this->_default_POA ();

  TAO_EC_Object_Deactivator deactivator;
  activate (consumer_ref,
            poa.in (),
            this,
            deactivator);

  // Connect as a consumer to the local EC.
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->lcl_ec_->for_consumers ();

  RtecEventChannelAdmin::ProxyPushSupplier_var proxy =
    consumer_admin->obtain_push_supplier ();
  ECG_Sender_Auto_Proxy_Disconnect new_proxy_disconnect (proxy.in ());

  proxy->connect_push_consumer (consumer_ref.in (),
                                sub);

  // Update resource managers.
  this->supplier_proxy_ = proxy._retn ();
  this->auto_proxy_disconnect_.set_command (new_proxy_disconnect);
  this->set_deactivator (deactivator);
}

void
TAO_ECG_UDP_Sender::reconnect (const RtecEventChannelAdmin::ConsumerQOS& sub)
{
  // Obtain our object reference from the POA.
  RtecEventComm::PushConsumer_var consumer_ref;
  PortableServer::POA_var poa = this->_default_POA ();

  CORBA::Object_var obj = poa->servant_to_reference (this);
  consumer_ref =
    RtecEventComm::PushConsumer::_narrow (obj.in ());

  if (CORBA::is_nil (consumer_ref.in ()))
    {
      throw CORBA::INTERNAL ();
    }

  // Reconnect.
  this->supplier_proxy_->connect_push_consumer (consumer_ref.in (),
                                                sub);
}

void
TAO_ECG_UDP_Sender::disconnect_push_consumer (void)
{
  // Prevent attempts to disconnect.
  this->auto_proxy_disconnect_.disallow_command ();

  this->shutdown ();
}

void
TAO_ECG_UDP_Sender::shutdown (void)
{
  this->supplier_proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();

  this->auto_proxy_disconnect_.execute ();

  this->addr_server_ = RtecUDPAdmin::AddrServer::_nil ();
  this->lcl_ec_ = RtecEventChannelAdmin::EventChannel::_nil ();

  this->deactivator_.deactivate ();
  this->cdr_sender_.shutdown ();
}

void
TAO_ECG_UDP_Sender::push (const RtecEventComm::EventSet &events)
{
  if (events.length () == 0)
    {
      // ORBSVCS_DEBUG ((EC_FORMAT (DEBUG,
      //                        "Nothing to multicast: "
      //                        "0-length EventSet.")));
      return;
    }

  // Send each event in a separate message.
  // @@ TODO It is interesting to group events destined to the
  // same mcast group in a single message.
  for (u_int i = 0; i < events.length (); ++i)
    {
      // To avoid loops we keep a TTL field on the events and skip the
      // events with TTL <= 0
      if (events[i].header.ttl <= 0)
        continue;

      const RtecEventComm::Event& e = events[i];

      // We need to modify the TTL field, but copying the entire event
      // would be wasteful; instead we create a new header and only
      // modify the header portion.
      RtecEventComm::EventHeader header = e.header;
      header.ttl--;

      // Start building the message
      TAO_OutputCDR cdr;

      // Marshal as if it was a sequence of one element, notice how we
      // marshal a modified version of the header, but the payload is
      // marshal without any extra copies.
      cdr.write_ulong (1);
      if (!(cdr << header)
          || !(cdr << e.data))
        throw CORBA::MARSHAL ();

      ACE_INET_Addr inet_addr;
      try
        {
          // Grab the right mcast group for this event...
          RtecUDPAdmin::UDP_Address_var udp_addr;

          this->addr_server_->get_address (header, udp_addr.out());
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
          this->addr_server_->get_addr (header, udp_addr);
          inet_addr.set (udp_addr.port, udp_addr.ipaddr);
        }

      this->cdr_sender_.send_message (cdr, inet_addr);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
