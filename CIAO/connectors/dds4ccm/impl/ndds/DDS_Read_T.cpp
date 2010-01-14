// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/ndds/DataReaderListener_T.h"
#include "dds4ccm/impl/ndds/DataWriterListener_T.h"
#include "dds4ccm/impl/ndds/Writer_T.h"
#include "dds4ccm/impl/ndds/Reader_T.h"
#include "dds4ccm/impl/ndds/DataListenerControl_T.h"
#include "dds4ccm/impl/ndds/PortStatusListener_T.h"

#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Read_T<DDS_TYPE, CCM_TYPE>::DDS_Read_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Read_T<DDS_TYPE, CCM_TYPE>::~DDS_Read_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Read_T<DDS_TYPE, CCM_TYPE>::configuration_complete (
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char* library_name,
  const char* profile_name)
{
  CIAO_TRACE ("DDS_Read_T<DDS_TYPE, CCM_TYPE>::configuration_complete");

  try
    {
      if (CORBA::is_nil (this->data_.in ()))
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
          ::CIAO::DDS4CCM::RTI::RTI_DataReader_i *rd =
            dynamic_cast < ::CIAO::DDS4CCM::RTI::RTI_DataReader_i *> (reader.in ());
          this->rti_reader_.set_impl (rd->get_impl ());
          this->data_ = ::DDS::CCM_DataReader::_narrow (reader);
          this->dds_read_.set_impl (reader);
          ::CIAO::DDS4CCM::RTI::RTI_Topic_i *rti_top = dynamic_cast < ::CIAO::DDS4CCM::RTI::RTI_Topic_i * > (topic);
          this->dds_read_.set_topic (rti_top->get_impl ());
        }
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_EMERGENCY, "DDS_Read_T::configuration_complete: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Read_T<DDS_TYPE, CCM_TYPE>::activate (
  ::CCM_DDS::PortStatusListener_ptr listener)
{
  CIAO_TRACE ("DDS_Read_T<DDS_TYPE, CCM_TYPE>::activate");

  try
    {
      if (CORBA::is_nil (this->status_.in ()))
        {
          this->status_ = new ::CIAO::DDS4CCM::RTI::PortStatusListener_T
            <DDS_TYPE, CCM_TYPE> (listener);
        }
      this->rti_reader_.set_listener (
        this->status_.in (),
        ::CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_EMERGENCY, "DDS_Read_T::activate: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Read_T<DDS_TYPE, CCM_TYPE>::passivate ()
{
  CIAO_TRACE ("DDS_Read_T<DDS_TYPE, CCM_TYPE>::passivate");

  try
    {
      this->rti_reader_.set_listener (
              ::DDS::DataReaderListener::_nil (),
              0);
      this->status_ = ::DDS::DataReaderListener::_nil ();
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_EMERGENCY, "DDS_Read_T::passivate: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Read_T<DDS_TYPE, CCM_TYPE>::remove (
  ::DDS::Subscriber_ptr subscriber)
{
  CIAO_TRACE ("DDS_Read_T<DDS_TYPE, CCM_TYPE>::remove");

  try
    {
      subscriber->delete_datareader (this->data_.in ());
      this->rti_reader_.set_impl (0);
      this->data_ = ::DDS::CCM_DataReader::_nil ();
      this->dds_read_.set_impl (0);
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_EMERGENCY, "DDS_Read_T::remove: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Read_T<DDS_TYPE, CCM_TYPE>::get_data (void)
{
  CIAO_TRACE ("DDS_Read_T<DDS_TYPE, CCM_TYPE>::get_data");

  return &this->dds_read_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_Read_T<DDS_TYPE, CCM_TYPE>::get_dds_entity (void)
{
  CIAO_TRACE ("DDS_Read_T<DDS_TYPE, CCM_TYPE>::get_dds_entity");

  return &this->rti_reader_;
}

