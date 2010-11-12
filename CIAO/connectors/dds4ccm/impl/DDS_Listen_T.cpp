// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/DataListenerControl_T.h"
#include "dds4ccm/impl/DataReaderListener_T.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::DDS_Listen_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::~DDS_Listen_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::set_component (
    typename CCM_TYPE::base_type::_ptr_type component)
{
  DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::set_component (component);
  this->data_control_->_set_component (component);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::activate (
  typename CCM_TYPE::listener_type::_ptr_type listener,
  ::CCM_DDS::PortStatusListener_ptr status,
  ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::activate");

  ::DDS::StatusMask const mask =
    DataReaderListener_type::get_mask (status);

  if (mask != 0)
    {
      if (::CORBA::is_nil (this->listener_.in ()))
        {
          ACE_NEW_THROW_EX (this->listener_,
                            DataReaderListener_type (
                              listener,
                              status,
                              this->data_control_,
                              reactor,
                              this->condition_manager_),
                            ::CORBA::NO_MEMORY ());
        }

      ::DDS::ReturnCode_t const retcode = this->dr_->set_listener (
        this->listener_.in (), mask);

      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Listen_T::activate - "
                        "Error while setting the listener on the listen - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::remove (
  ::DDS::Subscriber_ptr subscriber)
{
  DDS4CCM_TRACE ("DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::remove");

  this->data_control_->_set_component (CCM_TYPE::base_type::_nil ());

  DDSSubscriberBase_type::remove (subscriber);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::CCM_DataListenerControl_ptr
DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::get_data_control (void)
{
  DDS4CCM_TRACE ("DDS_Listen_T<DDS_TYPE, CCM_TYPE, FIXED>::get_data_control");

  return ::CCM_DDS::CCM_DataListenerControl::_duplicate (this->data_control_);
}

