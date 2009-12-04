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
      typename CCM_TYPE::context_type::_ptr_type context,
      CCM_DDS::ConnectorStatusListener_ptr error_listener,
      typename CCM_TYPE::statelistener_type::_ptr_type listener,
      ::CCM_DDS::PortStatusListener_ptr port_status_listener,
      ::CCM_DDS::DataListenerControl_ptr control)
      : context_ (CCM_TYPE::context_type::_duplicate (context)),
        error_listener_ (::CCM_DDS::ConnectorStatusListener::_duplicate (error_listener)),
        listener_ (CCM_TYPE::statelistener_type::_duplicate (listener)),
        port_status_listener_ (::CCM_DDS::PortStatusListener::_duplicate (port_status_listener)),
        control_ (::CCM_DDS::DataListenerControl::_duplicate (control))
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
      if (this->control_->mode () == ::CCM_DDS::ONE_BY_ONE)
        {
          for (::DDS_Long i = 0; i < data.length (); ++i)
            {
              if (sample_info[i].valid_data)
                {
                  if (sample_info[i].instance_state == 
                          ::DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE)
                    {
                      ::CCM_DDS::ReadInfo readinfo;
                      readinfo <<= sample_info[i];
                      listener_->on_deletion (data[i], readinfo);
                    }
                  else if (sample_info[i].view_state == 
                          ::DDS_NEW_VIEW_STATE)
                    {
                      ::CCM_DDS::ReadInfo readinfo;
                      readinfo <<= sample_info[i];
                      listener_->on_creation (data[i], readinfo);
                    }
                  else
                    {
                      ::CCM_DDS::ReadInfo info;
                      info <<= sample_info[i];
                      listener_->on_one_update (data[i], info);
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
              if (sample_info[i].valid_data)
                {
                  // first gather updates until we find a new or
                  // deleted sample.
                  if (sample_info[i].view_state != ::DDS_NEW_VIEW_STATE &&
                      sample_info[i].instance_state != ::DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE)
                    {
                      ++nr_of_updates;
                      updates.push_back (i);
                    }
                  else
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
                      listener_->on_many_updates (inst_seq, infoseq);
                      // now invoke on_creation or on_deletion
                      if (sample_info[i].view_state == ::DDS_NEW_VIEW_STATE)
                        {
                          ::CCM_DDS::ReadInfo readinfo;
                          readinfo <<= sample_info[i];
                          listener_->on_creation (data[i], readinfo);
                        }
                      else if (sample_info[i].instance_state != ::DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE)
                        {
                          ::CCM_DDS::ReadInfo readinfo;
                          readinfo <<= sample_info[i];
                          listener_->on_creation (data[i], readinfo);
                        }
                      //clean up
                      updates.clear ();
                      nr_of_updates = 0;
                    }
                }
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
      if (!CORBA::is_nil (this->port_status_listener_))
        {
          this->port_status_listener_->on_requested_deadline_missed (the_reader, status);
        }
      else
        {
          CIAO_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("DataReaderStateListener_T::on_requested_deadline_missed: ")
                      ACE_TEXT ("No portstatus listener installed\n")));
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
      if (!CORBA::is_nil (this->port_status_listener_))
        {
          this->port_status_listener_->on_sample_lost (the_reader, status);
        }
      else
        {
          CIAO_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("DataReaderStateListener_T::on_sample_lost: ")
                      ACE_TEXT ("No portstatus listener installed\n")));
        }
    }
  catch (...)
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("DataReaderStateListener_T::on_sample_lost: ")
                             ACE_TEXT ("DDS Exception caught\n")));
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

