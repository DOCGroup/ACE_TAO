// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/DataReaderListener_T.h"
#include "dds4ccm/impl/DataWriterListener_T.h"
#include "dds4ccm/impl/Writer_T.h"
#include "dds4ccm/impl/Getter_T.h"
#include "dds4ccm/impl/Reader_T.h"
#include "dds4ccm/impl/DataListenerControl_T.h"
#include "dds4ccm/impl/PortStatusListener_T.h"

#include "dds4ccm/impl/Log_Macros.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::DDS_Event_Connector_T (void) :
    TopicBaseConnector (),
    supplier_obtained_ (false),
    push_consumer_obtained_ (false),
    pull_consumer_obtained_ (false),
    supplier_ (),
    push_consumer_ (),
    pull_consumer_ ()
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::~DDS_Event_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
typename CCM_TYPE::writer_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_supplier_data (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_supplier_data");

  this->supplier_obtained_ = true;
  return this->supplier_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::CCM_DataWriter_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_supplier_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_supplier_dds_entity");

  this->supplier_obtained_ = true;
  return this->supplier_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
typename CCM_TYPE::getter_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_pull_consumer_fresh_data (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_pull_consumer_fresh_data");

  this->pull_consumer_obtained_ = true;
  return this->pull_consumer_.get_fresh_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_push_consumer_data (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_push_consumer_data");

  this->push_consumer_obtained_ = true;
  return this->push_consumer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_pull_consumer_data (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_pull_consumer_data");

  this->pull_consumer_obtained_ = true;
  return this->pull_consumer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
::CCM_DDS::CCM_ContentFilterSetting_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_pull_consumer_filter_config (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_pull_consumer_filter_config");

  this->pull_consumer_obtained_ = true;
  return this->pull_consumer_.get_filter_config ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
::CCM_DDS::CCM_DataListenerControl_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_push_consumer_data_control (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_push_consumer_data_control");

  this->push_consumer_obtained_ = true;
  return this->push_consumer_.get_data_control ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::CCM_DataReader_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_pull_consumer_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_pull_consumer_dds_entity");

  this->pull_consumer_obtained_ = true;
  return this->pull_consumer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
::CCM_DDS::QueryFilter*
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::pull_consumer_filter (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::pull_consumer_filter");

  return this->pull_consumer_.filter ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::pull_consumer_filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::pull_consumer_filter");

  this->pull_consumer_.filter (filter);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::CCM_DataReader_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_push_consumer_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_push_consumer_dds_entity");

  this->push_consumer_obtained_ = true;
  return this->push_consumer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
::CCM_DDS::CCM_ContentFilterSetting_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_push_consumer_filter_config (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_push_consumer_filter_config");

  this->push_consumer_obtained_ = true;
  return this->push_consumer_.get_filter_config ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
::CCM_DDS::QueryFilter*
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::push_consumer_filter (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::push_consumer_filter");

  return this->push_consumer_.filter ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::push_consumer_filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::push_consumer_filter");

  this->push_consumer_.filter (filter);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::configuration_complete (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::configuration_complete");

  try
    {
      TopicBaseConnector::configuration_complete ();

      this->push_consumer_obtained_ |=
        ! ::CORBA::is_nil (this->context_->get_connection_push_consumer_data_listener ());

      ::CCM_DDS::PortStatusListener_var push_consumer_psl =
        this->context_->get_connection_push_consumer_status ();
      this->push_consumer_obtained_ |= ! ::CORBA::is_nil (push_consumer_psl.in ());

      ::CCM_DDS::PortStatusListener_var pull_consumer_psl =
        this->context_->get_connection_pull_consumer_status ();
      this->pull_consumer_obtained_ |=
        ! ::CORBA::is_nil (pull_consumer_psl.in ());

      if (this->push_consumer_obtained_)
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                        "DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::configuration_complete - "
                        "Creating push consumer port.\n"));
          this->push_consumer_.configuration_complete (
            this,
            this->topic_.in (),
            this->subscriber_.in (),
            this->library_name_,
            this->profile_name_);
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                        "DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::configuration_complete - "
                        "No need to create push consumer port.\n"));
        }

      if (this->supplier_obtained_)
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                        "DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::configuration_complete - "
                        "Creating supplier port.\n"));
          this->supplier_.configuration_complete(
            this,
            this->topic_.in (),
            this->publisher_.in (),
            this->library_name_,
            this->profile_name_);
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                        "DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::configuration_complete - "
                        "No need to create supplier port.\n"));
        }

      if (this->pull_consumer_obtained_)
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                        "DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::configuration_complete - "
                        "Creating pull consumer port.\n"));
          this->pull_consumer_.configuration_complete (
            this,
            this->topic_.in (),
            this->subscriber_.in (),
            this->library_name_,
            this->profile_name_);
        }
      else
        {
          DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                        "DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::configuration_complete - "
                        "No need to create pull consumer port.\n"));
        }
    }
  catch (const ::CCM_DDS::InternalError &ex)
    {
      DDS4CCM_PRINT_INTERNAL_EXCEPTION (
                              1,
                              ::CIAO::DDS4CCM::translate_retcode (ex.error_code),
                              "DDS_Event_Connector_T::configuration_complete");
      throw ::CORBA::INTERNAL ();
    }
  catch (const ::CORBA::Exception& ex)
    {
      DDS4CCM_PRINT_CORBA_EXCEPTION (
                              1,
                              ex,
                              "DDS_Event_Connector_T::configuration_complete");
      throw;
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, CLINFO
                         "DDS_Event_Connector_T::configuration_complete - "
                         "Caught unexpected exception.\n"));
      throw ::CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::ccm_activate (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::ccm_activate");

  try
    {
      ACE_Reactor* reactor = 0;
#if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
      reactor = this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ();
#endif
      TopicBaseConnector::ccm_activate (reactor);

      if (this->push_consumer_obtained_)
        {
          this->push_consumer_.activate (
            this->context_->get_connection_push_consumer_data_listener (),
            this->context_->get_connection_push_consumer_status (),
            reactor);
        }

      if (this->supplier_obtained_)
        {
          this->supplier_.activate ();
        }

      if (this->pull_consumer_obtained_)
        {
          this->pull_consumer_.activate (
            this->context_->get_connection_pull_consumer_status (),
            reactor);
        }
    }
  catch (const ::CCM_DDS::InternalError &ex)
    {
      DDS4CCM_PRINT_INTERNAL_EXCEPTION (
                              1,
                              ::CIAO::DDS4CCM::translate_retcode (ex.error_code),
                              "DDS_Event_Connector_T::ccm_activate");
      throw ::CORBA::INTERNAL ();
    }
  catch (const ::CORBA::Exception& ex)
    {
      DDS4CCM_PRINT_CORBA_EXCEPTION (
                              1,
                              ex,
                              "DDS_Event_Connector_T::ccm_activate");
      throw;
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, CLINFO
                         "DDS_Event_Connector_T::ccm_activate - "
                         "Caught unexpected exception.\n"));
      throw ::CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::ccm_passivate (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::ccm_passivate");

  try
    {
      if (this->push_consumer_obtained_)
        {
          this->push_consumer_.passivate ();
        }

      if (this->supplier_obtained_)
        {
          this->supplier_.passivate ();
        }

      if (this->pull_consumer_obtained_)
        {
          this->pull_consumer_.passivate ();
        }
      TopicBaseConnector::ccm_passivate ();
    }
  catch (const ::CCM_DDS::InternalError &ex)
    {
      DDS4CCM_PRINT_INTERNAL_EXCEPTION (
                              1,
                              ::CIAO::DDS4CCM::translate_retcode (ex.error_code),
                              "DDS_Event_Connector_T::ccm_passivate");
      throw ::CORBA::INTERNAL ();
    }
  catch (const ::CORBA::Exception& ex)
    {
      DDS4CCM_PRINT_CORBA_EXCEPTION (
                              1,
                              ex,
                              "DDS_Event_Connector_T::ccm_passivate");
      throw;
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, CLINFO
                         "DDS_Event_Connector_T::ccm_passivate - "
                         "Caught unexpected exception.\n"));
      throw ::CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::ccm_remove (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::ccm_remove");

  try
    {
      if (this->push_consumer_obtained_)
        {
          this->push_consumer_.remove (this->subscriber_.in ());
        }

      if (this->supplier_obtained_)
        {
          this->supplier_.remove (this->publisher_.in ());
        }

      if (this->pull_consumer_obtained_)
        {
          this->pull_consumer_.remove (this->subscriber_.in ());
        }
      TopicBaseConnector::ccm_remove ();
    }
  catch (const ::CCM_DDS::InternalError &ex)
    {
      DDS4CCM_PRINT_INTERNAL_EXCEPTION (
                              1,
                              ::CIAO::DDS4CCM::translate_retcode (ex.error_code),
                              "DDS_Event_Connector_T::ccm_remove");
      throw ::CORBA::INTERNAL ();
    }
  catch (const ::CORBA::Exception& ex)
    {
      DDS4CCM_PRINT_CORBA_EXCEPTION (
                              1,
                              ex,
                              "DDS_Event_Connector_T::ccm_remove");
      throw;
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_EMERGENCY, CLINFO
                         "DDS_Event_Connector_T::ccm_remove - "
                         "Caught unexpected exception.\n"));
      throw ::CORBA::INTERNAL ();
    }
}
