// -*- C++ -*-

//=============================================================================
/**
 *  @file   Storable_Base.inl
 *
 *  $Id$
 *
 *  @author Bruce Trask <trask_b@ociweb.com>
 *  @author Chanaka Liyanaarachchi <chanaka@ociweb.com>
 *  @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#include "tao/Storable_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::Storable_Base::Storable_Base (bool use_backup)
  : use_backup_ (use_backup)
  , state_ (goodbit)
{
}

ACE_INLINE
TAO::Storable_Base::~Storable_Base ()
{
}

ACE_INLINE void
TAO::Storable_Base::clear (TAO::Storable_Base::Storable_State state)
{
  this->state_ = state;
}

ACE_INLINE void
TAO::Storable_Base::setstate (TAO::Storable_Base::Storable_State state)
{
  this->clear (static_cast <TAO::Storable_Base::Storable_State> (
               this->rdstate () | state));
}

ACE_INLINE TAO::Storable_Base::Storable_State
TAO::Storable_Base::rdstate (void) const
{
  return this->state_;
}

ACE_INLINE bool
TAO::Storable_Base::good (void) const
{
  return (this->state_ == goodbit);
}

ACE_INLINE bool
TAO::Storable_Base::bad (void) const
{
  return (this->state_ & badbit);
}

ACE_INLINE bool
TAO::Storable_Base::eof (void) const
{
  return (this->state_ & eofbit);
}

ACE_INLINE bool
TAO::Storable_Base::fail (void) const
{
  return (this->state_ & failbit);
}

TAO_END_VERSIONED_NAMESPACE_DECL
