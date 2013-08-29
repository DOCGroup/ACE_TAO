// $Id$

#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "ace/Reactor.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE, DDS4CCM_LISTENER_READ_TAKE LRT>
    DataReaderListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, LRT>::DataReaderListenerBase_T (
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
      DDS4CCM_TRACE ("DataReaderListenerBase_T::DataReaderListenerBase_T");
    }

    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE, DDS4CCM_LISTENER_READ_TAKE LRT>
    DataReaderListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, LRT>::~DataReaderListenerBase_T (void)
    {
      DDS4CCM_TRACE ("DataReaderListenerBase_T::~DataReaderListenerBase_T");
    }

    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE, DDS4CCM_LISTENER_READ_TAKE LRT>
    void
    DataReaderListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, LRT>::on_data_available (
      ::DDS::DataReader_ptr rdr)
    {
      DDS4CCM_TRACE ("DataReaderListenerBase_T::on_data_available");

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

    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE>
    ::DDS::ReturnCode_t
    DataReaderListener_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_TAKE>::get_data_i (
      typename TYPED_DDS_READER::_ptr_type reader,
      ::DDS::QueryCondition_ptr qc,
      SEQ_TYPE &data,
      ::DDS::SampleInfoSeq &sample_info,
      ::CORBA::Long max_samples)
    {
      DDS4CCM_TRACE ("DataReaderListener_T::get_data_i");

      ::DDS::ReturnCode_t result = ::DDS::RETCODE_OK;
      if (! ::CORBA::is_nil (qc))
        {
          ::DDS::ReadCondition_var rd = ::DDS::ReadCondition::_narrow (qc);
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
                                 ::DDS::NOT_READ_SAMPLE_STATE,
                                 ::DDS::NEW_VIEW_STATE | ::DDS::NOT_NEW_VIEW_STATE,
                                 ::DDS::ANY_INSTANCE_STATE);
        }
      return result;
    }

    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE>
    ::DDS::ReturnCode_t
    DataReaderListener_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_READ>::get_data_i (
      typename TYPED_DDS_READER::_ptr_type reader,
      ::DDS::QueryCondition_ptr qc,
      SEQ_TYPE &data,
      ::DDS::SampleInfoSeq &sample_info,
      ::CORBA::Long max_samples)
    {
      DDS4CCM_TRACE ("DataReaderListener_T::get_data_i");

      ::DDS::ReturnCode_t result = ::DDS::RETCODE_OK;
      if (! ::CORBA::is_nil (qc))
        {
          ::DDS::ReadCondition_var rd = ::DDS::ReadCondition::_narrow (qc);
          result = reader->read_w_condition (data,
                                             sample_info,
                                             max_samples,
                                             rd.in ());
        }
      else
        {
          result = reader->read (data,
                                 sample_info,
                                 max_samples,
                                 ::DDS::NOT_READ_SAMPLE_STATE,
                                 ::DDS::NEW_VIEW_STATE | ::DDS::NOT_NEW_VIEW_STATE,
                                 ::DDS::ANY_INSTANCE_STATE);
        }
      return result;
    }

    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE, DDS4CCM_LISTENER_READ_TAKE LRT>
    void
    DataReaderListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, LRT>::on_data_available_i (
      ::DDS::DataReader_ptr rdr)
    {
      DDS4CCM_TRACE ("DataReaderListenerBase_T::on_data_available_i");

      if (::CORBA::is_nil (rdr))
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                             ACE_TEXT ("DataReaderListenerBase_T::on_data_available_i - ")
                             ACE_TEXT ("No datareader received.\n")));
          return;
        }

      ::CCM_DDS::ListenerMode const mode = this->control_->mode ();
      if (mode == ::CCM_DDS::NOT_ENABLED)
        {
          return;
        }

      typename TYPED_DDS_READER::_var_type reader;
      reader = TYPED_DDS_READER::_narrow (rdr);

      if (::CORBA::is_nil (reader.in ()))
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                             ACE_TEXT ("DataReaderListenerBase_T::on_data_available_i - ")
                             ACE_TEXT ("Failed to narrow DataReader to a type ")
                             ACE_TEXT ("specific DataReader.\n")));
          return;
        }

      try
        {
          SEQ_TYPE data;
          ::DDS::SampleInfoSeq sample_info;
          ::CORBA::Long max_samples = 0;

          mode == ::CCM_DDS::ONE_BY_ONE
            ? max_samples = ::DDS::LENGTH_UNLIMITED
            : this->control_->max_delivered_data() == 0
              ? max_samples = ::DDS::LENGTH_UNLIMITED
              : max_samples = this->control_->max_delivered_data ();

          ::DDS::QueryCondition_var qc =
            this->condition_manager_.get_querycondition_listener ();

          ::DDS::ReturnCode_t const result =
            this->get_data_i (reader, qc.in (), data, sample_info, max_samples);

          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_INFO, DDS4CCM_INFO
                              ACE_TEXT ("DataReaderListenerBase_T::on_data_available_i - ")
                              ACE_TEXT ("Get data returned %C.\n"),
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
                      SEQ_TYPE inst_seq (nr_of_samples);
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
                            ACE_TEXT ("DataReaderListenerBase_T::on_data_available_i - ")
                            ACE_TEXT ("Error returning loan to DDS - <%C>\n"),
                            translate_retcode (retval)));
              // No exception here since this the DDS vendor doesn't expect this.
              // It will likely causes a crash in their implementation
            }
        }
      catch (const ::CORBA::BAD_INV_ORDER& ex)
        {
          DDS4CCM_PRINT_DEBUG_CORBA_EXCEPTION (
                                  DDS4CCM_LOG_LEVEL_ACTION,
                                  ex,
                                  "DataReaderListenerBase_T::on_data_available_i");
        }
      catch (const ::CORBA::Exception& ex)
        {
          DDS4CCM_PRINT_CORBA_EXCEPTION (
                                  DDS4CCM_LOG_LEVEL_ERROR,
                                  ex,
                                  "DataReaderListenerBase_T::on_data_available_i");
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("DataReaderListener_T::on_data_available_i - ")
                        ACE_TEXT ("Unexpected exception caught\n")));
        }
    }

    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE, DDS4CCM_LISTENER_READ_TAKE LRT>
    ::DDS::StatusMask
    DataReaderListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, LRT>::get_mask (
      typename CCM_TYPE::data_listener_type::_ptr_type listener,
      ::CCM_DDS::PortStatusListener_ptr status)
    {
      DDS4CCM_TRACE ("DataReaderListenerBase_T::get_mask");

      ::DDS::StatusMask mask = 0;

      if (! ::CORBA::is_nil (listener) ||
          DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
        {
          mask = ::DDS::DATA_AVAILABLE_STATUS;
          mask |= ::CIAO::DDS4CCM::PortStatusListener::get_mask (status);
        }

      if (DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
        {
          ACE_CString msk;
          translate_statusmask (msk, mask);
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                        "DataReaderListenerBase_T::get_mask - "
                        "Mask becomes %C\n",
                        msk.c_str ()));
        }
      return mask;
    }
  }
}
