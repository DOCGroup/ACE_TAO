// $Id$
//--------------------------------------------------
// TAO_AV_Policy
//--------------------------------------------------

ACE_INLINE TAO_AV_Policy::PolicyType
TAO_AV_Policy::type (void)
{
  return this->type_;
}

//--------------------------------------------------
// TAO_AV_SSRC_Policy
//--------------------------------------------------

ACE_INLINE CORBA::ULong
TAO_AV_SSRC_Policy::value (void)
{
  return this->ssrc_;
}


ACE_INLINE void
TAO_AV_SSRC_Policy::value (CORBA::ULong ssrc)
{
  this->ssrc_ = ssrc;
}

//--------------------------------------------------
// TAO_AV_Payload_Type_Policy
//--------------------------------------------------
ACE_INLINE void
TAO_AV_Payload_Type_Policy::value (int pt)
{
  this->payload_type_ = pt;
}

ACE_INLINE int
TAO_AV_Payload_Type_Policy::value (void)
{
  return this->payload_type_;
}

//--------------------------------------------------
// TAO_AV_Protocol_Object
//--------------------------------------------------
ACE_INLINE int
TAO_AV_Protocol_Object::set_policies (PolicyList &policy_list)
{
  this->policy_list_ = policy_list;
  return 0;
}

ACE_INLINE PolicyList
TAO_AV_Protocol_Object::get_policies (void)
{
  return this->policy_list_;
}


ACE_INLINE TAO_AV_Transport*
TAO_AV_Protocol_Object::transport (void)
{
  return this->transport_;
}

ACE_INLINE void
TAO_AV_Protocol_Object::transport (TAO_AV_Transport *transport)
{
  this->transport_ = transport;
}
