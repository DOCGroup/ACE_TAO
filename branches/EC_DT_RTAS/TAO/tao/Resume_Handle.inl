// -*- C++ -*-
//$Id$

ACE_INLINE
TAO_Resume_Handle::TAO_Resume_Handle (TAO_ORB_Core *orb_core,
                                      ACE_HANDLE h)
  : orb_core_ (orb_core),
    handle_ (h),
    flag_ (TAO_HANDLE_RESUMABLE)
{
}

ACE_INLINE
TAO_Resume_Handle::~TAO_Resume_Handle (void)
{
  if (this->flag_ == TAO_HANDLE_RESUMABLE)
    this->resume_handle ();

  this->orb_core_ = 0;
  this->handle_ = ACE_INVALID_HANDLE;
}


ACE_INLINE void
TAO_Resume_Handle::set_flag (TAO_Handle_Resume_Flag fl)
{
  this->flag_ = fl;
}

ACE_INLINE TAO_Resume_Handle &
TAO_Resume_Handle::operator= (const TAO_Resume_Handle &rhs)
{
  this->orb_core_ = rhs.orb_core_;
  this->handle_ = rhs.handle_;
  this->flag_ = rhs.flag_;

  return *this;
}
