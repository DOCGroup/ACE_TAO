// $Id$

#include "Policy.h"

TAO_AV_Policy::TAO_AV_Policy (TAO_AV_Policy::PolicyType type)
  :type_ (type)
{
}


TAO_AV_SSRC_Policy::TAO_AV_SSRC_Policy (CORBA::ULong ssrc)
  :TAO_AV_Policy (TAO_AV_Policy::TAO_AV_SSRC_POLICY),
   ssrc_ (ssrc)
{
}


TAO_AV_Payload_Type_Policy::TAO_AV_Payload_Type_Policy (int payload_type)
  :TAO_AV_Policy (TAO_AV_Policy::TAO_AV_PAYLOAD_TYPE_POLICY),
   payload_type_ (payload_type)
{
}

// TAO_AV_RTP_Sdes_Policy
TAO_AV_RTCP_Sdes_Policy::TAO_AV_RTCP_Sdes_Policy (void)
  :TAO_AV_Policy (TAO_AV_Policy::TAO_AV_RTCP_SDES_POLICY)
{
}

// TAO_AV_Timestamp_Policy
TAO_AV_Timestamp_Policy::TAO_AV_Timestamp_Policy (void)
  :TAO_AV_Policy (TAO_AV_Policy::TAO_AV_TIMESTAMP_POLICY)
{
}

TAO_AV_Policy *
TAO_AV_Policy_Manager::create_policy (TAO_AV_Policy::PolicyType type,
                                      void *value)
{
  TAO_AV_Policy *policy = 0;
  switch (type)
    {
    case TAO_AV_Policy::TAO_AV_PAYLOAD_TYPE_POLICY:
      {
        int *payload_type = ACE_static_cast (int *,value);
        if (payload_type == 0)
          ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_Policy_Manager::create_policy:Invalid value\n"),0);
        ACE_NEW_RETURN (policy,
                        TAO_AV_Payload_Type_Policy (*payload_type),
                        0);
      }
      break;
    case TAO_AV_Policy::TAO_AV_SSRC_POLICY:
      {
        CORBA::ULong *ssrc = ACE_static_cast (CORBA::ULong *,value);
        if (ssrc == 0)
          ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_Policy_Manager::create_policy:Invalid value\n"),0);
        ACE_NEW_RETURN (policy,
                        TAO_AV_SSRC_Policy (*ssrc),
                        0);
      }
      break;
    default:
      break;
    }
  return policy;
}

TAO_AV_Protocol_Object::TAO_AV_Protocol_Object (TAO_AV_Callback *callback,
                                                TAO_AV_Transport *transport)
  :transport_ (transport),
   callback_ (callback)
{
}

TAO_AV_Protocol_Object::~TAO_AV_Protocol_Object (void)
{
  //no-op
}

int
TAO_AV_Protocol_Object::start (void)
{
  return this->callback_->handle_start ();
}

int
TAO_AV_Protocol_Object::stop (void)
{
  return this->callback_->handle_stop ();
}

// int
// TAO_AV_Protocol_Object::send_frame (const iovec *iov,
//                                     int iovcnt,
//                                     TAO_AV_frame_info *frame_info)
// {
//   ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_Protocol_Object::send_frame\n"),-1);
// }

int
TAO_AV_Protocol_Object::set_policies (const PolicyList &policy_list)
{
  this->policy_list_ = policy_list;
  return 0;
}

PolicyList
TAO_AV_Protocol_Object::get_policies (void)
{
  return this->policy_list_;
}

// TAO_AV_Callback

TAO_AV_Callback::TAO_AV_Callback (void)
  :transport_ (0),
   protocol_object_ (0)
{
}

int
TAO_AV_Callback::handle_start (void)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Callback::handle_start\n"));
  return -1;
}

int
TAO_AV_Callback::handle_stop (void)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Callback::handle_stop\n"));
  return -1;
}

int
TAO_AV_Callback::receive_frame (ACE_Message_Block */*frame*/,
                                TAO_AV_frame_info *)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Callback::receive_frame\n"));
  return -1;
}

int
TAO_AV_Callback::handle_end_stream (void)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Callback::handle_end_stream\n"));
  return -1;
}

void
TAO_AV_Callback::get_timeout (ACE_Time_Value *&tv,
                              void *&arg)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Callback::get_timeout\n"));
}

int
TAO_AV_Callback::handle_timeout (void *arg)
{
  if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"TAO_AV_Callback::handle_timeout\n"));
  return 0;
}

// int
// TAO_AV_Callback::get_frame (ACE_Message_Block *&frame,
//                             TAO_AV_frame_info *&frame_info)
// {
//   return -1;
// }

TAO_AV_Transport*
TAO_AV_Callback::transport (void)
{
  return this->transport_;
}

void
TAO_AV_Callback::transport (TAO_AV_Transport *transport)
{
  this->transport_ = transport;
}

TAO_AV_Protocol_Object*
TAO_AV_Callback::protocol_object (void)
{
  return this->protocol_object_;
}

void
TAO_AV_Callback::protocol_object (TAO_AV_Protocol_Object *object)
{
  this->protocol_object_ = object;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Unbounded_Sequence<TAO_AV_Policy*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Unbounded_Sequence<TAO_AV_Policy*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
