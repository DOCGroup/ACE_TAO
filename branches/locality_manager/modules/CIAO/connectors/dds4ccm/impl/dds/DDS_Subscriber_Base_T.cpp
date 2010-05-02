// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::DDS_Subscriber_Base_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::~DDS_Subscriber_Base_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
bool
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete (
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char* library_name,
  const char* profile_name)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete");

  try
    {
      if (!this->data_reader_.get_impl ())
        {
          this->data_reader_.create_datareader (topic,
                                                subscriber,
                                                library_name,
                                                profile_name);
          this->dds_read_.set_impl (&this->data_reader_);
          return true;
        }
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Subscriber_Base_T::configuration_complete: Caught unexpected exception.\n"));
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
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Subscriber_Base_T::activate: Caught unexpected exception.\n"));
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
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Subscriber_Base_T::passivate: Caught unexpected exception.\n"));
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
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Subscriber_Base_T::remove: Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_data (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_data");

  return &this->dds_read_;
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::get_dds_entity");

  return &this->data_reader_;
}

