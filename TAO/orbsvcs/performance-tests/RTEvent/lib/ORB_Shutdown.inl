// -*- C++ -*-

/**
 * @file ORB_Shutdown.inl
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE void
ORB_Shutdown::operator () (CORBA::ORB_ptr orb)
{
  try
    {
      orb->shutdown (false);
    }
  catch (const CORBA::Exception&){ };
}
