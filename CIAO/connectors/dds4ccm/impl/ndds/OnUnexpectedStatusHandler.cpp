// $Id$

#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/ndds/OnUnexpectedStatusHandler.h"
#include "ciao/Logger/Log_Macros.h"

CIAO::DDS4CCM::On_Unexpected_Status_Handler::On_Unexpected_Status_Handler (
  ::CCM_DDS::ConnectorStatusListener_ptr error_listener,
  ::DDS::Entity_ptr entity,
  ::DDS::StatusKind status_kind) :
    error_listener_ (::CCM_DDS::ConnectorStatusListener::_duplicate (error_listener)),
    entity_ (::DDS::Entity::_duplicate (entity)),
    status_kind_ (status_kind)
{
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

CIAO::DDS4CCM::On_Unexpected_Status_Handler::~On_Unexpected_Status_Handler (void)
{
}

int
CIAO::DDS4CCM::On_Unexpected_Status_Handler::handle_exception (ACE_HANDLE)
{
  try
    {
      this->error_listener_->on_unexpected_status (this->entity_, this->status_kind_);
    }
  catch (...)
    {
    }
  return 0;
}

CIAO::DDS4CCM::OnRequestedOncompatibleQosHandler::OnRequestedOncompatibleQosHandler (
  ::CCM_DDS::ConnectorStatusListener_ptr csl,
  ::DDS::DataReader_ptr dr,
  const ::DDS::RequestedIncompatibleQosStatus &status) :
    csl_ (::CCM_DDS::ConnectorStatusListener::_duplicate (csl)),
    dr_ (::DDS::DataReader::_duplicate (dr)),
    status_ (status)
{
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

CIAO::DDS4CCM::OnRequestedOncompatibleQosHandler::~OnRequestedOncompatibleQosHandler (void)
{
}

int
CIAO::DDS4CCM::OnRequestedOncompatibleQosHandler::handle_exception (ACE_HANDLE)
{
  try
    {
      this->csl_->on_requested_incompatible_qos (this->dr_, this->status_);
    }
  catch (...)
    {
    }
  return 0;
}

CIAO::DDS4CCM::OnSampleRejectedHandler::OnSampleRejectedHandler (
  ::CCM_DDS::ConnectorStatusListener_ptr csl,
  ::DDS::DataReader_ptr dr,
  const ::DDS::SampleRejectedStatus &status) :
    csl_ (::CCM_DDS::ConnectorStatusListener::_duplicate (csl)),
    dr_ (::DDS::DataReader::_duplicate (dr)),
    status_ (status)
{
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

CIAO::DDS4CCM::OnSampleRejectedHandler::~OnSampleRejectedHandler (void)
{
}

int
CIAO::DDS4CCM::OnSampleRejectedHandler::handle_exception (ACE_HANDLE)
{
  try
    {
      this->csl_->on_sample_rejected (this->dr_, this->status_);
    }
  catch (...)
    {
    }
  return 0;
}

CIAO::DDS4CCM::OnInconsistentTopicHandler::OnInconsistentTopicHandler (
  ::CCM_DDS::ConnectorStatusListener_ptr csl,
  ::DDS::Topic_ptr tp,
  const ::DDS::InconsistentTopicStatus &status) :
    csl_ (::CCM_DDS::ConnectorStatusListener::_duplicate (csl)),
    tp_ (::DDS::Topic::_duplicate (tp)),
    status_ (status)
{
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

CIAO::DDS4CCM::OnInconsistentTopicHandler::~OnInconsistentTopicHandler (void)
{
}

int
CIAO::DDS4CCM::OnInconsistentTopicHandler::handle_exception (ACE_HANDLE)
{
  try
    {
      this->csl_->on_inconsistent_topic (this->tp_, this->status_);
    }
  catch (...)
    {
    }
  return 0;
}
