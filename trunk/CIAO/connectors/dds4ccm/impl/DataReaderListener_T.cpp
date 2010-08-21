// $Id$
#include "dds4ccm/impl/Utils.h"

#include "dds4ccm/impl/Log_Macros.h"
#include "ace/Reactor.h"

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DataReaderListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::DataReaderListener_T (
  typename CCM_TYPE::listener_type::_ptr_type listener,
  ::CCM_DDS::PortStatusListener_ptr port_status_listener,
  ::CCM_DDS::DataListenerControl_ptr control,
  ACE_Reactor * reactor,
  ConditionManager_type& condition_manager)
  : PortStatusListener_T <DDS_TYPE, CCM_TYPE, VENDOR_TYPE> (port_status_listener, reactor) ,
    listener_ (CCM_TYPE::listener_type::_duplicate (listener)),
    control_ (::CCM_DDS::DataListenerControl::_duplicate (control)),
    condition_manager_ (condition_manager)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReaderListener_T::DataReaderListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DataReaderListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~DataReaderListener_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReaderListener_T::~DataReaderListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DataReaderListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_data_available (
  ::DDS::DataReader_ptr rdr)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReaderListener_T::on_data_available");

  if (!::CORBA::is_nil (rdr) &&
      this->control_->mode () != ::CCM_DDS::NOT_ENABLED)
    {
      if (this->reactor_)
        {
          DataReaderHandler_type * rh = 0;
          ACE_NEW (rh, DataReaderHandler_type (this, rdr));

          ACE_Event_Handler_var safe_handler (rh);
          if (this->reactor_->notify (rh) != 0)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                            ACE_TEXT ("DataReaderListener_T::failed to use reactor.\n")));
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
CIAO::DDS4CCM::DataReaderListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::on_data_available_i (
  ::DDS::DataReader_ptr rdr)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReaderListener_T::on_data_available_i");

  if (::CORBA::is_nil (rdr) ||
      this->control_->mode () == ::CCM_DDS::NOT_ENABLED)
    {
      return;
    }

  ::CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> * reader =
    dynamic_cast < ::CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> *> (rdr);

  if (!reader)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                         ACE_TEXT ("DataReaderListener_T::on_data_available_i - ")
                         ACE_TEXT ("Failed to retrieve pointer to proxy from ")
                         ACE_TEXT ("DDSDataReader.\n")));
      return;
    }

  typename DDS_TYPE::dds_seq_type data;
  DDS_SampleInfoSeq sample_info;
  ::DDS::ReturnCode_t const result =
      reader->take (data,
                    sample_info,
                    DDS_LENGTH_UNLIMITED,
                    this->condition_manager_.get_querycondition_listener ());

  if (result == ::DDS::RETCODE_NO_DATA)
    {
      return;
    }
  else if (result != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
                          ACE_TEXT ("Unable to take data from data reader, ")
                          ACE_TEXT ("error %C.\n"),
                          translate_retcode (result)));
      return;
    }

  if (this->control_->mode () == ::CCM_DDS::ONE_BY_ONE)
    {
      for (::DDS_Long i = 0; i < data.length (); ++i)
        {
          if (sample_info[i].valid_data)
            {
              ::CCM_DDS::ReadInfo info;
              info <<= sample_info[i];
              this->listener_->on_one_data (data[i], info);
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

      if (nr_of_samples > 0)
        {
          typename CCM_TYPE::seq_type inst_seq (nr_of_samples);
          ::CCM_DDS::ReadInfoSeq infoseq (nr_of_samples);

          infoseq.length (nr_of_samples);
          inst_seq.length (nr_of_samples);

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
          this->listener_->on_many_data (inst_seq, infoseq);
        }
    }

  // Return the loan
  DDS::ReturnCode_t const retval = reader->return_loan (data, sample_info);
  if (retval != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
        "DataReaderListener_T::on_data_available_i - "
        "Error returning loan to DDS - <%C>\n",
        translate_retcode (retval)));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::StatusMask
CIAO::DDS4CCM::DataReaderListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_mask (
  ::CCM_DDS::PortStatusListener_ptr listener)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReaderListener_T::get_mask");

  ::DDS::StatusMask mask = ::DDS::DATA_AVAILABLE_STATUS;
  
  if (! ::CORBA::is_nil (listener) ||
      CIAO_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
    {
      mask |= PortStatusListener_type::get_mask (listener);
    }
    
  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, CLINFO
                 "DataReaderListener_T::get_mask - "
                 "Mask becomes %x\n",
                 mask));
                 
  return mask;
}
