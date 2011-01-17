// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_ECG_UDP_Receiver_Disconnect_Command::
TAO_ECG_UDP_Receiver_Disconnect_Command (void)
  : proxy_ ()
{
}

ACE_INLINE
TAO_ECG_UDP_Receiver_Disconnect_Command::
TAO_ECG_UDP_Receiver_Disconnect_Command (
                          RtecEventChannelAdmin::ProxyPushConsumer_ptr proxy)
  : proxy_ (RtecEventChannelAdmin::ProxyPushConsumer::_duplicate (proxy))
{
}

ACE_INLINE
TAO_ECG_UDP_Receiver_Disconnect_Command::
TAO_ECG_UDP_Receiver_Disconnect_Command (
                          const TAO_ECG_UDP_Receiver_Disconnect_Command & rhs)
  : proxy_ (RtecEventChannelAdmin::ProxyPushConsumer::_duplicate
            (rhs.proxy_.in ()))
{
}

ACE_INLINE
TAO_ECG_UDP_Receiver_Disconnect_Command &
TAO_ECG_UDP_Receiver_Disconnect_Command::operator= (
                         const TAO_ECG_UDP_Receiver_Disconnect_Command & rhs)
{
  this->proxy_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_duplicate (rhs.proxy_.in ());

  return *this;
}

ACE_INLINE void
TAO_ECG_UDP_Receiver_Disconnect_Command::
execute (void)
{
  if (CORBA::is_nil (this->proxy_.in ()))
    // We are not connected.
    return;

  RtecEventChannelAdmin::ProxyPushConsumer_var release_proxy =
    this->proxy_._retn ();

  release_proxy->disconnect_push_consumer ();
}

//***************************************************************************

ACE_INLINE
TAO_ECG_UDP_Receiver::TAO_ECG_UDP_Receiver (CORBA::Boolean perform_crc)
  : lcl_ec_ ()
  , addr_server_ ()
  , consumer_proxy_ ()
  , cdr_receiver_ (perform_crc)
  , handler_rptr_ ()
  , auto_proxy_disconnect_ ()
{
}

ACE_INLINE PortableServer::Servant_var<TAO_ECG_UDP_Receiver>
TAO_ECG_UDP_Receiver::create (CORBA::Boolean perform_crc)
{
  PortableServer::Servant_var<TAO_ECG_UDP_Receiver> r;
  ACE_NEW_RETURN (r,
                  TAO_ECG_UDP_Receiver (perform_crc),
                  r);
  return r;
}

ACE_INLINE void
TAO_ECG_UDP_Receiver::set_handler_shutdown (
                       TAO_ECG_Refcounted_Handler handler_shutdown_rptr)
{
  this->handler_rptr_ = handler_shutdown_rptr;
}

ACE_INLINE void
TAO_ECG_UDP_Receiver::get_addr (const RtecEventComm::EventHeader& header,
                                RtecUDPAdmin::UDP_Addr_out addr)
{
  if (CORBA::is_nil (this->addr_server_.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "TAO_ECG_UDP_Receiver::get_addr() called but "
                  "nil Address Server was supplied during "
                  "initialization through init().\n"));

      throw CORBA::INTERNAL ();
    }

  this->addr_server_->get_addr (header, addr);
}

ACE_INLINE void
TAO_ECG_UDP_Receiver::get_address (const RtecEventComm::EventHeader& header,
                                   RtecUDPAdmin::UDP_Address_out addr)
{
  if (CORBA::is_nil (this->addr_server_.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "TAO_ECG_UDP_Receiver::get_addr() called but "
                  "nil Address Server was supplied during "
                  "initialization through init().\n"));

      throw CORBA::INTERNAL ();
    }

  this->addr_server_->get_address (header, addr);
}

TAO_END_VERSIONED_NAMESPACE_DECL
