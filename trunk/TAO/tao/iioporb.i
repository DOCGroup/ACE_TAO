// $Id$

ACE_INLINE
IIOP_ORB::IIOP_ORB(void)
  : use_omg_ior_format_(CORBA::B_FALSE)
{
}

ACE_INLINE
IIOP_ORB::~IIOP_ORB(void)
{}


ACE_INLINE void
IIOP_ORB::use_omg_ior_format (CORBA::Boolean ior)
{
  use_omg_ior_format_ = ior;
}

ACE_INLINE CORBA::Boolean
IIOP_ORB::use_omg_ior_format (void)
{
  return use_omg_ior_format_;
}
