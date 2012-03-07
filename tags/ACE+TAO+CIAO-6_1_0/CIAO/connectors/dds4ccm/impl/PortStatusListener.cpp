// $Id$

#include "dds4ccm/impl/PortStatusListener.h"
#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds4ccm/impl/DDSCallbackStatusHandler.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    PortStatusListener::PortStatusListener (
        ::CCM_DDS::PortStatusListener_ptr port_status_listener,
         ACE_Reactor* reactor)
    : port_status_listener_ (::CCM_DDS::PortStatusListener::_duplicate (port_status_listener)),
      reactor_ (reactor)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::PortStatusListener::PortStatusListener");
    }

    PortStatusListener::~PortStatusListener (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::PortStatusListener::~PortStatusListener");
    }

    void
    PortStatusListener::on_requested_deadline_missed (
        ::DDS::DataReader_ptr the_reader,
         const ::DDS::RequestedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::PortStatusListener::on_requested_deadline_missed");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
          ACE_TEXT ("PortStatusListener::on_requested_deadline_missed: ")
          ACE_TEXT ("total count <%d> - total change <%d> - ")
          ACE_TEXT ("last instance") DDS_INSTANCE_HANDLE_FORMAT_SPECIFIER
          ACE_TEXT ("\n"),
          status.total_count, status.total_count_change,
          DDS_INSTANCE_HANDLE_LOG (status.last_instance_handle)));

      if (! ::CORBA::is_nil (this->port_status_listener_))
        {
          try
            {
              if (this->reactor_)
                {
                  ::CIAO::DDS4CCM::OnRequestedDeadlineMissedHandler* rh = 0;
                  ACE_NEW (rh,
                      ::CIAO::DDS4CCM::OnRequestedDeadlineMissedHandler (
                            this->port_status_listener_.in (), the_reader, status));
                  ACE_Event_Handler_var safe_handler (rh);
                  if (this->reactor_->notify (rh) != 0)
                    {
                      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                          ACE_TEXT ("PortStatusListener::on_requested_deadline_missed: ")
                          ACE_TEXT ("failed to use reactor.\n")));
                    }
                }
              else
                {
                  this->port_status_listener_->on_requested_deadline_missed (the_reader, status);
                }
            }
          catch (const ::CORBA::Exception& ex)
            {
              DDS4CCM_PRINT_CORBA_EXCEPTION (
                  DDS4CCM_LOG_LEVEL_ERROR,
                  ex,
                  "PortStatusListener::on_requested_deadline_missed");
            }
          catch (...)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                  "PortStatusListener::on_requested_deadline_missed - "
                  "Unexpected exception caught\n"));
            }
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
              ACE_TEXT ("PortStatusListener::on_requested_deadline_missed: ")
          ACE_TEXT ("No portstatus listener installed\n")));
        }
    }

    void
    PortStatusListener::on_sample_lost (
        ::DDS::DataReader_ptr the_reader,
         const ::DDS::SampleLostStatus & status)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::PortStatusListener::on_sample_lost");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
          ACE_TEXT ("PortStatusListener::on_sample_lost: ")
      ACE_TEXT ("total count <%d> - total change <%d>\n"),
      status.total_count, status.total_count_change));

      if (! ::CORBA::is_nil (this->port_status_listener_.in ()))
        {
          try
            {
              if (this->reactor_)
                {
                  ::CIAO::DDS4CCM::OnSampleLostHandler* rh = 0;
                  ACE_NEW (rh,
                      ::CIAO::DDS4CCM::OnSampleLostHandler (
                          this->port_status_listener_.in (),
                          the_reader,
                          status));
                  ACE_Event_Handler_var safe_handler (rh);
                  if (this->reactor_->notify (rh) != 0)
                    {
                      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                          ACE_TEXT ("PortStatusListener::on_sample_lost: ")
                      ACE_TEXT ("failed to use reactor.\n")));
                    }
                }
              else
                {
                  this->port_status_listener_->on_sample_lost (the_reader, status);
                }
            }
          catch (const ::CORBA::Exception& ex)
            {
              DDS4CCM_PRINT_CORBA_EXCEPTION (
                  DDS4CCM_LOG_LEVEL_ERROR,
                  ex,
                  "PortStatusListener::on_sample_lost");
            }
          catch (...)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                  "PortStatusListener::on_sample_lost - "
                  "Unexpected exception caught\n"));
            }
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
              ACE_TEXT ("PortStatusListener::on_sample_lost: ")
              ACE_TEXT ("No portstatus listener installed\n")));
        }
    }

    void
    PortStatusListener::on_requested_incompatible_qos (
        ::DDS::DataReader_ptr ,
         const ::DDS::RequestedIncompatibleQosStatus & )
    {
    }

    void
    PortStatusListener::on_sample_rejected (
        ::DDS::DataReader_ptr ,
         const ::DDS::SampleRejectedStatus & )
    {
    }

    void
    PortStatusListener::on_liveliness_changed (
        ::DDS::DataReader_ptr ,
         const ::DDS::LivelinessChangedStatus & )
    {
    }

    void
    PortStatusListener::on_data_available (
        ::DDS::DataReader_ptr )
    {
    }

    void
    PortStatusListener::on_subscription_matched (
        ::DDS::DataReader_ptr ,
         const ::DDS::SubscriptionMatchedStatus & )
    {
    }

    ::DDS::StatusMask
    PortStatusListener::get_mask (
         ::CCM_DDS::PortStatusListener_ptr psl)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::PortStatusListener::get_mask");

      ::DDS::StatusMask mask = 0;
      if (! ::CORBA::is_nil (psl) ||
          DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
        {
          mask = ::DDS::REQUESTED_DEADLINE_MISSED_STATUS |
                 ::DDS::SAMPLE_LOST_STATUS;
        }

      if (DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
        {
          ACE_CString msk;
          translate_statusmask (msk, mask);
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
              "PortStatusListener::get_mask - "
              "Mask becomes %C\n",
              msk.c_str ()));
        }
      return mask;
    }
  }
}
