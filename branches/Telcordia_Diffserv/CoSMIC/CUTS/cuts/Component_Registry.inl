// $Id$

//
// lock
//
CUTS_INLINE
ACE_RW_Thread_Mutex & CUTS_Component_Registry::lock (void)
{
  return this->lock_;
}

//
// is_registered
//
CUTS_INLINE
bool CUTS_Component_Registry::is_registered (const char * uuid)
{
  return this->registry_.find (uuid) != this->registry_.end ();
}
