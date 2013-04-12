// -*- C++ -*-
//
// $Id$

#include "ace/Null_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE PortableServer::Servant_var<TAO_ECG_UDP_Sender>
TAO_ECG_UDP_Sender::create (CORBA::Boolean crc)
{
  PortableServer::Servant_var<TAO_ECG_UDP_Sender> s;
  ACE_NEW_RETURN (s,
                  TAO_ECG_UDP_Sender (crc),
                  s);
  return s;
}

ACE_INLINE
TAO_ECG_UDP_Sender::TAO_ECG_UDP_Sender (CORBA::Boolean crc)
  : supplier_proxy_ ()
  , lcl_ec_ ()
  , addr_server_ ()
  , cdr_sender_ (crc)
  , auto_proxy_disconnect_ ()
{
}

ACE_INLINE CORBA::ULong
TAO_ECG_UDP_Sender::mtu (void) const
{
  return this->cdr_sender_.mtu ();
}

ACE_INLINE int
TAO_ECG_UDP_Sender::mtu (CORBA::ULong new_mtu)
{
  return this->cdr_sender_.mtu (new_mtu);
}

ACE_INLINE int
TAO_ECG_UDP_Sender::get_local_addr (ACE_INET_Addr& addr)
{
  return this->cdr_sender_.get_local_addr (addr);
}
//***************************************************************************

ACE_INLINE
TAO_ECG_UDP_Sender_Disconnect_Command::
TAO_ECG_UDP_Sender_Disconnect_Command (void)
  : proxy_ ()
{
}

ACE_INLINE
TAO_ECG_UDP_Sender_Disconnect_Command::
TAO_ECG_UDP_Sender_Disconnect_Command (
                          RtecEventChannelAdmin::ProxyPushSupplier_ptr proxy)
  : proxy_ (RtecEventChannelAdmin::ProxyPushSupplier::_duplicate (proxy))
{
}

ACE_INLINE
TAO_ECG_UDP_Sender_Disconnect_Command::
TAO_ECG_UDP_Sender_Disconnect_Command (
                              const TAO_ECG_UDP_Sender_Disconnect_Command & rhs)
  : proxy_ (RtecEventChannelAdmin::ProxyPushSupplier::_duplicate
            (rhs.proxy_.in ()))
{
}

ACE_INLINE TAO_ECG_UDP_Sender_Disconnect_Command &
TAO_ECG_UDP_Sender_Disconnect_Command::operator= (
                             const TAO_ECG_UDP_Sender_Disconnect_Command & rhs)
{
  this->proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_duplicate (rhs.proxy_.in ());

  return *this;
}

ACE_INLINE void
TAO_ECG_UDP_Sender_Disconnect_Command::execute (void)
{
  if (CORBA::is_nil (this->proxy_.in ()))
    // We are not connected.
    return;

  RtecEventChannelAdmin::ProxyPushSupplier_var release_proxy =
    this->proxy_._retn ();

  release_proxy->disconnect_push_supplier ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
