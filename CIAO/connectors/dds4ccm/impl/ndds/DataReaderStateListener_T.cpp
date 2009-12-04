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
      typename DDS_TYPE::dds_seq_type data;
      DDS_SampleInfoSeq sample_info;
      ::DDS::ReturnCode_t const result = reader->take (
                  data,
                  sample_info,
                  DDS_LENGTH_UNLIMITED,
                  DDS_NOT_READ_SAMPLE_STATE,
                  DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                  DDS_ANY_INSTANCE_STATE);
      if (result == DDS_RETCODE_NO_DATA)
          return;
      else if (result != DDS_RETCODE_OK)
        {
          CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Unable to take data from data reader, error %d.\n"), result));
        }
      if (sampleinfo.instance_state == ::DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE)
        {
          ::CCM_DDS::ReadInfo empty;
          empty <<= sample_info;
          listener_->on_deletion (instance, empty);
        }
      else if (sampleinfo.view_state == ::DDS_NEW_VIEW_STATE)
        {
          ::CCM_DDS::ReadInfo empty;
          empty <<= sample_info;
          listener_->on_creation (instance, empty);
        }
      else
        {
          if (this->mode_.value () == ::CCM_DDS::ONE_BY_ONE)
            {
              for (::DDS_Long i = 0; i < data.length (); ++i)
                {
                  if (sample_info[i].valid_data)
                    {
                      ::CCM_DDS::ReadInfo info;
                      info <<= sample_info[i];
                      listener_->on_one_update (data[i], info);
                    }
                }
            }
          else 
            {
              CORBA::ULong nr_of_samples = 0;
              for (::DDS_Long i = 0 ; i < sample_info.length(); i++)
                {
                  if (sample_info[i].valid_data)
                    {
                      ++nr_of_samples;
                    }
                }

              typename CCM_TYPE::seq_type::_var_type inst_seq = new typename CCM_TYPE::seq_type;
              ::CCM_DDS::ReadInfoSeq_var infoseq = new ::CCM_DDS::ReadInfoSeq;

              infoseq->length (nr_of_samples);
              inst_seq->length (nr_of_samples);

              // Copy the valid samples
              CORBA::ULong ix = 0;
              for (::DDS_Long i = 0 ; i < sample_info.length(); i++)
                {
                  if(sample_info[i].valid_data)
                    {
                      infoseq[ix] <<= sample_info[i];
                      inst_seq[ix] = data[i];
                      ++ix;
                    }
                }
              listener_->on_many_updates (inst_seq, infoseq);
            }
          // Return the loan
          reader->return_loan(data, sample_info);
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

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::on_liveliness_changed(
          ::DDS::DataReader* reader,
          const ::DDS::LivelinessChangedStatus& status)
{
  try
    {
      if (!CORBA::is_nil (this->error_listener_))
        {
          this->error_listener_->on_unexpected_status (reader, ::DDS::LIVELINESS_CHANGED_STATUS);
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("DataReaderStateListener_T::on_liveliness_changed: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::on_sample_rejected(
          ::DDS::DataReader* reader,
          const ::DDS::SampleRejectedStatus& status)
{
  try
    {
      if (!CORBA::is_nil (this->error_listener_))
        {
          this->error_listener_->on_sample_rejected (reader, status);
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("DataReaderStateListener_T::on_sample_rejected: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::on_subscription_matched(
          ::DDS::DataReader* reader,
          const ::DDS::SubscriptionMatchedStatus& status)
{
  try
    {
      if (!CORBA::is_nil (this->error_listener_))
        {
          this->error_listener_->on_unexpected_status (reader, ::DDS::SUBSCRIPTION_MATCHED_STATUS);
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("DataReaderStateListener_T::on_requested_on_subscription_matchedncompatible_qos: ")
                             ACE_TEXT ("DDS Exception caught\n")));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::get_mask (void)
{
  return DDS_DATA_AVAILABLE_STATUS | DDS_REQUESTED_DEADLINE_MISSED_STATUS | DDS_SAMPLE_LOST_STATUS | DDS_REQUESTED_INCOMPATIBLE_QOS_STATUS | DDS_SAMPLE_REJECTED_STATUS | DDS_LIVELINESS_CHANGED_STATUS | DDS_SUBSCRIPTION_MATCHED_STATUS;
}

