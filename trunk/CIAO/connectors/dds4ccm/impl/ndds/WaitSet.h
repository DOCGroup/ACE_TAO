/**
 * @author Marcel Smit (msmit@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef WAITSET_H_
#define WAITSET_H_

#include "dds4ccm/idl/ndds/ndds_dcpsC.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"

class DDSWaitSet;
struct DDSConditionSeq;

namespace CIAO
{
  namespace NDDS
  {
    /**
      * @class DDS_WaitSet_i
      *
      * @brief Representative (proxy) class for the RTI waitset.
      *
      * Implementation of the RTI waitset functionality.
      */
    class DDS4CCM_NDDS_Export DDS_WaitSet_i :
      public virtual ::DDS::WaitSet,
      public virtual ::CORBA::LocalObject
    {

    public:
      /// Constructor
      DDS_WaitSet_i ();

      /// Destructor
      virtual ~DDS_WaitSet_i (void);

      /**
        * Waits for DDS until the attached read/query conditions are met
        * or when a timeout occurs.
        *
        * active_conditions will contain conditions DDS has found. The
        * caller should check whether the appropriate conditions for him
        * has been set, using check_conditions
        */
      virtual ::DDS::ReturnCode_t
      wait (::DDS::ConditionSeq & active_conditions,
            const ::DDS::Duration_t & timeout);

      /**
        * Attaches a condition to the waitset. 'wait' will be waiting for
        * this condition.
        */
      virtual ::DDS::ReturnCode_t
      attach_condition (::DDS::Condition_ptr cond);

      /**
        * Detaches a condition from the waitset. 'wait' won't be waiting
        * for this condition anymore.
        */
      virtual ::DDS::ReturnCode_t
      detach_condition (::DDS::Condition_ptr cond);

      /**
        * Retrieves a list of attached conditions.
        */
      virtual ::DDS::ReturnCode_t
      get_conditions (::DDS::ConditionSeq & attached_conditions);

      /**
        * Returns the RTI waitset
        */
      DDSWaitSet * get_rti_entity (void);

      /**
        * Used in the Reader in order to check whether whether the
        * user has supplied the correct handle with the type specific
        * instance. See Reader_T::read_one_last and the DDS4CCM spec
        * for more info.
        *
        * Checks whether two DDS-handles are equal. If no error
        * occurs and the lookup_handle exists, this method will return
        * the lookup_handle.
        *
        * @todp We actually want this method to be in the DDS4CCM
        * connector but since we're not allowed to use RTI specific code
        * there it's located here.
        *
        * 'error' will be true if instance_handle != DDS_HANDLE_NIL and
        * when instance_handle != lookup_handle.
        *
        * non_existent will be true is lookup_hnd is DDS_HANDLE_NIL
        */
      ::DDS::InstanceHandle_t
      check_handle (const ::DDS::InstanceHandle_t & instance_handle,
                    const ::DDS::InstanceHandle_t & lookup_handle,
                    bool & error,
                    bool & non_existent);

      /**
        * With this method, the caller is able to find out
        * which conditions had triggered 'wait'.
        */
      bool check_condition (::DDS::ReadCondition_ptr rc,
                            ::DDS::QueryCondition_ptr qc,
                            ::DDS::Condition_ptr condition);

      /**
        * Creates the RTI waitset.
        */
      void init (void);

    private:
      DDSWaitSet * rti_entity_;

      DDSWaitSet * rti_entity ();

      /**
        * Converts the RTI condition(s) to the DDS4CCM condition(s)
        */
      void
      convert_conditions (const DDSConditionSeq& dds_conditions,
                          ::DDS::ConditionSeq & conditions);
    };
  }
}

#endif /* WAITSET_H_ */
