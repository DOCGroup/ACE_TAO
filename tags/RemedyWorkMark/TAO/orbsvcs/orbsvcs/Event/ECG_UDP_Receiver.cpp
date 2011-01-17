// $Id$

#include "orbsvcs/Event/ECG_UDP_Receiver.h"
#include "orbsvcs/Event_Utilities.h"
#include "ace/SString.h"

#if !defined(__ACE_INLINE__)
#include "orbsvcs/Event/ECG_UDP_Receiver.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ECG_UDP_Receiver::~TAO_ECG_UDP_Receiver (void)
{
  this->consumer_proxy_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();

  if (this->handler_rptr_.get ())
    this->handler_rptr_->shutdown ();
}

void
TAO_ECG_UDP_Receiver::init (RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
                            TAO_ECG_Refcounted_Endpoint ignore_from,
                            RtecUDPAdmin::AddrServer_ptr addr_server)
{
  // Verify arguments.
  // <addr_server> is allowed to be nil.  But then, if get_addr () method
  // is invoked, it throws an exception.
  if (CORBA::is_nil (lcl_ec))
    {
      ACE_ERROR ((LM_ERROR,
                  "TAO_ECG_UDP_Receiver::init(): "
                  "<lcl_ec> argument is nil.\n"));
      throw CORBA::INTERNAL ();
    }

  this->cdr_receiver_.init (ignore_from);

  this->lcl_ec_ =
    RtecEventChannelAdmin::EventChannel::_duplicate (lcl_ec);

  this->addr_server_ =
    RtecUDPAdmin::AddrServer::_duplicate (addr_server);
}

void
TAO_ECG_UDP_Receiver::connect (const RtecEventChannelAdmin::SupplierQOS& pub)
{
  if (CORBA::is_nil (this->lcl_ec_.in ()))
    {
      //FUZZ: disable check_for_lack_ACE_OS
      ACE_ERROR ((LM_ERROR,
                  "Error initializing TAO_ECG_UDP_Receiver: "
                  "init() hasn't been called before connect().\n"));
      //FUZZ: enable check_for_lack_ACE_OS

      throw CORBA::INTERNAL ();
    }

  if (pub.publications.length () == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "TAO_ECG_UDP_Receiver::connect(): "
                  "0-length publications argument.\n"));
      throw CORBA::INTERNAL ();
    }

  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    {
      this->new_connect (pub);
    }
  else
    {
      this->reconnect (pub);
    }
}

void
TAO_ECG_UDP_Receiver::new_connect (const RtecEventChannelAdmin::SupplierQOS& pub)
{
  // Activate with poa.
  RtecEventComm::PushSupplier_var supplier_ref;
  PortableServer::POA_var poa = this->_default_POA ();

  TAO_EC_Object_Deactivator deactivator;
  activate (supplier_ref,
            poa.in (),
            this,
            deactivator);

  // Connect as a supplier to the local EC.
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->lcl_ec_->for_suppliers ();

  RtecEventChannelAdmin::ProxyPushConsumer_var proxy =
    supplier_admin->obtain_push_consumer ();
  ECG_Receiver_Auto_Proxy_Disconnect new_proxy_disconnect (proxy.in ());

  proxy->connect_push_supplier (supplier_ref.in (),
                                pub);

  // Update resource managers.
  this->consumer_proxy_ = proxy._retn ();
  this->auto_proxy_disconnect_.set_command (new_proxy_disconnect);
  this->set_deactivator (deactivator);
}

void
TAO_ECG_UDP_Receiver::reconnect (const RtecEventChannelAdmin::SupplierQOS& pub)
{
  // Obtain our object reference from the POA.
  RtecEventComm::PushSupplier_var supplier_ref;
  PortableServer::POA_var poa = this->_default_POA ();

  CORBA::Object_var obj = poa->servant_to_reference (this);
  supplier_ref =
    RtecEventComm::PushSupplier::_narrow (obj.in ());

  if (CORBA::is_nil (supplier_ref.in ()))
    {
      throw CORBA::INTERNAL ();
    }

  // Reconnect.
  this->consumer_proxy_->connect_push_supplier (supplier_ref.in (),
                                                pub);
}

void
TAO_ECG_UDP_Receiver::disconnect_push_supplier (void)
{
  // Prevent attempts to disconnect.
  this->auto_proxy_disconnect_.disallow_command ();

  this->shutdown ();
}

void
TAO_ECG_UDP_Receiver::shutdown (void)
{
  if (this->handler_rptr_.get ())
    this->handler_rptr_->shutdown ();
  TAO_ECG_Refcounted_Handler empty_handler_rptr;
  this->handler_rptr_ = empty_handler_rptr;

  this->consumer_proxy_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();

  this->auto_proxy_disconnect_.execute ();

  this->deactivator_.deactivate ();

  this->cdr_receiver_.shutdown ();
}

// Helper class for using <cdr_receiver_>.
class TAO_ECG_Event_CDR_Decoder: public TAO_ECG_CDR_Processor
{
public:
  virtual int decode (TAO_InputCDR &cdr);

  RtecEventComm::EventSet events;
};

int
TAO_ECG_Event_CDR_Decoder::decode (TAO_InputCDR &cdr)
{
  if (!(cdr >> this->events))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Error decoding events cdr.\n"),
                        -1);
    }
  return 0;
}

int
TAO_ECG_UDP_Receiver::handle_input (ACE_SOCK_Dgram& dgram)
{
  try
    {
      // Make sure we are connected to the Event Channel before proceeding
      // any further.
      if (CORBA::is_nil (this->consumer_proxy_.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "TAO_ECG_UDP_Receiver::handle_input() "
                      "called but the Receiver is not connected "
                      "to an event channel. Shutting down the Receiver.\n"));
          this->shutdown ();

          return 0;
        }

      // Receive data.
      TAO_ECG_Event_CDR_Decoder cdr_decoder;
      int const result = this->cdr_receiver_.handle_input (dgram, &cdr_decoder);

      if (result == 0)
        // No data to act on.
        {
          return 0;
        }
      if (result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Error receiving multicasted events.\n"),
                            0);
        }

      this->consumer_proxy_->push (cdr_decoder.events);
    }

  catch (const CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR,
                  "Caught and swallowed EXCEPTION in "
                  "ECG_UDP_Receiver::handle_input: %C\n",
                  ex._info ().c_str ()));
    }
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
