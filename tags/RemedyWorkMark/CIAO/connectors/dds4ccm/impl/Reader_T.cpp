// // $Id$

#include "dds4ccm/impl/Utils.h"

#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds4ccm/impl/SampleInfo.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::Reader_T (void)
      : condition_manager_ (0)
    {
      DDS4CCM_TRACE ("Reader_T::Reader_T");
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::~Reader_T (void)
    {
      DDS4CCM_TRACE ("Reader_T::~Reader_T");
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    ::DDS::InstanceHandle_t
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::check_handle (
      const VALUE_TYPE& an_instance,
      const ::DDS::InstanceHandle_t & instance_handle)
    {
      ::DDS::InstanceHandle_t const lookup_hnd =
          this->dds_reader ()->lookup_instance (an_instance);

      return this->condition_manager_->check_handle (instance_handle,
                                                     lookup_hnd);
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    CORBA::ULong
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::get_nr_valid_samples (
      const ::DDS::SampleInfoSeq& sample_infos,
      const bool determine_last)
    {
      CORBA::ULong nr_of_samples = 0;
      if (determine_last)
        {
          for (::CORBA::ULong i = 0 ; i < sample_infos.length(); ++i)
            {
              if (sample_infos[i].valid_data &&
                  sample_infos[i].sample_rank == 0)
                {
                  ++nr_of_samples;
                }
            }
        }
      else
        {
          for (::CORBA::ULong i = 0 ; i < sample_infos.length(); ++i)
            {
              if (sample_infos[i].valid_data)
                {
                  ++nr_of_samples;
                }
            }
        }
      return nr_of_samples;
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::convert_data (
      const SEQ_VALUE_TYPE & all_data,
      SEQ_VALUE_TYPE & data_to_return,
      ::CCM_DDS::ReadInfoSeq& infos,
      const ::DDS::SampleInfoSeq & sample_info)
    {
      CORBA::ULong samples_to_return = this->get_nr_valid_samples (sample_info,
                                                                    false);
      infos.length (samples_to_return);
      data_to_return.length (samples_to_return);

      CORBA::ULong ix = 0;
      for (::CORBA::ULong i = 0 ; i < sample_info.length(); ++i)
        {
          if (sample_info[i].valid_data)
            {
              infos[ix] <<= sample_info[i];
              data_to_return[ix] = all_data[i];
              ++ix;
            }
        }
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::read_w_instance (
      SEQ_VALUE_TYPE & data,
      ::DDS::SampleInfoSeq & sample_info,
      const ::DDS::InstanceHandle_t & lookup_hnd)
    {
      DDS4CCM_TRACE ("Reader_T::read_w_instance");

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_INFO, DDS4CCM_INFO
                    ACE_TEXT ("Reader_T::read_w_instance - ")
                    ACE_TEXT ("Start reading with instance.\n")));
      ::DDS::ReturnCode_t const retval = this->dds_reader ()->read_instance (
                                            data,
                                            sample_info,
                                            ::DDS::LENGTH_UNLIMITED,
                                            lookup_hnd,
                                            ::DDS::READ_SAMPLE_STATE | ::DDS::NOT_READ_SAMPLE_STATE,
                                            ::DDS::NEW_VIEW_STATE | ::DDS::NOT_NEW_VIEW_STATE,
                                            ::DDS::ALIVE_INSTANCE_STATE);
      if (retval != ::DDS::RETCODE_OK && retval != ::DDS::RETCODE_NO_DATA)
        {
          this->return_loan (data, sample_info);
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("Reader_T::read_w_instance - ")
                        ACE_TEXT ("retval is %C\n"),
                        ::CIAO::DDS4CCM::translate_retcode(retval)));
          throw ::CCM_DDS::InternalError (retval, 0);
        }
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::read_wo_instance (
      SEQ_VALUE_TYPE & data,
      ::DDS::SampleInfoSeq & sample_info,
      ::DDS::QueryCondition_ptr qc)
    {
      DDS4CCM_TRACE ("Reader_T::read_wo_instance");
      ::DDS::ReturnCode_t retval = ::DDS::RETCODE_ERROR;

      if (! ::CORBA::is_nil (qc))
        {
          retval = this->dds_reader ()->read_w_condition (
                                                    data,
                                                    sample_info,
                                                    ::DDS::LENGTH_UNLIMITED,
                                                    qc);
        }
      else
        {
          retval = this->dds_reader ()->read (
                      data,
                      sample_info,
                      ::DDS::LENGTH_UNLIMITED,
                      ::DDS::READ_SAMPLE_STATE | ::DDS::NOT_READ_SAMPLE_STATE,
                      ::DDS::NEW_VIEW_STATE | ::DDS::NOT_NEW_VIEW_STATE,
                      ::DDS::ALIVE_INSTANCE_STATE);
        }
      if (retval != ::DDS::RETCODE_OK && retval != ::DDS::RETCODE_NO_DATA)
        {
          this->return_loan (data, sample_info);
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("Reader_T::read_wo_instance - ")
                        ACE_TEXT ("retval is %C\n"),
                        ::CIAO::DDS4CCM::translate_retcode(retval)));
          throw ::CCM_DDS::InternalError (retval, 0);
        }
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::return_loan (
      SEQ_VALUE_TYPE & data,
      ::DDS::SampleInfoSeq  sample_info)
    {
      // Return the loan
      ::DDS::ReturnCode_t const retval =
        this->dds_reader ()->return_loan (data, sample_info);
      if (retval != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "Reader_T::read_last - "
                        "Error returning loan to DDS - <%C>\n",
                        translate_retcode (retval)));
        }
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::read_last (
      SEQ_VALUE_TYPE& instances,
      ::CCM_DDS::ReadInfoSeq& infos)
    {
      // This function has to return the last sample of all instances
      ::DDS::SampleInfoSeq sample_info;
      SEQ_VALUE_TYPE data;

      ::DDS::QueryCondition_var qc = this->condition_manager_->get_querycondition_reader ();
      this->read_wo_instance (data,
                              sample_info,
                              qc.in ());

      // Determine how many samples to return
      CORBA::ULong samples_to_return = this->get_nr_valid_samples (sample_info,
                                                                    true);
      infos.length (samples_to_return);
      instances.length (samples_to_return);
      ::CORBA::ULong ix = 0;
      for (::CORBA::ULong i = 0 ; i < sample_info.length(); ++i)
        {
          if (sample_info[i].valid_data && sample_info[i].sample_rank == 0)
            {
              infos[ix] <<= sample_info[i];
              instances[ix] = data[i];
              ++ix;
            }
        }
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::read_all (
              SEQ_VALUE_TYPE& instances,
              ::CCM_DDS::ReadInfoSeq& infos)
    {
      // This function has to return all samples of all instances
      ::DDS::SampleInfoSeq sample_info;
      SEQ_VALUE_TYPE data;

      ::DDS::QueryCondition_var qc = this->condition_manager_->get_querycondition_reader ();
      this->read_wo_instance (data,
                              sample_info,
                              qc.in ());

      this->convert_data (data, instances, infos, sample_info);
      this->return_loan (instances, sample_info);
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::read_one_last (
      VALUE_TYPE& an_instance,
      ::CCM_DDS::ReadInfo_out info,
      const ::DDS::InstanceHandle_t & instance_handle)
    {
      ::DDS::InstanceHandle_t const lookup_hnd =
        this->check_handle (an_instance, instance_handle);

      ::DDS::SampleInfoSeq sample_info;
      SEQ_VALUE_TYPE data;

      this->read_w_instance (data,
                              sample_info,
                              lookup_hnd);

      ::CORBA::Long sample = data.length();
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                    ACE_TEXT ("Reader_T::read_one_last - ")
                    ACE_TEXT ("total number of samples <%u>\n"),
                    sample));
      while (sample > 0 && !sample_info[sample-1].valid_data)
        {
          --sample;
        }
      if (sample > 0)
        {
          if (sample_info[sample-1].valid_data)
            {
              an_instance = data[sample-1];
              info <<= sample_info[sample-1];
            }
        }
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::read_one_all (
      const VALUE_TYPE& an_instance,
      SEQ_VALUE_TYPE& instances,
      ::CCM_DDS::ReadInfoSeq& infos,
      const ::DDS::InstanceHandle_t & instance_handle)
    {
      ::DDS::InstanceHandle_t const lookup_hnd =
        this->check_handle (an_instance, instance_handle);

      ::DDS::SampleInfoSeq sample_info;
      SEQ_VALUE_TYPE data;

      this->read_w_instance (data,
                              sample_info,
                              lookup_hnd);
      this->convert_data (data, instances, infos, sample_info);
      this->return_loan (instances, sample_info);
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    ::CCM_DDS::QueryFilter *
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::query (void)
    {
      DDS4CCM_TRACE ("Reader_T::query");
      return this->condition_manager_->query ();
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::query (
      const ::CCM_DDS::QueryFilter & query)
    {
      DDS4CCM_TRACE ("Reader_T::query");

      return this->condition_manager_->query (query);
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    void
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::set_dds_reader (
      ::DDS::DataReader_ptr dr,
      ConditionManager * condition_manager)
    {
      DDS4CCM_TRACE ("Reader_T::set_dds_reader");
      this->dds_reader_ = TYPED_DDS_READER::_narrow (dr);
      this->condition_manager_ = condition_manager;
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    ::DDS::DataReader_ptr
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::get_dds_reader (void)
    {
      DDS4CCM_TRACE ("Reader_T::get_dds_reader");

      return ::DDS::DataReader::_duplicate (this->dds_reader_.in ());
    }

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    typename TYPED_DDS_READER::_ptr_type
    Reader_T<READER_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE>::dds_reader (void)
    {
      if (! ::CORBA::is_nil (this->dds_reader_))
        {
          return this->dds_reader_;
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "CIAO::DDS4CCM::Reader_T - "
                        "Throwing BAD_INV_ORDER.\n"));
          throw ::CORBA::BAD_INV_ORDER ();
        }
    }
  }
}
