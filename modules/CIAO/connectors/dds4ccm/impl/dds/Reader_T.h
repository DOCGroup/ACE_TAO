/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_READER_T
#define DDS_READER_T

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"
#include "dds4ccm/impl/dds4ccm_conf.h"
#include "dds4ccm/impl/dds/Getter_T.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::InstanceHandle_t DDS_InstanceHandle_t;
typedef ::DDS::SampleInfoSeq DDS_SampleInfoSeq;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace DDS_CCM
    {
      template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
      class Reader_T :
          public virtual CCM_TYPE::reader_type,
          public virtual ::CORBA::LocalObject,
          private virtual ACE_Copy_Disabled
      {
      public:
        /// Constructor
        Reader_T (void);

        /// Destructor
        virtual ~Reader_T (void);

        virtual void read_last (
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos);

        virtual void read_all (
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos);

        virtual void read_one_last (
          typename DDS_TYPE::value_type& an_instance,
          ::CCM_DDS::ReadInfo_out info,
          const ::DDS::InstanceHandle_t & instance_handle);

        virtual void read_one_all (
          const typename DDS_TYPE::value_type& an_instance,
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos,
          const ::DDS::InstanceHandle_t & instance_handle);

        virtual ::CCM_DDS::QueryFilter *filter (void);

        virtual void filter (const ::CCM_DDS::QueryFilter & filter);

        void set_contentfilteredtopic_data (
          const char * libary_name,
          const char * profile_name,
          Getter_T<DDS_TYPE, CCM_TYPE, FIXED> * dds_get=0);

        void set_impl (CCM_DDS_DataReader_i *reader);

        DDS_ReturnCode_t passivate (void);

      private:
        CCM_DDS_DataReader_i *reader_;

        Getter_T<DDS_TYPE, CCM_TYPE, FIXED> * dds_get_;

        ACE_CString library_name_;
        ACE_CString profile_name_;

        #if (DDS4CCM_USES_QUERY_CONDITION==1)
          ::DDSQueryCondition * qc_;
        #else
          ::DDSContentFilteredTopic * cft_;
        #endif

        typename DDS_TYPE::data_reader * impl (void);

        DDS_InstanceHandle_t check_handle (
          const typename DDS_TYPE::value_type& an_instance,
          const ::DDS::InstanceHandle_t & instance_handle);

        void read_with_instance (
          typename DDS_TYPE::dds_seq_type & data,
          const ::DDS_InstanceHandle_t & lookup_hnd,
          DDS_SampleInfoSeq & sample_info);

        void read_without_instance (
          typename DDS_TYPE::dds_seq_type & data,
          DDS_SampleInfoSeq & sample_info);

        CORBA::ULong get_nr_valid_samples (
          const DDS_SampleInfoSeq & sample_info,
          bool determine_last = false);

        void delete_datareader (
          ::DDSSubscriber * sub);

        void create_filter (
          const ::CCM_DDS::QueryFilter & filter);

        void create_contentfilteredtopic (
          const ::CCM_DDS::QueryFilter & filter,
          ::DDSSubscriber * sub);
      };
    }
  }
}

#include "dds4ccm/impl/dds/Reader_T.cpp"

#endif /* DDS_READER_T */

