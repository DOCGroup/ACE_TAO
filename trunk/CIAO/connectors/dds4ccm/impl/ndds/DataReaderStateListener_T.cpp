// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/DataReaderHandler_T.h"
#include "tao/ORB_Core.h"

#include <vector>

// Implementation skeleton constructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::DataReaderStateListener_T (
  typename CCM_TYPE::statelistener_type::_ptr_type listener,
  ::CCM_DDS::PortStatusListener_ptr port_status_listener,
  ::CCM_DDS::StateListenerControl_ptr control)
  : PortStatusListener_T <DDS_TYPE, CCM_TYPE> (port_status_listener) ,
    listener_ (CCM_TYPE::statelistener_type::_duplicate (listener)),
    control_ (::CCM_DDS::StateListenerControl::_duplicate (control))
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::DataReaderStateListener_T::DataReaderStateListener_T");
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

  if (this->control_->mode () == ::CCM_DDS::NOT_ENABLED)
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
      dynamic_cast< typename DDS_TYPE::data_reader * > ((rd->get_impl ()));

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
      ::DDS_Long max_samples = 0;

      this->control_->mode () == ::CCM_DDS::ONE_BY_ONE
        ? max_samples = DDS_LENGTH_UNLIMITED
        : this->control_->max_delivered_data() == 0
          ? max_samples = DDS_LENGTH_UNLIMITED
          : max_samples = this->control_->max_delivered_data ();

      ::DDS::ReturnCode_t const result = reader->take (
                  data,
                  sample_info,
                  max_samples,
                  DDS_NOT_READ_SAMPLE_STATE,
                  DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                  DDS_ANY_INSTANCE_STATE);
      if (result == DDS_RETCODE_NO_DATA)
          return;
      else if (result != DDS_RETCODE_OK)
        {
          CIAO_ERROR (1, (LM_ERROR, ACE_TEXT ("Unable to take data from data reader, error %d.\n"), result));
        }
      if (this->control_->mode () == ::CCM_DDS::ONE_BY_ONE)
        {
          for (::DDS_Long i = 0; i < data.length (); ++i)
            {
              // sample data may not be valid anymore when
              // deleted so don't check the valid_data flag
              // here.
              if (sample_info[i].instance_state ==
                      ::DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE)
                {
                  ::CCM_DDS::ReadInfo readinfo;
                  readinfo <<= sample_info[i];
                  this->listener_->on_deletion (data[i], readinfo);
                }
              if (sample_info[i].valid_data)
                {
                  if (sample_info[i].view_state ==
                          ::DDS_NEW_VIEW_STATE)
                    {
                      ::CCM_DDS::ReadInfo readinfo;
                      readinfo <<= sample_info[i];
                      this->listener_->on_creation (data[i], readinfo);
                    }
                  else
                    {
                      ::CCM_DDS::ReadInfo info;
                      info <<= sample_info[i];
                      this->listener_->on_one_update (data[i], info);
                    }
                }
            }
        }
      else if (this->control_->mode () == ::CCM_DDS::MANY_BY_MANY)
        {
          typedef std::vector<DDS_Long> Updates;
          Updates updates;

          CORBA::ULong nr_of_updates = 0;

          for (::DDS_Long i = 0 ; i < sample_info.length(); i++)
            {
              if ((sample_info[i].valid_data &&
                   sample_info[i].view_state == ::DDS_NEW_VIEW_STATE) ||
                  sample_info[i].instance_state == ::DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE)
                {
                  // new or delete found -> first send out the updated
                  // samples in one go
                  typename CCM_TYPE::seq_type::_var_type  inst_seq = new typename CCM_TYPE::seq_type;
                  ::CCM_DDS::ReadInfoSeq_var infoseq = new ::CCM_DDS::ReadInfoSeq;

                  infoseq->length (nr_of_updates);
                  inst_seq->length (nr_of_updates);
                  CORBA::ULong ix = 0;
                  for(Updates::iterator iter = updates.begin();
                      iter != updates.end();
                      ++iter)
                    {
                      infoseq[ix] <<= sample_info[*iter];
                      inst_seq[ix] = data[*iter];
                      ++ix;
                    }
                  this->listener_->on_many_updates (inst_seq, infoseq);
                  // now invoke on_creation or on_deletion
                  if (sample_info[i].valid_data &&
                      sample_info[i].view_state == ::DDS_NEW_VIEW_STATE)
                    {
                      ::CCM_DDS::ReadInfo readinfo;
                      readinfo <<= sample_info[i];
                      this->listener_->on_creation (data[i], readinfo);
                    }
                  else if (sample_info[i].instance_state == ::DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE)
                    {
                      ::CCM_DDS::ReadInfo readinfo;
                      readinfo <<= sample_info[i];
                      this->listener_->on_deletion (data[i], readinfo);
                    }
                  //clean up
                  updates.clear ();
                  nr_of_updates = 0;
                }
              else if (sample_info[i].valid_data)
                {
                  ++nr_of_updates;
                  updates.push_back (i);
                }
            }
          //send the latest updates.
          if (updates.size () > 0)
            {
              typename CCM_TYPE::seq_type::_var_type  inst_seq = new typename CCM_TYPE::seq_type;
              ::CCM_DDS::ReadInfoSeq_var infoseq = new ::CCM_DDS::ReadInfoSeq;

              infoseq->length (nr_of_updates);
              inst_seq->length (nr_of_updates);
              CORBA::ULong ix = 0;
              for(Updates::iterator iter = updates.begin();
                  iter != updates.end();
                  ++iter)
                {
                  infoseq[ix] <<= sample_info[*iter];
                  inst_seq[ix] = data[*iter];
                  ++ix;
                }
              this->listener_->on_many_updates (inst_seq, infoseq);
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
::DDS::StatusMask
CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::get_mask (void)
{
  return DDS_DATA_AVAILABLE_STATUS |
         DDS_REQUESTED_DEADLINE_MISSED_STATUS |
         DDS_SAMPLE_LOST_STATUS;
}

