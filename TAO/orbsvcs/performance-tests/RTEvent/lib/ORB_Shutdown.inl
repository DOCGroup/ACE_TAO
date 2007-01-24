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
  ACE_DECLARE_NEW_ENV;
  ACE_TRY
    {
      orb->shutdown (0
                     ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY { };
  ACE_ENDTRY;
}
