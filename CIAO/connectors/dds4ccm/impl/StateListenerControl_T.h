// -*- C++ -*-
// $Id$

#ifndef STATELISTENERCONTROL_T_H_
#define STATELISTENERCONTROL_T_H_

#include "dds4ccm/impl/DataListenerControl_T.h"
#include "dds4ccm/idl/dds4ccm_StateListenerControlEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <typename BASE_TYPE, typename CCM_TYPE>
class CCM_DDS_StateListenerControl_T
  : public virtual CCM_DDS_DataListenerControl_T<BASE_TYPE, CCM_TYPE>
{
public:
  /// Constructor
  CCM_DDS_StateListenerControl_T (void);

  /// Destructor
  virtual ~CCM_DDS_StateListenerControl_T (void);

  /// Set the is_filter_interpreted
  virtual ::CORBA::Boolean is_filter_interpreted (void);

  /// Get the is_filter_interpreted
  virtual void is_filter_interpreted (::CORBA::Boolean is_filter_interpreted);

  /// Set the component
  void _set_component (typename CCM_TYPE::base_type::_ptr_type component);
  virtual ::CORBA::Object_ptr _get_component (void);

private:
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CORBA::Boolean> is_filter_interpreted_;
  typename CCM_TYPE::base_type::_var_type component_;
};

#include "dds4ccm/impl/StateListenerControl_T.cpp"

#endif /* STATELISTENERCONTROL_T_H_ */

