// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/MProfile.h"
#include "tao/Pluggable.h"

ACE_RCSID(tao, MProfile, "$Id$")

#if !defined (__ACE_INLINE__)
# include "tao/MProfile.i"
#endif /* __ACE_INLINE__ */

int
TAO_MProfile::set (CORBA::ULong sz)
{
  if (sz == 0)
    {
      // We do, so release all of our profiles.
      for (TAO_PHandle h = 0; h < this->size_; h++ )
        if (this->pfiles_[h])
          {
            this->pfiles_[h]->_decr_refcnt ();
            this->pfiles_[h] = 0;
          }

      if (this->size_)
        delete [] this->pfiles_;

      if (this->fwded_mprofile_)
        delete this->fwded_mprofile_;

      this->pfiles_ = 0;
      this->current_ = 0;
      this->size_ = 0;
      this->last_= 0;

      return 0;
    }

  // See if we already have an existing profile list or if we need to
  // get ridof what we have.
  // @@ Fred, please be consistent with your use of this-> as a prefix
  // for data members.  
  if (size_)
    {
      // We do, so release all of our profiles.
      for (TAO_PHandle h = 0; h < size_; h++)
        if (this->pfiles_[h])
          {
            this->pfiles_[h]->_decr_refcnt ();
            this->pfiles_[h] = 0;
          }

      // Next see if we can reuse our profile list memory Since
      if (this->size_ != sz)
        {
          // we cant reuse memory since the array sized are different!
          delete [] this->pfiles_;

          ACE_NEW_RETURN (this->pfiles_,
                          TAO_Profile_ptr[sz],
                          -1);
        }
    }
  else
    // first time, initialize!
    ACE_NEW_RETURN (this->pfiles_,
                    TAO_Profile_ptr [sz],
                    -1);
  // this->pfiles_

  ACE_OS::memset (this->pfiles_,
                  0,
                  sizeof (TAO_Profile_ptr) * sz);
  size_ = sz;
  this->last_ = 0;
  this->current_ = 0;

  // @@ since we are being reset, get rid of fwd references!
  if (fwded_mprofile_)
    delete fwded_mprofile_;

  return size_;
}

int
TAO_MProfile::set (TAO_MProfile *mprofile)
{
  // NOTE: We use mprofile->last_ instead of mprofile->size_ to set
  // this->size_.  This is so we can use set () to trim a profile
  // list!!

  if (mprofile == 0)
    return this->set ((CORBA::ULong) 0);

  this->set (mprofile->last_);

  // set indexes ...
  this->last_ = mprofile->last_;

  // These are set in set (ULong);
  // this->current_ = 0;
  // this->fwded_mprofile_ = 0;

  // Now reference all profiles.
  for (TAO_PHandle h = 0 ; h < this->size_ ; h++ )
    {
      if (mprofile->pfiles_[h])
        {
          this->pfiles_[h] = mprofile->pfiles_[h];
          this->pfiles_[h]->_incr_refcnt ();
        }
    }

  if (mprofile->fwded_mprofile_)
    this->fwded_mprofile_ = mprofile->fwded_mprofile_;

  return 1;
}
