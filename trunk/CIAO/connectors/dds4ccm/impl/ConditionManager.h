// $Id$

#ifndef CONDITIONMANAGER_H_
#define CONDITIONMANAGER_H_

#include "dds4ccm/impl/ndds/WaitSet.h"
#include <dds4ccm/idl/dds4ccm_BaseC.h>

#include "dds4ccm/impl/dds4ccm_dds_impl_export.h"

namespace CCM_DDS
{
  class QueryFilter;
}

namespace CIAO
{
  namespace DDS4CCM
  {
    //============================================================
    // ConditionManager_T
    //============================================================
    class DDS4CCM_DDS_IMPL_Export ConditionManager
    {
    public:
      ConditionManager (void);
      ~ConditionManager (void);

      ::DDS::ReadCondition_ptr get_readcondition (void);

      ::DDS::QueryCondition_ptr get_querycondition_getter (void);
      ::DDS::QueryCondition_ptr get_querycondition_reader (void);
      ::DDS::QueryCondition_ptr get_querycondition_listener (void);

      ::DDS::InstanceHandle_t check_handle (
        const ::DDS::InstanceHandle_t & instance_handle,
        const ::DDS::InstanceHandle_t & lookup_handle);

      bool check_condition (::DDS::Condition_ptr condition);
      void init_readcondition (void);

      bool wait (::DDS::ConditionSeq  & active_conditions,
                 ::DDS::Duration_t & time_out);

      ::CCM_DDS::QueryFilter * query (void);

      void query (const ::CCM_DDS::QueryFilter & filter);

      void set_dds_entity (::DDS::DataReader_ptr dr);

      ::DDS::DataReader_ptr get_dds_entity (void);

      void passivate (void);

    private:
      ::DDS::DataReader_var dr_;

      //TODO: Not allowed
      ::CIAO::NDDS::DDS_WaitSet_i ws_;

      // Different QueryConditions since the sample mask
      // differs for all entities.
      ::DDS::QueryCondition_var qc_reader_;
      ::DDS::QueryCondition_var qc_getter_;
      ::DDS::QueryCondition_var qc_listener_;

      ::DDS::ReadCondition_var rd_condition_;

      ::DDS::DataReader_ptr dds_entity (void);

      void remove_condition (::DDS::QueryCondition_ptr qc,
                             const char * type);

      void remove_conditions (void);

      void attach_querycondition (void);

      void set_parameters (const ::CCM_DDS::QueryFilter & filter,
                           ::DDS::QueryCondition_ptr qc);
    };
  }
}

#endif /* CONDITIONMANAGER_H_ */
