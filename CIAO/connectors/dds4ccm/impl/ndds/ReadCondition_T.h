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

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"
#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DataReader_T;

    template <typename DDS_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DDS_ReadCondition_T :
      public virtual ::DDS::ReadCondition,
      public virtual ::CORBA::LocalObject
    {
    typedef DataReader_T<DDS_TYPE, VENDOR_TYPE> DataReader_type;

    public:
      /// Constructor
      DDS_ReadCondition_T (DDSReadCondition * rc);

      /// Destructor
      virtual ~DDS_ReadCondition_T (void);

      virtual ::CORBA::Boolean get_trigger_value (void);

      virtual ::DDS::SampleStateMask get_sample_state_mask (void);

      virtual ::DDS::ViewStateMask get_view_state_mask (void);

      virtual ::DDS::InstanceStateMask get_instance_state_mask (void);

      virtual ::DDS::DataReader_ptr get_datareader (void);

      DDSReadCondition * get_impl (void);

      void set_impl (DDSReadCondition * rc);

    private:
      DDSReadCondition *impl_;

      DDSReadCondition * impl (void);
    };
  }
}

#include "dds4ccm/impl/ndds/ReadCondition_T.cpp"

#endif /* READCONDITION_T_H_ */
