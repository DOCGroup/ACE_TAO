// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/MProfile.h"
#include "tao/Pluggable.h"

ACE_RCSID(tao, MProfile, "$Id$")

#if !defined (__ACE_INLINE__)
# include "tao/MProfile.i"
#endif /* __ACE_INLINE__ */

void
TAO_MProfile::cleanup (void)
{
  if (this->pfiles_ != 0)
    {
      for (TAO_PHandle i = 0; i < this->last_; ++i)
        if (this->pfiles_[i])
          this->pfiles_[i]->_decr_refcnt ();
      delete [] this->pfiles_;
      this->pfiles_ = 0;
    }

  this->current_ = 0;
  this->size_ = 0;
  this->last_ = 0;
}

int
TAO_MProfile::set (CORBA::ULong sz)
{
  if (sz == 0)
    {
      this->cleanup ();
      return 0;
    }

  // See if we already have an existing profile list or if we need to
  // get ridof what we have.  
  if (this->size_ != 0)
    {
      // Release all of our profiles.

      for (TAO_PHandle h = 0;
           h < this->size_;
           h++)
        if (this->pfiles_[h])
          {
            this->pfiles_[h]->_decr_refcnt ();
            this->pfiles_[h] = 0;
          }

      // Next see if we can reuse our profile list memory
      if (this->size_ != sz)
        {
          // we cant reuse memory since the array sized are different!
          // @@ Fred: if sz < this->size_ you could avoid this memory
          //    allocation, you only need another flag to keep the
          //    "capacity".
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

  this->size_ = sz;
  this->last_ = 0;
  this->current_ = 0;

  for (TAO_PHandle i = 0; i != this->size_; ++i)
    this->pfiles_[i] = 0;

  return this->size_;
}

int
TAO_MProfile::set (const TAO_MProfile &mprofile)
{
  // NOTE: We use mprofile->last_ instead of mprofile->size_ to set
  // this->size_.  This is so we can use set () to trim a profile
  // list!!

  this->set (mprofile.last_);

  // set indexes ...
  this->last_ = mprofile.last_;

  // These are set in set (ULong);
  // this->current_ = 0;
  // this->forward_from_ = 0;

  // Now reference all profiles.

  for (TAO_PHandle h = 0; h < this->size_; h++)
    {
      this->pfiles_[h] = mprofile.pfiles_[h];
      if (this->pfiles_[h] != 0)
        this->pfiles_[h]->_incr_refcnt ();
    }

  return 1;
}

int
TAO_MProfile::add_profile (TAO_Profile *pfile)
{
  // skip by the used slots
  if (last_ == size_) // full!
    return -1;

  pfiles_[last_++] = pfile;

  if (pfile && pfile->_incr_refcnt () == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Unable to increment reference count in add_profile!\n"),
                      -1);
  return last_ - 1;
}

CORBA::Boolean
TAO_MProfile::is_equivalent (TAO_MProfile *first,
                             TAO_MProfile *second,
                             CORBA::Environment &env)
{
  // Two profile lists are equivalent iff at least one of the profiles
  // form the first list is_equivalent to at least one of the profiles
  // from the second list!!
  TAO_Profile_ptr *pfiles1 = first->pfiles ();
  TAO_Profile_ptr *pfiles2 = second->pfiles ();
  TAO_PHandle first_cnt = first->profile_count ();
  TAO_PHandle second_cnt = second->profile_count ();

  for (TAO_PHandle h1 = 0; h1 < first_cnt;h1++)
    for (TAO_PHandle h2 = 0; h2 < second_cnt; h2++ )
      if (pfiles1[h1]->is_equivalent (pfiles2[h2], env))
        return 1;

  return 0;
}

CORBA::ULong
TAO_MProfile::hash (CORBA::ULong max, CORBA::Environment &env)
{
  CORBA::ULong hashval = 0;

  if (last_ == 0)
    return 0;

  for (TAO_PHandle h=0; h < last_ ; h++)
    hashval += pfiles_[h]->hash (max, env);

  // The above hash function return an ULong between 0 and max here we
  // simply take the average value and round.
  return hashval / last_;
}
