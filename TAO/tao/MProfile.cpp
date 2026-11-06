// -*- C++ -*-
#include "tao/MProfile.h"
#include "tao/Profile.h"
#include "tao/PolicyC.h"
#include "tao/ORB_Constants.h"
#include "tao/SystemException.h"

#include "ace/Log_Msg.h"
#include "ace/Guard_T.h"
#include "ace/CORBA_macros.h"
#include <new>

#if !defined (__ACE_INLINE__)
# include "tao/MProfile.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_MProfile::~TAO_MProfile ()
{
  if (this->policy_list_ != nullptr)
    {
      CORBA::ULong const len = this->policy_list_->length ();
      for (CORBA::ULong i = 0; i < len; ++i)
        {
          try
            {
              CORBA::Policy_ptr policy = (*this->policy_list_)[i];
              policy->destroy ();
            }
          catch (const ::CORBA::Exception&)
            {
              // Ignore all exceptions to allow other policies to be
              // destroyed.
            }
        }

      delete this->policy_list_;
    }

  this->cleanup ();
}

void
TAO_MProfile::cleanup ()
{
  for (TAO_PHandle i = 0; i < this->last_; ++i)
    {
      if (this->pfiles_[i] != nullptr)
        this->pfiles_[i]->_decr_refcnt ();
    }

  std::vector<TAO_Profile *> ().swap (this->pfiles_);
  this->current_ = 0;
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

  for (TAO_Profile *&pfile : this->pfiles_)
    {
      if (pfile != nullptr)
        {
          pfile->_decr_refcnt ();
          pfile = nullptr;
        }
    }

  if (static_cast<size_t> (sz) > this->pfiles_.max_size ())
    return -1;

  if (this->pfiles_.size () < sz)
    {
      try
        {
          this->pfiles_.resize (sz, nullptr);
        }
      catch (std::bad_alloc const&)
        {
          return -1;
        }
    }

  this->last_ = 0;
  this->current_ = 0;
  return static_cast<int> (this->pfiles_.size ());
}

int
TAO_MProfile::set (const TAO_MProfile &mprofile)
{
  // Copy only the used profiles, not the source's unused capacity.
  if (this->set (mprofile.last_) < 0)
    return -1;

  // set indexes ...
  this->last_ = mprofile.last_;

  // These are set in set (ULong);
  // this->current_ = 0;
  // this->forward_from_ = 0;

  // Now reference all profiles.
  for (TAO_PHandle h = 0; h < this->last_; ++h)
    {
      this->pfiles_[h] = mprofile.pfiles_[h];
      if (this->pfiles_[h] != nullptr)
        this->pfiles_[h]->_incr_refcnt ();
    }

  return 1;
}

// Not thread safe!
int
TAO_MProfile::grow (CORBA::ULong sz)
{
  if (sz <= this->pfiles_.size ())
    return 0;

  if (static_cast<size_t> (sz) > this->pfiles_.max_size ())
    return -1;

  try
    {
      this->pfiles_.resize (sz, nullptr);
    }
  catch (std::bad_alloc const&)
    {
      return -1;
    }

  return 0;
}

int
TAO_MProfile::add_profile (TAO_Profile *pfile)
{
  // skip by the used slots
  if (last_ == this->pfiles_.size ()) // full!
    {
      if (this->grow (static_cast<CORBA::ULong> (this->pfiles_.size () + 1)) < 0)
        return -1;
    }

  pfiles_[last_++] = pfile;

  if (pfile && pfile->_incr_refcnt () == 0)
    TAOLIB_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Unable to increment reference ")
                       ACE_TEXT ("count in add_profile!\n")),
                      -1);

  return last_ - 1;
}

