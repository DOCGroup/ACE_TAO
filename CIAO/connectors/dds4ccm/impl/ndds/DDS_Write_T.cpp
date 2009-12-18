// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/ndds/DataReaderListener_T.h"
#include "dds4ccm/impl/ndds/DataWriterListener_T.h"
#include "dds4ccm/impl/ndds/DataListenerControl_T.h"
#include "dds4ccm/impl/ndds/PortStatusListener_T.h"

#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Write_T<DDS_TYPE, CCM_TYPE>::DDS_Write_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Write_T<DDS_TYPE, CCM_TYPE>::~DDS_Write_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Write_T<DDS_TYPE, CCM_TYPE>::init (
  ::DDS::Topic_ptr topic,
  ::DDS::Publisher_ptr publisher,
  const char* library_name,
  const char* profile_name)
{
  CIAO_TRACE ("DDS_Write_T<DDS_TYPE, CCM_TYPE>::init");

  try
    {
      if (CORBA::is_nil (this->writer_.in ()))
        {
          this->writer_listener_ = new ::CIAO::DDS4CCM::DataWriterListener_T
            <DDS_TYPE, CCM_TYPE> ();

          if (library_name && profile_name)
            {
              ::DDS::DataWriter_var dwv_tmp = publisher->
                create_datawriter_with_profile (
                  topic,
                  library_name,
                  profile_name,
                  this->writer_listener_.in (),
                  ::CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
              DDSDataWriter *rw = dynamic_cast < DDSDataWriter *> (dwv_tmp.in ());
              this->rti_writer_.set_impl (rw);
              this->writer_ = ::DDS::CCM_DataWriter::_narrow (dwv_tmp);
              this->writer_t_.data_writer (dwv_tmp);
            }
            else
            {
              ::DDS::DataWriterQos dwqos;
              ::DDS::DataWriter_var dwv_tmp = publisher->
                create_datawriter (
                  topic,
                  dwqos,
                  this->writer_listener_.in (),
                  ::CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
              DDSDataWriter *rw = dynamic_cast < DDSDataWriter *> (dwv_tmp.in ());
              this->rti_writer_.set_impl (rw);
              this->writer_ = ::DDS::CCM_DataWriter::_narrow (dwv_tmp);
              this->writer_t_.data_writer (dwv_tmp);
            }
        }
    }
  catch (...)
    {
      CIAO_ERROR ((LM_ERROR, "Caught unknown C++ exception while configuring port info_in_\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::writer_type::_ptr_type
DDS_Write_T<DDS_TYPE, CCM_TYPE>::get_data (void)
{
  CIAO_TRACE ("DDS_Write_T<DDS_TYPE, CCM_TYPE>::get_data");

  return &this->writer_t_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataWriter_ptr
DDS_Write_T<DDS_TYPE, CCM_TYPE>::get_dds_entity (void)
{
  CIAO_TRACE ("DDS_Write_T<DDS_TYPE, CCM_TYPE>::get_dds_entity");

  return &this->rti_writer_;
}

