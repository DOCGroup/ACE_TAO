/**
 * @file RTServer_Setup.inl
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE PortableServer::POA_ptr
RTServer_Setup::poa (void)
{
  return this->rtpoa_setup_->poa ();
}

ACE_INLINE RTCORBA_Setup *
RTServer_Setup::rtcorba_setup (void)
{
  return this->rtcorba_setup_.get ();
}
