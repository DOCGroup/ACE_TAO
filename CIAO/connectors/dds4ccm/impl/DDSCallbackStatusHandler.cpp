// $Id$

#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/DDSCallbackStatusHandler.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

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
      catch (const ::CORBA::Exception& ex)
        {
          DDS4CCM_PRINT_CORBA_EXCEPTION (
                                  DDS4CCM_LOG_LEVEL_ERROR,
                                  ex,
                                  "OnUnexpectedStatusHandler::handle_exception");
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            "OnUnexpectedStatusHandler::handle_exception - "
            "Unexpected exception caught for <%C>\n",
            translate_statuskind (this->status_kind_)));

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
      catch (const ::CORBA::Exception& ex)
        {
          DDS4CCM_PRINT_CORBA_EXCEPTION (
                                  DDS4CCM_LOG_LEVEL_ERROR,
                                  ex,
                                  "OnRequestedOncompatibleQosHandler::handle_exception");
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            "OnRequestedOncompatibleQosHandler::handle_exception - "
            "Unexpected exception caught\n"));
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
      catch (const ::CORBA::Exception& ex)
        {
          DDS4CCM_PRINT_CORBA_EXCEPTION (
                                  DDS4CCM_LOG_LEVEL_ERROR,
                                  ex,
                                  "OnSampleRejectedHandler::handle_exception");
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            "OnSampleRejectedHandler::handle_exception - "
            "Unexpected exception caught\n"));
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
      catch (const ::CORBA::Exception& ex)
        {
          DDS4CCM_PRINT_CORBA_EXCEPTION (
                                  DDS4CCM_LOG_LEVEL_ERROR,
                                  ex,
                                  "OnInconsistentTopicHandler::handle_exception");
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            "OnInconsistentTopicHandler::handle_exception - "
            "Unexpected exception caught\n"));
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
      catch (const ::CORBA::Exception& ex)
        {
          DDS4CCM_PRINT_CORBA_EXCEPTION (
                                  DDS4CCM_LOG_LEVEL_ERROR,
                                  ex,
                                  "OnOfferedDeadlineMissedHandler::handle_exception");
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            "OnOfferedDeadlineMissedHandler::handle_exception - "
            "Unexpected exception caught\n"));
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
      catch (const ::CORBA::Exception& ex)
        {
          DDS4CCM_PRINT_CORBA_EXCEPTION (
                                  DDS4CCM_LOG_LEVEL_ERROR,
                                  ex,
                                  "OnOfferedIncompatibleQoSHandler::handle_exception");
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            "OnOfferedIncompatibleQoSHandler::handle_exception - "
            "Unexpected exception caught\n"));
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
      catch (const ::CORBA::Exception& ex)
        {
          DDS4CCM_PRINT_CORBA_EXCEPTION (
                                  DDS4CCM_LOG_LEVEL_ERROR,
                                  ex,
                                  "OnRequestedDeadlineMissedHandler::handle_exception");
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            "OnRequestedDeadlineMissedHandler::handle_exception - "
            "Unexpected exception caught\n"));
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
      catch (const ::CORBA::Exception& ex)
        {
          DDS4CCM_PRINT_CORBA_EXCEPTION (
                                  DDS4CCM_LOG_LEVEL_ERROR,
                                  ex,
                                  "OnSampleLostHandler::handle_exception");
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            "OnSampleLostHandler::handle_exception - "
            "Unexpected exception caught\n"));
        }
      return 0;
    }
  }
}
