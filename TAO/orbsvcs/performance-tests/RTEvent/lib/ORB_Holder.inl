/**
 * @file ORB_Holder.inl
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE CORBA::ORB_ptr
ORB_Holder::orb (void)
{
  return CORBA::ORB::_duplicate (this->orb_.in ());
}

ACE_INLINE
ORB_Holder::operator CORBA::ORB_ptr () const
{
  return this->orb_.in ();
}

ACE_INLINE CORBA::ORB_ptr
ORB_Holder::operator-> () const
{
  return this->orb_.in ();
}
