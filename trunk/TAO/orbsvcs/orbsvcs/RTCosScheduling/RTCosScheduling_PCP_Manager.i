/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   PCP_Manager.h
 *
 *  $Id$
 *
 *  @author Matt Murphy <murphym@cs.uri.edu>
 *  @author based upon work by Greg Cooper
 *  @author University of Rhode Island
 */
//=============================================================================

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
