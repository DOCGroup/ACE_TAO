// -*- C++ -*-
//
// $Id$

ACE_INLINE TAO_ORB_Table *
TAO_ORB_Table::instance (void)
{
  return TAO_Singleton<TAO_ORB_Table, TAO_SYNCH_MUTEX>::instance ();
}

ACE_INLINE TAO_ORB_Core *
TAO_ORB_Table::first_orb (void)
{
  return this->first_orb_;
}
