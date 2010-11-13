// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::DDS_Subscriber_Base_T (void)
  : configuration_complete_ (false)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::~DDS_Subscriber_Base_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::set_component (
    ::CORBA::Object_ptr component)
{
  this->dds_read_->_set_component (component);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::configuration_complete (
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char* library_name,
  const char* profile_name)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::configuration_complete");

  this->configuration_complete_ = true;

  ::DDS::DataReader_var dr = this->dds_read_->get_dds_reader ();
  if (::CORBA::is_nil (dr.in ()))
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
          dr = subscriber->create_datareader_with_profile (
                                          td.in (),
                                          library_name,
                                          profile_name,
                                          ::DDS::DataReaderListener::_nil (),
                                          0);
        }
      else
        {
          ::DDS::DataReaderQos_var drqos;
          dr = subscriber->create_datareader (
                                          td.in (),
                                          drqos.in (),
                                          ::DDS::DataReaderListener::_nil (),
                                          0);
        }
      this->dds_read_->set_dds_reader (dr.in (),
                                       &this->condition_manager_);
      this->ccm_data_reader_->set_dds_entity (dr.in ());
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::activate (
  ::CCM_DDS::PortStatusListener_ptr status,
  ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::activate");

  ::DDS::StatusMask const mask =
      ::CIAO::DDS4CCM::PortStatusListener::get_mask (status);

  if (mask != 0)
    {
      if (::CORBA::is_nil (this->listener_.in ()))
        {
          ACE_NEW_THROW_EX (this->listener_,
                            ::CIAO::DDS4CCM::PortStatusListener (status, reactor),
                            ::CORBA::NO_MEMORY ());
        }

      ::DDS::DataReader_var dr = this->dds_read_->get_dds_reader ();
      if (!::CORBA::is_nil (dr.in ()))
        {
          ::DDS::ReturnCode_t const retcode = dr->set_listener (
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
      else
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Subscriber_Base_T::activate - "
                        "Error while retrieving the DataReader\n"));
          throw ::CORBA::INTERNAL ();
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::passivate ()
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::passivate");

  this->condition_manager_.passivate ();
  ::DDS::DataReader_var reader = this->dds_read_->get_dds_reader ();
  if (!::CORBA::is_nil (this->listener_.in ()) &&
      !::CORBA::is_nil (reader.in ()) )
    {
      ::DDS::ReturnCode_t const retcode =
        reader->set_listener (::DDS::DataReaderListener::_nil (), 0);
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

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::remove (
  ::DDS::Subscriber_ptr subscriber)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::remove");

  ::DDS::ReturnCode_t const retval =
    subscriber->delete_datareader (this->dds_read_->get_dds_reader ());
  if (retval != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    ACE_TEXT ("DDS_Subscriber_Base_T::remove - ")
                    ACE_TEXT ("Unable to delete DataReader: <%C>\n"),
                    ::CIAO::DDS4CCM::translate_retcode (retval)));
      throw ::CORBA::INTERNAL ();
    }
  this->cft_setting_->delete_contentfilteredtopic (subscriber);
  this->dds_read_->_set_component (::CORBA::Object::_nil ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::data_type::_ptr_type
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::get_data (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::get_data");

  return CCM_TYPE::data_type::_duplicate (this->dds_read_);
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::dds_entity_type::_ptr_type
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::get_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::get_dds_entity");

  return CCM_TYPE::dds_entity_type::_duplicate (this->ccm_data_reader_);
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::filter_config_type::_ptr_type
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::get_filter_config (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::get_filter_config");

  return ::CCM_DDS::CCM_ContentFilterSetting::_duplicate (this->cft_setting_);
}

template <typename DDS_TYPE, typename CCM_TYPE>
::CCM_DDS::QueryFilter *
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::filter (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::filter");

  return this->cft_setting_->filter ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE>::filter");
  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->cft_setting_->filter (filter);
    }
}
