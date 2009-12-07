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
DDS_Read_T<DDS_TYPE, CCM_TYPE>::init (
  ::CCM_DDS::PortStatusListener_ptr listener,
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char* library_name,
  const char* profile_name)
{
  CIAO_TRACE ("DDS_Read_T<DDS_TYPE, CCM_TYPE>::init");

  try
    {
      if (CORBA::is_nil (this->status_.in ()))
        {
          this->status_ = new ::CIAO::DDS4CCM::RTI::PortStatusListener_T
            <DDS_TYPE, CCM_TYPE> (listener);
        }

      if (CORBA::is_nil (this->data_.in ()))
        {
          if (profile_name && library_name)
            {
              ::DDS::DataReader_var reader =
                subscriber->create_datareader_with_profile (
                  topic,
                  library_name,
                  profile_name,
                  this->status_.in (),
                  ::CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
              this->data_ = ::DDS::CCM_DataReader::_narrow (reader);
            }
          else
            {
              ::DDS::DataReaderQos drqos;
              ::DDS::DataReader_var reader =
                subscriber->create_datareader (
                  topic,
                  drqos,
                  this->status_.in (),
                  ::CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
              this->data_ = ::DDS::CCM_DataReader::_narrow (reader);
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
DDS_Read_T<DDS_TYPE, CCM_TYPE>::get_data (void)
{
  CIAO_TRACE ("DDS_Read_T<DDS_TYPE, CCM_TYPE>::get_data");

  return new CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE> (
          this->data_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_Read_T<DDS_TYPE, CCM_TYPE>::get_dds_entity (void)
{
  CIAO_TRACE ("DDS_Read_T<DDS_TYPE, CCM_TYPE>::get_dds_entity");

  return this->data_.in ();
}

