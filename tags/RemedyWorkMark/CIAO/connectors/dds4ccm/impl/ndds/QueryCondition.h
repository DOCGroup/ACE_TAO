/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef QUERYCONDITION_T_H_
#define QUERYCONDITION_T_H_

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "tao/LocalObject.h"

class DDSQueryCondition;

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_QueryCondition_i :
      public virtual ::DDS::QueryCondition,
      public virtual ::CORBA::LocalObject
    {
    public:
      /// Constructor
      DDS_QueryCondition_i (::DDSQueryCondition * qc,
                          ::DDS::DataReader_ptr dr);

      /// Destructor
      virtual ~DDS_QueryCondition_i (void);

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

      ::DDSQueryCondition * get_rti_entity (void);

      void set_rti_entity (::DDSQueryCondition *entity);

    private:
      ::DDSQueryCondition * rti_entity_;
      ::DDS::DataReader_var dr_;

      ::DDSQueryCondition * rti_entity ();
    };
  }
}

#endif /* QUERYCONDITION_T_H_ */
