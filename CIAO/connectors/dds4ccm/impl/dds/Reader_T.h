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
#include "dds4ccm/impl/dds/DataReader_T.h"

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

        virtual ::CCM_DDS::QueryFilter *filter (void);

        virtual void filter (const ::CCM_DDS::QueryFilter & filter);

        void set_impl (DataReader_T<DDS_TYPE, CCM_TYPE> * dr);

      private:
        DataReader_T<DDS_TYPE, CCM_TYPE> * reader_;

        DataReader_T<DDS_TYPE, CCM_TYPE> * impl (void);

        DDS_InstanceHandle_t check_handle (
          const typename DDS_TYPE::value_type& an_instance,
          const ::DDS::InstanceHandle_t & instance_handle);

        CORBA::ULong get_nr_valid_samples (
          const DDS_SampleInfoSeq & sample_info,
          bool determine_last = false);
      };
    }
  }
}

#include "dds4ccm/impl/dds/Reader_T.cpp"

#endif /* DDS_READER_T */

