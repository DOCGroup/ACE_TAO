ACE_INLINE CORBA_Boolean
ROA::shutting_down()
{
  return do_exit;
}

ACE_INLINE CORBA_ORB_ptr
ROA::orb() const
{
  return _orb;
}
