/* -*- C++ -*- */
// $Id$

#if (TAO_HAS_RT_CORBA == 1)

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

#endif /* TAO_HAS_RT_CORBA == 1 */