int
TAO_MProfile::add_profiles (TAO_MProfile *pfiles)
{
  // this->pfiles_.size() == total number of profiles we can hold
  // this->last_ == the index of the last profile
  CORBA::ULong const space =
    static_cast<CORBA::ULong> (this->pfiles_.size ()) - this->last_;

  if (space < pfiles->last_)
    {
      // we need to grow!
     if (this->grow (this->last_ + pfiles->last_) < 0)
       return -1;
    }

  // copy over profiles
  for (TAO_PHandle h = 0; h < pfiles->last_; ++h)
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
// this->size () = 7; current_ = 3; last_ = 7
//   0, 1, 2, 3, 4, 5, 6}
// { _, _, _, _, ..., _}
//
int
TAO_MProfile::remove_profile (const TAO_Profile *pfile)
{
  TAO_PHandle h;
  int found = 0;
  for (h = 0; h < this->last_; ++h)
    {
      if (this->pfiles_[h]->is_equivalent (pfile))
        { // remove it!
          TAO_Profile *old = this->pfiles_[h];
          this->pfiles_[h] = nullptr;
          old->_decr_refcnt ();
          // shift other profiles up one
          // note, if h == last_ - 1 then do nothing.
          for (TAO_PHandle inner = h; inner < this->last_ - 1; ++inner)
            {
              this->pfiles_[inner] = this->pfiles_[inner + 1];
            }
          // subtract 1 from last_ to indicate we have one fewer profiles
          this->last_--;
          this->pfiles_[this->last_] = nullptr;
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
  for (TAO_PHandle h = 0; h < pfiles->last_; ++h)
    {
      if (this->remove_profile (pfiles->pfiles_[h]) < 0)
        return -1;
    }
  return 0;
}

CORBA::Boolean
TAO_MProfile::is_equivalent (const TAO_MProfile *rhs)
{
  // Two profile lists are equivalent if at least one of the profiles
  // from the first list is_equivalent to at least one of the profiles
  // from the second list!!
  for (TAO_PHandle h1 = 0; h1 < this->last_; ++h1)
    for (TAO_PHandle h2 = 0; h2 < rhs->last_; ++h2)
      if (this->pfiles_[h1]->is_equivalent (rhs->pfiles_[h2]))
        return 1;

  return 0;
}

CORBA::ULong
TAO_MProfile::hash (CORBA::ULong max)
{
  CORBA::ULong hashval = 0;

  if (this->last_ == 0)
    return 0;

  for (TAO_PHandle h = 0; h < this->last_ ; ++h)
    {
      hashval += pfiles_[h]->hash (max);
    }

  // The above hash function return an ULong between 0 and max here we
  // simply take the average value and round.
  //return hashval / this->last_;
  // Changed to a mod value instead of an average.
  return hashval % max;
}

void
TAO_MProfile::create_policy_list ()
{
  ACE_NEW_THROW_EX (this->policy_list_,
                    CORBA::PolicyList,
                    CORBA::NO_MEMORY (0,
                                      CORBA::COMPLETED_NO));
}

void
TAO_MProfile::init_policy_list ()
{
  // The first time this method is called
  // it causes the initialization of the policies
  // for the current profile.

  this->get_current_profile ()->get_policies (*this->policy_list_);

  this->is_policy_list_initialized_ = true;
}

CORBA::PolicyList *
TAO_MProfile::policy_list ()
{
  if (!this->is_policy_list_initialized_)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                        guard,
                        this->mutex_,
                        nullptr);

      if (this->policy_list_ == nullptr)
        {
          this->create_policy_list ();

          this->init_policy_list ();
        }
    }
  CORBA::PolicyList *ret_val = nullptr;
  ACE_NEW_THROW_EX (ret_val,
                    CORBA::PolicyList (*this->policy_list_),
                    CORBA::NO_MEMORY (0,
                                      CORBA::COMPLETED_NO));

  return ret_val;
}

int
TAO_MProfile::give_shared_profile (TAO_Profile *pfile)
{
  for (unsigned i = 0; i < this->last_; i++)
    if (pfile->tag() == this->pfiles_[i]->tag() &&
        pfile->compare_key(this->pfiles_[i]))
      {
        this->pfiles_[i]->add_generic_endpoint(pfile->endpoint());
        pfile->_decr_refcnt();
        return i;
      }
  return this->give_profile(pfile, 0);
}


TAO_END_VERSIONED_NAMESPACE_DECL
