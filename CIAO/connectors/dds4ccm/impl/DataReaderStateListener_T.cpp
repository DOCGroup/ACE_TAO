// $Id$
#include "dds4ccm/impl/Utils.h"

#include "dds4ccm/impl/Log_Macros.h"
#include "dds4ccm/impl/DataReaderHandler_T.h"
#include "tao/ORB_Core.h"

#include <vector>

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::DataReaderStateListener_T (
  typename CCM_TYPE::statelistener_type::_ptr_type listener,
  ::CCM_DDS::PortStatusListener_ptr port_status_listener,
  StateListenerControl_type& control,
  ACE_Reactor* reactor,
  ConditionManager_type& condition_manager)
  : PortStatusListener_T <DDS_TYPE, CCM_TYPE, VENDOR_TYPE> (port_status_listener, reactor) ,
    listener_ (CCM_TYPE::statelistener_type::_duplicate (listener)),
    control_ (control),
    condition_manager_ (condition_manager)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReaderStateListener_T::DataReaderStateListener_T");
}

// Implementation skeleton destructor
template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~DataReaderStateListener_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReaderStateListener_T::~DataReaderStateListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_data_available(
  ::DDS::DataReader_ptr rdr)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReaderStateListener_T::on_data_available");
  if (!::CORBA::is_nil (rdr) &&
     this->control_.mode () != ::CCM_DDS::NOT_ENABLED)
    {
      if (this->reactor_)
        {
          DataReaderStateHandler_type* rh = 0;
          ACE_NEW (rh, DataReaderStateHandler_type (this, rdr));
          ACE_Event_Handler_var safe_handler (rh);
          if (this->reactor_->notify (rh) != 0)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                            ACE_TEXT ("DataReaderStateHandler_T::")
                            ACE_TEXT ("failed to use reactor.\n")));
            }
        }
      else
        {
          this->on_data_available_i (rdr);
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_data_available_i (
  ::DDS::DataReader_ptr rdr)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReaderStateListener_T::on_data_available_i");

  if (::CORBA::is_nil (rdr) ||
      this->control_.mode () == ::CCM_DDS::NOT_ENABLED)
    {
      return;
    }

  ::CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> * reader =
    dynamic_cast < ::CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> *> (rdr);

  if (!reader)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                    ACE_TEXT ("DataReaderStateListener_T::on_data_available_i - ")
                    ACE_TEXT ("Failed to retrieve pointer to proxy from ")
                    ACE_TEXT ("DDSDataReader.\n")));
      return;
    }

  try
    {
      typename DDS_TYPE::dds_seq_type data;
      ::DDS_SampleInfoSeq sample_info;
      ::DDS_Long max_samples = 0;

      this->control_.mode () == ::CCM_DDS::ONE_BY_ONE
        ? max_samples = DDS_LENGTH_UNLIMITED
        : this->control_.max_delivered_data() == 0
          ? max_samples = DDS_LENGTH_UNLIMITED
          : max_samples = this->control_.max_delivered_data ();

      ::DDS::ReturnCode_t const result = reader->take (
                  data,
                  sample_info,
                  max_samples,
                  this->condition_manager_.get_querycondition_listener ());
      if (result == DDS_RETCODE_NO_DATA)
        {
          return;
        }
      else if (result != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                        ACE_TEXT ("Unable to take data from data reader, ")
                        ACE_TEXT ("error %d.\n"), result));
        }
      if (this->control_.mode () == ::CCM_DDS::ONE_BY_ONE)
        {
          for (::DDS_Long i = 0; i < data.length (); ++i)
            {
              // Sample data may not be valid anymore when
              // deleted so don't check the valid_data flag
              // here.
              if (sample_info[i].instance_state ==
                      ::DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE)
                {
                  ::CCM_DDS::ReadInfo readinfo;
                  readinfo <<= sample_info[i];
                  this->listener_->on_deletion (data[i], readinfo);
                }
              else if (sample_info[i].valid_data)
                {
                  if (sample_info[i].view_state == ::DDS_NEW_VIEW_STATE)
                    {
                      ::CCM_DDS::ReadInfo readinfo;
                      readinfo <<= sample_info[i];
                      this->listener_->on_creation (data[i], readinfo);
                    }
                  else
                    {
                      ::CCM_DDS::ReadInfo readinfo;
                      readinfo <<= sample_info[i];
                      this->listener_->on_one_update (data[i], readinfo);
                    }
                }
            }
        }
      else if (this->control_.mode () == ::CCM_DDS::MANY_BY_MANY)
        {
          typedef std::vector<DDS_Long> Updates;
          Updates updates;

          for (::DDS_Long i = 0 ; i < sample_info.length(); i++)
            {
              if ((sample_info[i].valid_data &&
                   sample_info[i].view_state == ::DDS_NEW_VIEW_STATE) ||
                  sample_info[i].instance_state == ::DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE)
                {
                  if (updates.size () > 0)
                    {
                      // Sample_new or sample_delete found -> first send out the
                      // updated samples in one go
                      typename CCM_TYPE::seq_type inst_seq (updates.size ());
                      ::CCM_DDS::ReadInfoSeq infoseq (updates.size ());

                      infoseq.length (updates.size ());
                      inst_seq.length (updates.size ());
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
                      // Clean up
                      updates.clear ();
                    }
                  // Now invoke on_creation or on_deletion
                  if (sample_info[i].valid_data &&
                      sample_info[i].view_state == ::DDS_NEW_VIEW_STATE)
                    {
                      ::CCM_DDS::ReadInfo readinfo;
                      readinfo <<= sample_info[i];
                      this->listener_->on_creation (data[i], readinfo);
                    }
                  else if (sample_info[i].instance_state ==
                           ::DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE)
                    {
                      ::CCM_DDS::ReadInfo readinfo;
                      readinfo <<= sample_info[i];
                      this->listener_->on_deletion (data[i], readinfo);
                    }
                }
              else if (sample_info[i].valid_data)
                {
                  updates.push_back (i);
                }
            }
          // Send the latest updates.
          if (updates.size () > 0)
            {
              typename CCM_TYPE::seq_type inst_seq (updates.size ());
              ::CCM_DDS::ReadInfoSeq infoseq (updates.size ());

              infoseq.length (updates.size ());
              inst_seq.length (updates.size ());
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
        }
      // Return the loan
      DDS_ReturnCode_t const retval = reader->return_loan (data, sample_info);
      if (retval != DDS_RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
            "CIAO::DDS4CCM::DataReaderStateListener_T::on_data_available_i - "
            "Error returning loan to DDS - <%C>\n",
            translate_retcode (retval)));
        }
    }
  catch (...)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
        "CIAO::DDS4CCM::DataReaderStateListener_T::on_data_available_i - "
        "Unexpected exception caught\n"));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_mask (
  typename CCM_TYPE::statelistener_type::_ptr_type listener)
{
  ::DDS::StatusMask mask = 0;

  if (! ::CORBA::is_nil (listener) ||
      CIAO_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
    {
      mask = ::DDS::DATA_AVAILABLE_STATUS |
             ::DDS::REQUESTED_DEADLINE_MISSED_STATUS |
             ::DDS::SAMPLE_LOST_STATUS;
    }

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, CLINFO
                 "DataReaderStateListener_T::get_mask - "
                 "Mask becomes %x\n",
                 mask));

  return mask;
}

