// -*- C++ -*-

//=============================================================================
/**
 *  @file   RTCosScheduling_PCP_Manager.inl
 *
 *  $Id$
 *
 *  @author Matt Murphy <murphym@cs.uri.edu>
 *  @author based upon work by Greg Cooper
 *  @author University of Rhode Island
 */
//=============================================================================

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * Returns a pointer to the node containing the highest ceiling (the
 * first node in the list of held locks)
 */
ACE_INLINE TAO::CosSchedulingLockNode *
TAO::CosSchedulingLockList::highest_ceiling()
{
  return this->granted_->next();
}


/**
 * Returns a pointer to the node with the highest priority
 * (from the first node in the list of pending locks
 */
ACE_INLINE TAO::CosSchedulingLockNode *
TAO::CosSchedulingLockList::highest_priority()
{
  return this->pending_->next();
}


/**
 * Returns the mThreadID data member
 */
ACE_INLINE int
TAO::PCP_Manager::threadID()
{
  return this->threadID_;
}


/**
 * Creates a new PCP manager object using the lists and
 * synchronization objects found in shared memory.
 */
ACE_INLINE TAO::PCP_Manager
TAO::PCP_Manager_Factory::New_PCP_Manager(RTCORBA::Current_var current)
{
  return TAO::PCP_Manager(this->locks_, &this->mutex_, current);
}

ACE_INLINE struct TAO::CosSchedulingLockNode *
TAO::CosSchedulingLockNode::next()
{
  /// INT_MAX is a special value indicating the end of a list
  if (this->next_offset_ == INT_MAX)
    {
      return 0;
    }
  else
    {
      return this + this->next_offset_;
    }
}

ACE_INLINE void
TAO::CosSchedulingLockNode::next(const struct CosSchedulingLockNode *next_lock)
{
  /// INT_MAX is a special value indicating the end of a list
  if (next_lock == 0)
    {
      this->next_offset_ = INT_MAX;
    }
  else
    {
      this->next_offset_ = next_lock - this;
    }
}

ACE_INLINE const TAO::CosSchedulingLockNode&
TAO::CosSchedulingLockNode::operator=(const CosSchedulingLockNode& L)
{
  this->threadID_           = L.threadID_;
  this->priority_ceiling_   = L.priority_ceiling_;
  this->priority_           = L.priority_;
  this->elevated_priority_  = L.elevated_priority_;

  return *this;
}

TAO_END_VERSIONED_NAMESPACE_DECL
