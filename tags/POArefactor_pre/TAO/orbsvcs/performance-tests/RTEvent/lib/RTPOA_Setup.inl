/**
 * @file RTPOA_Setup.inl
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE PortableServer::POA_ptr
RTPOA_Setup::poa (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
