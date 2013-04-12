// $Id$

#ifndef CONDITIONMANAGER_H_
#define CONDITIONMANAGER_H_

#include "dds4ccm/idl/dds4ccm_BaseC.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_NDDS==1)
#include "dds4ccm/impl/ndds/WaitSet.h"
#elif (CIAO_DDS4CCM_OPENDDS==1)
#include "dds/DCPS/WaitSet.h"
#endif

#include "dds4ccm/impl/dds4ccm_dds_impl_export.h"

namespace CCM_DDS
{
  struct QueryFilter;
}

namespace CIAO
{
  namespace DDS4CCM
  {
    /**
      * @class ConditionManager
      *
      * @brief Helper class
      *
      * Manages the read and query conditions for the Getter,
      * Reader and listeners.
      */
    class DDS4CCM_DDS_IMPL_Export ConditionManager
    {
    public:
      /// Constructor
      ConditionManager (void);

      /// Destructor
      ~ConditionManager (void);

      /// Returns the read condition in general
      ::DDS::ReadCondition_ptr get_readcondition (void);

      /// Returns the query condition for the getter
      ::DDS::QueryCondition_ptr get_querycondition_getter (void);
      /// Returns the query condition for the reader
      ::DDS::QueryCondition_ptr get_querycondition_reader (void);
      /// Returns the query condition for the listener
      ::DDS::QueryCondition_ptr get_querycondition_listener (void);

      /**
        * Proxy method.
        * See WaitSet::check_handle in ndds/WaitSet.h
        * for more information on this.
        *
        * If WaitSet::check_handle set 'error' to true, a
        * ::CCM_DDS::InternalError exception is called.
        * Error will be ::DDS::RETCODE_ERROR
        *
        * If WaitSet::check_handle set 'non_existent' to true,
        * a ::CCM_DDS::NonExistent exception is thrown.
        */
      ::DDS::InstanceHandle_t check_handle (
        DDS_INSTANCE_HANDLE_T_IN instance_handle,
        DDS_INSTANCE_HANDLE_T_IN lookup_handle);

      /**
        * Getter functionality.
        * Creates a read_condition for the getter.
        * It will also initialize the waitset and attaches the
        * read_condition to the waitset.
        */
      void init_readcondition (void);

      /**
        * Getter functionality, waits for the attached conditions.
        * Only returns true when one of the getter conditions has triggered
        * and we have data
        */
      bool wait (const ::DDS::Duration_t & time_out);

      /**
        * Returns the internal filter. This is the DDS4CCM QueryFilter
        * (a representation of the DDS query).
        *
        * If the QueryFilter hasn't been initialized, a ::CCM_DDS::InternalError
        * exception with error code ::DDS::RETCODE_ERROR is thrown.
        */
      ::CCM_DDS::QueryFilter * query (void);

      /**
        * Sets the DDS4CCM query filter.
        * The following rules apply when a filter is set:
        *
        * If there's a read condition set, this will be removed.
        * The read condition is replaced by a query condition.
        *
        * If there're no query conditions yet (i.e. the filter hasn't
        * been set before), three query conditions are created:
        * 1. One for the Getter
        * 2. One for the Reader
        * 3. One for the Listener
        * Each query condition has it's own sample_state,
        * instance_state and view_state set but the filters are
        * the same.
        *
        * Off course the query condition for the getter will be
        * attached to the waitset.
        *
        * If the internal filter has already been set, this method sets
        * the new parameters for this filter on all three query conditions.
        *
        * If the filter expression (the actual query) is empty, all three query
        * conditions will be removed (the query condition for the getter will be
        * detached from the waitset first) and the initial read condition will
        * created (see init_readcondition()).
        */
      void query (const ::CCM_DDS::QueryFilter & filter);

      /**
        * Sets the internal datareader.
        * Applies to Getter, Reader and Listener
        */
      void set_dds_entity (::DDS::DataReader_ptr dr);

      /**
        * Retrieves the internal datareader.
        */
      ::DDS::DataReader_ptr get_dds_entity (void);

      /**
        * Detaches conditions from the waitset and
        * removes all conditions by calling
        * delete_readcondition on the DataReader.
        */
      void passivate (void);

    private:
      ::DDS::DataReader_var dr_;

      //TODO: Not allowed
#if (CIAO_DDS4CCM_NDDS==1)
      ::CIAO::NDDS::DDS_WaitSet_i ws_;
#elif (CIAO_DDS4CCM_OPENDDS==1)
      ::DDS::WaitSet ws_;
#endif

      /**
        * Different QueryConditions since the sample mask
        * differs for all entities.
        */
      ::DDS::QueryCondition_var qc_reader_;
      ::DDS::QueryCondition_var qc_getter_;
      ::DDS::QueryCondition_var qc_listener_;

      ::DDS::ReadCondition_var rd_condition_;

      /**
        * Internal getter for the DataReader.
        * If the internal dds_entity is not set before this method
        * is called, a CORBA::BAD_INV_ORDER exception is thrown.
        */
      ::DDS::DataReader_ptr dds_entity (void);

      /**
        * Removes the internal (read/query) condition by invoking
        * delete_condition on the DataReader.
        */
      void remove_condition (::DDS::QueryCondition_ptr qc,
                             const char * type);

      /**
        * Calling 'remove_condition' for each query and read
        * condition.
        *
        * The query condition for the getter will be detached from
        * the waitset first.
        */
      void remove_conditions (void);

      /**
        * Getter functionality.
        *
        * Attaches the query condition for the getter to the
        * waitset. It the waitset doesn't exist, it will be
        * created.
        */
      void attach_querycondition (void);

      /**
        * Generic method which sets the parameters of the given
        * filter on the given query condition.
        */
      void set_parameters (const ::CCM_DDS::QueryFilter & filter,
                           ::DDS::QueryCondition_ptr qc);
    };
  }
}

#endif /* CONDITIONMANAGER_H_ */
