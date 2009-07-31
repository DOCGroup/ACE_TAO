/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
 
#ifndef CIAO_RTI_QUERYCONDITION_H
#define CIAO_RTI_QUERYCONDITION_H

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_impl_export.h"

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_QueryCondition_i :
        public virtual ::DDS::CCM_QueryCondition
      {
      public:
        // Constructor
        RTI_QueryCondition_i (DDSQueryCondition *sc);

        // Destructor
        virtual ~RTI_QueryCondition_i (void);

        virtual ::CORBA::Boolean
        get_trigger_value (void);

        virtual ::DDS::SampleStateMask
        get_sample_state_mask (void);

        virtual ::DDS::ViewStateMask
        get_view_state_mask (void);

        virtual ::DDS::InstanceStateMask
        get_instance_state_mask (void);

        virtual ::DDS::DataReader_ptr
        get_datareader (void);

        virtual char *
        get_query_expression (void);

        virtual ::DDS::ReturnCode_t
        get_query_parameters (
          ::DDS::StringSeq & query_parameters);

        virtual ::DDS::ReturnCode_t
        set_query_parameters (
          const ::DDS::StringSeq & query_parameters);

        DDSQueryCondition * get_query_condition (void);
      private:
        DDSQueryCondition *qc_;
      };
    }
  }
}

#endif /* CIAO_RTI_QUERYCONDITION_H */
