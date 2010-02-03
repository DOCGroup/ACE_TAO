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
DDS_Update_T<DDS_TYPE, CCM_TYPE>::configuration_complete (
  ::DDS::Topic_ptr topic,
  ::DDS::Publisher_ptr publisher,
  const char* library_name,
  const char* profile_name)
{
  CIAO_TRACE ("DDS_Update_T<DDS_TYPE, CCM_TYPE>::configuration_complete");
  if (CORBA::is_nil  (this->data_writer_.in ()))
    {
      try
        {
          ::DDS::DataWriter_var dwv_tmp;
          if (library_name && profile_name)
            {
              dwv_tmp = publisher->create_datawriter_with_profile (
                  topic,
                  library_name,
                  profile_name,
                  ::DDS::DataWriterListener::_nil (),
                  0);
            }
          else
            {
              ::DDS::DataWriterQos dwqos;
              dwv_tmp = publisher->create_datawriter (
                  topic,
                  dwqos,
                  ::DDS::DataWriterListener::_nil (),
                  0);
            }
          ::CIAO::DDS4CCM::RTI::RTI_DataWriter_i  *rw =
            dynamic_cast < ::CIAO::DDS4CCM::RTI::RTI_DataWriter_i  *> (dwv_tmp.in ());
          this->rti_writer_.set_impl (rw->get_impl ());
          this->data_writer_ = ::DDS::CCM_DataWriter::_narrow (dwv_tmp);
          this->dds_update_.set_impl (dwv_tmp);
        }
      catch (...)
        {
          CIAO_ERROR (1, (LM_EMERGENCY, "DDS_Update_T::configuration_complete: Caught unknown c++ exception.\n"));
          throw CORBA::INTERNAL ();
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Update_T<DDS_TYPE, CCM_TYPE>::activate ()
{
  CIAO_TRACE ("DDS_Update_T<DDS_TYPE, CCM_TYPE>::activate");
  try
    {
      if (CORBA::is_nil (this->data_listener_.in ()))
        {
          this->data_listener_ = new ::CIAO::DDS4CCM::DataWriterListener_T
            <DDS_TYPE, CCM_TYPE> ();
        }
      this->rti_writer_.set_listener (
        this->data_listener_.in (),
        ::CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_EMERGENCY, "DDS_Update_T::activate: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Update_T<DDS_TYPE, CCM_TYPE>::passivate ()
{
  CIAO_TRACE ("DDS_Update_T<DDS_TYPE, CCM_TYPE>::passivate");
  try
    {
      this->rti_writer_.set_listener (
        ::DDS::DataWriterListener::_nil (),
        0);
      this->data_listener_ = ::DDS::DataWriterListener::_nil ();
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_EMERGENCY, "DDS_Update_T::passivate: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Update_T<DDS_TYPE, CCM_TYPE>::remove (
  ::DDS::Publisher_ptr publisher)
{
  CIAO_TRACE ("DDS_Update_T<DDS_TYPE, CCM_TYPE>::remove");
  try
    {
      publisher->delete_datawriter (this->data_writer_.in ());
      this->rti_writer_.set_impl (0);
      this->data_writer_ = ::DDS::CCM_DataWriter::_nil ();
      this->dds_update_.set_impl (0);
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_EMERGENCY, "DDS_Update_T::remove: Caught unknown c++ exception.\n"));
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

