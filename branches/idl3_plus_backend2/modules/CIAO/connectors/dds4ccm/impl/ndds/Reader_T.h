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

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      template <typename DDS_TYPE, typename CCM_TYPE>
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

        void set_impl (::DDS::DataReader_ptr reader);

      private:
        typename DDS_TYPE::data_reader *impl_;
        ::DDS::DataReader_ptr reader_;

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
      };
    }
  }
}

#include "dds4ccm/impl/ndds/Reader_T.cpp"

#endif /* DDS_READER_T */

