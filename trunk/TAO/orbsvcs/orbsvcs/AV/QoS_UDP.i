/* -*- C++ -*- */

// $Id$

//----------------------------------------------------------------------
// TAO_AV_UDP_Flow_Handler
//----------------------------------------------------------------------
ACE_INLINE
ACE_SOCK_Dgram_Mcast_QoS *
TAO_AV_UDP_QoS_Flow_Handler::get_socket (void)
{
  return &this->qos_sock_dgram_;
}

ACE_INLINE
int
TAO_AV_UDP_QoS_Flow_Handler::open (ACE_Addr &address)
{
  //    return this->qos_sock_dgram_.open (address);
  return 0;
}

ACE_INLINE ACE_QoS_Session*
TAO_AV_UDP_QoS_Flow_Handler::qos_session (void)
{       
  return this->qos_session_;
}

ACE_INLINE void
TAO_AV_UDP_QoS_Flow_Handler::qos_session (ACE_QoS_Session *qos_session)
{
  this->qos_session_ = qos_session;
}
