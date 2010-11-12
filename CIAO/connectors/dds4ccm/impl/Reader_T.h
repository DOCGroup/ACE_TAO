/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef READER_T_H_
#define READER_T_H_

#include "dds4ccm/idl/ccm_ddsC.h"
#include "ace/Copy_Disabled.h"
#include "dds4ccm/impl/dds4ccm_conf.h"
#include "dds4ccm/impl/LocalObject_T.h"

#include "dds4ccm/impl/ConditionManager_T.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace DDS_CCM
    {
      template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
      class Reader_T :
          public virtual CCM_TYPE::reader_type,
          public virtual LocalObject_T<CCM_TYPE>,
          private virtual ACE_Copy_Disabled
      {
      typedef ConditionManager_T<DDS_TYPE> ConditionManager_type;

      public:
        /// Constructor
        Reader_T (void);

        /// Destructor
        virtual ~Reader_T (void);

        virtual void read_last (
          typename CCM_TYPE::seq_type& instances,
          ::CCM_DDS::ReadInfoSeq& infos);

        virtual void read_all (
          typename CCM_TYPE::seq_type& instances,
          ::CCM_DDS::ReadInfoSeq& infos);

        virtual void read_one_last (
          typename DDS_TYPE::value_type& an_instance,
          ::CCM_DDS::ReadInfo_out info,
          const ::DDS::InstanceHandle_t & instance_handle);

        virtual void read_one_all (
          const typename DDS_TYPE::value_type& an_instance,
          typename CCM_TYPE::seq_type& instances,
          ::CCM_DDS::ReadInfoSeq& infos,
          const ::DDS::InstanceHandle_t & instance_handle);

        virtual ::CCM_DDS::QueryFilter *query (void);

        virtual void query (const ::CCM_DDS::QueryFilter & filter);

        void set_dds_entity (::DDS::DataReader_ptr dr,
                             ConditionManager_type * condition_manager);

      private:
        typename DDS_TYPE::typed_reader_type::_var_type dds_reader_;
        ConditionManager_type * condition_manager_;

        typename DDS_TYPE::typed_reader_type::_ptr_type
        dds_entity (void);

        // Helper methods
        ::DDS::InstanceHandle_t check_handle (
          const typename DDS_TYPE::value_type& an_instance,
          const ::DDS::InstanceHandle_t & instance_handle);

        void convert_sample_infos (
          ::CCM_DDS::ReadInfoSeq& infos,
          ::DDS::SampleInfoSeq sample_info);

        void read_wo_instance (
          typename DDS_TYPE::seq_type & data,
          ::DDS::SampleInfoSeq & sample_info,
          ::DDS::QueryCondition_ptr qc);

        void read_w_instance (
          typename DDS_TYPE::seq_type & data,
          ::DDS::SampleInfoSeq & sample_info,
          const ::DDS::InstanceHandle_t & lookup_hnd);

        void return_loan (
          typename DDS_TYPE::seq_type & data,
          ::DDS::SampleInfoSeq sample_info);
      };
    }
  }
}

#include "dds4ccm/impl/Reader_T.cpp"

#endif /* READER_T_H_ */

