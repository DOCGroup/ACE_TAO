// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/DataReaderHandler_T.h"
#include "tao/ORB_Core.h"

// Implementation skeleton constructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::DataReaderStateListener_T (
      typename CCM_TYPE::context_type::_ptr_type context,
      CCM_DDS::ConnectorStatusListener_ptr error_listener,
      typename CCM_TYPE::statelistener_type::_ptr_type listener,
      ::CCM_DDS::PortStatusListener_ptr port_status_listener,
      ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::ListenerMode> &mode,
      ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::DataNumber_t> &max_delivered_data)
      : context_ (CCM_TYPE::context_type::_duplicate (context)),
        error_listener_ (::CCM_DDS::ConnectorStatusListener::_duplicate (error_listener)),
        listener_ (CCM_TYPE::statelistener_type::_duplicate (listener)),
        mode_ (mode),
        max_delivered_data_ (max_delivered_data)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::DataReaderStateListener_T::DataReaderStateListener_T");
  this->info_out_portstatus_ =
   ::CCM_DDS::PortStatusListener::_duplicate (port_status_listener);
}

// Implementation skeleton destructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::~DataReaderStateListener_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::DataReaderStateListener_T::~DataReaderStateListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::on_data_available(::DDS::DataReader *rdr)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::DataReaderStateListener_T::on_data_available");

  if (this->mode_.value () == ::CCM_DDS::NOT_ENABLED)
    return;

  ::CIAO::DDS4CCM::RTI::RTI_DataReader_i* rd =
      dynamic_cast < ::CIAO::DDS4CCM::RTI::RTI_DataReader_i*>(rdr);
  if (!rd)
    {
      /* In this specific case, this will never fail */
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("DataReaderStateListener_T::dynamic_cast failed.\n")));
      return;
    }

  typename DDS_TYPE::data_reader * reader =
      dynamic_cast< typename DDS_TYPE::data_reader * > ((rd->get_datareader ()));

  if (!reader)
    {
      /* In this specific case, this will never fail */
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("DataReaderStateListener_T::narrow failed.\n")));
      return;
    }

  try
    {
      // Loop until there are messages available in the queue
      for(;;)
        {
          typename DDS_TYPE::value_type instance;
          ::DDS_SampleInfo sampleinfo;
          ::DDS::ReturnCode_t const result = reader->take_next_sample(instance,
                                                                      sampleinfo);
          if (result == DDS_RETCODE_NO_DATA)
              break;
          else if (result != DDS_RETCODE_OK)
            {
              CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Unable to take data from data reader, error %d.\n"), result));
            }
          if (sampleinfo.instance_state == ::DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE)
            {
              ::CCM_DDS::ReadInfo empty;
              empty <<= sampleinfo;
              listener_->on_deletion (instance, empty);
            }
          else if (sampleinfo.view_state == ::DDS_NEW_VIEW_STATE)
            {
              ::CCM_DDS::ReadInfo empty;
              empty <<= sampleinfo;
              listener_->on_creation (instance, empty);
            }
          else if (sampleinfo.valid_data)
            {
              CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("DataReaderHandler_T : found valid data\n")));
              ::CCM_DDS::ReadInfo empty;
              empty <<= sampleinfo;
              listener_->on_one_update (instance, empty);
            }
        }
    }
  catch (...)
    {
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::on_requested_deadline_missed (
                                              ::DDS::DataReader_ptr the_reader,
                                               const ::DDS::RequestedDeadlineMissedStatus & status)
{
  try
    {
      if (!CORBA::is_nil (this->info_out_portstatus_))
        {
          this->info_out_portstatus_->on_requested_deadline_missed (the_reader, status);
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("DataReaderStateListener_T::on_requested_deadline_missed: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::on_sample_lost (
                                  ::DDS::DataReader_ptr the_reader,
                                  const ::DDS::SampleLostStatus & status)
{
  try
    {
      if (!CORBA::is_nil (this->info_out_portstatus_))
        {
          this->info_out_portstatus_->on_sample_lost (the_reader, status);
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("DataReaderStateListener_T::on_sample_lost: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::on_requested_incompatible_qos (
                                ::DDS::DataReader_ptr the_reader,
                                const ::DDS::RequestedIncompatibleQosStatus & status)
{
  try
    {
      if (!CORBA::is_nil (this->error_listener_))
        {
          this->error_listener_->on_requested_incompatible_qos (the_reader, status);
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("DataReaderStateListener_T::on_requested_incompatible_qos: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

