// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/DataReaderStateListener_T.h"
#include "dds4ccm/impl/DataWriterListener_T.h"
#include "dds4ccm/impl/Updater_T.h"
#include "dds4ccm/impl/DataWriter_T.h"

#include "dds4ccm/impl/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
DDS_Update_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::DDS_Update_T (void) :
  ccm_dds_writer_ (0)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
DDS_Update_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~DDS_Update_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Update_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::configuration_complete (
  typename CCM_TYPE::base_type::_ptr_type component,
  ::DDS::Topic_ptr topic,
  ::DDS::Publisher_ptr publisher,
  const char* library_name,
  const char* profile_name)
{
  DDS4CCM_TRACE ("DDS_Update_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::configuration_complete");
  if (!this->ccm_dds_writer_.get_impl ())
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
      DataWriter_type  *rw = dynamic_cast < DataWriter_type *> (dwv_tmp.in ());

      if (!rw)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                        "DDS_Update_T::configuration_complete - "
                        "Unable to cast created DataWriter proxy to its "
                        "internal represenation.\n"));
          throw ::CORBA::INTERNAL ();
        }
      this->ccm_dds_writer_.set_impl (rw->get_impl ());
      this->dds_update_.set_impl (&this->ccm_dds_writer_);
      this->dds_update_._set_component (component);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Update_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::activate ()
{
  DDS4CCM_TRACE ("DDS_Update_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::activate");
  if (::CORBA::is_nil (this->data_listener_.in ()))
    {
      ACE_NEW_THROW_EX (this->data_listener_,
                        DataWriterListener_type (),
                        ::CORBA::NO_MEMORY ());
    }

  ::DDS::ReturnCode_t const retcode = this->ccm_dds_writer_.set_listener (
    this->data_listener_.in (),
    DataWriterListener_type::get_mask ());

  if (retcode != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                    "DDS_Update_T::activate - "
                    "Error while setting the listener on the updater - <%C>\n",
                    ::CIAO::DDS4CCM::translate_retcode (retcode)));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Update_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::passivate ()
{
  DDS4CCM_TRACE ("DDS_Update_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::passivate");
  ::DDS::ReturnCode_t const retcode = this->ccm_dds_writer_.set_listener (
                                              ::DDS::DataWriterListener::_nil (),
                                              0);
  if (retcode != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                    "DDS_Update_T::passivate - "
                    "Error while setting the listener on the writer - <%C>\n",
                    ::CIAO::DDS4CCM::translate_retcode (retcode)));
    }

  this->data_listener_ = ::DDS::DataWriterListener::_nil ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Update_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::remove (
  ::DDS::Publisher_ptr publisher)
{
  DDS4CCM_TRACE ("DDS_Update_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::remove");
  ::DDS::ReturnCode_t const retval =
    publisher->delete_datawriter (&this->ccm_dds_writer_);
  if (retval != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
        "DDS_Update_T::remove - "
        "Unable to delete DataWriter: <%C>\n",
        ::CIAO::DDS4CCM::translate_retcode (retval)));
      throw ::CORBA::INTERNAL ();
    }
  this->ccm_dds_writer_.set_impl (0);
  this->dds_update_._set_component (CCM_TYPE::base_type::_nil ());
  this->dds_update_.set_impl (0);
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
typename CCM_TYPE::updater_type::_ptr_type
DDS_Update_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_data (void)
{
  return CCM_TYPE::updater_type::_duplicate (&this->dds_update_);
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::CCM_DataWriter_ptr
DDS_Update_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_dds_entity (void)
{
  return ::DDS::CCM_DataWriter::_duplicate (&this->ccm_dds_writer_);
}

