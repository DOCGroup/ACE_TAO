/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_QUERYCONDITION_H
#define DDS4CCM_QUERYCONDITION_H

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/dds/dds4ccm_dds_impl_export.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_NDDS==1)
#include "ndds/ndds_cpp.h"
#endif

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::QueryCondition DDSQueryCondition;
#endif

#include "tao/LocalObject.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_IMPL_Export CCM_DDS_QueryCondition_i :
      public virtual ::DDS::CCM_QueryCondition,
      public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      CCM_DDS_QueryCondition_i (DDSQueryCondition * qc);

      /// Destructor
      virtual ~CCM_DDS_QueryCondition_i (void);

      virtual ::CORBA::Boolean get_trigger_value (void);

      virtual ::DDS::SampleStateMask get_sample_state_mask (void);

      virtual ::DDS::ViewStateMask get_view_state_mask (void);

      virtual ::DDS::InstanceStateMask get_instance_state_mask (void);

      virtual ::DDS::DataReader_ptr get_datareader (void);

      virtual char *get_query_expression (void);

      virtual ::DDS::ReturnCode_t
      get_query_parameters (::DDS::StringSeq & query_parameters);

      virtual ::DDS::ReturnCode_t
      set_query_parameters (const ::DDS::StringSeq & query_parameters);

      DDSQueryCondition * get_impl (void);

      void set_impl (DDSQueryCondition * qc);

    private:
      DDSQueryCondition * impl_;

      DDSQueryCondition * impl ();
    };
  }
}

#endif /* DDS4CCM_QUERYCONDITION_H */
