// -*- C++ -*-
// $Id$

#include "DataListenerControl.h"

CCM_DDS_DataListenerControl_i::CCM_DDS_DataListenerControl_i (
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::ListenerMode> &mode,
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::DataNumber_t> &max_delivered_data)
    : mode_ (mode),
      max_delivered_data_ (max_delivered_data)
{
}

CCM_DDS_DataListenerControl_i::~CCM_DDS_DataListenerControl_i (void)
{
}

::CCM_DDS::ListenerMode
CCM_DDS_DataListenerControl_i::mode (void)
{
  return mode_.value ();
}

void
CCM_DDS_DataListenerControl_i::mode (::CCM_DDS::ListenerMode mode)
{
  this->mode_ = mode;
}

::CCM_DDS::DataNumber_t
CCM_DDS_DataListenerControl_i::max_delivered_data (void)
{
  return max_delivered_data_.value ();
}

void
CCM_DDS_DataListenerControl_i::max_delivered_data (
  ::CCM_DDS::DataNumber_t max_delivered_data)
{
  this->max_delivered_data_ = max_delivered_data;
}

