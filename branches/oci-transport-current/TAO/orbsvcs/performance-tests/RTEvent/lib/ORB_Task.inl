/**
 * @file ORB_Task.inl
 *
` * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE CORBA::ORB_ptr
ORB_Task::operator-> (void) const
{
  return this->orb_.in ();
}
