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
#include "dds4ccm/impl/LocalObject.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    class ConditionManager;

    template <typename READER_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE>
    class Reader_T :
        public virtual READER_TYPE,
        public virtual LocalObject,
        private virtual ACE_Copy_Disabled
    {
    public:
      /// Constructor
      Reader_T (void);

      /// Destructor
      virtual ~Reader_T (void);

      virtual void read_last (
        SEQ_VALUE_TYPE& instances,
        ::CCM_DDS::ReadInfoSeq& infos);

      virtual void read_all (
        SEQ_VALUE_TYPE& instances,
        ::CCM_DDS::ReadInfoSeq& infos);

      virtual void read_one_last (
        VALUE_TYPE& an_instance,
        ::CCM_DDS::ReadInfo_out info,
        const ::DDS::InstanceHandle_t & instance_handle);

      virtual void read_one_all (
        const VALUE_TYPE& an_instance,
        SEQ_VALUE_TYPE& instances,
        ::CCM_DDS::ReadInfoSeq& infos,
        const ::DDS::InstanceHandle_t & instance_handle);

      virtual ::CCM_DDS::QueryFilter *query (void);

      virtual void query (const ::CCM_DDS::QueryFilter & filter);

      void set_dds_reader (::DDS::DataReader_ptr dr,
                              ::CIAO::DDS4CCM::ConditionManager * condition_manager);

      ::DDS::DataReader_ptr get_dds_reader (void);

    private:
      typename TYPED_DDS_READER::_var_type dds_reader_;
      ::CIAO::DDS4CCM::ConditionManager * condition_manager_;

      typename TYPED_DDS_READER::_ptr_type dds_reader (void);

      // Helper methods
      ::CORBA::ULong get_nr_valid_samples (
        const ::DDS::SampleInfoSeq& sample_infos,
        const bool determine_last);

      ::DDS::InstanceHandle_t check_handle (
        const VALUE_TYPE& an_instance,
        const ::DDS::InstanceHandle_t & instance_handle);

      void convert_data (
        const SEQ_VALUE_TYPE& all_data,
        SEQ_VALUE_TYPE& data_to_return,
        ::CCM_DDS::ReadInfoSeq& infos,
        const ::DDS::SampleInfoSeq & sample_info);

      void read_wo_instance (
        SEQ_VALUE_TYPE& data,
        ::DDS::SampleInfoSeq & sample_info,
        ::DDS::QueryCondition_ptr qc);

      void read_w_instance (
        SEQ_VALUE_TYPE& data,
        ::DDS::SampleInfoSeq & sample_info,
        const ::DDS::InstanceHandle_t & lookup_hnd);

      void return_loan (
        SEQ_VALUE_TYPE& data,
        ::DDS::SampleInfoSeq sample_info);
    };
  }
}

#include "dds4ccm/impl/Reader_T.cpp"

#endif /* READER_T_H_ */

