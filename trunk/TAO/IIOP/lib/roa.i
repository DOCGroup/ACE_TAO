ACE_INLINE
CORBA_Boolean
ROA::shutting_down()
{
  return do_exit_;
}

ACE_INLINE
CORBA_ORB_ptr
ROA::orb() const
{
  return orb_;
}

ACE_INLINE
ACE_INET_Addr
ROA::get_addr() const
{
  return addr_;
}
