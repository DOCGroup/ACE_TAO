/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef READCONDITION_T_H_
#define READCONDITION_T_H_

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "dds4ccm/idl/ndds/ndds_dcpsC.h"
#include "tao/LocalObject.h"

class DDSReadCondition;
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_ReadCondition_i :
      public virtual ::DDS::ReadCondition,
      public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      DDS_ReadCondition_i (::DDSReadCondition * rc,
                         ::DDS::DataReader_ptr dr);

      /// Destructor
      virtual ~DDS_ReadCondition_i (void);

      virtual ::CORBA::Boolean get_trigger_value (void);

      virtual ::DDS::SampleStateMask get_sample_state_mask (void);

      virtual ::DDS::ViewStateMask get_view_state_mask (void);

      virtual ::DDS::InstanceStateMask get_instance_state_mask (void);

      virtual ::DDS::DataReader_ptr get_datareader (void);

      ::DDSReadCondition * get_rti_entity (void);

      void set_rti_entity (::DDSReadCondition* entity);

    private:
      ::DDSReadCondition *rti_entity_;
      ::DDS::DataReader_var dr_;

      ::DDSReadCondition * rti_entity (void);
    };
  }
}

#endif /* READCONDITION_T_H_ */
