/* -*- C++ -*- */
// $Id$

ACE_INLINE RTCORBA::PriorityBands &
TAO_PriorityBandedConnectionPolicy::priority_bands_rep (void)
{
  return priority_bands_;
}

ACE_INLINE RTCORBA::ProtocolList &
TAO_ClientProtocolPolicy::protocols_rep (void)
{
  return protocols_;
}

ACE_INLINE RTCORBA::ProtocolList &
TAO_ServerProtocolPolicy::protocols_rep (void)
{
  return protocols_;
}

ACE_INLINE RTCORBA::PriorityModel
TAO_PriorityModelPolicy::get_priority_model (void)
{
  return this->priority_model_;
}

