// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/MProfile.h"
#include "tao/Pluggable.h"

ACE_INLINE
TAO_MProfile::TAO_MProfile (CORBA::ULong sz)
  :  fwded_mprofile_(0),
     pfiles_ (0),
     current_ (0),
     size_ (0),
     last_ (0)
{
  this->set (sz);
}

ACE_INLINE
TAO_MProfile::TAO_MProfile (TAO_MProfile *mprofiles)
  :  fwded_mprofile_(0),
     pfiles_ (0),
     current_ (0),
     size_ (0),
     last_ (0)
{
  this->set (mprofiles);
}

ACE_INLINE
TAO_MProfile::~TAO_MProfile (void)
 {
  if (this->pfiles_)
    for (TAO_PHandle h = 0; h < last_; h++)
      if (this->pfiles_[h])
        this->pfiles_[h]->_decr_refcnt ();

  delete [] pfiles_;
  pfiles_ = 0;

}

// cyclic get next.  It will simply cycle through the complete list.

ACE_INLINE TAO_Profile *
TAO_MProfile::get_cnext (void)
{
  if (last_ == 0)
      return 0;

  if (current_ == last_)
      current_ = 0;

  return pfiles_[current_++];
}

// This will return the next element until either null is found or the
// end of list.  It then continues to return NULL until rewind.

ACE_INLINE TAO_Profile *
TAO_MProfile::get_next (void)
{
  // Nolist or EndOfList
  if (last_ == 0 || current_ == last_)
    return 0;
  else
    return pfiles_[current_++];
}

ACE_INLINE TAO_Profile *
TAO_MProfile::get_cprev (void)
{
  if (last_ == 0)
    return 0;
  else if (last_ == 1)
    current_=1;
  else if (current_ > 1)
    current_--;
  else // current_ == 0 or 1, 0 => list never read before and == 1
    current_ = last_;

  return pfiles_[current_ - 1];
}

ACE_INLINE TAO_Profile *
TAO_MProfile::get_prev (void)
{
  if (last_ == 0 || current_ <= 1)
    // No List of BeginningOfList
    return 0;
  if (current_ > 1)
    current_--;
 
  return pfiles_[current_ - 1];
}

// does not affect the current_ setting!

ACE_INLINE TAO_Profile *
TAO_MProfile::get_profile (TAO_PHandle handle)
{
  if (handle < last_)
    return pfiles_[handle];
  else
    return 0;
}
  
ACE_INLINE TAO_Profile *
TAO_MProfile::get_current_profile (void)
{
  if (last_ == 0) 
    return 0;
  if (current_ == 0)
    // means list has not been read before.
    current_ = 1;

  return pfiles_[current_ - 1];
}

ACE_INLINE TAO_PHandle 
TAO_MProfile::get_current_handle (void)
{
  if (current_ > 0)
    return current_ - 1;
  else 
    return 0;
}

ACE_INLINE void 
TAO_MProfile::rewind (void)
{
  current_ = 0;
}

ACE_INLINE int 
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

ACE_INLINE int 
TAO_MProfile::give_profile (TAO_Profile *pfile)
{
  // skip by the used slots
  if (last_ == size_) // full!
    return -1;
   
  pfiles_[last_++] = pfile;

  return last_ - 1;
}

ACE_INLINE void
TAO_MProfile::fwded_mprofile (TAO_MProfile *fwded)
{
  this->fwded_mprofile_ = fwded;
}

ACE_INLINE TAO_MProfile *
TAO_MProfile::fwded_mprofile (void)
{
  return this->fwded_mprofile_;
}

ACE_INLINE CORBA::ULong
TAO_MProfile::profile_count (void)
{
  return this->last_;
}

ACE_INLINE TAO_Profile_ptr *
TAO_MProfile::pfiles (void) const
{
  return this->pfiles_;
}

ACE_INLINE CORBA::Boolean
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

ACE_INLINE CORBA::ULong
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
