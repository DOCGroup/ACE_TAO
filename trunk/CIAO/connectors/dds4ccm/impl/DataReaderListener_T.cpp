// $Id$

#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "ace/Reactor.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    DataReaderListener_T<DDS_TYPE, CCM_TYPE>::DataReaderListener_T (
      typename CCM_TYPE::data_listener_type::_ptr_type listener,
      ::CCM_DDS::PortStatusListener_ptr port_status_listener,
      ::CCM_DDS::DataListenerControl_ptr control,
      ACE_Reactor * reactor,
      ConditionManager& condition_manager)
      : PortStatusListener (port_status_listener, reactor) ,
        listener_ (CCM_TYPE::data_listener_type::_duplicate (listener)),
        control_ (::CCM_DDS::DataListenerControl::_duplicate (control)),
        condition_manager_ (condition_manager)
    {
      DDS4CCM_TRACE ("DataReaderListener_T::DataReaderListener_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    DataReaderListener_T<DDS_TYPE, CCM_TYPE>::~DataReaderListener_T (void)
    {
      DDS4CCM_TRACE ("DataReaderListener_T::~DataReaderListener_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    DataReaderListener_T<DDS_TYPE, CCM_TYPE>::on_data_available (
      ::DDS::DataReader_ptr rdr)
    {
      DDS4CCM_TRACE ("DataReaderListener_T::on_data_available");

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
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                                ACE_TEXT ("DataReaderListener_T::on_data_available - ")
                                ACE_TEXT ("failed to use reactor.\n")));
                }
            }
          else
            {
              this->on_data_available_i (rdr);
            }
        }
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    DataReaderListener_T<DDS_TYPE, CCM_TYPE>::on_data_available_i (
      ::DDS::DataReader_ptr rdr)
    {
      DDS4CCM_TRACE ("DataReaderListener_T::on_data_available_i");

      if (::CORBA::is_nil (rdr))
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                             ACE_TEXT ("DataReaderListener_T::on_data_available_i - ")
                             ACE_TEXT ("No datareader received.\n")));
          return;
        }

      ::CCM_DDS::ListenerMode const mode = this->control_->mode ();
      if (mode == ::CCM_DDS::NOT_ENABLED)
        {
          return;
        }

      typename DDS_TYPE::typed_reader_type::_var_type reader;
      reader = DDS_TYPE::typed_reader_type::_narrow (rdr);

      if (::CORBA::is_nil (reader.in ()))
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                             ACE_TEXT ("DataReaderListener_T::on_data_available_i - ")
                             ACE_TEXT ("Failed to retrieve pointer to proxy from ")
                             ACE_TEXT ("DDSDataReader.\n")));
          return;
        }

      typename DDS_TYPE::seq_type data;
      ::DDS::SampleInfoSeq sample_info;
      ::CORBA::Long max_samples = 0;

      mode == ::CCM_DDS::ONE_BY_ONE
        ? max_samples = ::DDS::LENGTH_UNLIMITED
        : this->control_->max_delivered_data() == 0
          ? max_samples = ::DDS::LENGTH_UNLIMITED
          : max_samples = this->control_->max_delivered_data ();

      ::DDS::QueryCondition_var qc =
        this->condition_manager_.get_querycondition_listener ();

      ::DDS::ReturnCode_t result = ::DDS::RETCODE_OK;

      if (! ::CORBA::is_nil (qc.in ()))
        {
          ::DDS::ReadCondition_var rd = ::DDS::ReadCondition::_narrow (qc.in ());
          result = reader->take_w_condition (data,
                                             sample_info,
                                             max_samples,
                                             rd.in ());
        }
      else
        {
          result = reader->take (data,
                                 sample_info,
                                 max_samples,
                                 DDS_NOT_READ_SAMPLE_STATE,
                                 DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                 DDS_ANY_INSTANCE_STATE);
        }

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_INFO, DDS4CCM_INFO
                          ACE_TEXT ("DataReaderListener_T::on_data_available_i - ")
                          ACE_TEXT ("Take data returned %C.\n"),
                          translate_retcode (result)));

      if (result == ::DDS::RETCODE_OK)
        {
          if (mode == ::CCM_DDS::ONE_BY_ONE)
            {
              for (::CORBA::ULong i = 0; i < data.length (); ++i)
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
              for (::CORBA::ULong i = 0 ; i < sample_info.length(); i++)
                {
                  if (sample_info[i].valid_data)
                    {
                      ++nr_of_samples;
                    }
                }

              if (nr_of_samples > 0)
                {
                  typename DDS_TYPE::seq_type inst_seq (nr_of_samples);
                  ::CCM_DDS::ReadInfoSeq infoseq (nr_of_samples);

                  infoseq.length (nr_of_samples);
                  inst_seq.length (nr_of_samples);

                  // Copy the valid samples
                  CORBA::ULong ix = 0;
                  for (::CORBA::ULong i = 0 ; i < sample_info.length(); i++)
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
        }

      // Return the loan
      DDS::ReturnCode_t const retval = reader->return_loan (data, sample_info);
      if (retval != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("DataReaderListener_T::on_data_available_i - ")
                        ACE_TEXT ("Error returning loan to DDS - <%C>\n"),
                        translate_retcode (retval)));
        }
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::StatusMask
    DataReaderListener_T<DDS_TYPE, CCM_TYPE>::get_mask (
      ::CCM_DDS::PortStatusListener_ptr listener)
    {
      DDS4CCM_TRACE ("DataReaderListener_T::get_mask");

      ::DDS::StatusMask mask = ::DDS::DATA_AVAILABLE_STATUS;
      if (! ::CORBA::is_nil (listener) ||
          DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
        {
          mask |= ::CIAO::DDS4CCM::PortStatusListener::get_mask (listener);
        }

      if (DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
        {
          ACE_CString msk = "";
          translate_statusmask (msk, mask);
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                        "DataReaderListener_T::get_mask - "
                        "Mask becomes %C\n",
                        msk.c_str ()));
        }
      return mask;
    }
  }
}
