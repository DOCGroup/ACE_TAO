/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDSWAITSET_H_
#define DDSWAITSET_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"

#include "ndds/ndds_cpp.h"

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

      DDSWaitSet * get_impl (void);

      void init (void);

    private:
      DDSWaitSet * impl_;

      DDSWaitSet * impl ();

      void
      convert_conditions (const DDSConditionSeq& dds_conditions,
                          ::DDS::ConditionSeq & conditions);
    };
  }
}

#endif /* DDSWAITSET_H_ */
