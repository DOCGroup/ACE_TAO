// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::DDS_Subscriber_Base_T (void)
  : data_reader_ (0),
    configuration_complete_ (false)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::~DDS_Subscriber_Base_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::configuration_complete (
  typename CCM_TYPE::base_type::_ptr_type component,
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char* library_name,
  const char* profile_name)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::configuration_complete");

  this->configuration_complete_ = true;

  if (!this->data_reader_.get_impl ())
    {
      if (ACE_OS::strlen (this->cft_setting_.filter ()->expression.in ()) > 0)
        {
          ::DDS::ContentFilteredTopic_var cft =
            this->cft_setting_.create_contentfilteredtopic (topic,
                                                            subscriber);
          if (CORBA::is_nil (cft.in ()))
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR,
                            "DDS_Subscriber_Base_T::configuration_complete: "
                            "Error creating ContentFilteredTopic.\n"));
              throw ::CORBA::INTERNAL ();
            }
          this->data_reader_.create_datareader (cft,
                                                subscriber,
                                                library_name,
                                                profile_name);
        }
      else
        {
          this->data_reader_.create_datareader (topic,
                                                subscriber,
                                                library_name,
                                                profile_name);
        }
      this->dds_read_.set_impl (&this->data_reader_,
                                &this->condition_manager_);
      this->dds_read_._set_component (component);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::activate (
  ::CCM_DDS::PortStatusListener_ptr status,
  ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::activate");

  if (::CORBA::is_nil (this->listener_.in ()))
    {
      ACE_NEW_THROW_EX (this->listener_,
                        PortStatusListener_type (status, reactor),
                        ::CORBA::NO_MEMORY ());
    }
    
  ::DDS::ReturnCode_t const retcode = this->data_reader_.set_listener (
      this->listener_.in (),
      PortStatusListener_type::get_mask (status));

  if (retcode != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                    "DDS_Subscriber_Base_T::activate - "
                    "Error while setting the listener on the subscriber - <%C>\n",
                    ::CIAO::DDS4CCM::translate_retcode (retcode)));
      throw ::CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::passivate ()
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::passivate");

  this->condition_manager_.passivate ();
  this->data_reader_.passivate ();
  this->listener_ = ::DDS::DataReaderListener::_nil ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::remove (
  ::DDS::Subscriber_ptr subscriber)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::remove");

  this->data_reader_.delete_datareader (subscriber);
  this->cft_setting_.delete_contentfilteredtopic (subscriber);
  this->dds_read_._set_component (CCM_TYPE::base_type::_nil ());
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_data (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_data");

  return CCM_TYPE::reader_type::_duplicate (&this->dds_read_);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::CCM_DataReader_ptr
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_dds_entity");

  return ::DDS::CCM_DataReader::_duplicate (&this->data_reader_);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
::CCM_DDS::CCM_ContentFilterSetting_ptr
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_filter_config (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_filter_config");

  return ::CCM_DDS::CCM_ContentFilterSetting::_duplicate (&this->cft_setting_);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
::CCM_DDS::QueryFilter *
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::filter (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::filter");

  return this->cft_setting_.filter ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::filter");
  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->cft_setting_.filter (filter);
    }
}
