/**
 * @file RTCORBA_Setup.inl
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE const RTCORBA::ThreadpoolLanes &
RTCORBA_Setup::lanes () const
{
  return this->lanes_;
}

ACE_INLINE RTCORBA::Priority
RTCORBA_Setup::process_priority () const
{
  return this->process_priority_;
}
