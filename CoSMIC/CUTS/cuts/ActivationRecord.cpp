// $Id$

#include "cuts/ActivationRecord.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/ActivationRecord.inl"
#endif

//=============================================================================
/*
 * CUTS_Activation_Record
 */
//=============================================================================

//
// CUTS_Activation_Record
//
CUTS_Activation_Record::CUTS_Activation_Record (void)
: active_ (false),
  owner_ (-1),
  transit_time_ (ACE_Time_Value::zero),
  queue_time_ (ACE_Time_Value::zero)
{

}

//
// activate
//
void CUTS_Activation_Record::activate (long owner)
{
  this->owner_ = owner;
  this->active_ = true;
  this->start_time_ = ACE_OS::gettimeofday ();
}

//
// ~CUTS_Activation_Record
//
CUTS_Activation_Record::~CUTS_Activation_Record (void)
{

}

//
// reset
//
void CUTS_Activation_Record::reset (void)
{
  this->transit_time_ = ACE_Time_Value::zero;
  this->queue_time_ = ACE_Time_Value::zero;
  this->entries_.clear ();
}

//=============================================================================
/*
 * CUTS_Cached_Activation_Record
 */
//=============================================================================

//
// CUTS_Cached_Activation_Record
//
CUTS_Cached_Activation_Record::CUTS_Cached_Activation_Record (void)
: next_ (0)
{

}

//
// CUTS_Cached_Activation_Record
//
CUTS_Cached_Activation_Record::~CUTS_Cached_Activation_Record (void)
{

}
