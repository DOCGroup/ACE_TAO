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

#if (CIAO_DDS4CCM_NDDS==1)
#include "ndds/ndds_cpp.h"
#endif

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::ReadCondition DDSReadCondition;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class DataReader_T;

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    class CCM_DDS_ReadCondition_T :
      public virtual ::DDS::CCM_ReadCondition,
      public virtual ::CORBA::LocalObject
    {
    typedef DataReader_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> DataReader_type;

    public:
      /// Constructor
      CCM_DDS_ReadCondition_T (DDSReadCondition * rc);

      /// Destructor
      virtual ~CCM_DDS_ReadCondition_T (void);

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

#include "dds4ccm/impl/ReadCondition_T.cpp"

#endif /* READCONDITION_T_H_ */
