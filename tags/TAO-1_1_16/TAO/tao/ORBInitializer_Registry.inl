// -*- C++ -*-
//
// $Id$

ACE_INLINE TAO_ORBInitializer_Registry *
TAO_ORBInitializer_Registry::instance (void)
{
  return
    TAO_Singleton<TAO_ORBInitializer_Registry, TAO_SYNCH_MUTEX>::instance ();
}
