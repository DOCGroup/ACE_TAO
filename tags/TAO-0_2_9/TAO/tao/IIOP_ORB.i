// $Id$

ACE_INLINE
IIOP_ORB::IIOP_ORB(void)
  : use_omg_ior_format_(0),
    optimize_collocation_objects_ (1)
{
}

ACE_INLINE
IIOP_ORB::~IIOP_ORB(void)
{
}


ACE_INLINE void
IIOP_ORB::_use_omg_ior_format (CORBA::Boolean ior)
{
  use_omg_ior_format_ = ior;
}

ACE_INLINE CORBA::Boolean
IIOP_ORB::_use_omg_ior_format (void)
{
  return use_omg_ior_format_;
}

ACE_INLINE void
IIOP_ORB::_optimize_collocation_objects (CORBA::Boolean opt)
{
  optimize_collocation_objects_ = opt;
}

ACE_INLINE CORBA::Boolean
IIOP_ORB::_optimize_collocation_objects (void)
{
  return optimize_collocation_objects_;
}
