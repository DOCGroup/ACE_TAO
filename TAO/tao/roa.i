ACE_INLINE CORBA::Boolean
ROA::shutting_down (void)
{
  return do_exit_;
}

ACE_INLINE CORBA::ORB_ptr
ROA::orb (void) const
{
  return orb_;
}
