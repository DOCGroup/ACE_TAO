// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::DDS_Subscriber_Base_T (void)
  : configuration_complete_ (false)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::~DDS_Subscriber_Base_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::set_component (
    typename CCM_TYPE::base_type::_ptr_type component)
{
  this->dds_read_->_set_component (component);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete (
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char* library_name,
  const char* profile_name)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete");

  this->configuration_complete_ = true;

  if (::CORBA::is_nil (this->dr_.in ()))
    {
      ::DDS::TopicDescription_var td;
      ::CCM_DDS::QueryFilter_var filter = this->cft_setting_->filter ();
      if (ACE_OS::strlen (filter->expression.in ()) > 0)
        {
          ::DDS::ContentFilteredTopic_var cft =
            this->cft_setting_->create_contentfilteredtopic (topic, subscriber);
          if (CORBA::is_nil (cft.in ()))
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR,
                            "DDS_Subscriber_Base_T::configuration_complete: "
                            "Error creating ContentFilteredTopic.\n"));
              throw ::CORBA::INTERNAL ();
            }
          td = ::DDS::TopicDescription::_narrow (cft.in ());
        }
      else
        {
          td = ::DDS::TopicDescription::_narrow (topic);
        }
      if (library_name && profile_name)
        {
          this->dr_ = subscriber->create_datareader_with_profile (
                                          td.in (),
                                          library_name,
                                          profile_name,
                                          ::DDS::DataReaderListener::_nil (),
                                          0);
        }
      else
        {
          ::DDS::DataReaderQos_var drqos;
          this->dr_ = subscriber->create_datareader (
                                          td.in (),
                                          drqos.in (),
                                          ::DDS::DataReaderListener::_nil (),
                                          0);
        }
      this->dds_read_->set_dds_entity (this->dr_.in (),
                                       &this->condition_manager_);
      this->ccm_data_reader_->set_dds_entity (this->dr_.in ());
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::activate (
  ::CCM_DDS::PortStatusListener_ptr status,
  ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::activate");

  ::DDS::StatusMask const mask =
    PortStatusListener_type::get_mask (status);

  if (mask != 0)
    {
      if (::CORBA::is_nil (this->listener_.in ()))
        {
          ACE_NEW_THROW_EX (this->listener_,
                            PortStatusListener_type (status, reactor),
                            ::CORBA::NO_MEMORY ());
        }

      ::DDS::ReturnCode_t const retcode = this->dr_->set_listener (
          this->listener_.in (), mask);

      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Subscriber_Base_T::activate - "
                        "Error while setting the listener on the subscriber - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::passivate ()
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::passivate");

  this->condition_manager_.passivate ();

  if (!::CORBA::is_nil (this->listener_.in ()))
    {
      ::DDS::ReturnCode_t const retcode =
        this->dr_->set_listener (::DDS::DataReaderListener::_nil (), 0);
      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Subscriber_Base_T::passivate - "
                        "Error while setting the listener on the reader - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }

      this->listener_ = ::DDS::DataReaderListener::_nil ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::remove (
  ::DDS::Subscriber_ptr subscriber)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::remove");

  //TODO: reimplement this
//   this->data_reader_->delete_datareader (subscriber);
  this->cft_setting_->delete_contentfilteredtopic (subscriber);
  this->dds_read_->_set_component (CCM_TYPE::base_type::_nil ());
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_data (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_data");

  return CCM_TYPE::reader_type::_duplicate (this->dds_read_);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_dds_entity");

  return ::DDS::CCM_DataReader::_duplicate (this->ccm_data_reader_);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::CCM_ContentFilterSetting_ptr
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_filter_config (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_filter_config");

  return ::CCM_DDS::CCM_ContentFilterSetting::_duplicate (this->cft_setting_);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::QueryFilter *
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::filter (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::filter");

  return this->cft_setting_->filter ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::filter");
  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->cft_setting_->filter (filter);
    }
}
