// $Id$

#include "dds4ccm/impl/SubscriberListener.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds4ccm/impl/DDSCallbackStatusHandler.h"
#include "dds4ccm/impl/Utils.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    SubscriberListener::SubscriberListener (
        ::CCM_DDS::ConnectorStatusListener_ptr error_listener,
         ACE_Reactor* reactor)
    : error_listener_ (::CCM_DDS::ConnectorStatusListener::_duplicate (error_listener)),
      reactor_ (reactor)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::SubscriberListener::SubscriberListener");
    }

    SubscriberListener::~SubscriberListener (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::SubscriberListener::~SubscriberListener");
    }

    void
    SubscriberListener::on_requested_incompatible_qos (
        ::DDS::DataReader_ptr reader,
         const ::DDS::RequestedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::SubscriberListener::on_requested_incompatible_qos");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
          ACE_TEXT ("SubscriberListener::on_requested_incompatible_qos: ")
          ACE_TEXT ("total count <%d> - total change <%d> - ")
          ACE_TEXT ("last policy id <%C> - policies:\n"),
      status.total_count, status.total_count_change,
      translate_qospolicyid_t (status.last_policy_id)));
      for (CORBA::ULong i = 0; i < status.policies.length (); ++i)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG,
              ACE_TEXT ("\t\tid <%C> - count <%d>\n"),
              translate_qospolicyid_t (status.policies[i].policy_id),
              status.policies[i].count));
        }

      if (! ::CORBA::is_nil (this->error_listener_))
        {
          try
          {
              if (this->reactor_)
                {
                  ::CIAO::DDS4CCM::OnRequestedOncompatibleQosHandler* rh = 0;
                  ACE_NEW (rh,
                      ::CIAO::DDS4CCM::OnRequestedOncompatibleQosHandler (
                          this->error_listener_, reader, status));
                  ACE_Event_Handler_var safe_handler (rh);
                  if (this->reactor_->notify (rh) != 0)
                    {
                      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                          ACE_TEXT ("SubscriberListener::")
                      ACE_TEXT ("on_requested_incompatible_qos - ")
                      ACE_TEXT ("failed to use reactor.\n")));
                    }
                }
              else
                {
                  this->error_listener_->on_requested_incompatible_qos (reader, status);
                }
          }
          catch (const ::CORBA::Exception& ex)
          {
              DDS4CCM_PRINT_CORBA_EXCEPTION (
                  DDS4CCM_LOG_LEVEL_ERROR,
                  ex,
                  "SubscriberListener::on_requested_incompatible_qos");
          }
          catch (...)
          {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                  "SubscriberListener::on_requested_incompatible_qos - "
                  "Unexpected exception caught\n"));
          }
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
              ACE_TEXT ("SubscriberListener::on_requested_incompatible_qos - ")
          ACE_TEXT ("No error listener connected\n")));
        }
    }

    void
    SubscriberListener::on_unexpected_status (
        ::DDS::Entity_ptr entity,
         ::DDS::StatusKind status_kind)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::SubscriberListener::on_unexpected_status");

      if (! ::CORBA::is_nil (this->error_listener_))
        {
          try
          {
              if (this->reactor_)
                {
                  ::CIAO::DDS4CCM::OnUnexpectedStatusHandler* rh = 0;
                  ACE_NEW (rh,
                      ::CIAO::DDS4CCM::OnUnexpectedStatusHandler (
                          this->error_listener_, entity, status_kind));
                  ACE_Event_Handler_var safe_handler (rh);
                  if (this->reactor_->notify (rh) != 0)
                    {
                      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                          ACE_TEXT ("SubscriberListener::on_unexpected_status - ")
                      ACE_TEXT ("failed to use reactor.\n")));
                    }
                }
              else
                {
                  this->error_listener_->on_unexpected_status (entity, status_kind);
                }
          }
          catch (const ::CORBA::Exception& ex)
          {
              DDS4CCM_PRINT_CORBA_EXCEPTION (
                  DDS4CCM_LOG_LEVEL_ERROR,
                  ex,
                  "SubscriberListener::on_unexpected_status");
          }
          catch (...)
          {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                  "SubscriberListener::on_unexpected_status - "
                  "Unexpected exception caught for <%C>\n",
                  translate_statuskind (status_kind)));
          }
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
              ACE_TEXT ("SubscriberListener::on_unexpected_status - ")
          ACE_TEXT ("No error listener connected for <%C>\n"),
          translate_statuskind (status_kind)));
        }
    }

    void
    SubscriberListener::on_liveliness_changed (
        ::DDS::DataReader* reader,
         const ::DDS::LivelinessChangedStatus& status)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::SubscriberListener::on_liveliness_changed");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
          ACE_TEXT ("SubscriberListener::on_liveliness_changed: ")
      ACE_TEXT ("alive count <%d> - not alive count <%d> - ")
      ACE_TEXT ("alive count change <%d> - not alive count change <%d> ")
      ACE_TEXT ("last publication handle length <%d> - isValid <%d>\n"),
      status.alive_count, status.not_alive_count,
      status.alive_count_change, status.not_alive_count_change,
      status.last_publication_handle.length,
      status.last_publication_handle.isValid));

      this->on_unexpected_status (reader, ::DDS::LIVELINESS_CHANGED_STATUS);
    }

    void
    SubscriberListener::on_sample_rejected (
        ::DDS::DataReader_ptr reader,
         const ::DDS::SampleRejectedStatus& status)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::SubscriberListener::on_sample_rejected");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
          ACE_TEXT ("SubscriberListener::on_sample_rejected: ")
      ACE_TEXT ("total count <%d> - count change <%d> - ")
      ACE_TEXT ("last reason <%C> - last instance handle ")
      ACE_TEXT ("<length <%d> - isValid <%d>\n"),
      status.total_count, status.total_count_change,
      translate_rejectedstatuskind (status.last_reason),
      status.last_instance_handle.length,
      status.last_instance_handle.isValid));

      if (! ::CORBA::is_nil (this->error_listener_))
        {
          try
          {
              if (this->reactor_)
                {
                  ::CIAO::DDS4CCM::OnSampleRejectedHandler* rh = 0;
                  ACE_NEW (rh,
                      ::CIAO::DDS4CCM::OnSampleRejectedHandler (
                          this->error_listener_, reader, status));
                  ACE_Event_Handler_var safe_handler (rh);
                  if (this->reactor_->notify (rh) != 0)
                    {
                      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                          ACE_TEXT ("SubscriberListener::on_sample_rejected - ")
                      ACE_TEXT ("failed to use reactor.\n")));
                    }
                }
              else
                {
                  this->error_listener_->on_sample_rejected (reader, status);
                }
          }
          catch (const ::CORBA::Exception& ex)
          {
              DDS4CCM_PRINT_CORBA_EXCEPTION (
                  DDS4CCM_LOG_LEVEL_ERROR,
                  ex,
                  "SubscriberListener::on_sample_rejected");
          }
          catch (...)
          {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                  "SubscriberListener::on_sample_rejected - "
                  "Unexpected exception caught\n"));
          }
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
              ACE_TEXT ("SubscriberListener::on_sample_rejected - ")
          ACE_TEXT ("No error listener connected\n")));
        }
    }

    void
    SubscriberListener::on_subscription_matched(
        ::DDS::DataReader* reader,
         const ::DDS::SubscriptionMatchedStatus& status)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::SubscriberListener::on_subscription_matched");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
          ACE_TEXT ("SubscriberListener::on_subscription_matched: ")
      ACE_TEXT ("total count <%d> - count change <%d> - ")
      ACE_TEXT ("current count <%d> - current count change <%d> ")
      ACE_TEXT ("last publication handle length <%d> - isValid <%d>\n"),
      status.total_count, status.total_count_change,
      status.current_count, status.current_count_change,
      status.last_publication_handle.length,
      status.last_publication_handle.isValid));

      this->on_unexpected_status (reader, ::DDS::SUBSCRIPTION_MATCHED_STATUS);
    }

    void
    SubscriberListener::on_data_on_readers(
        ::DDS::Subscriber *)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::SubscriberListener::on_data_on_readers");
      //do nothing by design
    }

    void
    SubscriberListener::on_requested_deadline_missed (
        ::DDS::DataReader_ptr,
         const ::DDS::RequestedDeadlineMissedStatus &)
    {
    }

    void
    SubscriberListener::on_data_available (
        ::DDS::DataReader_ptr)
    {
    }

    void
    SubscriberListener::on_sample_lost (
        ::DDS::DataReader_ptr,
         const ::DDS::SampleLostStatus &)
    {
    }

    ::DDS::StatusMask
    SubscriberListener::get_mask (
         ::CCM_DDS::ConnectorStatusListener_ptr csl)
    {
      ::DDS::StatusMask mask = 0;

      if (! ::CORBA::is_nil (csl) ||
          DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
        {
          mask = ::DDS::REQUESTED_INCOMPATIBLE_QOS_STATUS |
              ::DDS::SAMPLE_REJECTED_STATUS |
               ::DDS::LIVELINESS_CHANGED_STATUS |
                ::DDS::SUBSCRIPTION_MATCHED_STATUS;
        }

      if (DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
        {
          ACE_CString msk;
          translate_statusmask (msk, mask);
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
              "SubscriberListener::get_mask - "
              "Mask becomes %C\n",
              msk.c_str ()));
        }
      return mask;
    }
  }
}

