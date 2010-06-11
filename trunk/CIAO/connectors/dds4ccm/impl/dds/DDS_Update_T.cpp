// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/dds/DataReaderStateListener_T.h"
#include "dds4ccm/impl/dds/DataWriterListener_T.h"
#include "dds4ccm/impl/dds/Updater_T.h"
#include "dds4ccm/impl/dds/DataWriter.h"

#include "dds4ccm/impl/dds/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Update_T<DDS_TYPE, CCM_TYPE>::DDS_Update_T (void) :
  ccm_dds_writer_ (0)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Update_T<DDS_TYPE, CCM_TYPE>::~DDS_Update_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Update_T<DDS_TYPE, CCM_TYPE>::configuration_complete (
  typename CCM_TYPE::base_type::_ptr_type component,
  ::DDS::Topic_ptr topic,
  ::DDS::Publisher_ptr publisher,
  const char* library_name,
  const char* profile_name)
{
  DDS4CCM_TRACE ("DDS_Update_T<DDS_TYPE, CCM_TYPE>::configuration_complete");
  if (!this->ccm_dds_writer_.get_impl ())
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
          ::CIAO::DDS4CCM::CCM_DDS_DataWriter_i  *rw =
            dynamic_cast < ::CIAO::DDS4CCM::CCM_DDS_DataWriter_i  *> (dwv_tmp.in ());
          this->ccm_dds_writer_.set_impl (rw->get_impl ());
          this->dds_update_.set_impl (&this->ccm_dds_writer_);
          this->dds_update_._set_component (component);
        }
      catch (...)
        {
          DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Update_T::configuration_complete: Caught unexpected exception.\n"));
          throw CORBA::INTERNAL ();
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Update_T<DDS_TYPE, CCM_TYPE>::activate ()
{
  DDS4CCM_TRACE ("DDS_Update_T<DDS_TYPE, CCM_TYPE>::activate");
  try
    {
      if (::CORBA::is_nil (this->data_listener_.in ()))
        {
          ACE_NEW_THROW_EX (this->data_listener_,
                            DataWriterListener (),
                            CORBA::NO_MEMORY ());
        }
      this->ccm_dds_writer_.set_listener (
        this->data_listener_.in (),
        DataWriterListener::get_mask ());
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY,
                         "DDS_Update_T::activate: Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Update_T<DDS_TYPE, CCM_TYPE>::passivate ()
{
  DDS4CCM_TRACE ("DDS_Update_T<DDS_TYPE, CCM_TYPE>::passivate");
  try
    {
      this->ccm_dds_writer_.set_listener (
        ::DDS::DataWriterListener::_nil (),
        0);
      this->data_listener_ = ::DDS::DataWriterListener::_nil ();
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY,
                         "DDS_Update_T::passivate: Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Update_T<DDS_TYPE, CCM_TYPE>::remove (
  ::DDS::Publisher_ptr publisher)
{
  DDS4CCM_TRACE ("DDS_Update_T<DDS_TYPE, CCM_TYPE>::remove");
  try
    {
      ::DDS::ReturnCode_t const retval =
        publisher->delete_datawriter (&this->ccm_dds_writer_);
      if (retval != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
            "DDS_Update_T::remove - "
            "Unable to delete DataWriter: <%C>\n",
            ::CIAO::DDS4CCM::translate_retcode (retval)));
          throw ::CORBA::INTERNAL ();
        }
      this->ccm_dds_writer_.set_impl (0);
      this->dds_update_.set_impl (0);
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, "DDS_Update_T::remove: Caught unexpected exception.\n"));
      throw ::CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::updater_type::_ptr_type
DDS_Update_T<DDS_TYPE, CCM_TYPE>::get_data (void)
{
  return CCM_TYPE::updater_type::_duplicate (&this->dds_update_);
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataWriter_ptr
DDS_Update_T<DDS_TYPE, CCM_TYPE>::get_dds_entity (void)
{
  return ::DDS::CCM_DataWriter::_duplicate (&this->ccm_dds_writer_);
}

