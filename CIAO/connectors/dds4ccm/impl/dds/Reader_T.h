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
      /**
        @class Reader_T<DDS_TYPE,CCM_TYPE>

        @brief Templated base class for the Reader port.

        Spec: Behavior of a Reader is as follows:
              - Underlying DDS read operations will be performed with the following DDS access parameters:
              - SampleStateMask: READ or NO_READ,
              - ViewStateMask: NEW or NOT_NEW,
              - InstanceStateMask: ALIVE,
              - Through the query as specified in the filter ("" means no query).

        Note: This interface is the basis for a passive data reader (i.e. a component that
              just looks at the data as they are). It is also very useful for the reactive
              data getters (i.e. components that need to react to new data, whether they
              choose to get them in pull mode or be notified in push mode) in their initialization
              phase. This is the reason why all the DDS ports on the subscribing side will
              embed a Reader basic port.
      */

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

        /**
          Spec : read_last returns the last sample of all instances.
                 Any DDS error when reading the data will be reported by an
                 InternalError exception.
         */
        virtual void read_last (
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos);

        /**
          Spec : read_all returns all samples of all instances.
                 Any DDS error when reading the data will be
                 reported by an InternalError exception.
         */
        virtual void read_all (
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos);

        /**
          Spec : read_one_last returns the last sample of a given instance.
                 The targeted instance is designated by the passed instance
                 handle (instance_handle) if not DDS::HANDLE_NIL or by the
                 key value in the passed data (datum) otherwise. If a valid
                 handle is passed, it must be in accordance with the key
                 value of the passed data otherwise an InternalError exception
                 is raised with the returned DDS error code. More generally,
                 any DDS error when reading the data will be reported by an
                 InternalError exception.
                 In case the instance does not exist (no data are registered
                 for that instance in DDS), the exception NonExistent is raised.
                 In case of a keyless topic, the last value in the topic will be
                 returned as DDS considers all values in such a topic as samples
                 of one unique instance.
         */
        virtual void read_one_last (
          typename DDS_TYPE::value_type& an_instance,
          ::CCM_DDS::ReadInfo_out info,
          const ::DDS::InstanceHandle_t & instance_handle);

        /**
          Spec : read_one_all returns all the samples of a given instance.
                 The targeted instance is designated by the passed instance
                 handle (instance_handle) if not DDS::HANDLE_NIL or by the
                 key value in the passed data (datum) otherwise. If a valid
                 handle is passed, it must be in accordance with the key value
                 of the passed data otherwise an InternalError exception is
                 raised with the returned DDS error code. More generally, any
                 DDS error when reading the data will be reported by an
                 InternalError exception.
                 In case the instance does not exist (no data are registered for
                 that instance in DDS), the exception NonExistent is raised.
         */
        virtual void read_one_all (
          const typename DDS_TYPE::value_type& an_instance,
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos,
          const ::DDS::InstanceHandle_t & instance_handle);

        /**
          Spec : QueryFilter gathers in a single structure a query and its related
                 parameters. The QueryFilter attribute placed on the Reader interface
                 acts as a filter for all the read operations made through a port
                 where such a Reader is attached.
                 An empty string query means no query.
                 This query and its related parameters are for DDS use and must comply
                 with DDS rules (c.f. DDS specification for more details). Any attempt
                 to set the attribute with values that are not accepted by DDS will
                 result in a InternalError exception.
         */
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

