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

#include "orbsvcs/Event/ECG_UDP_Sender.h"
#include "orbsvcs/Event_Utilities.h"

#if !defined(__ACE_INLINE__)
#include "ECG_UDP_Sender.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, ECG_UDP_Sender, "$Id$")

// ****************************************************************

TAO_ECG_UDP_Sender::~TAO_ECG_UDP_Sender (void)
{
}

void
TAO_ECG_UDP_Sender::init (RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
                          RtecUDPAdmin::AddrServer_ptr addr_server,
                          TAO_ECG_Refcounted_Endpoint endpoint_rptr
                          ACE_ENV_ARG_DECL)
{
  if (CORBA::is_nil (lcl_ec))
    {
      ACE_ERROR ((LM_ERROR, "TAO_ECG_UDP_Sender::init(): "
                            "<lcl_ec> argument is nil."));
      ACE_THROW (CORBA::INTERNAL ());
    }

  if (CORBA::is_nil (addr_server))
    {
      ACE_ERROR ((LM_ERROR, "TAO_ECG_UDP_Sender::init(): "
                            "address server argument is nil."));
      ACE_THROW (CORBA::INTERNAL ());
    }

  this->cdr_sender_.init (endpoint_rptr ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->lcl_ec_ =
    RtecEventChannelAdmin::EventChannel::_duplicate (lcl_ec);

  this->addr_server_ =
    RtecUDPAdmin::AddrServer::_duplicate (addr_server);
}

void
TAO_ECG_UDP_Sender::connect (const RtecEventChannelAdmin::ConsumerQOS& sub
                             ACE_ENV_ARG_DECL)
{
  if (CORBA::is_nil (this->lcl_ec_.in ()))
    {
      ACE_ERROR ((LM_ERROR, "Error initializing TAO_ECG_UDP_Sender: "
                            "init() has not been called before connect()."));
      ACE_THROW (CORBA::INTERNAL ());
    }

  if (sub.dependencies.length () == 0)
    {
      ACE_ERROR ((LM_ERROR, "TAO_ECG_UDP_Sender::connect(): "
                            "0-length subscriptions argument."));
      ACE_THROW (CORBA::INTERNAL ());
    }

  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    {
      this->new_connect (sub ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->reconnect (sub ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_ECG_UDP_Sender::new_connect (const RtecEventChannelAdmin::ConsumerQOS& sub
                                 ACE_ENV_ARG_DECL)
{
  // Activate with poa.
  RtecEventComm::PushConsumer_var consumer_ref;
  PortableServer::POA_var poa = this->_default_POA ();

  TAO_EC_Object_Deactivator deactivator;
  activate (consumer_ref,
            poa.in (),
            this,
            deactivator
            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Connect as a consumer to the local EC.
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->lcl_ec_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventChannelAdmin::ProxyPushSupplier_var proxy =
    consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  ECG_Sender_Auto_Proxy_Disconnect new_proxy_disconnect (proxy.in ());

  proxy->connect_push_consumer (consumer_ref.in (),
                                sub
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Update resource managers.
  this->supplier_proxy_ = proxy._retn ();
  this->auto_proxy_disconnect_.set_command (new_proxy_disconnect);
  this->set_deactivator (deactivator);
}

void
TAO_ECG_UDP_Sender::reconnect (const RtecEventChannelAdmin::ConsumerQOS& sub
                               ACE_ENV_ARG_DECL)
{
  // Obtain our object reference from the POA.
  RtecEventComm::PushConsumer_var consumer_ref;
  PortableServer::POA_var poa = this->_default_POA ();

  CORBA::Object_var obj = poa->servant_to_reference (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  consumer_ref =
    RtecEventComm::PushConsumer::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (consumer_ref.in ()))
    {
      ACE_THROW (CORBA::INTERNAL ());
    }

  // Reconnect.
  this->supplier_proxy_->connect_push_consumer (consumer_ref.in (),
                                                sub
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_ECG_UDP_Sender::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Prevent attempts to disconnect.
  this->auto_proxy_disconnect_.disallow_command ();

  this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_ECG_UDP_Sender::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->supplier_proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();

  this->auto_proxy_disconnect_.execute ();

  this->addr_server_ = RtecUDPAdmin::AddrServer::_nil ();
  this->lcl_ec_ = RtecEventChannelAdmin::EventChannel::_nil ();

  this->deactivator_.deactivate ();
  this->cdr_sender_.shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_ECG_UDP_Sender::push (const RtecEventComm::EventSet &events
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (events.length () == 0)
    {
      // ACE_DEBUG ((EC_FORMAT (DEBUG,
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
        ACE_THROW (CORBA::MARSHAL ());

      // Grab the right mcast group for this event...
      RtecUDPAdmin::UDP_Addr udp_addr;
      this->addr_server_->get_addr (header, udp_addr ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_INET_Addr inet_addr (udp_addr.port,
                               udp_addr.ipaddr);

      this->cdr_sender_.send_message (cdr, inet_addr ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template void activate(TAO_Objref_Var_T<RtecEventComm::PushConsumer, RtecEventComm::tao_PushConsumer_life>&, PortableServer::POA*, TAO_ServantBase*, TAO_EC_Object_Deactivator&);

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate void activate(TAO_Objref_Var_T<RtecEventComm::PushConsumer, RtecEventComm::tao_PushConsumer_life>&, PortableServer::POA*, TAO_ServantBase*, TAO_EC_Object_Deactivator&);

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
