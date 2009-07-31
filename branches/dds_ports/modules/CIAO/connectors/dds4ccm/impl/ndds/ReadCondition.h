/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
 
#ifndef CIAO_RTI_READCONDITION_H
#define CIAO_RTI_READCONDITION_H

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_impl_export.h"

#include "ndds/ndds_cpp.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_ReadCondition_i :
        public virtual ::DDS::CCM_ReadCondition,
        public virtual ::CORBA::LocalObject
      {
      public:
        // Constructor
        RTI_ReadCondition_i (DDSReadCondition *rc);

        // Destructor
        virtual ~RTI_ReadCondition_i (void);

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

        DDSReadCondition * get_readcondition (void);
      private:
        DDSReadCondition *rc_;
      };
    }
  }
}

#endif /* CIAO_RTI_READCONDITION_H */
