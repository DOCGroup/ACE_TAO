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

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"
#include "dds4ccm/impl/dds4ccm_conf.h"
#include "dds4ccm/impl/LocalObject_T.h"

#include "dds4ccm/impl/ConditionManager_T.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::InstanceHandle_t DDS_InstanceHandle_t;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace DDS_CCM
    {
      template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
      class Reader_T :
          public virtual CCM_TYPE::reader_type,
          public virtual LocalObject_T<CCM_TYPE>,
          private virtual ACE_Copy_Disabled
      {
      typedef DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>
        DataReader_type;
      typedef ConditionManager_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>
        ConditionManager_type;

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

        void set_impl (DataReader_type * dr,
                       ConditionManager_type * condition_manager);

      private:
        DataReader_type * reader_;
        ConditionManager_type * condition_manager_;

        DataReader_type * impl (void);

        DDS_InstanceHandle_t check_handle (
          const typename DDS_TYPE::value_type& an_instance,
          const ::DDS::InstanceHandle_t & instance_handle);

        CORBA::ULong get_nr_valid_samples (
          const typename DDS_TYPE::sampleinfo_seq_type & sample_info,
          bool determine_last = false);
      };
    }
  }
}

#include "dds4ccm/impl/Reader_T.cpp"

#endif /* READER_T_H_ */

