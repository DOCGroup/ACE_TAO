/* -*- C++ -*- */
// $Id$
//--------------------------------------------------
// TAO_AV_Policy
//--------------------------------------------------

ACE_INLINE CORBA::ULong
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


// TAO_AV_RTCP_Sdes_Policy
ACE_INLINE TAO_AV_RTCP_Sdes &
TAO_AV_RTCP_Sdes_Policy::value (void)
{
  return this->sdes_;
}

ACE_INLINE void
TAO_AV_RTCP_Sdes_Policy::value (const TAO_AV_RTCP_Sdes &sdes_val)
{
  this->sdes_ = sdes_val;
}

//----------------------------------------------------------------------
// TAO_AV_SFP_Credit_Policy
//----------------------------------------------------------------------

ACE_INLINE
void
TAO_AV_SFP_Credit_Policy::value (int credit)
{
  this->value_ = credit;
}

ACE_INLINE
int
TAO_AV_SFP_Credit_Policy::value (void)
{
  return this->value_;
}
