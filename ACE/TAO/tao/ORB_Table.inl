// -*- C++ -*-
//
// $Id$

#include "tao/ORB_Core.h"

#include <algorithm>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE ::TAO_ORB_Core *
TAO::ORB_Table::first_orb (void)
{
  return this->first_orb_;
}

ACE_INLINE TAO::ORB_Table::iterator
TAO::ORB_Table::begin (void)
{
  return this->table_.begin ();
}

ACE_INLINE TAO::ORB_Table::iterator
TAO::ORB_Table::end (void)
{
  return this->table_.end ();
}

ACE_INLINE ::TAO_ORB_Core* const *
TAO::ORB_Table::get_orbs (size_t& num_orbs)
{
  num_orbs = this->num_orbs_;
  return this->orbs_;
}

/// Accessor to the underlying table_
ACE_INLINE TAO::ORB_Table::Table *
TAO::ORB_Table::table (void)
{
  return &this->table_;
}

ACE_INLINE TAO_SYNCH_MUTEX &
TAO::ORB_Table::lock (void)
{
  return this->lock_;
}

// ****************************************************************

ACE_INLINE
TAO::ORB_Core_Ref_Counter::ORB_Core_Ref_Counter (void)
  : core_ (0)
{
}

ACE_INLINE
TAO::ORB_Core_Ref_Counter::ORB_Core_Ref_Counter (::TAO_ORB_Core * core)
  : core_ (core)
{
//   ACE_ASSERT (core != 0);

  // Note that the TAO_ORB_Core pointer should always be valid in this
  // constructor.

  // Claim ownership.
  (void) this->core_->_incr_refcnt ();
}

ACE_INLINE
TAO::ORB_Core_Ref_Counter::~ORB_Core_Ref_Counter (void)
{
  if (this->core_)
    (void) this->core_->_decr_refcnt (); // Should never throw.
}

ACE_INLINE
TAO::ORB_Core_Ref_Counter::ORB_Core_Ref_Counter (
  TAO::ORB_Core_Ref_Counter const & rhs)
  : core_ (rhs.core_)
{
  // Claim ownership.
  if (this->core_)
    (void) this->core_->_incr_refcnt ();
}

ACE_INLINE void
TAO::ORB_Core_Ref_Counter::operator= (TAO::ORB_Core_Ref_Counter const & rhs)
{
  // Strongly exception safe.  May not be strictly necessary, but
  // let's do things the right way, regardless.
  TAO::ORB_Core_Ref_Counter tmp (rhs);
  std::swap (this->core_, tmp.core_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
