// CORBA dup/release build on top of COM's (why not).

ACE_INLINE void
CORBA::release (CORBA::POA_ptr obj)
{
  if (obj)
    obj->Release ();
}

ACE_INLINE CORBA::Boolean
CORBA_POA::shutting_down (void)
{
  return do_exit_;
}

ACE_INLINE CORBA::ORB_ptr
CORBA_POA::orb (void) const
{
  return orb_;
}
