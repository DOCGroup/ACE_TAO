/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CONDITION_H_
#define CONDITION_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"

class DDSCondition;

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_Condition_i :
      public virtual ::DDS::Condition,
      public virtual ::CORBA::LocalObject
    {

    public:
      /// Constructor
      DDS_Condition_i (DDSCondition * condition);

      /// Destructor
      virtual ~DDS_Condition_i (void);

      virtual ::CORBA::Boolean
      get_trigger_value (void);

      DDSCondition * get_rti_entity (void);

      void set_rti_entity (DDSCondition * cond);

    private:
      DDSCondition * rti_entity_;

      DDSCondition * rti_entity ();
    };
  }
}

#endif /* CONDITION_H_ */
