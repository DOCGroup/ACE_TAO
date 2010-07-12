// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/DataListenerControl_T.h"
#include "dds4ccm/impl/DataReaderListener_T.h"
#include "dds4ccm/impl/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::DDS_Listen_T (void) :
  data_control_ (new CCM_DDS_DataListenerControl_T
    < ::CCM_DDS::CCM_DataListenerControl, CCM_TYPE> ())
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::~DDS_Listen_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
bool
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::configuration_complete (
  typename CCM_TYPE::base_type::_ptr_type component,
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char* library_name,
  const char* profile_name)
{
  DDS4CCM_TRACE ("DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::configuration_complete");

  if (DDSSubscriberBase_type::configuration_complete (
            component,
            topic,
            subscriber,
            library_name,
            profile_name))
    {
      DataListenerControl_type *dds_dlc = dynamic_cast < DataListenerControl_type * >
        (this->data_control_.in ());
      if (dds_dlc)
        {
          dds_dlc->_set_component (component);
          return true;
        }
      else
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "DDS_Listen_T::configuration_complete - "
                            "Unable to cast DataListenerControl.\n"));
          return false;
        }
    }
  return false;
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::activate (
  typename CCM_TYPE::listener_type::_ptr_type listener,
  ::CCM_DDS::PortStatusListener_ptr status,
  ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::activate");

  if (::CORBA::is_nil (this->listener_.in ()))
    {
      ACE_NEW_THROW_EX (this->listener_,
                        DataReaderListener_type (
                          listener,
                          status,
                          this->data_control_.in (),
                          reactor,
                          &this->condition_manager_),
                        CORBA::NO_MEMORY ());
    }
  this->data_reader_.set_listener (
    this->listener_.in (),
    DataReaderListener_type::get_mask (status));
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::remove (
  ::DDS::Subscriber_ptr subscriber)
{
  DDS4CCM_TRACE ("DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::remove");

  DataListenerControl_type *dds_dlc = dynamic_cast < DataListenerControl_type * >
    (this->data_control_.in ());
  if (dds_dlc)
    {
      dds_dlc->_set_component (CCM_TYPE::base_type::_nil ());
    }
  else
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "DDS_Listen_T::remove - "
                        "Unable to cast DataListenerControl.\n"));
    }

  DDSSubscriberBase_type::remove (subscriber);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
::CCM_DDS::CCM_DataListenerControl_ptr
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_data_control (void)
{
  DDS4CCM_TRACE ("DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_data_control");

  return ::CCM_DDS::CCM_DataListenerControl::_duplicate (this->data_control_.in ());
}

