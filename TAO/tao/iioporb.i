// $Id$

ACE_INLINE
IIOP_ORB::IIOP_ORB(void)
  : use_omg_ior_format_(CORBA_B_FALSE)
{}

ACE_INLINE
IIOP_ORB::~IIOP_ORB(void)
{}


ACE_INLINE void
IIOP_ORB::use_omg_ior_format (CORBA_Boolean ior)
{
  use_omg_ior_format_ = ior;
}

ACE_INLINE CORBA_Boolean
IIOP_ORB::use_omg_ior_format (void)
{
  return use_omg_ior_format_;
}
