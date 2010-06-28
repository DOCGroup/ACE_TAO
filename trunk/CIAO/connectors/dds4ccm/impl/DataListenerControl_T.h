// -*- C++ -*-
// $Id$

#ifndef DATALISTENERCONTROL_T_H_
#define DATALISTENERCONTROL_T_H_

#include "dds4ccm/idl/dds4ccm_DataListenerControlEC.h"
#include "tao/LocalObject.h"
#include "ace/Copy_Disabled.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <typename BASE_TYPE, typename CCM_TYPE>
class CCM_DDS_DataListenerControl_T
  : public virtual BASE_TYPE,
    public virtual ::CORBA::LocalObject,
    private ACE_Copy_Disabled
{
public:
  /// Constructor
  CCM_DDS_DataListenerControl_T (void);

  /// Destructor
  virtual ~CCM_DDS_DataListenerControl_T (void);

  /// Set the mode
  virtual ::CCM_DDS::ListenerMode mode (void);

  /// Get the mode
  virtual void mode (::CCM_DDS::ListenerMode mode);

  /// Set the max_delivered_data
  virtual ::CCM_DDS::DataNumber_t max_delivered_data (void);

  /// Get the max_delivered_data
  virtual void max_delivered_data (::CCM_DDS::DataNumber_t max_delivered_data);

  /// Set the component
  void _set_component (typename CCM_TYPE::base_type::_ptr_type component);
  virtual ::CORBA::Object_ptr _get_component (void);

protected:
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::ListenerMode> mode_;
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::DataNumber_t> max_delivered_data_;

private:
  typename CCM_TYPE::base_type::_var_type component_;
};

#include "dds4ccm/impl/DataListenerControl_T.cpp"

#endif /* DATALISTENERCONTROL_T_H_ */

