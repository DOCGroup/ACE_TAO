// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/ndds/DataReaderStateListener_T.h"
#include "dds4ccm/impl/ndds/DataWriterListener_T.h"
#include "dds4ccm/impl/ndds/Updater_T.h"
#include "dds4ccm/impl/ndds/DataWriter.h"

#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Update_T<DDS_TYPE, CCM_TYPE>::DDS_Update_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Update_T<DDS_TYPE, CCM_TYPE>::~DDS_Update_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Update_T<DDS_TYPE, CCM_TYPE>::init (
  ::DDS::Topic_ptr topic,
  ::DDS::Publisher_ptr publisher,
  const char* library_name,
  const char* profile_name)
{
  try
    {
      if (CORBA::is_nil  (this->data_writer_.in ()))
        {
          this->data_listener_ = new ::CIAO::DDS4CCM::DataWriterListener_T
            <DDS_TYPE, CCM_TYPE> ();

          if (library_name && profile_name)
            {
              ::DDS::DataWriter_var dwv_tmp = publisher->
                create_datawriter_with_profile (
                  topic,
                  library_name,
                  profile_name,
                  this->data_listener_.in (),
                  ::CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
              DDSDataWriter *rw = dynamic_cast < DDSDataWriter *> (dwv_tmp.in ());
              this->rti_writer_.set_impl (rw);
              this->data_writer_ = ::DDS::CCM_DataWriter::_narrow (dwv_tmp);
              this->dds_update_.set_impl (dwv_tmp);
            }
          else
            {
              ::DDS::DataWriterQos dwqos;
              ::DDS::DataWriter_var dwv_tmp = publisher->
                create_datawriter (
                  topic,
                  dwqos,
                  this->data_listener_.in (),
                  ::CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
              DDSDataWriter *rw = dynamic_cast < DDSDataWriter *> (dwv_tmp.in ());
              this->rti_writer_.set_impl (rw);
              this->data_writer_ = ::DDS::CCM_DataWriter::_narrow (dwv_tmp);
              this->dds_update_.set_impl (dwv_tmp);
            }
        }
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_ERROR, "Caught unknown C++ exception while configuring port info_in_\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::updater_type::_ptr_type
DDS_Update_T<DDS_TYPE, CCM_TYPE>::get_data (void)
{
  return &this->dds_update_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataWriter_ptr
DDS_Update_T<DDS_TYPE, CCM_TYPE>::get_dds_entity (void)
{
  return &this->rti_writer_;
}

