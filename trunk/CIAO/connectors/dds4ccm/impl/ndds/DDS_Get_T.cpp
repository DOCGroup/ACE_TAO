// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/ndds/DataReaderListener_T.h"
#include "dds4ccm/impl/ndds/DataWriterListener_T.h"
#include "dds4ccm/impl/ndds/Writer_T.h"
#include "dds4ccm/impl/ndds/Getter_T.h"
#include "dds4ccm/impl/ndds/Reader_T.h"
#include "dds4ccm/impl/ndds/DataListenerControl_T.h"
#include "dds4ccm/impl/ndds/PortStatusListener_T.h"

#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Get_T<DDS_TYPE, CCM_TYPE>::DDS_Get_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Get_T<DDS_TYPE, CCM_TYPE>::~DDS_Get_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Get_T<DDS_TYPE, CCM_TYPE>::init (
  ::CCM_DDS::PortStatusListener_ptr listener,
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char* library_name,
  const char* profile_name)
{
  CIAO_TRACE ("DDS_Get_T<DDS_TYPE, CCM_TYPE>::init");

  try
    {
      if (CORBA::is_nil (this->status_.in ()))
        {
          this->status_ = new ::CIAO::DDS4CCM::RTI::PortStatusListener_T
            <DDS_TYPE, CCM_TYPE> (listener);
        }

      if (CORBA::is_nil (this->fresh_data_.in ()))
        {
          if (profile_name && library_name)
            {
              this->fresh_data_ =
                subscriber->create_datareader_with_profile (
                  topic,
                  library_name,
                  profile_name,
                  this->status_.in (),
                  ::CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
            }
          else
            {
              ::DDS::DataReaderQos drqos;
              this->fresh_data_ =
                  subscriber->create_datareader (
                    topic,
                    drqos,
                    this->status_.in (),
                    ::CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
            }
        }

      if (CORBA::is_nil (this->data_.in ()))
        {
          if (profile_name && library_name)
            {
              this->data_ =
                subscriber->create_datareader_with_profile (
                  topic,
                  library_name,
                  profile_name,
                  this->status_.in (),
                  ::CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
            }
          else
            {
              ::DDS::DataReaderQos drqos;
              this->data_ =
                  subscriber->create_datareader (
                    topic,
                    drqos,
                    this->status_.in (),
                    ::CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
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
typename CCM_TYPE::getter_type::_ptr_type
DDS_Get_T<DDS_TYPE, CCM_TYPE>::get_fresh_data (void)
{
  CIAO_TRACE ("DDS_Get_T<DDS_TYPE, CCM_TYPE>::get_fresh_data");

  return new CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE> (
          this->fresh_data_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Get_T<DDS_TYPE, CCM_TYPE>::get_data (void)
{
  CIAO_TRACE ("DDS_Get_T<DDS_TYPE, CCM_TYPE>::get_data");

  return new CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE> (
          this->data_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_Get_T<DDS_TYPE, CCM_TYPE>::get_dds_entity (void)
{
  CIAO_TRACE ("DDS_Get_T<DDS_TYPE, CCM_TYPE>::get_dds_entity");
  return ::DDS::CCM_DataReader::_nil ();
}

