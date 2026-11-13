// Fill_ACE_QoS.cpp
#include "Fill_ACE_QoS.h"

#include <utility>

iovec const Fill_ACE_QoS::iov_ = {nullptr, 0};

Fill_ACE_QoS::Fill_ACE_QoS ()
  : default_traffic_ (std::make_unique<ACE_Flow_Spec> (
      ACE_QOS_NOT_SPECIFIED,
      ACE_QOS_NOT_SPECIFIED,
      ACE_QOS_NOT_SPECIFIED,
      ACE_QOS_NOT_SPECIFIED,
      ACE_QOS_NOT_SPECIFIED,
      ACE_SERVICETYPE_NOTRAFFIC,
      ACE_QOS_NOT_SPECIFIED,
      ACE_QOS_NOT_SPECIFIED,
      25,
      1))
{
}

int
Fill_ACE_QoS::fill_simplex_receiver_qos (ACE_QoS &ace_qos,
                                         ACE_CString const &recv_flow_name)
{
  ACE_Flow_Spec *recv_flow_spec {};

  if (this->flow_spec_map_.find (recv_flow_name, recv_flow_spec) != 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "Unable to find a FlowSpec with name %s",
                       recv_flow_name.c_str ()),
                      -1);
  ace_qos.receiving_flowspec (recv_flow_spec);
  ace_qos.sending_flowspec (this->default_traffic_.get ());
  ace_qos.provider_specific (Fill_ACE_QoS::iov_);

  return 0;
}

int
Fill_ACE_QoS::fill_simplex_sender_qos (ACE_QoS &ace_qos,
                                       ACE_CString const &send_flow_name)
{
  ACE_Flow_Spec *send_flow_spec {};

  if (this->flow_spec_map_.find (send_flow_name, send_flow_spec) != 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "Unable to find a FlowSpec with name %s",
                       send_flow_name.c_str ()),
                      -1);

  ace_qos.receiving_flowspec (this->default_traffic_.get ());
  ace_qos.sending_flowspec (send_flow_spec);
  ace_qos.provider_specific (Fill_ACE_QoS::iov_);

  return 0;
}

int
Fill_ACE_QoS::fill_duplex_qos (ACE_QoS &ace_qos,
                               ACE_CString const &recv_flow_name,
                               ACE_CString const &send_flow_name)
{
  ACE_Flow_Spec *send_flow_spec {};
  ACE_Flow_Spec *recv_flow_spec {};

  if (this->flow_spec_map_.find (recv_flow_name, recv_flow_spec) != 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "Unable to find a FlowSpec with name %s",
                       recv_flow_name.c_str ()),
                      -1);

  if (this->flow_spec_map_.find (send_flow_name, send_flow_spec) != 0)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       "Unable to find a FlowSpec with name %s",
                       send_flow_name.c_str ()),
                      -1);

  ace_qos.receiving_flowspec (recv_flow_spec);
  ace_qos.sending_flowspec (send_flow_spec);
  ace_qos.provider_specific (Fill_ACE_QoS::iov_);

  return 0;
}

int
Fill_ACE_QoS::bind_flow_spec (ACE_CString const &name,
                              std::unique_ptr<ACE_Flow_Spec> flow_spec)
{
  if (flow_spec == nullptr)
    return -1;

  ACE_Flow_Spec * const flow_spec_ptr = flow_spec.get ();
  this->flow_specs_.push_back (std::move (flow_spec));
  int const result = this->flow_spec_map_.bind (name, flow_spec_ptr);
  if (result != 0)
    this->flow_specs_.pop_back ();

  return result;
}
