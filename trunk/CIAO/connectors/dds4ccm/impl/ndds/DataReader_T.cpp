// $Id$

#include "dds4ccm/impl/ndds/DataReaderListener.h"
#include "dds4ccm/impl/ndds/StatusCondition.h"
#include "dds4ccm/impl/ndds/ReadCondition.h"
#include "dds4ccm/impl/ndds/QueryCondition.h"
#include "dds4ccm/impl/ndds/Subscriber.h"
#include "dds4ccm/impl/ndds/TopicDescription.h"
#include "dds4ccm/impl/ndds/Topic.h"
#include "dds4ccm/impl/ndds/ContentFilteredTopic.h"

#include "dds4ccm/impl/ndds/convertors/SampleInfoSeq.h"
#include "dds4ccm/impl/ndds/convertors/SampleInfo.h"
#include "dds4ccm/impl/ndds/convertors/SampleLostStatus.h"
#include "dds4ccm/impl/ndds/convertors/SubscriptionMatchedStatus.h"
#include "dds4ccm/impl/ndds/convertors/RequestedDeadlineMissedStatus.h"
#include "dds4ccm/impl/ndds/convertors/SampleRejectedStatus.h"
#include "dds4ccm/impl/ndds/convertors/LivelinessChangedStatus.h"
#include "dds4ccm/impl/ndds/convertors/PublicationBuiltinTopicData.h"
#include "dds4ccm/impl/ndds/convertors/RequestedIncompatibleQosStatus.h"
#include "dds4ccm/impl/ndds/convertors/Duration_t.h"
#include "dds4ccm/impl/ndds/convertors/InstanceHandle_t.h"
#include "dds4ccm/impl/ndds/convertors/InstanceHandleSeq.h"
#include "dds4ccm/impl/ndds/convertors/DataReaderQos.h"
#include "dds4ccm/impl/ndds/convertors/StringSeq.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace NDDS
  {
    template <typename DDS_TYPE>
    DataReader_T<DDS_TYPE>::DataReader_T (
      DDSDataReader * dr,
      ::DDS::DomainParticipant_ptr dp)
      : rti_entity_ (0),
        dp_ (::DDS::DomainParticipant::_duplicate (dp)),
        lst_mask_ (0)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::DataReader_T");
      if (dr)
        {
          this->rti_entity_ = DDS_TYPE::datareader_type::narrow (dr);
        }
    }

    template <typename DDS_TYPE>
    DataReader_T<DDS_TYPE>::~DataReader_T (void)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::~DataReader_T");
    }

    /// For the requirement : 'samples ordered by instances' the following settings
    /// are necessary: ordered_access -> true and
    /// DDS_INSTANCE_PRESENTATION_QOS (default)
    template <typename DDS_TYPE>
    CORBA::ULong
    DataReader_T<DDS_TYPE>::get_nr_valid_samples (
      const typename DDS_TYPE::sampleinfo_seq_type& sample_infos)
    {
      CORBA::ULong nr_of_samples = 0;
      for (::DDS_Long i = 0 ; i < sample_infos.length(); ++i)
        {
          if (sample_infos[i].valid_data)
            {
              ++nr_of_samples;
            }
        }
      return nr_of_samples;
    }

    template <typename DDS_TYPE>
    void
    DataReader_T<DDS_TYPE>::complete_read (
        typename DDS_TYPE::dds_seq_type & dds_data_values,
        typename DDS_TYPE::seq_type & data_values,
        DDS_SampleInfoSeq & dds_sample_infos,
        ::DDS::SampleInfoSeq & sample_infos,
        const ::DDS::ReturnCode_t & retcode,
        const char * method_name)
    {
      if (retcode == ::DDS::RETCODE_OK)
        {
          ::CORBA::ULong const nr_of_valid_samples =
            this->get_nr_valid_samples (dds_sample_infos);

          data_values.length (nr_of_valid_samples);
          sample_infos.length (nr_of_valid_samples);

          // Copy the valid samples
          CORBA::ULong ix = 0;
          for (::DDS_Long i = 0 ; i < dds_sample_infos.length(); ++i)
            {
              if (dds_sample_infos[i].valid_data)
                {
                  (sample_infos)[ix] <<= dds_sample_infos[i];
                  (data_values)[ix] = dds_data_values[i];
                  ++ix;
                }
            }
        }
      else
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("datareader_t::%C - ")
                        ACE_TEXT ("error while reading samples from dds - <%c>\n"),
                        method_name,
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
        }
      ::DDS::ReturnCode_t const retcode_return_loan =
        this->rti_entity ()->return_loan (dds_data_values, dds_sample_infos);
      if (retcode_return_loan != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("DataReader_T::%C - ")
                        ACE_TEXT ("Error returning loan to DDS - <%C>\n"),
                        method_name,
                        ::CIAO::DDS4CCM::translate_retcode (retcode_return_loan)));
        }
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::read (
        typename DDS_TYPE::seq_type & data_values,
        ::DDS::SampleInfoSeq & sample_infos,
        ::CORBA::Long max_samples,
        ::DDS::SampleStateMask sample_states,
        ::DDS::ViewStateMask view_states,
        ::DDS::InstanceStateMask instance_states)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::read");

      DDS_SampleInfoSeq dds_sample_infos;
      dds_sample_infos <<= sample_infos;

      ::DDS_SampleStateMask dds_sample_states = DDS_ANY_SAMPLE_STATE;
      dds_sample_states <<= sample_states;

      ::DDS_ViewStateMask dds_view_states = DDS_ANY_VIEW_STATE;
      dds_view_states <<= view_states;

      ::DDS_InstanceStateMask dds_instance_states = DDS_ANY_INSTANCE_STATE;
      dds_instance_states <<= instance_states;

      typename DDS_TYPE::dds_seq_type dds_data_values;

      ::DDS::ReturnCode_t const retcode_read =
        this->rti_entity ()->read (dds_data_values,
                                   dds_sample_infos,
                                   max_samples,
                                   dds_sample_states,
                                   dds_view_states,
                                   dds_instance_states);
      this->complete_read (dds_data_values, data_values,
                           dds_sample_infos, sample_infos,
                           retcode_read, "read");
      return retcode_read;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::take (
      typename DDS_TYPE::seq_type & data_values,
      ::DDS::SampleInfoSeq & sample_infos,
      ::CORBA::Long max_samples,
      ::DDS::SampleStateMask sample_states,
      ::DDS::ViewStateMask view_states,
      ::DDS::InstanceStateMask instance_states)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::take");

      DDS_SampleInfoSeq dds_sample_infos;
      dds_sample_infos <<= sample_infos;

      ::DDS_SampleStateMask dds_sample_states = DDS_ANY_SAMPLE_STATE;
      dds_sample_states <<= sample_states;

      ::DDS_ViewStateMask dds_view_states = DDS_ANY_VIEW_STATE;
      dds_view_states <<= view_states;

      ::DDS_InstanceStateMask dds_instance_states = DDS_ANY_INSTANCE_STATE;
      dds_instance_states <<= instance_states;

      typename DDS_TYPE::dds_seq_type dds_data_values;

      ::DDS::ReturnCode_t const retcode_take =
        this->rti_entity ()->take (dds_data_values,
                                   dds_sample_infos,
                                   max_samples,
                                   dds_sample_states,
                                   dds_view_states,
                                   dds_instance_states);
      this->complete_read (dds_data_values, data_values,
                           dds_sample_infos, sample_infos,
                           retcode_take, "take");
      return retcode_take;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::read_w_condition (
      typename DDS_TYPE::seq_type & data_values,
      ::DDS::SampleInfoSeq & sample_infos,
      ::CORBA::Long max_samples,
      ::DDS::ReadCondition_ptr a_condition)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::read_w_condition");

      DDS_SampleInfoSeq dds_sample_infos;
      dds_sample_infos <<= sample_infos;

      DDS_ReadCondition_i * dds_condition_proxy =
        dynamic_cast <DDS_ReadCondition_i *>(a_condition);
      DDSReadCondition * dds_condition = 0;
      if (dds_condition_proxy)
        {
          dds_condition = dds_condition_proxy->get_rti_entity ();
        }
      typename DDS_TYPE::dds_seq_type dds_data_values;

      ::DDS::ReturnCode_t const retcode_read_w_condition =
        this->rti_entity ()->read_w_condition (dds_data_values,
                                               dds_sample_infos,
                                               max_samples,
                                               dds_condition);
      this->complete_read (dds_data_values, data_values,
                           dds_sample_infos, sample_infos,
                           retcode_read_w_condition, "read_w_condition");
      return retcode_read_w_condition;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::take_w_condition (
      typename DDS_TYPE::seq_type & data_values,
      ::DDS::SampleInfoSeq & sample_infos,
      ::CORBA::Long max_samples,
      ::DDS::ReadCondition_ptr a_condition)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::take_w_condition");

      DDS_SampleInfoSeq dds_sample_infos;
      dds_sample_infos <<= sample_infos;

      DDS_ReadCondition_i * dds_condition_proxy =
        dynamic_cast <DDS_ReadCondition_i *>(a_condition);
      DDSReadCondition * dds_condition = 0;
      if (dds_condition_proxy)
        {
          dds_condition = dds_condition_proxy->get_rti_entity ();
        }
      typename DDS_TYPE::dds_seq_type dds_data_values;

      ::DDS::ReturnCode_t const retcode_take_w_condition =
        this->rti_entity ()->take_w_condition (dds_data_values,
                                               dds_sample_infos,
                                               max_samples,
                                               dds_condition);
      this->complete_read (dds_data_values, data_values,
                           dds_sample_infos, sample_infos,
                           retcode_take_w_condition, "read_w_condition");
      return retcode_take_w_condition;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::read_next_sample (
      typename DDS_TYPE::value_type & data_values,
      ::DDS::SampleInfo & sample_info)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::read_next_sample");

      DDS_SampleInfo dds_sample_info;
      dds_sample_info <<= sample_info;

      ::DDS::ReturnCode_t const retcode_read_next_sample =
        this->rti_entity ()->read_next_sample (data_values,
                                               dds_sample_info);
      sample_info <<= dds_sample_info;
      return retcode_read_next_sample;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::take_next_sample (
      typename DDS_TYPE::value_type & data_values,
      ::DDS::SampleInfo & sample_info)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::take_next_sample");

      DDS_SampleInfo dds_sample_info;
      dds_sample_info <<= sample_info;

      ::DDS::ReturnCode_t const retcode_take_next_sample =
        this->rti_entity ()->take_next_sample (data_values,
                                               dds_sample_info);
      sample_info <<= dds_sample_info;
      return retcode_take_next_sample ;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::read_instance (
      typename DDS_TYPE::seq_type & data_values,
      ::DDS::SampleInfoSeq & sample_infos,
      ::CORBA::Long max_samples,
      const ::DDS::InstanceHandle_t & a_handle,
      ::DDS::SampleStateMask sample_states,
      ::DDS::ViewStateMask view_states,
      ::DDS::InstanceStateMask instance_states)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::read_instance");

      DDS_SampleInfoSeq dds_sample_infos;
      dds_sample_infos <<= sample_infos;

      ::DDS_SampleStateMask dds_sample_states = DDS_ANY_SAMPLE_STATE;
      dds_sample_states <<= sample_states;

      ::DDS_ViewStateMask dds_view_states = DDS_ANY_VIEW_STATE;
      dds_view_states <<= view_states;

      ::DDS_InstanceStateMask dds_instance_states = DDS_ANY_INSTANCE_STATE;
      dds_instance_states <<= instance_states;

      ::DDS_InstanceHandle_t dds_handle = ::DDS_HANDLE_NIL;
      dds_handle <<= a_handle;

      typename DDS_TYPE::dds_seq_type dds_data_values;

      ::DDS::ReturnCode_t const retcode_read_instance =
        this->rti_entity ()->read_instance (dds_data_values,
                                            dds_sample_infos,
                                            max_samples,
                                            dds_handle,
                                            dds_sample_states,
                                            dds_view_states,
                                            dds_instance_states);
      this->complete_read (dds_data_values, data_values,
                           dds_sample_infos, sample_infos,
                           retcode_read_instance, "read_instance");

      return retcode_read_instance;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::take_instance (
      typename DDS_TYPE::seq_type & data_values,
      ::DDS::SampleInfoSeq & sample_infos,
      ::CORBA::Long max_samples,
      const ::DDS::InstanceHandle_t & a_handle,
      ::DDS::SampleStateMask sample_states,
      ::DDS::ViewStateMask view_states,
      ::DDS::InstanceStateMask instance_states)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::take_instance");

      DDS_SampleInfoSeq dds_sample_infos;
      dds_sample_infos <<= sample_infos;

      ::DDS_SampleStateMask dds_sample_states = DDS_ANY_SAMPLE_STATE;
      dds_sample_states <<= sample_states;

      ::DDS_ViewStateMask dds_view_states = DDS_ANY_VIEW_STATE;
      dds_view_states <<= view_states;

      ::DDS_InstanceStateMask dds_instance_states = DDS_ANY_INSTANCE_STATE;
      dds_instance_states <<= instance_states;

      ::DDS_InstanceHandle_t dds_handle = ::DDS_HANDLE_NIL;
      dds_handle <<= a_handle;

      typename DDS_TYPE::dds_seq_type dds_data_values;

      ::DDS::ReturnCode_t const retcode_take_instance =
        this->rti_entity ()->take_instance (dds_data_values,
                                            dds_sample_infos,
                                            max_samples,
                                            dds_handle,
                                            dds_sample_states,
                                            dds_view_states,
                                            dds_instance_states);
      this->complete_read (dds_data_values, data_values,
                           dds_sample_infos, sample_infos,
                           retcode_take_instance, "take_instance");
      return retcode_take_instance;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::read_next_instance (
      typename DDS_TYPE::seq_type & data_values,
      ::DDS::SampleInfoSeq & sample_infos,
      ::CORBA::Long max_samples,
      const ::DDS::InstanceHandle_t & a_handle,
      ::DDS::SampleStateMask sample_states,
      ::DDS::ViewStateMask view_states,
      ::DDS::InstanceStateMask instance_states)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::read_next_instance");

      DDS_SampleInfoSeq dds_sample_infos;
      dds_sample_infos <<= sample_infos;

      ::DDS_SampleStateMask dds_sample_states = DDS_ANY_SAMPLE_STATE;
      dds_sample_states <<= sample_states;

      ::DDS_ViewStateMask dds_view_states = DDS_ANY_VIEW_STATE;
      dds_view_states <<= view_states;

      ::DDS_InstanceStateMask dds_instance_states = DDS_ANY_INSTANCE_STATE;
      dds_instance_states <<= instance_states;

      ::DDS_InstanceHandle_t dds_handle = ::DDS_HANDLE_NIL;
      dds_handle <<= a_handle;

      typename DDS_TYPE::dds_seq_type dds_data_values;

      ::DDS::ReturnCode_t const retcode_read_next_instance =
        this->rti_entity ()->read_next_instance (dds_data_values,
                                                 dds_sample_infos,
                                                 max_samples,
                                                 dds_handle,
                                                 dds_sample_states,
                                                 dds_view_states,
                                                 dds_instance_states);
      this->complete_read (dds_data_values, data_values,
                           dds_sample_infos, sample_infos,
                           retcode_read_next_instance, "read_next_instance");
      return retcode_read_next_instance;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::take_next_instance (
      typename DDS_TYPE::seq_type & data_values,
      ::DDS::SampleInfoSeq & sample_infos,
      ::CORBA::Long max_samples,
      const ::DDS::InstanceHandle_t & a_handle,
      ::DDS::SampleStateMask sample_states,
      ::DDS::ViewStateMask view_states,
      ::DDS::InstanceStateMask instance_states)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::take_next_instance");

      DDS_SampleInfoSeq dds_sample_infos;
      dds_sample_infos <<= sample_infos;

      ::DDS_SampleStateMask dds_sample_states = DDS_ANY_SAMPLE_STATE;
      dds_sample_states <<= sample_states;

      ::DDS_ViewStateMask dds_view_states = DDS_ANY_VIEW_STATE;
      dds_view_states <<= view_states;

      ::DDS_InstanceStateMask dds_instance_states = DDS_ANY_INSTANCE_STATE;
      dds_instance_states <<= instance_states;

      ::DDS_InstanceHandle_t dds_handle = ::DDS_HANDLE_NIL;
      dds_handle <<= a_handle;

      typename DDS_TYPE::dds_seq_type dds_data_values;

      ::DDS::ReturnCode_t const retcode_take_next_instance =
        this->rti_entity ()->take_next_instance (dds_data_values,
                                                 dds_sample_infos,
                                                 max_samples,
                                                 dds_handle,
                                                 dds_sample_states,
                                                 dds_view_states,
                                                 dds_instance_states);
      this->complete_read (dds_data_values, data_values,
                           dds_sample_infos, sample_infos,
                           retcode_take_next_instance, "take_next_instance");
      return retcode_take_next_instance;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::read_next_instance_w_condition (
      typename DDS_TYPE::seq_type & data_values,
      ::DDS::SampleInfoSeq & sample_infos,
      ::CORBA::Long max_samples,
      const ::DDS::InstanceHandle_t & previous_handle,
      ::DDS::ReadCondition_ptr a_condition)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::read_next_instance_w_condition");

      DDS_SampleInfoSeq dds_sample_infos;
      dds_sample_infos <<= sample_infos;

      ::DDS_InstanceHandle_t dds_handle = ::DDS_HANDLE_NIL;
      dds_handle <<= previous_handle;

      DDS_ReadCondition_i * dds_condition_proxy =
        dynamic_cast <DDS_ReadCondition_i *>(a_condition);
      DDSReadCondition * dds_condition = 0;
      if (dds_condition_proxy)
        {
          dds_condition = dds_condition_proxy->get_rti_entity ();
        }

      typename DDS_TYPE::dds_seq_type dds_data_values;

      ::DDS::ReturnCode_t const retcode_read_next_instance_w_condition =
        this->rti_entity ()->read_next_instance_w_condition (dds_data_values,
                                                             dds_sample_infos,
                                                             max_samples,
                                                             dds_handle,
                                                             dds_condition);
      this->complete_read (dds_data_values, data_values,
                           dds_sample_infos, sample_infos,
                           retcode_read_next_instance_w_condition,
                           "read_next_instance_w_condition");
      return retcode_read_next_instance_w_condition;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::take_next_instance_w_condition (
      typename DDS_TYPE::seq_type & data_values,
      ::DDS::SampleInfoSeq & sample_infos,
      ::CORBA::Long max_samples,
      const ::DDS::InstanceHandle_t & previous_handle,
      ::DDS::ReadCondition_ptr a_condition)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::take_next_instance_w_condition");

      DDS_SampleInfoSeq dds_sample_infos;
      dds_sample_infos <<= sample_infos;

      ::DDS_InstanceHandle_t dds_handle = ::DDS_HANDLE_NIL;
      dds_handle <<= previous_handle;

      DDS_ReadCondition_i * dds_condition_proxy =
        dynamic_cast <DDS_ReadCondition_i *>(a_condition);
      DDSReadCondition * dds_condition = 0;
      if (dds_condition_proxy)
        {
          dds_condition = dds_condition_proxy->get_rti_entity ();
        }

      typename DDS_TYPE::dds_seq_type dds_data_values;

      ::DDS::ReturnCode_t const retcode_take_next_instance_w_condition =
        this->rti_entity ()->take_next_instance_w_condition (dds_data_values,
                                                             dds_sample_infos,
                                                             max_samples,
                                                             dds_handle,
                                                             dds_condition);
      this->complete_read (dds_data_values, data_values,
                           dds_sample_infos, sample_infos,
                           retcode_take_next_instance_w_condition,
                           "take_next_instance_w_condition");
      return retcode_take_next_instance_w_condition;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::return_loan (
      typename DDS_TYPE::seq_type & /*data_values*/,
      ::DDS::SampleInfoSeq & /*sample_infos*/)
    {
      throw ::CORBA::NO_IMPLEMENT ();
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::get_key_value (
      typename DDS_TYPE::value_type & key_holder,
      const ::DDS::InstanceHandle_t & handle)
    {
      ::DDS_InstanceHandle_t dds_handle = ::DDS_HANDLE_NIL;
      dds_handle <<= handle;

      return this->rti_entity ()->get_key_value (key_holder,
                                                 dds_handle);
    }

    template <typename DDS_TYPE>
    void
    DataReader_T<DDS_TYPE>::log_query_condition (
      DDSQueryCondition * qc)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::log_query_condition");

      if (DDS4CCM_debug_level >= DDS4CCM_LOG_LEVEL_DDS_STATUS)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                        ACE_TEXT ("DataReader_T::")
                        ACE_TEXT ("read_wo_instance - read_w_condition:\n")));
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                        ACE_TEXT ("\t\texpression <%C>\n"),
                        qc->get_query_expression ()));
          ::DDS_StringSeq dds_qp;
          qc->get_query_parameters (dds_qp);
          for (DDS_Long i = 0; i < dds_qp.length (); ++i)
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                            ACE_TEXT ("\t\tparameter %d <%C>\n"),
                            i + 1,
                            dds_qp[i]));
            }
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                        "\t\tinstance state mask <%d>\n",
                        qc->get_instance_state_mask ()));
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                        "\t\tsample state mask <%d>\n",
                        qc->get_sample_state_mask ()));
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_DEBUG, DDS4CCM_INFO
                        "\t\tview state mask <%d>\n",
                        qc->get_view_state_mask ()));
        }
    }

    template <typename DDS_TYPE>
    void
    DataReader_T<DDS_TYPE>::read_wo_instance (
      typename DDS_TYPE::dds_seq_type & data,
      typename DDS_TYPE::sampleinfo_seq_type & sample_info,
      ::DDS::QueryCondition_ptr qc)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::read_wo_instance");

      DDS_ReturnCode_t retval = DDS_RETCODE_ERROR;

      if (! ::CORBA::is_nil (qc))
        {
          DDS_QueryCondition_i * dds_qc =
            dynamic_cast <DDS_QueryCondition_i *>(qc);
          this->log_query_condition (dds_qc->get_rti_entity ());
          retval = this->rti_entity ()->read_w_condition (data,
                                                    sample_info,
                                                    DDS_LENGTH_UNLIMITED,
                                                    dds_qc->get_rti_entity ());
        }
      else
        {
          retval = this->rti_entity ()->read (data,
                                        sample_info,
                                        DDS_LENGTH_UNLIMITED,
                                        DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE,
                                        DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                        DDS_ALIVE_INSTANCE_STATE);
        }
      if (retval != DDS_RETCODE_OK && retval != DDS_RETCODE_NO_DATA)
        {
          ::DDS::ReturnCode_t const retval = this->return_loan (data, sample_info);
          if (retval != ::DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            ACE_TEXT ("DataReader_T::read_wo_instance - ")
                            ACE_TEXT ("Error returning loan to DDS - <%C>\n"),
                            ::CIAO::DDS4CCM::translate_retcode (retval)));
            }
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                                ACE_TEXT ("DataReader_T::read_wo_instance - ")
                                ACE_TEXT ("retval is %C\n"),
                                ::CIAO::DDS4CCM::translate_retcode(retval)));
          throw ::CCM_DDS::InternalError (retval, 0);
        }
    }

    template <typename DDS_TYPE>
    void
    DataReader_T<DDS_TYPE>::read_w_instance (
      typename DDS_TYPE::dds_seq_type & data,
      const ::DDS::InstanceHandle_t & lookup_hnd,
      typename DDS_TYPE::sampleinfo_seq_type & sample_info)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::read_w_instance");

      ::DDS_InstanceHandle_t hnd = DDS_HANDLE_NIL;
      hnd <<= lookup_hnd;
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION_STARTING, (LM_INFO, DDS4CCM_INFO
                    ACE_TEXT ("DataReader_T::read_w_instance - ")
                    ACE_TEXT ("Start reading with instance.\n")));
      DDS_ReturnCode_t const retval = this->rti_entity ()->read_instance (
                                            data,
                                            sample_info,
                                            DDS_LENGTH_UNLIMITED,
                                            hnd,
                                            DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                                            DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                            DDS_ALIVE_INSTANCE_STATE);
      if (retval != DDS_RETCODE_OK && retval != DDS_RETCODE_NO_DATA)
        {
          ::DDS::ReturnCode_t const retval = this->return_loan (data, sample_info);
          if (retval != ::DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            ACE_TEXT ("DataReader_T::read_w_instance - ")
                            ACE_TEXT ("Error returning loan to DDS - <%C>\n"),
                            ::CIAO::DDS4CCM::translate_retcode (retval)));
            }
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("DataReader_T::read_w_instance - ")
                        ACE_TEXT ("retval is %C\n"),
                        ::CIAO::DDS4CCM::translate_retcode(retval)));
          throw ::CCM_DDS::InternalError (retval, 0);
        }
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::get (
      typename DDS_TYPE::dds_seq_type & data,
      typename DDS_TYPE::sampleinfo_seq_type & sample_info,
      const DDS_Long & max_samples,
      ::DDS::QueryCondition_ptr qc)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get");

      if (! ::CORBA::is_nil (qc))
        {
          DDS_QueryCondition_i * dds_qc =
            dynamic_cast <DDS_QueryCondition_i *>(qc);
          this->log_query_condition (dds_qc->get_rti_entity ());
          return this->rti_entity ()->read_w_condition (data,
                                                  sample_info,
                                                  max_samples,
                                                  dds_qc->get_rti_entity ());
      }
      return DDS_RETCODE_ERROR;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::get (
      typename DDS_TYPE::dds_seq_type & data,
      typename DDS_TYPE::sampleinfo_seq_type & sample_info,
      const DDS_Long & max_samples,
      ::DDS::ReadCondition_ptr rd)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::get");

      if (! ::CORBA::is_nil (rd))
        {
          DDS_ReadCondition_i * dds_rd =
            dynamic_cast <DDS_ReadCondition_i *> (rd);
          return this->rti_entity ()->read_w_condition (data,
                                                  sample_info,
                                                  max_samples,
                                                  dds_rd->get_rti_entity ());
        }
      return DDS_RETCODE_ERROR;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::take (
      typename DDS_TYPE::dds_seq_type & data,
      typename DDS_TYPE::sampleinfo_seq_type & sample_info,
      const DDS_Long & max_samples,
      ::DDS::QueryCondition_ptr qc)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::take");

      if (! ::CORBA::is_nil (qc))
        {
          DDS_QueryCondition_i * dds_qc =
            dynamic_cast <DDS_QueryCondition_i *>(qc);
          this->log_query_condition (dds_qc->get_rti_entity ());
          return this->rti_entity ()->take_w_condition (data,
                                                  sample_info,
                                                  max_samples,
                                                  dds_qc->get_rti_entity ());
        }
      else
        {
          return this->rti_entity ()->take (data,
                                      sample_info,
                                      max_samples,
                                      DDS_NOT_READ_SAMPLE_STATE,
                                      DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                      DDS_ANY_INSTANCE_STATE);
        }
    }

    template <typename DDS_TYPE>
    ::DDS::InstanceHandle_t
    DataReader_T<DDS_TYPE>::check_handle (
      const typename DDS_TYPE::value_type& an_instance,
      const ::DDS::InstanceHandle_t & instance_handle)
    {
      ::DDS_InstanceHandle_t hnd = ::DDS_HANDLE_NIL;
      hnd <<= instance_handle;

      ::DDS_InstanceHandle_t const lookup_hnd =
          this->rti_entity ()->lookup_instance (an_instance);

      if (!DDS_InstanceHandle_equals (&hnd, &::DDS_HANDLE_NIL) &&
          !DDS_InstanceHandle_equals (&hnd, &lookup_hnd))
        {
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }
      if (DDS_InstanceHandle_equals (&lookup_hnd, &::DDS_HANDLE_NIL))
        {
          throw ::CCM_DDS::NonExistent ();
        }
      ::DDS::InstanceHandle_t ret = ::DDS::HANDLE_NIL;
      ret <<= lookup_hnd;
      return ret;
    }

    template <typename DDS_TYPE>
    ::DDS::InstanceHandle_t
    DataReader_T<DDS_TYPE>::lookup_instance (
      const typename DDS_TYPE::value_type& an_instance)
    {
      ::DDS_InstanceHandle_t hnd =
        this->rti_entity ()->lookup_instance (an_instance);
      ::DDS::InstanceHandle_t ret = ::DDS::HANDLE_NIL;
      ret <<= hnd;
      return ret;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::return_loan (
      typename DDS_TYPE::dds_seq_type & data,
      typename DDS_TYPE::sampleinfo_seq_type & sample_info)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::return_loan");

      return this->rti_entity ()->return_loan (data, sample_info);
    }

    template <typename DDS_TYPE>
    void
    DataReader_T<DDS_TYPE>::passivate ()
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::passivate");

      ::DDS::ReturnCode_t const retcode = this->set_listener (
                                                  ::DDS::DataReaderListener::_nil (),
                                                  0);
      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DataReader_T::passivate - "
                        "Error while setting the listener on the data reader - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }
    }

    template <typename DDS_TYPE>
    void
    DataReader_T<DDS_TYPE>::create_datareader (
      ::DDS::ContentFilteredTopic_ptr topic,
      ::DDS::Subscriber_ptr subscriber,
      const char * library_name,
      const char * profile_name)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DataReader_T::create_datareader");

      ::DDS::DataReader_var reader;
      if (library_name && profile_name)
        {
          reader = subscriber->create_datareader_with_profile (
              topic,
              library_name,
              profile_name,
              ::DDS::DataReaderListener::_nil (),
              0);
        }
      else
        {
          ::DDS::DataReaderQos drqos;
          reader = subscriber->create_datareader (
                topic,
                drqos,
                ::DDS::DataReaderListener::_nil (),
                0);
        }
    typedef DataReader_T<DDS_TYPE> DataReader_type;
      DataReader_type * dds_dr = dynamic_cast < DataReader_type * > (reader.in ());
      if (dds_dr)
        {
          this->set_rti_entity (dds_dr->get_rti_entity (), this->dp_.in ());
        }
    }

    template <typename DDS_TYPE>
    void
    DataReader_T<DDS_TYPE>::create_datareader (
      ::DDS::Topic_ptr topic,
      ::DDS::Subscriber_ptr subscriber,
      const char * library_name,
      const char * profile_name)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::create_datareader");

      ::DDS::DataReader_var reader;

      if (library_name && profile_name)
        {
          reader = subscriber->create_datareader_with_profile (
              topic,
              library_name,
              profile_name,
              ::DDS::DataReaderListener::_nil (),
              0);
        }
      else
        {
          ::DDS::DataReaderQos drqos;
          reader = subscriber->create_datareader (
                topic,
                drqos,
                ::DDS::DataReaderListener::_nil (),
                0);
        }

    typedef DataReader_T<DDS_TYPE> DataReader_type;
      DataReader_type * dds_dr = dynamic_cast < DataReader_type * > (reader.in ());
      if (dds_dr)
        {
          this->set_rti_entity (dds_dr->get_rti_entity (), this->dp_.in ());
        }
    }

    template <typename DDS_TYPE>
    void
    DataReader_T<DDS_TYPE>::delete_datareader (
      ::DDS::Subscriber_ptr subscriber)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::delete_datareader");

      ::DDS::ReturnCode_t const retval = subscriber->delete_datareader (this);
      if (retval != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        ACE_TEXT ("DataReader_T::delete_datareader - ")
                        ACE_TEXT ("Unable to delete DataReader: <%C>\n"),
                        ::CIAO::DDS4CCM::translate_retcode (retval)));
          throw ::CORBA::INTERNAL ();
        }
      this->rti_entity_ = 0;
    }

    template <typename DDS_TYPE>
    typename DDS_TYPE::datareader_type *
    DataReader_T<DDS_TYPE>::rti_entity (void)
    {
      if (this->rti_entity_)
        {
          return this->rti_entity_;
        }
      else
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "DataReader_T<DDS_TYPE>::rti_entity - "
                        "Throwing BAD_INV_ORDER.\n"));
          throw ::CORBA::BAD_INV_ORDER ();
        }
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::enable (void)
    {
      return this->rti_entity ()->enable ();
    }

    template <typename DDS_TYPE>
    ::DDS::StatusCondition_ptr
    DataReader_T<DDS_TYPE>::get_statuscondition (void)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::get_statuscondition");

      ::DDS::StatusCondition_var retval;
      DDSStatusCondition* sc = this->rti_entity ()->get_statuscondition ();
      if (sc)
        {
          ACE_NEW_THROW_EX (retval,
                            DDS_StatusCondition_i (sc, this->dp_.in ()),
                            ::CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }

    template <typename DDS_TYPE>
    ::DDS::StatusMask
    DataReader_T<DDS_TYPE>::get_status_changes (void)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::get_status_changes");

      return this->rti_entity ()->get_status_changes ();
    }

    template <typename DDS_TYPE>
    DDS_INSTANCE_HANDLE_T_RETN
    DataReader_T<DDS_TYPE>::get_instance_handle (void)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::get_instance_handle");

      ::DDS_InstanceHandle_t const rtihandle =
        this->rti_entity ()->get_instance_handle ();
      ::DDS::InstanceHandle_t handle;
      handle <<= rtihandle;
      return handle;
    }

    template <typename DDS_TYPE>
    ::DDS::ReadCondition_ptr
    DataReader_T<DDS_TYPE>::create_readcondition (
      ::DDS::SampleStateMask sample_states,
      ::DDS::ViewStateMask view_states,
      ::DDS::InstanceStateMask instance_states)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::create_readcondition");

      ::DDS::ReadCondition_var retval;
      DDSReadCondition* rc =
        this->rti_entity ()->create_readcondition (sample_states,
                                            view_states,
                                            instance_states);
      if (rc)
        {
          ACE_NEW_THROW_EX (retval,
                            DDS_ReadCondition_i (rc, this),
                            ::CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }

    template <typename DDS_TYPE>
    ::DDS::QueryCondition_ptr
    DataReader_T<DDS_TYPE>::create_querycondition (
      ::DDS::SampleStateMask sample_states,
      ::DDS::ViewStateMask view_states,
      ::DDS::InstanceStateMask instance_states,
      const char * query_expression,
      const ::DDS::StringSeq & query_parameters)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::create_querycondition");

      ::DDS::QueryCondition_var retval;
      DDS_StringSeq parameters;
      parameters <<= query_parameters;
      DDSQueryCondition* qc = this->rti_entity ()->create_querycondition (
                                    sample_states,
                                    view_states,
                                    instance_states,
                                    query_expression,
                                    parameters);

      if (qc)
        {
          ACE_NEW_THROW_EX (retval,
                            DDS_QueryCondition_i (qc, this),
                            ::CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::delete_readcondition (
      ::DDS::ReadCondition_ptr a_condition)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::delete_readcondition");

      ::DDSReadCondition * dds_rc = 0;
      DDS_ReadCondition_i * rc = dynamic_cast< DDS_ReadCondition_i *> (a_condition);
      DDS_QueryCondition_i * qc = dynamic_cast< DDS_QueryCondition_i *> (a_condition);

      if (!rc)
        {
          if (!qc)
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_DEBUG, DDS4CCM_INFO
                            ACE_TEXT ("DataReader_T::delete_readcondition - ")
                            ACE_TEXT ("Unable to cast provided condition to a typed ")
                            ACE_TEXT ("read condition nor a typed query condition\n")));
              return ::DDS::RETCODE_BAD_PARAMETER;
            }
          dds_rc = dynamic_cast < ::DDSReadCondition *> (qc->get_rti_entity ());
          if (!dds_rc)
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_CAST_ERROR, (LM_DEBUG, DDS4CCM_INFO
                            ACE_TEXT ("DataReader_T::delete_readcondition - ")
                            ACE_TEXT ("Unable to cast DDSQueryCondition to a ")
                            ACE_TEXT ("DDSReadCondition\n")));
              return ::DDS::RETCODE_BAD_PARAMETER;
            }
        }
      else
        {
          dds_rc = rc->get_rti_entity ();
        }
      ::DDS::ReturnCode_t retcode = this->rti_entity ()->delete_readcondition (dds_rc);
      if (retcode == ::DDS::RETCODE_OK)
        {
          if (rc)
            {
              rc->set_rti_entity (0);
            }
          if (qc)
            {
              qc->set_rti_entity (0);
            }
        }
      return retcode;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::delete_contained_entities (void)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::delete_contained_entities");

      return this->rti_entity ()->delete_contained_entities ();
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::set_qos (
      const ::DDS::DataReaderQos &qos)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::set_qos");

      ::DDS_DataReaderQos ddsqos;
      ddsqos <<= qos;
      return this->rti_entity ()->set_qos (ddsqos);
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::get_qos (
      ::DDS::DataReaderQos &qos)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::get_qos");

      ::DDS_DataReaderQos ddsqos;
      ::DDS_ReturnCode_t const retval = this->rti_entity ()->get_qos (ddsqos);
      qos <<= ddsqos;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::set_listener (
      ::DDS::DataReaderListener_ptr a_listener,
      ::DDS::StatusMask mask)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T<DDS_TYPE>::set_listener");

      // Delete the previously set listener
      DDSDataReaderListener *listener = this->rti_entity ()->get_listener ();
      delete listener;
      listener = 0;

      if (! ::CORBA::is_nil (a_listener))
        {
          ACE_NEW_THROW_EX (listener,
                            DDS_DataReaderListener_i (
                              a_listener,
                              this),
                            ::CORBA::NO_MEMORY ());
        }
      this->lst_mask_ = mask;
      return this->rti_entity ()->set_listener (listener, mask);
    }

    template <typename DDS_TYPE>
    ::DDS::DataReaderListener_ptr
    DataReader_T<DDS_TYPE>::get_listener (void)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T<DDS_TYPE>::get_listener");

      DDSDataReaderListener *drl = this->rti_entity ()->get_listener ();
      DDS_DataReaderListener_i *ccm_dds_drl =
        dynamic_cast <DDS_DataReaderListener_i *> (drl);
      if (!ccm_dds_drl)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, "DataReader_T"
                                    "::get_listener - "
                                    "DDS returned a NIL listener.\n"));
          return ::DDS::DataReaderListener::_nil ();
        }
      return ccm_dds_drl->get_datareaderlistener ();
    }

    template <typename DDS_TYPE>
    ::DDS::TopicDescription_ptr
    DataReader_T<DDS_TYPE>::get_topicdescription (void)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::get_topicdescription");

      ::DDS::TopicDescription_var dds_td;
      DDSTopicDescription* td = this->rti_entity ()->get_topicdescription ();
      DDSTopic * tp = 0;
      if (td)
        {
          tp = DDSTopic::narrow (td);

          if (tp)
            {
              ACE_NEW_THROW_EX (dds_td,
                                DDS_Topic_i (tp, this->dp_.in ()),
                                ::CORBA::NO_MEMORY ());
            }
          else
            {
              ::DDSContentFilteredTopic * cft =
                DDSContentFilteredTopic::narrow (td);
              if (cft)
                {
                  ACE_NEW_THROW_EX (dds_td,
                                    DDS_ContentFilteredTopic_i (cft, this->dp_.in ()),
                                    ::CORBA::NO_MEMORY ());
                }
            }
        }
      return dds_td._retn ();
    }

    template <typename DDS_TYPE>
    ::DDS::Subscriber_ptr
    DataReader_T<DDS_TYPE>::get_subscriber (void)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::get_subscriber");

    /*  ::DDS::Subscriber_var dds_td;
      ::DDSSubscriber* subscriber = this->rti_entity ()->get_subscriber ();
      ACE_NEW_THROW_EX (dds_td,
                        Subscriber_type (subscriber),
                        ::CORBA::NO_MEMORY ());
      return dds_td._retn ();*/
      return 0;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::get_sample_rejected_status (
      ::DDS::SampleRejectedStatus & status)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::get_sample_rejected_status");

      DDS_SampleRejectedStatus ddsstatus;
      ::DDS::ReturnCode_t const retval = this->rti_entity ()->get_sample_rejected_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::get_liveliness_changed_status (
      ::DDS::LivelinessChangedStatus & status)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::get_liveliness_changed_status");

      DDS_LivelinessChangedStatus ddsstatus;
      ::DDS::ReturnCode_t const retval = this->rti_entity ()->get_liveliness_changed_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::get_requested_deadline_missed_status (
      ::DDS::RequestedDeadlineMissedStatus & status)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::get_requested_deadline_missed_status");

      DDS_RequestedDeadlineMissedStatus ddsstatus;
      ::DDS::ReturnCode_t const retval = this->rti_entity ()->get_requested_deadline_missed_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::get_requested_incompatible_qos_status (
      ::DDS::RequestedIncompatibleQosStatus & status)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::get_requested_incompatible_qos_status");

      DDS_RequestedIncompatibleQosStatus ddsstatus;
      ::DDS::ReturnCode_t const retval =
        this->rti_entity ()->get_requested_incompatible_qos_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::get_subscription_matched_status (
      ::DDS::SubscriptionMatchedStatus & status)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::get_subscription_matched_status");

      ::DDS_SubscriptionMatchedStatus ddsstatus;
      ::DDS::ReturnCode_t const retval = this->rti_entity ()->get_subscription_matched_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::get_sample_lost_status (
      ::DDS::SampleLostStatus & status)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::get_sample_lost_status");

      DDS_SampleLostStatus ddsstatus;
      ::DDS::ReturnCode_t const retval = this->rti_entity ()->get_sample_lost_status (ddsstatus);
      status <<= ddsstatus;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::wait_for_historical_data (
      const ::DDS::Duration_t & max_wait)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::wait_for_historical_data");

      ::DDS_Duration_t rtiduration;
      rtiduration <<= max_wait;
      return this->rti_entity ()->wait_for_historical_data (rtiduration);
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::get_matched_publications (
      ::DDS::InstanceHandleSeq & publication_handles)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T<DDS_TYPE>::get_matched_publications");

      ::DDS_InstanceHandleSeq rtiseq;
      ::DDS::ReturnCode_t const retval = this->rti_entity ()->get_matched_publications (rtiseq);
      publication_handles <<= rtiseq;
      return retval;
    }

    template <typename DDS_TYPE>
    ::DDS::ReturnCode_t
    DataReader_T<DDS_TYPE>::get_matched_publication_data (
      ::DDS::PublicationBuiltinTopicData & publication_data,
      DDS_INSTANCE_HANDLE_T_IN publication_handle)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::get_matched_publication_data");

      ::DDS_PublicationBuiltinTopicData ccm_dds_pub_data;
      ::DDS_InstanceHandle_t ccm_dds_pub_handle;

      ccm_dds_pub_data <<= publication_data;
      ccm_dds_pub_handle <<= publication_handle;
      ::DDS::ReturnCode_t const retcode = this->rti_entity ()->
                    get_matched_publication_data (ccm_dds_pub_data,
                                                  ccm_dds_pub_handle);
      publication_data <<= ccm_dds_pub_data;
      return retcode;
    }

    template <typename DDS_TYPE>
    ::DDS::StatusMask
    DataReader_T<DDS_TYPE>::get_mask (void)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::get_mask");

      return this->lst_mask_;
    }

    template <typename DDS_TYPE>
    void
    DataReader_T<DDS_TYPE>::set_rti_entity (
      ::DDSDataReader* dr, ::DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("CIAO::NDDS::DataReader_T::set_impl");

      this->rti_entity_ = DDS_TYPE::datareader_type::narrow (dr);
      this->dp_ = ::DDS::DomainParticipant::_duplicate (dp);
    }

    template <typename DDS_TYPE>
    ::DDSDataReader*
    DataReader_T<DDS_TYPE>::get_rti_entity (void)
    {
      return this->rti_entity_;
    }
  }
}
