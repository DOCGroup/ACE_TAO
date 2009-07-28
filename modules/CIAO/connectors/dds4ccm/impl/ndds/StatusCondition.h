/**
 * @file StatusCondition.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_STATUSCONDITION_H
#define CIAO_RTI_STATUSCONDITION_H

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
      class DDS4CCM_NDDS_IMPL_Export RTI_StatusCondition_i :
        public virtual DDS::CCM_StatusCondition
      {
      public:
        // Constructor
        RTI_StatusCondition_i (DDSStatusCondition *sc);

        // Destructor
        virtual ~RTI_StatusCondition_i (void);

        virtual ::CORBA::Boolean
        get_trigger_value (void);

        virtual ::DDS::StatusMask
        get_enabled_statuses (void);

        virtual ::DDS::ReturnCode_t
        set_enabled_statuses (
          ::DDS::StatusMask mask);

        virtual ::DDS::Entity_ptr
        get_entity (void);

        DDSStatusCondition * get_status_condition (void);
      private:
        DDSStatusCondition *sc_;
      };
    }
  }
}

#endif /* CIAO_RTI_STATUSCONDITION_H */
