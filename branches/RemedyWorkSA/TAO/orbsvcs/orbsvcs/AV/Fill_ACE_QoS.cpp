// $Id$

#include "orbsvcs/AV/Fill_ACE_QoS.h"

#if defined (ACE_HAS_RAPI) || defined (ACE_HAS_WINSOCK2_GQOS)



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

const iovec Fill_ACE_QoS::iov_ = {0,0};

Fill_ACE_QoS::Fill_ACE_QoS (void)
{
  ACE_NEW (this->default_traffic_,
           ACE_Flow_Spec  (ACE_QOS_NOT_SPECIFIED,
                           ACE_QOS_NOT_SPECIFIED,
                           ACE_QOS_NOT_SPECIFIED,
                           ACE_QOS_NOT_SPECIFIED,
                           ACE_QOS_NOT_SPECIFIED,
                           ACE_SERVICETYPE_NOTRAFFIC,
                           ACE_QOS_NOT_SPECIFIED,
                           ACE_QOS_NOT_SPECIFIED,
                           25,
                           1));
}

// destructor.
Fill_ACE_QoS::~Fill_ACE_QoS (void)
{}

int
Fill_ACE_QoS::fill_simplex_receiver_qos (ACE_QoS &ace_qos,
                                         ACE_Flow_Spec *ace_flow_spec)
{
  ace_qos.receiving_flowspec (ace_flow_spec);
  ace_qos.sending_flowspec ((this->default_traffic_));
  ace_qos.provider_specific (Fill_ACE_QoS::iov_);

  return 0;
}


int
Fill_ACE_QoS::fill_simplex_sender_qos (ACE_QoS &ace_qos,
                                       ACE_Flow_Spec *sender_flow_spec)
{
  ace_qos.receiving_flowspec ((this->default_traffic_));
  ace_qos.sending_flowspec (sender_flow_spec);
  ace_qos.provider_specific (Fill_ACE_QoS::iov_);

  return 0;
}

int
Fill_ACE_QoS::fill_duplex_qos (ACE_QoS &ace_qos,
                               ACE_Flow_Spec *recv_flow_spec,
                               ACE_Flow_Spec *sender_flow_spec)
{
  ace_qos.receiving_flowspec (recv_flow_spec);
  ace_qos.sending_flowspec (sender_flow_spec);
  ace_qos.provider_specific (Fill_ACE_QoS::iov_);

  return 0;
}

Fill_ACE_QoS::FLOW_SPEC_HASH_MAP&
Fill_ACE_QoS::map (void)
{
  return this->flow_spec_map_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_RAPI || ACE_HAS_WINSOCK2_GQOS */
