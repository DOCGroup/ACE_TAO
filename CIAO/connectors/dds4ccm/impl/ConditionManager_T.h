// $Id$

#ifndef CONDITIONMANAGER_T_H_
#define CONDITIONMANAGER_T_H_

namespace CIAO
{
  namespace NDDS
  {
    template <typename DDS_TYPE>
    class DataReader_T;
  }

  namespace DDS4CCM
  {
    //============================================================
    // ConditionManager_T
    //============================================================
    template <typename DDS_TYPE>
    class ConditionManager_T
    {
    typedef ::CIAO::NDDS::DataReader_T<DDS_TYPE> DataReader_type;

    public:
      ConditionManager_T (void);
      ~ConditionManager_T (void);

      ::CIAO::NDDS::DDS_ReadCondition_i  * get_readcondition (void);

      ::CIAO::NDDS::DDS_QueryCondition_i * get_querycondition_getter (void);
      ::CIAO::NDDS::DDS_QueryCondition_i * get_querycondition_reader (void);
      ::CIAO::NDDS::DDS_QueryCondition_i * get_querycondition_listener (void);

      void init_readcondition (void);

      bool wait (DDSConditionSeq & active_conditions,
                 DDS_Duration_t & time_out);

      ::CCM_DDS::QueryFilter * query (void);

      void query (const ::CCM_DDS::QueryFilter & filter);

      void set_impl (DataReader_type * impl);

      DataReader_type * get_impl (void);

      void passivate (void);

    private:
      DataReader_type * impl_;

      DDSWaitSet * ws_;

      // Different QueryConditions since the sample mask
      // differs for all entities.
      ::DDS::QueryCondition_var qc_reader_;
      ::DDS::QueryCondition_var qc_getter_;
      ::DDS::QueryCondition_var qc_listener_;

      ::DDS::ReadCondition_var rd_condition_;

      DataReader_type * impl (void);

      ::CIAO::NDDS::DDS_QueryCondition_i * get_querycondition (
        ::DDS::QueryCondition_ptr dds_qc);

      void remove_condition (::DDS::QueryCondition_ptr qc,
                             const char * type);

      void remove_conditions (void);

      void attach_querycondition (void);

      void set_parameters (const ::CCM_DDS::QueryFilter & filter,
                           ::DDS::QueryCondition_ptr qc);
    };
  }
}

#include "dds4ccm/impl/ConditionManager_T.cpp"

#endif /* CONDITIONMANAGER_T_H_ */
