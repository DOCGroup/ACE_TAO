// $Id$
#include "dds4ccm/impl/Utils.h"

#include "dds4ccm/impl/Log_Macros.h"
#include "dds4ccm/impl/DataReaderHandler_T.h"
#include "tao/ORB_Core.h"

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::PortStatusListener_T (
      ::CCM_DDS::PortStatusListener_ptr port_status_listener,
      ACE_Reactor* reactor)
      : port_status_listener_ (::CCM_DDS::PortStatusListener::_duplicate (port_status_listener)),
        reactor_ (reactor)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::PortStatusListener_T::PortStatusListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~PortStatusListener_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::PortStatusListener_T::~PortStatusListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_requested_deadline_missed (
  ::DDS::DataReader_ptr the_reader,
  const ::DDS::RequestedDeadlineMissedStatus & status)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::PortStatusListener_T::on_requested_deadline_missed");

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, CLINFO
                ACE_TEXT ("PortStatusListener_T::on_requested_deadline_missed: ")
                ACE_TEXT ("total count <%d> - total change <%d> - ")
                ACE_TEXT ("last instance handle <length <%l> - isValid <%l>\n"),
                status.total_count, status.total_count_change,
                status.last_instance_handle.length, status.last_instance_handle.isValid));

  if (! ::CORBA::is_nil (this->port_status_listener_))
    {
      try
        {
          if (this->reactor_)
            {
              ::CIAO::DDS4CCM::OnRequestedDeadlineMissedHandler* rh = 0;
              ACE_NEW (rh,
                       ::CIAO::DDS4CCM::OnRequestedDeadlineMissedHandler (
                         this->port_status_listener_, the_reader, status));
              ACE_Event_Handler_var safe_handler (rh);
              if (this->reactor_->notify (rh) != 0)
                {
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                              ACE_TEXT ("PortStatusListener_T::on_requested_deadline_missed: ")
                              ACE_TEXT ("failed to use reactor.\n")));
                }
            }
          else
            {
              this->port_status_listener_->on_requested_deadline_missed (the_reader, status);
            }
        }
      catch (...)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, CLINFO
                        ACE_TEXT ("PortStatusListener_T::on_requested_deadline_missed: ")
                        ACE_TEXT ("DDS Exception caught\n")));
        }
    }
  else
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, CLINFO
                    ACE_TEXT ("PortStatusListener_T::on_requested_deadline_missed: ")
                    ACE_TEXT ("No portstatus listener installed\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_sample_lost (
  ::DDS::DataReader_ptr the_reader,
  const ::DDS::SampleLostStatus & status)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::PortStatusListener_T::on_sample_lost");

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, CLINFO
                ACE_TEXT ("PortStatusListener_T::on_sample_lost: ")
                ACE_TEXT ("total count <%d> - total change <%d>\n"),
                status.total_count, status.total_count_change));

  if (! ::CORBA::is_nil (this->port_status_listener_))
    {
      try
        {
          if (this->reactor_)
            {
              ::CIAO::DDS4CCM::OnSampleLostHandler* rh = 0;
              ACE_NEW (rh,
                       ::CIAO::DDS4CCM::OnSampleLostHandler (
                         this->port_status_listener_,
                         the_reader,
                         status));
              ACE_Event_Handler_var safe_handler (rh);
              if (this->reactor_->notify (rh) != 0)
                {
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                                ACE_TEXT ("PortStatusListener_T::on_sample_lost: ")
                                ACE_TEXT ("failed to use reactor.\n")));
                }
            }
          else
            {
              this->port_status_listener_->on_sample_lost (the_reader, status);
            }
        }
      catch (...)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        ACE_TEXT ("PortStatusListener_T::on_sample_lost: ")
                        ACE_TEXT ("DDS Exception caught\n")));
        }
    }
  else
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                    ACE_TEXT ("PortStatusListener_T::on_sample_lost: ")
                    ACE_TEXT ("No portstatus listener installed\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_requested_incompatible_qos (
  ::DDS::DataReader_ptr ,
  const ::DDS::RequestedIncompatibleQosStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_sample_rejected (
  ::DDS::DataReader_ptr ,
  const ::DDS::SampleRejectedStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_liveliness_changed (
  ::DDS::DataReader_ptr ,
  const ::DDS::LivelinessChangedStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_data_available (
  ::DDS::DataReader_ptr )
{
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_subscription_matched (
  ::DDS::DataReader_ptr ,
  const ::DDS::SubscriptionMatchedStatus & )
{
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_mask (
  ::CCM_DDS::PortStatusListener_ptr psl)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::PortStatusListener_T::get_mask");

  if (! ::CORBA::is_nil (psl) ||
      CIAO_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, CLINFO
                    "PortStatusListener_T::get_mask - "
                    "Mask becomes %d\n",
                    DDS_REQUESTED_DEADLINE_MISSED_STATUS |
                    DDS_SAMPLE_LOST_STATUS));
      return DDS_REQUESTED_DEADLINE_MISSED_STATUS |
             DDS_SAMPLE_LOST_STATUS;
    }
  else
    {
      return 0;
    }
}
