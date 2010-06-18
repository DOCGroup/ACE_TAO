// $Id$

#include "dds4ccm/impl/dds/Utils.h"
#include "dds4ccm/impl/dds/DDSCallbackStatusHandler.h"
#include "dds4ccm/impl/dds/Log_Macros.h"

//============================================================
// On_Unexpected_Status_Handler
//============================================================
namespace CIAO
{
  namespace DDS4CCM
  {
    OnUnexpectedStatusHandler::OnUnexpectedStatusHandler (
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

    OnUnexpectedStatusHandler::~OnUnexpectedStatusHandler (void)
    {
    }

    int
    OnUnexpectedStatusHandler::handle_exception (ACE_HANDLE)
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

    //============================================================
    // OnRequestedOncompatibleQosHandler
    //============================================================
    OnRequestedOncompatibleQosHandler::OnRequestedOncompatibleQosHandler (
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

    OnRequestedOncompatibleQosHandler::~OnRequestedOncompatibleQosHandler (void)
    {
    }

    int
    OnRequestedOncompatibleQosHandler::handle_exception (ACE_HANDLE)
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

    //============================================================
    // OnSampleRejectedHandler
    //============================================================
    OnSampleRejectedHandler::OnSampleRejectedHandler (
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

    OnSampleRejectedHandler::~OnSampleRejectedHandler (void)
    {
    }

    int
    OnSampleRejectedHandler::handle_exception (ACE_HANDLE)
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

    //============================================================
    // OnInconsistentTopicHandler
    //============================================================
    OnInconsistentTopicHandler::OnInconsistentTopicHandler (
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

    OnInconsistentTopicHandler::~OnInconsistentTopicHandler (void)
    {
    }

    int
    OnInconsistentTopicHandler::handle_exception (ACE_HANDLE)
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

    //============================================================
    // OnOfferedDeadlineMissedHandler
    //============================================================
    OnOfferedDeadlineMissedHandler::OnOfferedDeadlineMissedHandler (
      ::CCM_DDS::ConnectorStatusListener_ptr csl,
      ::DDS::DataWriter_ptr dw,
      const ::DDS::OfferedDeadlineMissedStatus &status) :
        csl_ (::CCM_DDS::ConnectorStatusListener::_duplicate (csl)),
        dw_ (::DDS::DataWriter::_duplicate (dw)),
        status_ (status)
    {
      this->reference_counting_policy ().value
        (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
    }

    OnOfferedDeadlineMissedHandler::~OnOfferedDeadlineMissedHandler (void)
    {
    }

    int
    OnOfferedDeadlineMissedHandler::handle_exception (ACE_HANDLE)
    {
      try
        {
          this->csl_->on_offered_deadline_missed (this->dw_, this->status_);
        }
      catch (...)
        {
        }
      return 0;
    }

    //============================================================
    // OnOfferedIncompatibleQoSHandler
    //============================================================
    OnOfferedIncompatibleQoSHandler::OnOfferedIncompatibleQoSHandler (
      ::CCM_DDS::ConnectorStatusListener_ptr csl,
      ::DDS::DataWriter_ptr dw,
      const ::DDS::OfferedIncompatibleQosStatus &status) :
        csl_ (::CCM_DDS::ConnectorStatusListener::_duplicate (csl)),
        dw_ (::DDS::DataWriter::_duplicate (dw)),
        status_ (status)
    {
      this->reference_counting_policy ().value
        (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
    }

    OnOfferedIncompatibleQoSHandler::~OnOfferedIncompatibleQoSHandler (void)
    {
    }

    int
    OnOfferedIncompatibleQoSHandler::handle_exception (ACE_HANDLE)
    {
      try
        {
          this->csl_->on_offered_incompatible_qos (this->dw_, this->status_);
        }
      catch (...)
        {
        }
      return 0;
    }

    //============================================================
    // OnRequestedDeadlineMissedHandler
    //============================================================
    OnRequestedDeadlineMissedHandler::OnRequestedDeadlineMissedHandler (
      ::CCM_DDS::PortStatusListener_ptr psl,
      ::DDS::DataReader_ptr dr,
      const ::DDS::RequestedDeadlineMissedStatus & status) :
        psl_ (::CCM_DDS::PortStatusListener::_duplicate (psl)),
        dr_ (::DDS::DataReader::_duplicate (dr)),
        status_ (status)
    {
    }

    OnRequestedDeadlineMissedHandler::~OnRequestedDeadlineMissedHandler ()
    {
    }

    int
    OnRequestedDeadlineMissedHandler::handle_exception (ACE_HANDLE)
    {
      try
        {
          this->psl_->on_requested_deadline_missed (this->dr_, this->status_);
        }
      catch (...)
        {
        }
      return 0;
    }

    //============================================================
    // OnSampleLostHandler
    //============================================================
    OnSampleLostHandler::OnSampleLostHandler (
      ::CCM_DDS::PortStatusListener_ptr psl,
      ::DDS::DataReader_ptr dr,
      const ::DDS::SampleLostStatus & status) :
        psl_ (::CCM_DDS::PortStatusListener::_duplicate (psl)),
        dr_ (::DDS::DataReader::_duplicate (dr)),
        status_ (status)
    {
    }

    OnSampleLostHandler::~OnSampleLostHandler ()
    {
    }

    int
    OnSampleLostHandler::handle_exception (ACE_HANDLE)
    {
      try
        {
          this->psl_->on_sample_lost (this->dr_, this->status_);
        }
      catch (...)
        {
        }
      return 0;
    }
  }
}
