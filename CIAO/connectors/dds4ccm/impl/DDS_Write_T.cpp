// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/DataReaderListener_T.h"
#include "dds4ccm/impl/DataListenerControl_T.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename CCM_TYPE, typename DDS_TYPE>
DDS_Write_T<CCM_TYPE, DDS_TYPE>::DDS_Write_T (void)
{
}

template <typename CCM_TYPE, typename DDS_TYPE>
DDS_Write_T<CCM_TYPE, DDS_TYPE>::~DDS_Write_T (void)
{
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_Write_T<CCM_TYPE, DDS_TYPE>::set_component (
  ::CORBA::Object_ptr component)
{
  this->dds_write_->_set_component (component);
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_Write_T<CCM_TYPE, DDS_TYPE>::configuration_complete (
  ::DDS::Topic_ptr topic,
  ::DDS::Publisher_ptr publisher,
  const char* library_name,
  const char* profile_name)
{
  DDS4CCM_TRACE ("DDS_Write_T<CCM_TYPE, DDS_TYPE>::configuration_complete");
  ::DDS::DataWriter_var dw = this->dds_write_->get_dds_writer ();
  if (::CORBA::is_nil (dw.in ()))
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
      if (::CORBA::is_nil (dwv_tmp.in ()))
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                        "DDS_Write_T::configuration_complete - "
                        "Error: Proxy returned a nil datawriter.\n"));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }
      this->dds_write_->set_dds_writer (dwv_tmp.in ());
      this->ccm_data_writer_->set_dds_entity (dwv_tmp.in ());
    }
}
template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_Write_T<CCM_TYPE, DDS_TYPE>::activate ()
{
  DDS4CCM_TRACE ("DDS_Write_T<CCM_TYPE, DDS_TYPE>::activate");
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_Write_T<CCM_TYPE, DDS_TYPE>::passivate ()
{
  DDS4CCM_TRACE ("DDS_Write_T<CCM_TYPE, DDS_TYPE>::passivate");
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_Write_T<CCM_TYPE, DDS_TYPE>::remove (
  ::DDS::Publisher_ptr publisher)
{
  DDS4CCM_TRACE ("DDS_Write_T<CCM_TYPE, DDS_TYPE>::remove");

  ::DDS::DataWriter_var writer = this->dds_write_->get_dds_writer ();
  if (!::CORBA::is_nil (writer.in ()))
    {
      DDS::ReturnCode_t const retcode =
          publisher->delete_datawriter (writer.in ());

      if (retcode == ::DDS::RETCODE_OK)
        {
          this->dds_write_->_set_component (::CORBA::Object::_nil ());
          this->dds_write_->set_dds_writer (::DDS::DataWriter::_nil ());
          this->ccm_data_writer_->set_dds_entity (::DDS::DataWriter::_nil ());
        }
      else
        {
          throw ::CCM_DDS::InternalError (retcode, 0);
        }
    }
}

template <typename CCM_TYPE, typename DDS_TYPE>
typename CCM_TYPE::data_type::_ptr_type
DDS_Write_T<CCM_TYPE, DDS_TYPE>::get_data (void)
{
  DDS4CCM_TRACE ("DDS_Write_T<CCM_TYPE, DDS_TYPE>::get_data");

  return CCM_TYPE::data_type::_duplicate (this->dds_write_);
}

template <typename CCM_TYPE, typename DDS_TYPE>
typename CCM_TYPE::dds_entity_type::_ptr_type
DDS_Write_T<CCM_TYPE, DDS_TYPE>::get_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Write_T<CCM_TYPE, DDS_TYPE>::get_dds_entity");
  return ::DDS::CCM_DataWriter::_duplicate (this->ccm_data_writer_);
}

