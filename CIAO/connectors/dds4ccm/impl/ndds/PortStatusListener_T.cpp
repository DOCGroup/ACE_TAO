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

  if (!CORBA::is_nil (this->port_status_listener_))
    {
      try
        {
          this->port_status_listener_->on_requested_deadline_missed (the_reader, status);
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

  if (!CORBA::is_nil (this->port_status_listener_))
    {
      try
        {
          this->port_status_listener_->on_sample_lost (the_reader, status);
        }
      catch (...)
        {
          CIAO_DEBUG (6, (LM_DEBUG,
                          ACE_TEXT ("PortStatusListener_T::on_sample_lost: ")
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
