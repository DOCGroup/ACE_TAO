// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/DataReaderHandler_T.h"
#include "tao/ORB_Core.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::PortStatusListener_T (
      ::CCM_DDS::PortStatusListener_ptr port_status_listener,
      ACE_Reactor* reactor)
      : port_status_listener_ (::CCM_DDS::PortStatusListener::_duplicate (port_status_listener)),
        reactor_ (reactor)
{
  CIAO_TRACE ("CIAO::DDS4CCM::PortStatusListener_T::PortStatusListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::~PortStatusListener_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::PortStatusListener_T::~PortStatusListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::on_requested_deadline_missed (
  ::DDS::DataReader_ptr the_reader,
  const ::DDS::RequestedDeadlineMissedStatus & status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::PortStatusListener_T::on_requested_deadline_missed");

  CIAO_DEBUG (10, (LM_DEBUG, CLINFO
              ACE_TEXT ("PortStatusListener_T::on_requested_deadline_missed: ")
              ACE_TEXT ("total count <%d> - total change <%d> - ")
              ACE_TEXT ("last instance handle <length <%l> - isValid <%l>>\n"),
              status.total_count, status.total_count_change,
              status.last_instance_handle.length, status.last_instance_handle.isValid));

  if (!CORBA::is_nil (this->port_status_listener_))
    {
      try
        {
          if (this->reactor_)
            {
              ::CIAO::DDS4CCM::OnRequestedDeadlineMissedHandler* rh =
              new ::CIAO::DDS4CCM::OnRequestedDeadlineMissedHandler (
                this->port_status_listener_, the_reader, status);
              ACE_Event_Handler_var safe_handler (rh);
              if (this->reactor_->notify (rh) != 0)
                {
                  ACE_ERROR ((LM_ERROR, CLINFO
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
          CIAO_DEBUG (6, (LM_DEBUG, ACE_TEXT ("PortStatusListener_T::on_requested_deadline_missed: ")
                                 ACE_TEXT ("DDS Exception caught\n")));
        }
    }
  else
    {
      CIAO_DEBUG (6, (LM_DEBUG,
                  ACE_TEXT ("PortStatusListener_T::on_requested_deadline_missed: ")
                  ACE_TEXT ("No portstatus listener installed\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::on_sample_lost (
  ::DDS::DataReader_ptr the_reader,
  const ::DDS::SampleLostStatus & status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::PortStatusListener_T::on_sample_lost");

  CIAO_DEBUG (10, (LM_DEBUG, CLINFO
              ACE_TEXT ("PortStatusListener_T::on_sample_lost: ")
              ACE_TEXT ("total count <%d> - total change <%d>\n"),
              status.total_count, status.total_count_change));

  if (!CORBA::is_nil (this->port_status_listener_))
    {
      try
        {
          if (this->reactor_)
            {
              ::CIAO::DDS4CCM::OnSampleLostHandler* rh =
              new ::CIAO::DDS4CCM::OnSampleLostHandler (
                this->port_status_listener_, the_reader, status);
              ACE_Event_Handler_var safe_handler (rh);
              if (this->reactor_->notify (rh) != 0)
                {
                  ACE_ERROR ((LM_ERROR, CLINFO
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
          CIAO_DEBUG (6, (LM_DEBUG, ACE_TEXT ("PortStatusListener_T::on_sample_lost: ")
                                 ACE_TEXT ("DDS Exception caught\n")));
        }
    }
  else
    {
      CIAO_DEBUG (6, (LM_DEBUG,
                  ACE_TEXT ("PortStatusListener_T::on_sample_lost: ")
                  ACE_TEXT ("No portstatus listener installed\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask (void)
{
  return DDS_REQUESTED_DEADLINE_MISSED_STATUS |
         DDS_SAMPLE_LOST_STATUS;
}
