// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/DataReaderStateListener_T.h"
#include "dds4ccm/impl/Updater_T.h"
#include "dds4ccm/impl/Reader_T.h"
#include "dds4ccm/impl/PublisherListener_T.h"
#include "dds4ccm/impl/SubscriberListener.h"
#include "dds4ccm/impl/DataListenerControl_T.h"
#include "dds4ccm/impl/StateListenerControl_T.h"

#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::DDS_State_Connector_T (void) :
    TopicBaseConnector (),
    observable_obtained_ (false),
    passive_observer_obtained_ (false),
    pull_observer_obtained_ (false),
    push_observer_obtained_ (false),
    push_state_observer_obtained_ (false)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::~DDS_State_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::updater_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_observable_data (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_observable_data");

  this->observable_obtained_ = true;
  this->observable_.set_component (this);
  return this->observable_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataWriter_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_observable_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_observable_dds_entity");

  this->observable_obtained_ = true;
  this->observable_.set_component (this);
  return this->observable_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_passive_observer_data (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_passive_observer_data");

  this->passive_observer_obtained_ = true;
  this->passive_observer_.set_component (this);
  return this->passive_observer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_passive_observer_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_passive_observer_dds_entity");

  this->passive_observer_obtained_ = true;
  this->passive_observer_.set_component (this);
  return this->passive_observer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::CCM_ContentFilterSetting_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_passive_observer_filter_config (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_passive_observer_filter_config");

  this->passive_observer_obtained_ = true;
  this->passive_observer_.set_component (this);
  return this->passive_observer_.get_filter_config ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::QueryFilter*
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::passive_observer_filter (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::passive_observer_filter");

  return this->passive_observer_.filter ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::passive_observer_filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::passive_observer_filter");

  this->passive_observer_.filter (filter);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_observer_data (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_observer_data");

  this->pull_observer_obtained_ = true;
  this->pull_observer_.set_component (this);
  return this->pull_observer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::getter_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_observer_fresh_data (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_observer_fresh_data");

  this->pull_observer_obtained_ = true;
  this->pull_observer_.set_component (this);
  return this->pull_observer_.get_fresh_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_observer_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_observer_dds_entity");

  this->pull_observer_obtained_ = true;
  this->pull_observer_.set_component (this);
  return this->pull_observer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::CCM_ContentFilterSetting_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_observer_filter_config (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_observer_filter_config");

  this->pull_observer_obtained_ = true;
  this->pull_observer_.set_component (this);
  return this->pull_observer_.get_filter_config ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::QueryFilter*
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::pull_observer_filter (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::pull_observer_filter");

  return this->pull_observer_.filter ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::pull_observer_filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::pull_observer_filter");

  this->pull_observer_.filter (filter);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_observer_data (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_observer_data");

  this->push_observer_obtained_ = true;
  this->push_observer_.set_component (this);
  return this->push_observer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::CCM_DataListenerControl_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_observer_data_control (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_observer_data_control");

  this->push_observer_obtained_ = true;
  this->push_observer_.set_component (this);
  return this->push_observer_.get_data_control ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_observer_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_observer_dds_entity");

  this->push_observer_obtained_ = true;
  this->push_observer_.set_component (this);
  return this->push_observer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::CCM_ContentFilterSetting_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_observer_filter_config (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_observer_filter_config");

  this->push_observer_obtained_ = true;
  this->push_observer_.set_component (this);
  return this->push_observer_.get_filter_config ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::QueryFilter*
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::push_observer_filter (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::push_observer_filter");

  return this->push_observer_.filter ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::push_observer_filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::push_observer_filter");

  this->push_observer_.filter (filter);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_state_observer_data (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_state_observer_data");

  this->push_state_observer_obtained_ = true;
  this->push_state_observer_.set_component (this);
  return this->push_state_observer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::CCM_StateListenerControl_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_state_observer_data_control (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_state_observer_data_control");

  this->push_state_observer_obtained_ = true;
  this->push_state_observer_.set_component (this);
  return this->push_state_observer_.get_data_control ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_state_observer_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_state_observer_dds_entity");

  this->push_state_observer_obtained_ = true;
  this->push_state_observer_.set_component (this);
  return this->push_state_observer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::CCM_ContentFilterSetting_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_state_observer_filter_config (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_state_observer_filter_config");

  this->push_state_observer_obtained_ = true;
  this->push_state_observer_.set_component (this);
  return this->push_state_observer_.get_filter_config ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::QueryFilter*
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::push_state_observer_filter (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::push_state_observer_filter");

  return this->push_state_observer_.filter ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::push_state_observer_filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::push_state_observer_filter");

  this->push_state_observer_.filter (filter);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::topic_name (
  const char * topic_name)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::topic_name");

  if (this->late_binded (topic_name))
    {
      this->do_configuration_complete ();
      this->do_ccm_activate ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::do_configuration_complete (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::do_configuration_complete");

  TopicBaseConnector::configuration_complete ();

  ::CCM_DDS::PortStatusListener_var push_observer_psl =
    this->context_->get_connection_push_observer_status ();
  typename CCM_TYPE::listener_type::_var_type push_observer_dl =
    this->context_->get_connection_push_observer_data_listener ();
  this->push_observer_obtained_ |= ! ::CORBA::is_nil (push_observer_psl.in ());
  this->push_observer_obtained_ |= ! ::CORBA::is_nil (push_observer_dl.in ());

  ::CCM_DDS::PortStatusListener_var push_state_observer_psl =
    this->context_->get_connection_push_state_observer_status ();
  typename CCM_TYPE::statelistener_type::_var_type push_state_observer_dl =
    this->context_->get_connection_push_state_observer_data_listener ();
  this->push_state_observer_obtained_ |= ! ::CORBA::is_nil (push_state_observer_psl.in ());
  this->push_state_observer_obtained_ |= ! ::CORBA::is_nil (push_state_observer_dl.in ());

  ::CCM_DDS::PortStatusListener_var pull_observer_psl =
    this->context_->get_connection_pull_observer_status ();
  this->pull_observer_obtained_ |= ! ::CORBA::is_nil (pull_observer_psl.in ());

  ::CCM_DDS::PortStatusListener_var passive_observer_psl =
    this->context_->get_connection_passive_observer_status ();
  this->passive_observer_obtained_ |= ! ::CORBA::is_nil (passive_observer_psl.in ());

  if (this->observable_obtained_)
    {
      this->observable_.configuration_complete (
        this->topic_.in (),
        this->publisher_.in (),
        this->library_name_,
        this->profile_name_);
    }

  if (this->push_observer_obtained_)
    {
      this->push_observer_.configuration_complete (
        this->topic_.in (),
        this->subscriber_.in (),
        this->library_name_,
        this->profile_name_);
    }

  if (this->push_state_observer_obtained_)
    {
      this->push_state_observer_.configuration_complete (
        this->topic_.in (),
        this->subscriber_.in (),
        this->library_name_,
        this->profile_name_);
    }

  if (this->pull_observer_obtained_)
    {
      this->pull_observer_.configuration_complete (
        this->topic_.in (),
        this->subscriber_.in (),
        this->library_name_,
        this->profile_name_);
    }

  if (this->passive_observer_obtained_)
    {
      this->passive_observer_.configuration_complete (
        this->topic_.in (),
        this->subscriber_.in (),
        this->library_name_,
        this->profile_name_);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete");

  try
    {
      if (!this->late_binding ())
        {
          this->do_configuration_complete ();
        }
    }
  catch (const ::CCM_DDS::InternalError &ex)
    {
      DDS4CCM_PRINT_INTERNAL_EXCEPTION (
                              DDS4CCM_LOG_LEVEL_ERROR,
                              ::CIAO::DDS4CCM::translate_retcode (ex.error_code),
                              "DDS_State_Connector_T::configuration_complete");
      throw ::CORBA::INTERNAL ();
    }
  catch (const ::CORBA::Exception& ex)
    {
      DDS4CCM_PRINT_CORBA_EXCEPTION (
                              DDS4CCM_LOG_LEVEL_ERROR,
                              ex,
                              "DDS_State_Connector_T::configuration_complete");
      throw;
    }
  catch (...)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DDS_State_Connector_T::configuration_complete - "
                    "Caught unexpected exception.\n"));
      throw ::CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::do_ccm_activate (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::do_ccm_activate");

  ACE_Reactor* reactor = 0;

#if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
  reactor = this->reactor ();

  if (!reactor)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DDS_State_Connector_T::ccm_activate - "
                    "No reactor to perform required context switch.\n"));
      throw ::CORBA::INTERNAL ();
    }
#endif

  TopicBaseConnector::ccm_activate (reactor);

  if (this->observable_obtained_)
    {
      this->observable_.activate ();
    }

  if (this->push_observer_obtained_)
    {
      typename CCM_TYPE::listener_type::_var_type push_observer_dl =
        this->context_->get_connection_push_observer_data_listener ();
      ::CCM_DDS::PortStatusListener_var push_observer_psl =
        this->context_->get_connection_push_observer_status ();

      this->push_observer_.activate (
        push_observer_dl.in (),
        push_observer_psl.in (),
        reactor);
    }

  if (this->push_state_observer_obtained_)
    {
      typename CCM_TYPE::statelistener_type::_var_type push_state_observer_dl =
        this->context_->get_connection_push_state_observer_data_listener ();

      ::CCM_DDS::PortStatusListener_var push_state_observer_psl =
        this->context_->get_connection_push_state_observer_status ();

      this->push_state_observer_.activate (
        push_state_observer_dl.in (),
        push_state_observer_psl.in (),
        reactor);
    }

  if (this->pull_observer_obtained_)
    {
      ::CCM_DDS::PortStatusListener_var pull_observer_psl =
        this->context_->get_connection_pull_observer_status ();

      this->pull_observer_.activate (
        pull_observer_psl.in (),
        reactor);
    }

  if (this->passive_observer_obtained_)
    {
      ::CCM_DDS::PortStatusListener_var passive_observer_psl =
        this->context_->get_connection_passive_observer_status ();

      this->passive_observer_.activate (
        passive_observer_psl.in (),
        reactor);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_activate (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_activate");

  try
    {
      if (!this->late_binding ())
        {
          this->do_ccm_activate ();
        }
    }
  catch (const ::CCM_DDS::InternalError &ex)
    {
      DDS4CCM_PRINT_INTERNAL_EXCEPTION (
                              DDS4CCM_LOG_LEVEL_ERROR,
                              ::CIAO::DDS4CCM::translate_retcode (ex.error_code),
                              "DDS_State_Connector_T::ccm_activate");
      throw ::CORBA::INTERNAL ();
    }
  catch (const ::CORBA::Exception& ex)
    {
      DDS4CCM_PRINT_CORBA_EXCEPTION (
                              DDS4CCM_LOG_LEVEL_ERROR,
                              ex,
                              "DDS_State_Connector_T::ccm_activate");
      throw;
    }
  catch (...)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DDS_State_Connector_T::ccm_activate - "
                    "Caught unexpected exception.\n"));
      throw ::CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_passivate (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_passivate");

  try
    {
      if (ACE_OS::strlen (this->topic_name_) != 0)
        {
          if (this->observable_obtained_)
            {
              this->observable_.passivate ();
            }

          if (this->push_observer_obtained_)
            {
              this->push_observer_.passivate ();
            }

          if (this->push_state_observer_obtained_)
            {
              this->push_state_observer_.passivate ();
            }

          if (this->pull_observer_obtained_)
            {
              this->pull_observer_.passivate ();
            }

          if (this->passive_observer_obtained_)
            {
              this->passive_observer_.passivate ();
            }

          TopicBaseConnector::ccm_passivate ();
        }
    }
  catch (const ::CCM_DDS::InternalError &ex)
    {
      DDS4CCM_PRINT_INTERNAL_EXCEPTION (
                              DDS4CCM_LOG_LEVEL_ERROR,
                              ::CIAO::DDS4CCM::translate_retcode (ex.error_code),
                              "DDS_State_Connector_T::ccm_passivate");
      throw ::CORBA::INTERNAL ();
    }
  catch (const ::CORBA::Exception& ex)
    {
      DDS4CCM_PRINT_CORBA_EXCEPTION (
                              DDS4CCM_LOG_LEVEL_ERROR,
                              ex,
                              "DDS_State_Connector_T::ccm_passivate");
      throw;
    }
  catch (...)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DDS_State_Connector_T::ccm_passivate - "
                    "Caught unexpected exception.\n"));
      throw ::CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_remove (void)
{
  DDS4CCM_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_remove");

  try
    {
      if (ACE_OS::strlen (this->topic_name_) != 0)
        {
          if (this->observable_obtained_)
            {
              this->observable_.remove (this->publisher_.in ());
            }

          if (this->push_observer_obtained_)
            {
              this->push_observer_.remove (this->subscriber_.in ());
            }

          if (this->push_state_observer_obtained_)
            {
              this->push_state_observer_.remove (this->subscriber_.in ());
            }

          if (this->pull_observer_obtained_)
            {
              this->pull_observer_.remove (this->subscriber_.in ());
            }

          if (this->passive_observer_obtained_)
            {
              this->passive_observer_.remove (this->subscriber_.in ());
            }
          TopicBaseConnector::ccm_remove ();
        }
    }
  catch (const ::CCM_DDS::InternalError &ex)
    {
      DDS4CCM_PRINT_INTERNAL_EXCEPTION (
                              DDS4CCM_LOG_LEVEL_ERROR,
                              ::CIAO::DDS4CCM::translate_retcode (ex.error_code),
                              "DDS_State_Connector_T::ccm_remove");
      throw ::CORBA::INTERNAL ();
    }
  catch (const ::CORBA::Exception& ex)
    {
      DDS4CCM_PRINT_CORBA_EXCEPTION (
                              DDS4CCM_LOG_LEVEL_ERROR,
                              ex,
                              "DDS_State_Connector_T::ccm_remove");
      throw;
    }
  catch (...)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DDS_State_Connector_T::ccm_remove - "
                    "Caught unexpected exception.\n"));
      throw ::CORBA::INTERNAL ();
    }
}

