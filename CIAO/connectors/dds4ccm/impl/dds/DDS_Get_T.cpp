// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/dds/DataReaderListener_T.h"
#include "dds4ccm/impl/dds/DataWriterListener_T.h"
#include "dds4ccm/impl/dds/Reader_T.h"
#include "dds4ccm/impl/dds/DataListenerControl_T.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::DDS_Get_T (void) :
  ccm_dds_reader_ (0)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::~DDS_Get_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete (
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char* library_name,
  const char* profile_name)
{
  DDS4CCM_TRACE ("DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete");

  try
    {
      if (::CORBA::is_nil (this->data_reader_.in ()))
        {
          ::DDS::DataReader_var reader;
          if (profile_name && library_name)
            {
              reader = subscriber->create_datareader_with_profile (
                  topic,
                  library_name,
                  profile_name,
                  ::DDS::DataReaderListener::_nil (),
                  0);
            }
          else
            {
              ::DDS::DataReaderQos drqos;
              reader = subscriber->create_datareader (
                    topic,
                    drqos,
                    ::DDS::DataReaderListener::_nil (),
                    0);
            }
          ::CIAO::DDS4CCM::CCM_DDS_DataReader_i *rd =
            dynamic_cast < ::CIAO::DDS4CCM::CCM_DDS_DataReader_i *> (reader.in ());
          this->ccm_dds_reader_.set_impl (rd->get_impl ());
          this->data_reader_ = ::DDS::CCM_DataReader::_narrow (reader);
          this->dds_get_.set_impl (reader);
          this->dds_read_.set_impl (reader);
          this->dds_read_.set_qos (library_name, profile_name);
        }
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Get_T::configuration_complete: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::activate (
  ::CCM_DDS::PortStatusListener_ptr listener,
  ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::activate");
  try
    {
      if (::CORBA::is_nil (this->status_.in ()))
        {
          ACE_NEW_THROW_EX (this->status_,
                            PortStatusListener (listener, reactor),
                            CORBA::NO_MEMORY ());
        }
      this->ccm_dds_reader_.set_listener (
          this->status_.in (),
          ::CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask (
            listener));
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Get_T::activate: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::passivate (void)
{
  DDS4CCM_TRACE ("DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::passivate");
  try
    {
      this->dds_get_.passivate ();
      this->ccm_dds_reader_.set_listener (
              ::DDS::DataReaderListener::_nil (),
              0);
      this->status_ = ::DDS::DataReaderListener::_nil ();
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Get_T::passivate: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::remove (
  ::DDS::Subscriber_ptr subscriber)
{
  DDS4CCM_TRACE ("DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::remove");
  try
    {
      subscriber->delete_datareader (this->data_reader_.in ());
      this->data_reader_ = ::DDS::CCM_DataReader::_nil ();
      this->dds_get_.set_impl (0);
      this->dds_read_.set_impl (0);
      this->ccm_dds_reader_.set_impl (0);
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Get_T::remove: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::getter_type::_ptr_type
DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::get_fresh_data (void)
{
  DDS4CCM_TRACE ("DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::get_fresh_data");

  return &this->dds_get_;
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::get_data (void)
{
  DDS4CCM_TRACE ("DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::get_data");

  return &this->dds_read_;
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::get_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::get_dds_entity");

  return &this->ccm_dds_reader_;
}

