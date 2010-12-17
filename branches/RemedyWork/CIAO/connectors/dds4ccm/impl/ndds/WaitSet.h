/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef WAITSET_H_
#define WAITSET_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"

class DDSWaitSet;
struct DDSConditionSeq;

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_WaitSet_i :
      public virtual ::DDS::WaitSet,
      public virtual ::CORBA::LocalObject
    {

    public:
      /// Constructor
      DDS_WaitSet_i ();

      /// Destructor
      virtual ~DDS_WaitSet_i (void);

      virtual ::DDS::ReturnCode_t
      wait (::DDS::ConditionSeq & active_conditions,
            const ::DDS::Duration_t & timeout);

      virtual ::DDS::ReturnCode_t
      attach_condition (::DDS::Condition_ptr cond);

      virtual ::DDS::ReturnCode_t
      detach_condition (::DDS::Condition_ptr cond);

      virtual ::DDS::ReturnCode_t
      get_conditions (::DDS::ConditionSeq & attached_conditions);

      DDSWaitSet * get_rti_entity (void);

      ::DDS::InstanceHandle_t
      check_handle (const ::DDS::InstanceHandle_t & instance_handle,
                    const ::DDS::InstanceHandle_t & lookup_handle);

      bool check_condition (::DDS::ReadCondition_ptr rc,
                            ::DDS::QueryCondition_ptr qc,
                            ::DDS::Condition_ptr condition);


      void init (void);

    private:
      DDSWaitSet * rti_entity_;

      DDSWaitSet * rti_entity ();

      void
      convert_conditions (const DDSConditionSeq& dds_conditions,
                          ::DDS::ConditionSeq & conditions);
    };
  }
}

#endif /* WAITSET_H_ */
