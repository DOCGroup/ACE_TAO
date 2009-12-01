// -*- C++ -*-
// $Id$

#include "StateListenerControl.h"

CCM_DDS_StateListenerControl_i::CCM_DDS_StateListenerControl_i (
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::ListenerMode> &mode,
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::DataNumber_t> &max_delivered_data,
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CORBA::Boolean> &is_filter_interpreted)
    : mode_ (mode),
      max_delivered_data_ (max_delivered_data),
      is_filter_interpreted_ (is_filter_interpreted)
{
}

CCM_DDS_StateListenerControl_i::~CCM_DDS_StateListenerControl_i (void)
{
}

::CCM_DDS::ListenerMode
CCM_DDS_StateListenerControl_i::mode (void)
{
  return mode_.value ();
}

void
CCM_DDS_StateListenerControl_i::mode (::CCM_DDS::ListenerMode mode)
{
  this->mode_ = mode;
}

::CCM_DDS::DataNumber_t
CCM_DDS_StateListenerControl_i::max_delivered_data (void)
{
  return max_delivered_data_.value ();
}

void
CCM_DDS_StateListenerControl_i::max_delivered_data (
  ::CCM_DDS::DataNumber_t max_delivered_data)
{
  this->max_delivered_data_ = max_delivered_data;
}

::CORBA::Boolean
CCM_DDS_StateListenerControl_i::is_filter_interpreted (void)
{
  return this->is_filter_interpreted_.value ();
}

void
CCM_DDS_StateListenerControl_i::is_filter_interpreted (::CORBA::Boolean is_filter_interpreted)
{
  this->is_filter_interpreted_ = is_filter_interpreted;
}

