/**
 * @file ORB_Task.inl
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE CORBA::ORB_ptr
ORB_Task::operator-> () const
{
  return this->orb_.in ();
}
