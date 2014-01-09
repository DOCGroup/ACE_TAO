// -*- C++ -*-
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_MProfile::TAO_MProfile (CORBA::ULong sz)
  :  policy_list_ (0),
     is_policy_list_initialized_ (false),
     forward_from_(0),
     pfiles_ (0),
     current_ (0),
     size_ (0),
     last_ (0)
{
  this->set (sz);
}

ACE_INLINE
TAO_MProfile::TAO_MProfile (const TAO_MProfile &mprofiles)
  :  policy_list_ (0),
     is_policy_list_initialized_ (false),
     forward_from_(0),
     pfiles_ (0),
     current_ (0),
     size_ (0),
     last_ (0)
{
  this->set (mprofiles);
}

ACE_INLINE TAO_MProfile&
TAO_MProfile::operator= (const TAO_MProfile& rhs)
{
  if (this == &rhs)
    return *this;

  this->set (rhs);
  return *this;
}

/// Cyclic get next.  It will simply cycle through the complete list.
ACE_INLINE TAO_Profile *
TAO_MProfile::get_cnext (void)
{
  if (last_ == 0)
    return 0;

  if (current_ == last_)
    current_ = 0;

  return pfiles_[current_++];
}

/// This will return the next element until either null is found or the
/// end of list.  It then continues to return NULL until rewind.
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
    --current_;
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
    --current_;

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

ACE_INLINE TAO_PHandle
TAO_MProfile::get_current_handle (void) const
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
TAO_MProfile::give_profile (TAO_Profile *pfile, int share)
{
  if (share)
    return this->give_shared_profile(pfile);
  // skip by the used slots
  if (last_ == size_) // full!
    return -1;

  pfiles_[last_++] = pfile;

  return last_ - 1;
}

ACE_INLINE
void
TAO_MProfile::forward_from (TAO_MProfile *from)
{
  this->forward_from_ = from;
}

ACE_INLINE
TAO_MProfile *
TAO_MProfile::forward_from (void)
{
  return this->forward_from_;
}

ACE_INLINE CORBA::ULong
TAO_MProfile::profile_count (void) const
{
  return this->last_;
}

ACE_INLINE CORBA::ULong
TAO_MProfile::size (void) const
{
  return this->size_;
}

ACE_INLINE const TAO_Profile*
TAO_MProfile::get_profile (CORBA::ULong slot) const
{
  if (slot >= this->last_)
    return 0;
  return this->pfiles_[slot];
}

ACE_INLINE TAO_Profile **
TAO_MProfile::pfiles (void) const
{
  return this->pfiles_;
}

ACE_INLINE void
TAO_MProfile::policy_list (CORBA::PolicyList *policy_list)
{
  this->policy_list_ = policy_list;
}

TAO_END_VERSIONED_NAMESPACE_DECL
