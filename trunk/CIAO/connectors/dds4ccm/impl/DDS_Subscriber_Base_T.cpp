// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/logger/Log_Macros.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
#include "dds/DCPS/Marked_Default_Qos.h"
#endif

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
  this->ccm_data_reader_->_set_component (component);
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
void
DDS_Subscriber_Base_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::configuration_complete (
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char * qos_profile,
  DDS_XML_QOS_PARSER_TYPE* qos_xml)
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
#endif
        {
          ::DDS::DataReaderQos drqos;
          DDS::ReturnCode_t const retcode =
            subscriber->get_default_datareader_qos (drqos);

          if (retcode != DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                  "DDS_Subscriber_Base_T::configuration_complete - "
                  "Error: Unable to retrieve get_default_datareader_qos: <%C>\n",
                  ::CIAO::DDS4CCM::translate_retcode (retcode)));
              throw ::CCM_DDS::InternalError (retcode, 0);
            }

          if (qos_profile && qos_xml)
            {
              CORBA::String_var name = topic->get_name ();
              DDS::ReturnCode_t const retcode_dr_qos = qos_xml->get_datareader_qos (
                                          drqos,
                                          qos_profile,
                                          name.in ());

              if (retcode_dr_qos != DDS::RETCODE_OK)
                {
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                      "DDS_Subscriber_Base_T::configuration_complete - "
                      "Error: Unable to retrieve datawriter QOS from XML: <%C>\n",
                      ::CIAO::DDS4CCM::translate_retcode (retcode_dr_qos)));
                  throw ::CCM_DDS::InternalError (retcode_dr_qos, 0);
                }
            }

#if defined GEN_OSTREAM_OPS
          if (DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
            {
              std::stringstream output;
              output << drqos;
              std::string message = output.str();
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_INFO, DDS4CCM_INFO
                            ACE_TEXT ("DDS_Subscriber_Base_T::configuration_complete - ")
                            ACE_TEXT ("Using datareader QOS <%C>\n"), message.c_str()));
            }
#endif

          dr = subscriber->create_datareader (
                                          td.in (),
                                          drqos,
                                          ::DDS::DataReaderListener::_nil (),
                                          0);
        }
      if (::CORBA::is_nil (dr.in ()))
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Subscriber_Base_T::configuration_complete - "
                        "Error: DDS returned a nil datareader.\n"));
          throw ::CORBA::INTERNAL ();
        }

      this->dds_read_->set_dds_reader (dr.in (), &this->condition_manager_);
      this->ccm_data_reader_->set_dds_entity (dr.in ());
      this->condition_manager_.set_dds_entity (dr.in ());

      DDS::ReturnCode_t const retcode = dr->enable ();
      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Subscriber_Base_T::configuration_complete - "
                        "Error: Unable to enable the datareader: <%C>.\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }
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
          DDS::ReturnCode_t const retcode = dr->set_listener (
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
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DDS_Subscriber_Base_T::passivate - "
                    "Setting the listener on the reader to nil\n"));

      DDS::ReturnCode_t const retcode =
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

  this->condition_manager_.set_dds_entity (::DDS::DataReader::_nil ());
  this->dds_read_->set_dds_reader (::DDS::DataReader::_nil (), 0);
  this->ccm_data_reader_->set_dds_entity (::DDS::DataReader::_nil ());

  if (!::CORBA::is_nil (dr.in ()))
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_TRACE, DDS4CCM_INFO
                  "DDS_Subscriber_Base_T::remove - "
                  "Going to delete DataReader "
                  DDS_ENTITY_FORMAT_SPECIFIER
                  " from subscriber "
                  DDS_ENTITY_FORMAT_SPECIFIER
                  "\n",
                  DDS_ENTITY_LOG (dr.in ()),
                  DDS_ENTITY_LOG (subscriber)));

      DDS::ReturnCode_t const retval =
        subscriber->delete_datareader (dr.in ());

      if (retval != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("DDS_Subscriber_Base_T::remove - ")
                        ACE_TEXT ("Unable to delete DataReader: <%C>\n"),
                        ::CIAO::DDS4CCM::translate_retcode (retval)));
          throw ::CORBA::INTERNAL ();
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_INFO, DDS4CCM_INFO
              "DDS_Subscriber_Base_T::remove - "
              "Deleted DataReader\n"));
        }
    }

  this->cft_setting_->delete_contentfilteredtopic (subscriber);
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
