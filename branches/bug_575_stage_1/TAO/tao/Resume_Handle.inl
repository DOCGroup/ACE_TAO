// -*- C++ -*-
//$Id$

TAO_Resume_Handle::TAO_Resume_Handle (TAO_ORB_Core *orb_core,
                                      ACE_HANDLE h)
  : orb_core_ (orb_core),
    handle_ (h),
    flag_ (0)
{
}

TAO_Resume_Handle::~TAO_Resume_Handle (void)
{
  if (this->flag_ == 0)
    this->resume_handle ();

  this->orb_core_ = 0;
  this->flag_ = 0;
  this->handle_ = ACE_INVALID_HANDLE;
}
