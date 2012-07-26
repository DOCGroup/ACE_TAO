// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  ACE_UNUSED_ARG( address );
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

ACE_INLINE void
TAO_AV_UDP_QoS_Flow_Handler::negotiator (AVStreams::Negotiator_ptr negotiator)
{
  this->negotiator_ = AVStreams::Negotiator::_duplicate (negotiator);
}

ACE_INLINE void
TAO_AV_UDP_QoS_Flow_Handler::endpoint (TAO_Base_StreamEndPoint *endpoint)
{
  this->endpoint_ = endpoint;
}

ACE_INLINE TAO_Base_StreamEndPoint *
TAO_AV_UDP_QoS_Flow_Handler::endpoint (void)
{
  return this->endpoint_;
}

ACE_INLINE void
TAO_AV_UDP_QoS_Flow_Handler::flowspec_entry (TAO_FlowSpec_Entry *entry)
{
  this->entry_ = entry;
}

ACE_INLINE TAO_FlowSpec_Entry *
TAO_AV_UDP_QoS_Flow_Handler::flowspec_entry (void)
{
  return this->entry_;
}

ACE_INLINE void
TAO_AV_UDP_QoS_Flow_Handler::av_core (TAO_AV_Core *avcore)
{
  this->av_core_ = avcore;
}

ACE_INLINE TAO_AV_Core *
TAO_AV_UDP_QoS_Flow_Handler::av_core (void)
{
  return this->av_core_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
