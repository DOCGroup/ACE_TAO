/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Common dataReader class for the Getter, Reader,
 * and, Listen port
 *
 */
#ifndef DDS_DATAREADER_T_H
#define DDS_DATAREADER_T_H

#include "dds4ccm/impl/dds/DataReader.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::InstanceHandle_t DDS_InstanceHandle_t;
typedef ::DDS::SampleInfoSeq DDS_SampleInfoSeq;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    class DataReader_T
      : public virtual CCM_DDS_DataReader_i
    {
    public:
      /// Constructor
      DataReader_T (void);

      /// Destructor
      virtual ~DataReader_T (void);

      void read_w_instance (
        typename DDS_TYPE::dds_seq_type & data,
        const ::DDS_InstanceHandle_t & lookup_hnd,
        ::DDS_SampleInfoSeq & sample_info);

      void read_wo_instance (
        typename DDS_TYPE::dds_seq_type & data,
        ::DDS_SampleInfoSeq & sample_info);

      DDS_ReturnCode_t read_w_condition (
        typename DDS_TYPE::dds_seq_type & data,
        ::DDS_SampleInfoSeq & sample_info,
        ::DDS_Long max_samples);

      DDS_ReturnCode_t take (
        typename DDS_TYPE::dds_seq_type & data,
        ::DDS_SampleInfoSeq & sample_info,
        ::DDS_Long max_samples);

      DDS_InstanceHandle_t
      lookup_instance (const typename DDS_TYPE::value_type& an_instance);

      DDS_ReturnCode_t
      return_loan (
        typename DDS_TYPE::dds_seq_type & data,
        ::DDS_SampleInfoSeq & sample_info);

      void delete_datareader (::DDS::Subscriber_ptr subscriber);

      ::CCM_DDS::QueryFilter *query (void);

      void query (const ::CCM_DDS::QueryFilter & filter);

      void
      set_filter (const ::CCM_DDS::QueryFilter & filter,
                  DDSQueryCondition * qc);

      // Getter related methods.
      void remove_condition (DDSQueryCondition * qc, const char * type);

      void remove_conditions (void);

      DDSReadCondition *get_readcondition (void);

      void create_readcondition (void);

      DDSQueryCondition *get_querycondition (void);

      void attach_querycondition (void);

      bool
      wait (DDSConditionSeq & active_conditions, DDS_Duration_t & time_out);

      void set_proxy (DDSDataReader * dr);

      void passivate (void);

      void
      create_datareader (::DDS::ContentFilteredTopic_ptr topic,
                         ::DDS::Subscriber_ptr subscriber,
                         const char * library_name,
                         const char * profile_name);

      void
      create_datareader (::DDS::Topic_ptr topic,
                          ::DDS::Subscriber_ptr subscriber,
                          const char * library_name,
                          const char * profile_name);

    private:
      typename DDS_TYPE::data_reader * impl_;

      ::DDSReadCondition * rd_condition_;

      ::DDSWaitSet * ws_;

      // Different QueryConditions since the sample mask
      // differs for all entities.
      DDSQueryCondition * qc_reader_;
      DDSQueryCondition * qc_getter_;
      DDSQueryCondition * qc_listener_;

      typename DDS_TYPE::data_reader * impl (void);
    };
  }
}

#include "dds4ccm/impl/dds/DataReader_T.cpp"

#endif /* DDS_DATAREADER_T_H */
