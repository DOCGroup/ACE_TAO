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

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "tao/LocalObject.h"

class DDSStatusCondition;

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_StatusCondition_i :
      public virtual ::DDS::StatusCondition,
      public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      DDS_StatusCondition_i (::DDSStatusCondition *sc,
                           ::DDS::Entity_ptr entity);

      /// Destructor
      virtual ~DDS_StatusCondition_i (void);

      virtual ::CORBA::Boolean get_trigger_value (void);

      virtual ::DDS::StatusMask get_enabled_statuses (void);

      virtual ::DDS::ReturnCode_t set_enabled_statuses (::DDS::StatusMask mask);

      virtual ::DDS::Entity_ptr get_entity (void);

      ::DDSStatusCondition * get_rti_entity (void);

    private:
      ::DDSStatusCondition * rti_entity_;
      ::DDS::Entity_var entity_;
      ::DDSStatusCondition * rti_entity (void);
    };
  }
}

#endif /* DDS4CCM_STATUSCONDITION_H */
