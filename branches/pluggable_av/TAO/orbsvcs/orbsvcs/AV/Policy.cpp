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

// TAO_AV_Callback
int
TAO_AV_Callback::handle_start (void)
{
  return -1;
}

int
TAO_AV_Callback::handle_stop (void)
{
  return -1;
}

int
TAO_AV_Callback::receive_frame (ACE_Message_Block */*frame*/)
{
  return -1;
}

int
TAO_AV_Callback::handle_end_stream (void)
{
  return -1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Unbounded_Sequence<TAO_AV_Policy*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Unbounded_Sequence<TAO_AV_Policy*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

