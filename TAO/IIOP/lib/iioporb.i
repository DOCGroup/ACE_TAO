ACE_INLINE
void
IIOP_ORB::use_omg_ior_format(CORBA_Boolean ior)
{
  use_omg_ior_format_ = ior;
}

ACE_INLINE
CORBA_Boolean
IIOP_ORB::use_omg_ior_format(void)
{
  return use_omg_ior_format_;
}
