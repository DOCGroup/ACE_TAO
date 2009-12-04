// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/DataReaderHandler_T.h"
#include "tao/ORB_Core.h"

// Implementation skeleton constructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::PortStatusListener_T (
      typename CCM_TYPE::context_type::_ptr_type context,
      ::CCM_DDS::PortStatusListener_ptr port_status_listener)
      : context_ (CCM_TYPE::context_type::_duplicate (context)),
        port_status_listener_ (::CCM_DDS::PortStatusListener::_duplicate (port_status_listener))
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::PortStatusListener_T::PortStatusListener_T");
}

// Implementation skeleton destructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::~PortStatusListener_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::PortStatusListener_T::~PortStatusListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::on_requested_deadline_missed (
  ::DDS::DataReader_ptr the_reader,
  const ::DDS::RequestedDeadlineMissedStatus & status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::PortStatusListener_T::on_requested_deadline_missed");

  try
    {
      if (!CORBA::is_nil (this->port_status_listener_))
        {
          this->port_status_listener_->on_requested_deadline_missed (the_reader, status);
        }
      else
        {
          CIAO_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("PortStatusListener_T::on_requested_deadline_missed: ")
                      ACE_TEXT ("No portstatus listener installed\n")));
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("PortStatusListener_T::on_requested_deadline_missed: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::on_sample_lost (
  ::DDS::DataReader_ptr the_reader,
  const ::DDS::SampleLostStatus & status)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::PortStatusListener_T::on_sample_lost");

  try
    {
      if (!CORBA::is_nil (this->port_status_listener_))
        {
          this->port_status_listener_->on_sample_lost (the_reader, status);
        }
      else
        {
          CIAO_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("PortStatusListener_T::on_sample_lost: ")
                      ACE_TEXT ("No portstatus listener installed\n")));
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("PortStatusListener_T::on_sample_lost: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask (void)
{
  return DDS_REQUESTED_DEADLINE_MISSED_STATUS |
         DDS_SAMPLE_LOST_STATUS;
}
