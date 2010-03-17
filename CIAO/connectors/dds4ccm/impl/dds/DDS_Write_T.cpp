// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/dds/DataReaderListener_T.h"
#include "dds4ccm/impl/dds/DataWriterListener_T.h"
#include "dds4ccm/impl/dds/DataListenerControl_T.h"
#include "dds4ccm/impl/dds/PortStatusListener_T.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Write_T<DDS_TYPE, CCM_TYPE>::DDS_Write_T (void) :
  ccm_dds_writer_i (0)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Write_T<DDS_TYPE, CCM_TYPE>::~DDS_Write_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Write_T<DDS_TYPE, CCM_TYPE>::configuration_complete (
  ::DDS::Topic_ptr topic,
  ::DDS::Publisher_ptr publisher,
  const char* library_name,
  const char* profile_name)
{
  DDS4CCM_TRACE ("DDS_Write_T<DDS_TYPE, CCM_TYPE>::configuration_complete");

  try
    {
      if (::CORBA::is_nil (this->data_writer_.in ()))
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
          ::CIAO::DDS4CCM::CCM_DDS_DataWriter_i  *rw =
            dynamic_cast < ::CIAO::DDS4CCM::CCM_DDS_DataWriter_i  *> (dwv_tmp.in ());
          this->ccm_dds_writer_i.set_impl (rw->get_impl ());
          this->data_writer_ = ::DDS::CCM_DataWriter::_narrow (dwv_tmp);
          this->writer_t_.set_impl (dwv_tmp);
        }
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Write_T::configuration_complete: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}
template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Write_T<DDS_TYPE, CCM_TYPE>::activate ()
{
  DDS4CCM_TRACE ("DDS_Write_T<DDS_TYPE, CCM_TYPE>::activate");
  try
    {
      if (::CORBA::is_nil (this->data_listener_.in ()))
        {
          ACE_NEW_THROW_EX (this->data_listener_,
                            DataWriterListener (),
                            CORBA::NO_MEMORY ());
        }
      this->ccm_dds_writer_i.set_listener (
        this->data_listener_.in (),
        ::CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Write_T::activate: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Write_T<DDS_TYPE, CCM_TYPE>::passivate ()
{
  DDS4CCM_TRACE ("DDS_Write_T<DDS_TYPE, CCM_TYPE>::passivate");
  try
    {
      this->ccm_dds_writer_i.set_listener (
        ::DDS::DataWriterListener::_nil (),
        0);
      this->data_listener_ = ::DDS::DataWriterListener::_nil ();
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Write_T::passivate: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Write_T<DDS_TYPE, CCM_TYPE>::remove (
  ::DDS::Publisher_ptr publisher)
{
  DDS4CCM_TRACE ("DDS_Write_T<DDS_TYPE, CCM_TYPE>::remove");
  try
    {
      publisher->delete_datawriter (this->data_writer_.in ());
      this->ccm_dds_writer_i.set_impl (0);
      this->data_writer_ = ::DDS::CCM_DataWriter::_nil ();
      this->writer_t_.set_impl (0);
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Write_T::remove: Caught unknown c++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}


template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::writer_type::_ptr_type
DDS_Write_T<DDS_TYPE, CCM_TYPE>::get_data (void)
{
  DDS4CCM_TRACE ("DDS_Write_T<DDS_TYPE, CCM_TYPE>::get_data");

  return &this->writer_t_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataWriter_ptr
DDS_Write_T<DDS_TYPE, CCM_TYPE>::get_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Write_T<DDS_TYPE, CCM_TYPE>::get_dds_entity");

  return &this->ccm_dds_writer_i;
}

