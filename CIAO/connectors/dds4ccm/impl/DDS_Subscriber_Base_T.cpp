// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::DDS_Subscriber_Base_T (void)
  : configuration_complete_ (false)
{
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::~DDS_Subscriber_Base_T (void)
{
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
void
DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::set_component (
    ::CORBA::Object_ptr component)
{
  this->dds_read_->_set_component (component);
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
void
DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::configuration_complete (
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char * qos_profile)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::configuration_complete");

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
#if (CIAO_DDS4CCM_NDDS==1)
      if (qos_profile)
        {
          dr = subscriber->create_datareader_with_profile (
                                          td.in (),
                                          qos_profile,
                                          ::DDS::DataReaderListener::_nil (),
                                          0);
        }
      else
#else
        ACE_UNUSED_ARG (qos_profile);
#endif
        {
          ::DDS::DataReaderQos_var drqos;
          dr = subscriber->create_datareader (
                                          td.in (),
                                          drqos.in (),
                                          ::DDS::DataReaderListener::_nil (),
                                          0);
        }
      ::DDS::ReturnCode_t retcode = dr->enable ();
      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Subscriber_Base_T::configuration_complete - "
                        "Error: Unable to enable the datareader: <%C>.\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }

      this->condition_manager_.set_dds_entity (dr.in ());
      this->dds_read_->set_dds_reader (dr.in (),
                                       &this->condition_manager_);
      this->ccm_data_reader_->set_dds_entity (dr.in ());
    }
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
void
DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::activate (
  ::CCM_DDS::PortStatusListener_ptr status,
  ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::activate");

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

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
void
DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::passivate ()
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::passivate");

  this->condition_manager_.passivate ();
  ::DDS::DataReader_var reader = this->dds_read_->get_dds_reader ();
  ::DDS::DataReaderListener_var reader_listener = this->listener_._retn ();
  if (!::CORBA::is_nil (reader_listener.in ()) &&
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
    }
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
void
DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::remove (
  ::DDS::Subscriber_ptr subscriber)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::remove");

  ::DDS::DataReader_var dr = this->dds_read_->get_dds_reader ();
  if (!::CORBA::is_nil (dr.in ()))
    {
      ::DDS::ReturnCode_t const retval =
        subscriber->delete_datareader (dr.in ());
      if (retval != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("DDS_Subscriber_Base_T::remove - ")
                        ACE_TEXT ("Unable to delete DataReader: <%C>\n"),
                        ::CIAO::DDS4CCM::translate_retcode (retval)));
          throw ::CORBA::INTERNAL ();
        }
    }
  this->cft_setting_->delete_contentfilteredtopic (subscriber);
  this->dds_read_->_set_component (::CORBA::Object::_nil ());
  this->dds_read_->set_dds_reader (::DDS::DataReader::_nil (),
                                   0);
  this->condition_manager_.set_dds_entity (::DDS::DataReader::_nil ());
  this->ccm_data_reader_->set_dds_entity (::DDS::DataReader::_nil ());
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
typename CCM_TYPE::data_type::_ptr_type
DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::get_data (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::get_data");

  return CCM_TYPE::data_type::_duplicate (this->dds_read_);
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
typename CCM_TYPE::dds_entity_type::_ptr_type
DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::get_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::get_dds_entity");

  return CCM_TYPE::dds_entity_type::_duplicate (this->ccm_data_reader_);
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
typename CCM_TYPE::filter_config_type::_ptr_type
DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::get_filter_config (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::get_filter_config");

  return ::CCM_DDS::CCM_ContentFilterSetting::_duplicate (this->cft_setting_);
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
::CCM_DDS::QueryFilter *
DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::filter (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::filter");

  return this->cft_setting_->filter ();
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
void
DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::filter");
  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->cft_setting_->filter (filter);
    }
}
