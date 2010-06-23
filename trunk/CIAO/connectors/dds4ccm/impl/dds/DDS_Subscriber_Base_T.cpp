// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/dds/Log_Macros.h"
#include "dds4ccm/impl/dds/ContentFilteredTopic.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::DDS_Subscriber_Base_T (void)
  : data_reader_ (0),
    configuration_complete_ (false)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::~DDS_Subscriber_Base_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
bool
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete (
  typename CCM_TYPE::base_type::_ptr_type component,
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char* library_name,
  const char* profile_name)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete");

  this->configuration_complete_ = true;

  try
    {
      if (!this->data_reader_.get_impl ())
        {
          if (ACE_OS::strlen (this->cft_setting_.filter ()->expression.in ()) > 0)
            {
              ::DDS::ContentFilteredTopic_var cft =
                this->cft_setting_.create_contentfilteredtopic (topic,
                                                                subscriber);
              if (CORBA::is_nil (cft.in ()))
                {
                  DDS4CCM_ERROR (1, (LM_ERROR, "DDS_Subscriber_Base_T::configuration_complete: "
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
          this->dds_read_.set_impl (&this->data_reader_);
          this->dds_read_._set_component (component);
          return true;
        }
    }
  catch (const ::CCM_DDS::InternalError &)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY,
                         CLINFO "DDS_Subscriber_Base_T::configuration_complete: "
                         "Caught CCM_DDS internal exception.\n"));
      throw CORBA::INTERNAL ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("DDS_Subscriber_Base_T::configuration_complete");
      DDS4CCM_ERROR (1, (LM_EMERGENCY,
                         CLINFO "DDS_Subscriber_Base_T::configuration_complete: "
                         "Caught internal exception.\n"));
      throw CORBA::INTERNAL ();
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY,
                         CLINFO "DDS_Subscriber_Base_T::configuration_complete: "
                         "Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
  return false;
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::activate (
  ::CCM_DDS::PortStatusListener_ptr status,
  ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::activate");

  try
    {
      if (::CORBA::is_nil (this->listener_.in ()))
        {
          ACE_NEW_THROW_EX (this->listener_,
                            PortStatusListener (status, reactor),
                            CORBA::NO_MEMORY ());
        }
      this->data_reader_.set_listener (
        this->listener_.in (),
        PortStatusListener::get_mask (status));
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, CLINFO "DDS_Subscriber_Base_T::activate: "
                                              "Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::passivate ()
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::passivate");

  try
    {
      this->data_reader_.passivate ();
      this->listener_ = ::DDS::DataReaderListener::_nil ();
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, CLINFO "DDS_Subscriber_Base_T::passivate: "
                                              "Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::remove (
  ::DDS::Subscriber_ptr subscriber)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::remove");

  try
    {
      this->data_reader_.delete_datareader (subscriber);
      this->cft_setting_.delete_contentfilteredtopic (subscriber);
      this->dds_read_._set_component (CCM_TYPE::base_type::_nil ());
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, CLINFO "DDS_Subscriber_Base_T::remove: "
                                              "Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_data (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_data");

  return CCM_TYPE::reader_type::_duplicate (&this->dds_read_);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_dds_entity");

  return ::DDS::CCM_DataReader::_duplicate (&this->data_reader_);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::CCM_ContentFilterSetting_ptr
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_filter_config (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_filter_config");

  return ::CCM_DDS::CCM_ContentFilterSetting::_duplicate (&this->cft_setting_);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::QueryFilter *
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::filter (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::filter");

  return this->cft_setting_.filter ();
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
      if (ACE_OS::strlen (filter.expression.in ()) == 0)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO "DDS_Subscriber_Base_T::get_filter_config: "
                                       "Filter expression not set. Unable to create "
                                       "ContentFilterSetting interface.\n"));
          throw ::CCM_DDS::InternalError ();
        }
      this->cft_setting_.filter (filter);
    }
}
