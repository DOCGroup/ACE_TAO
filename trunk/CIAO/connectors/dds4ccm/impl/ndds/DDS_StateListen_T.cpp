// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/ndds/DataReaderStateListener_T.h"
#include "dds4ccm/impl/ndds/Reader_T.h"
#include "dds4ccm/impl/ndds/StateListenerControl_T.h"
#include "dds4ccm/impl/ndds/PortStatusListener_T.h"

#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_StateListen_T<DDS_TYPE, CCM_TYPE>::DDS_StateListen_T (void) :
  data_control_ (new CCM_DDS_StateListenerControl_T
    < ::CCM_DDS::CCM_StateListenerControl> ())
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_StateListen_T<DDS_TYPE, CCM_TYPE>::~DDS_StateListen_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_StateListen_T<DDS_TYPE, CCM_TYPE>::init (
  typename CCM_TYPE::statelistener_type::_ptr_type listener,
  ::CCM_DDS::PortStatusListener_ptr status,
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char* library_name,
  const char* profile_name)
{
  CIAO_TRACE ("DDS_StateListen_T<DDS_TYPE, CCM_TYPE>::init");

  try
    {
      if (CORBA::is_nil (this->data_listener_.in ()))
        {
          this->data_listener_ = new ::CIAO::DDS4CCM::RTI::DataReaderStateListener_T
            <DDS_TYPE, CCM_TYPE> (
              listener,
              status,
              data_control_.in ());
        }

      if (CORBA::is_nil (this->data_reader_.in ()))
        {
          if (library_name && profile_name)
            {
              ::DDS::DataReader_var reader =
                  subscriber->create_datareader_with_profile (
                    topic,
                    library_name,
                    profile_name,
                    this->data_listener_.in (),
                    ::CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
              DDSDataReader *rd = dynamic_cast < DDSDataReader *> (reader.in ());
              this->rti_reader_.set_impl (rd);
              this->data_reader_ = ::DDS::CCM_DataReader::_narrow (reader);
              this->dds_read_.data_reader (this->data_reader_.in ());
            }
          else
            {
              ::DDS::DataReaderQos drqos;
              ::DDS::DataReader_var reader =
                  subscriber->create_datareader (
                    topic,
                    drqos,
                    this->data_listener_.in (),
                    ::CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
              DDSDataReader *rd = dynamic_cast < DDSDataReader *> (reader.in ());
              this->rti_reader_.set_impl (rd);
              this->data_reader_ = ::DDS::CCM_DataReader::_narrow (reader);
              this->dds_read_.data_reader (this->data_reader_.in ());
            }
        }
    }
  catch (...)
    {
      CIAO_ERROR ((LM_EMERGENCY, "Caught unknown c++ exception while creating subscriber entities\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_StateListen_T<DDS_TYPE, CCM_TYPE>::get_data (void)
{
  CIAO_TRACE ("DDS_StateListen_T<DDS_TYPE, CCM_TYPE>::get_data");

  return &this->dds_read_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_StateListen_T<DDS_TYPE, CCM_TYPE>::get_dds_entity (void)
{
  CIAO_TRACE ("DDS_StateListen_T<DDS_TYPE, CCM_TYPE>::get_dds_entity");

  return ::DDS::CCM_DataReader::_duplicate (this->data_reader_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
::CCM_DDS::CCM_StateListenerControl_ptr
DDS_StateListen_T<DDS_TYPE, CCM_TYPE>::get_data_control (void)
{
  CIAO_TRACE ("DDS_StateListen_T<DDS_TYPE, CCM_TYPE>::get_data_control");

  return ::CCM_DDS::CCM_StateListenerControl::_duplicate (
    this->data_control_.in ());
}

