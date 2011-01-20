// -*- C++ -*-

/**
 * @file ORB_Shutdown.inl
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE void
ORB_Shutdown::operator () (CORBA::ORB_ptr orb)
{
  try
    {
      orb->shutdown (0);
    }
  catch (const CORBA::Exception&){ };
}
