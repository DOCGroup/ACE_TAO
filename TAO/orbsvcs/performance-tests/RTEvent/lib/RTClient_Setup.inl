/**
 * @file RTClient_Setup.inl
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE int
RTClient_Setup::use_rt_corba () const
{
  return this->use_rt_corba_;
}

ACE_INLINE RTCORBA_Setup *
RTClient_Setup::rtcorba_setup ()
{
  return this->rtcorba_setup_.get ();
}
