/**
 * @file ORB_Shutdown.inl
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE void
ORB_Shutdown::operator () (CORBA::ORB_ptr orb
                           ACE_ENV_ARG_DECL)
{
  orb->shutdown (0
                 ACE_ENV_SINGLE_ARG_PARAMETER);
}
