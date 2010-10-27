// -*- C++ -*-
// $Id$

#include "ace/Guard_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::TP_Synch_Helper::TP_Synch_Helper()
  : state_(PENDING),
    condition_(this->lock_)
{
}


ACE_INLINE
TAO::CSD::TP_Synch_Helper::~TP_Synch_Helper()
{
}


ACE_INLINE
bool
TAO::CSD::TP_Synch_Helper::wait_while_pending()
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->lock_, false);

  while (this->state_ == PENDING)
    {
      this->condition_.wait();
    }

  return (this->state_ == DISPATCHED);
}


ACE_INLINE
void
TAO::CSD::TP_Synch_Helper::dispatched()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);
  this->state_ = DISPATCHED;
  this->condition_.signal();
}


ACE_INLINE
void
TAO::CSD::TP_Synch_Helper::cancelled()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);
  this->state_ = CANCELLED;
  this->condition_.signal();
}

TAO_END_VERSIONED_NAMESPACE_DECL
