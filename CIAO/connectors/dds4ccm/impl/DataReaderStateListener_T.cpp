// $Id$

#include "dds4ccm/impl/Utils.h"

#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds4ccm/impl/DataReaderHandler_T.h"
#include "tao/ORB_Core.h"

#include <vector>

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE, DDS4CCM_LISTENER_READ_TAKE LRT>
    DataReaderStateListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, LRT>::DataReaderStateListenerBase_T (
      typename CCM_TYPE::data_listener_type::_ptr_type listener,
      ::CCM_DDS::PortStatusListener_ptr port_status_listener,
      ::CCM_DDS::StateListenerControl_ptr control,
      ACE_Reactor* reactor,
      ConditionManager& condition_manager)
      : PortStatusListener (port_status_listener, reactor) ,
        listener_ (CCM_TYPE::data_listener_type::_duplicate (listener)),
        control_ (::CCM_DDS::StateListenerControl::_duplicate (control)),
        condition_manager_ (condition_manager)
    {
      DDS4CCM_TRACE ("DataReaderStateListenerBase_T::DataReaderStateListenerBase_T");
    }

    // Implementation skeleton destructor
    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE, DDS4CCM_LISTENER_READ_TAKE LRT>
    DataReaderStateListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, LRT>::~DataReaderStateListenerBase_T (void)
    {
      DDS4CCM_TRACE ("DataReaderStateListenerBase_T::~DataReaderStateListenerBase_T");
    }

    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE, DDS4CCM_LISTENER_READ_TAKE LRT>
    void
    DataReaderStateListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, LRT>::on_data_available(
      ::DDS::DataReader_ptr rdr)
    {
      DDS4CCM_TRACE ("DataReaderStateListenerBase_T::on_data_available");

      if (!::CORBA::is_nil (rdr) &&
         this->control_->mode () != ::CCM_DDS::NOT_ENABLED)
        {
          if (this->reactor_)
            {
              DataReaderStateHandler_type* rh = 0;
              ACE_NEW (rh, DataReaderStateHandler_type (this, rdr));
              ACE_Event_Handler_var safe_handler (rh);
              if (this->reactor_->notify (rh) != 0)
                {
                  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                                ACE_TEXT ("DataReaderStateListenerBase_T::on_data_available")
                                ACE_TEXT ("failed to use reactor.\n")));
                }
            }
          else
            {
              this->on_data_available_i (rdr);
            }
        }
    }

    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE, DDS4CCM_LISTENER_READ_TAKE LRT>
    void
    DataReaderStateListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, LRT>::on_data_available_i (
      ::DDS::DataReader_ptr rdr)
    {
      DDS4CCM_TRACE ("DataReaderStateListenerBase_T::on_data_available_i");

      ::CCM_DDS::ListenerMode const mode = this->control_->mode ();
      if (::CORBA::is_nil (rdr) ||
          mode == ::CCM_DDS::NOT_ENABLED)
        {
          return;
        }

      typename TYPED_DDS_READER::_var_type reader;
      reader = TYPED_DDS_READER::_narrow (rdr);

      if (::CORBA::is_nil (reader.in ()))
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("DataReaderStateListenerBase_T::on_data_available_i - ")
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

          if (result == ::DDS::RETCODE_NO_DATA)
            {
              return;
            }
          else if (result != ::DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            ACE_TEXT ("DataReaderStateListenerBase_T::on_data_available_i - ")
                            ACE_TEXT ("Unable to get data from data reader, ")
                            ACE_TEXT ("error %C.\n"), translate_retcode (result)));
            }
          if (mode == ::CCM_DDS::ONE_BY_ONE)
            {
              for (::CORBA::ULong i = 0; i < data.length (); ++i)
                {
                  // Sample data may not be valid anymore when
                  // deleted so don't check the valid_data flag
                  // here.
                  ::CCM_DDS::ReadInfo readinfo;
                  readinfo <<= sample_info[i];
                  if (sample_info[i].instance_state ==
                          ::DDS::NOT_ALIVE_DISPOSED_INSTANCE_STATE)
                    {
                      this->listener_->on_deletion (data[i], readinfo);
                    }
                  else if (sample_info[i].valid_data)
                    {
                      if (sample_info[i].view_state == ::DDS::NEW_VIEW_STATE)
                        {
                          this->listener_->on_creation (data[i], readinfo);
                        }
                      else
                        {
                          this->listener_->on_one_update (data[i], readinfo);
                        }
                    }
                }
            }
          else if (mode == ::CCM_DDS::MANY_BY_MANY)
            {
              typedef std::vector< ::CORBA::Long > Updates;
              Updates updates;

              for (::CORBA::ULong i = 0 ; i < sample_info.length(); i++)
                {
                  if ((sample_info[i].valid_data &&
                       sample_info[i].view_state == ::DDS::NEW_VIEW_STATE) ||
                       sample_info[i].instance_state == ::DDS::NOT_ALIVE_DISPOSED_INSTANCE_STATE)
                    {
                      if (updates.size () > 0)
                        {
                          // Sample_new or sample_delete found -> first send out the
                          // updated samples in one go
                          SEQ_TYPE inst_seq (updates.size ());
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
                      ::CCM_DDS::ReadInfo readinfo;
                      readinfo <<= sample_info[i];
                      if (sample_info[i].valid_data &&
                          sample_info[i].view_state == ::DDS::NEW_VIEW_STATE)
                        {
                          this->listener_->on_creation (data[i], readinfo);
                        }
                      else if (sample_info[i].instance_state ==
                               ::DDS::NOT_ALIVE_DISPOSED_INSTANCE_STATE)
                        {
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
                  SEQ_TYPE inst_seq (updates.size ());
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
          ::DDS::ReturnCode_t const retval = reader->return_loan (data, sample_info);
          if (retval != ::DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            ACE_TEXT ("DataReaderStateListenerBase_T::on_data_available_i - ")
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
                                  "DataReaderStateListenerBase_T::on_data_available_i");
        }
      catch (const ::CORBA::Exception& ex)
        {
          DDS4CCM_PRINT_CORBA_EXCEPTION (
                                  DDS4CCM_LOG_LEVEL_ERROR,
                                  ex,
                                  "DataReaderStateListenerBase_T::on_data_available_i");
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("DataReaderStateListenerBase_T::on_data_available_i - ")
                        ACE_TEXT ("Unexpected exception caught\n")));
        }
    }

    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE, DDS4CCM_LISTENER_READ_TAKE LRT>
    ::DDS::StatusMask
    DataReaderStateListenerBase_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, LRT>::get_mask (
      typename CCM_TYPE::data_listener_type::_ptr_type listener)
    {
      ::DDS::StatusMask mask = 0;

      if (! ::CORBA::is_nil (listener) ||
          DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
        {
          mask = ::DDS::DATA_AVAILABLE_STATUS |
                 ::DDS::REQUESTED_DEADLINE_MISSED_STATUS |
                 ::DDS::SAMPLE_LOST_STATUS;
        }

      if (DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
        {
          ACE_CString msk;
          translate_statusmask (msk, mask);
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                        "DataReaderStateListenerBase_T::get_mask - "
                        "Mask becomes %C\n",
                        msk.c_str ()));
        }
      return mask;
    }

    template <typename CCM_TYPE, typename TYPED_DDS_READER, typename SEQ_TYPE>
    ::DDS::ReturnCode_t
    DataReaderStateListener_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_TAKE>::get_data_i (
      typename TYPED_DDS_READER::_ptr_type reader,
      ::DDS::QueryCondition_ptr qc,
      SEQ_TYPE &data,
      ::DDS::SampleInfoSeq &sample_info,
      ::CORBA::Long max_samples)
    {
      DDS4CCM_TRACE ("DataReaderStateListener_T::get_data_i");

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
    DataReaderStateListener_T<CCM_TYPE, TYPED_DDS_READER, SEQ_TYPE, CIAO::DDS4CCM::DDS4CCM_READ>::get_data_i (
      typename TYPED_DDS_READER::_ptr_type reader,
      ::DDS::QueryCondition_ptr qc,
      SEQ_TYPE &data,
      ::DDS::SampleInfoSeq &sample_info,
      ::CORBA::Long max_samples)
    {
      DDS4CCM_TRACE ("DataReaderStateListener_T::get_data_i");

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
  }
}

