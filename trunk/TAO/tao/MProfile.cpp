// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/MProfile.h"
#include "tao/Pluggable.h"
#include "tao/Environment.h"

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
      if (this->size_ < sz)
        {
          // we cant reuse memory since the current array is too small!
          delete [] this->pfiles_;

          ACE_NEW_RETURN (this->pfiles_,
                          TAO_Profile *[sz],
                          -1);
          this->size_ = sz;
        }
      // else , leave this->size and this->pfiles alone!
    }
  else
    {
      // first time, initialize!
      ACE_NEW_RETURN (this->pfiles_,
                      TAO_Profile *[sz],
                      -1);
      this->size_ = sz;
    }

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
TAO_MProfile::add_profiles (TAO_MProfile *pfiles)
{
  // this->size_ == total number of profiles we can hold
  // this->last_ == the index of the last profile
  CORBA::ULong space = this->size_ - this->last_;

  if (space < pfiles->last_)
    {
      // we need to grow!
     if (this->grow (this->last_ + pfiles->last_) < 0)
       return -1;
    }

  // copy over profiles
  for (TAO_PHandle h = 0;h < pfiles->last_;h++)
    {
      if (this->add_profile (pfiles->pfiles_[h]) < 0)
        return -1;
    }
  return 0;
}


// the layout for a full list of 7 Profiles.
//
// last_ == one past the last valid entry, so if the list has 2
//          profiles then last_ equals 2.
// current_ == index of the next profile to be returned (or one past
//             the last returned.
//
// this->size_ = 7; current_ = 3; last_ = 7
//   0, 1, 2, 3, 4, 5, 6}
// { _, _, _, _, ..., _}
//
int
TAO_MProfile::remove_profile (const TAO_Profile *pfile)
{
  TAO_PHandle h;
  int found=0;
  for (h = 0;h < this->last_;h++)
    {
      if (this->pfiles_[h]->is_equivalent (pfile))
        { // remove it!
          TAO_Profile *old = this->pfiles_[h];
          this->pfiles_[h] = 0;
          old->_decr_refcnt ();
          // shift other profiles up one
          // note, if h == last_ - 1 then do nothing.
          for (TAO_PHandle inner = h;inner < this->last_ - 1;inner++)
            {
              this->pfiles_[inner] = this->pfiles_[inner + 1];
            }
          // subtract 1 from last_ to indicate we have one fewer profiles
          this->last_--;
          found = 1;
          break;
        }
    }
  if ( found == 0)
    return -1; // profile not found.
  return 0;
}

int
TAO_MProfile::remove_profiles (const TAO_MProfile *pfiles)
{
  for (TAO_PHandle h = 0;h < pfiles->last_;h++)
    {
      if (this->remove_profile (pfiles->pfiles_[h]) < 0)
        return -1;
    }
  return 0;
}

CORBA::Boolean
TAO_MProfile::is_equivalent (const TAO_MProfile *rhs)
{
  // Two profile lists are equivalent iff at least one of the profiles
  // form the first list is_equivalent to at least one of the profiles
  // from the second list!!
  for (TAO_PHandle h1 = 0; h1 < this->last_;h1++)
    for (TAO_PHandle h2 = 0; h2 < rhs->last_; h2++ )
      if (this->pfiles_[h1]->is_equivalent (rhs->pfiles_[h2]))
        return 1;

  return 0;
}

CORBA::ULong
TAO_MProfile::hash (CORBA::ULong max, CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::ULong hashval = 0;

  if (last_ == 0)
    return 0;

  for (TAO_PHandle h=0; h < last_ ; h++)
    {
      hashval += pfiles_[h]->hash (max, ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }

  // The above hash function return an ULong between 0 and max here we
  // simply take the average value and round.
  return hashval / last_;
}

void
TAO_MProfile::init_policy_list (void)
{
  // The first time this method is called
  // it causes the initialization of the policies
  // for the current profile.

  this->get_current_profile ()->policies ();
  this->is_policy_list_initialized_ = 1;
}

CORBA::PolicyList*
TAO_MProfile::policy_list (void)
{
  if (this->is_policy_list_initialized_)
    return this->policy_list_;
  else
    {
      ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX,
                        guard,
                        this->mutex_,
                        0);
      this->create_policy_list ();
      this->init_policy_list ();
    }

  return this->policy_list_;
}
