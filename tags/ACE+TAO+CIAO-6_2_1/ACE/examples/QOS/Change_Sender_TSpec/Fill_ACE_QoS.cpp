// Fill_ACE_QoS.cpp
// $Id$

#include "Fill_ACE_QoS.h"



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
                                         const ACE_CString &recv_flow_name)
{
  ACE_Flow_Spec *recv_flow_spec = 0;

  if (this->map ().find (recv_flow_name, recv_flow_spec) != 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "Unable to find a FlowSpec with name %s",
                       recv_flow_name.c_str ()),
                      -1);
  ace_qos.receiving_flowspec (recv_flow_spec);
  ace_qos.sending_flowspec ((this->default_traffic_));
  ace_qos.provider_specific (Fill_ACE_QoS::iov_);

  return 0;
}


int
Fill_ACE_QoS::fill_simplex_sender_qos (ACE_QoS &ace_qos,
                                       const ACE_CString &send_flow_name)
{
  ACE_Flow_Spec *send_flow_spec = 0;

  if (this->map ().find (send_flow_name, send_flow_spec) != 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "Unable to find a FlowSpec with name %s",
                       send_flow_name.c_str ()),
                      -1);

  ace_qos.receiving_flowspec ((this->default_traffic_));
  ace_qos.sending_flowspec (send_flow_spec);
  ace_qos.provider_specific (Fill_ACE_QoS::iov_);

  return 0;
}

int
Fill_ACE_QoS::fill_duplex_qos (ACE_QoS &ace_qos,
                               const ACE_CString &recv_flow_name,
                               const ACE_CString &send_flow_name)
{
  ACE_Flow_Spec *send_flow_spec = 0;
  ACE_Flow_Spec *recv_flow_spec = 0;

  if (this->map ().find (recv_flow_name, recv_flow_spec) != 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "Unable to find a FlowSpec with name %s",
                       recv_flow_name.c_str ()),
                      -1);

  if (this->map ().find (send_flow_name, send_flow_spec) != 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "Unable to find a FlowSpec with name %s",
                       send_flow_name.c_str ()),
                      -1);

  ace_qos.receiving_flowspec (recv_flow_spec);
  ace_qos.sending_flowspec (send_flow_spec);
  ace_qos.provider_specific (Fill_ACE_QoS::iov_);

  return 0;
}

Fill_ACE_QoS::FLOW_SPEC_HASH_MAP&
Fill_ACE_QoS::map (void)
{
  return this->flow_spec_map_;
}

