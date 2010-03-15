/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_STATUSCONDITION_H
#define DDS4CCM_STATUSCONDITION_H

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/dds/dds4ccm_dds_impl_export.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_NDDS==1)
# include "ndds/ndds_cpp.h"
#endif

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::StatusCondition DDSStatusCondition;
#endif

#include "tao/LocalObject.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_IMPL_Export CCM_DDS_StatusCondition_i :
      public virtual ::DDS::CCM_StatusCondition,
      public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      CCM_DDS_StatusCondition_i (DDSStatusCondition *sc);

      /// Destructor
      virtual ~CCM_DDS_StatusCondition_i (void);

      virtual ::CORBA::Boolean get_trigger_value (void);

      virtual ::DDS::StatusMask get_enabled_statuses (void);

      virtual ::DDS::ReturnCode_t set_enabled_statuses (::DDS::StatusMask mask);

      virtual ::DDS::Entity_ptr get_entity (void);

      DDSStatusCondition * get_impl (void);

      void set_impl (DDSStatusCondition * sc);

    private:
      DDSStatusCondition * impl_;
      DDSStatusCondition * impl (void);
    };
  }
}

#endif /* DDS4CCM_STATUSCONDITION_H */
