// $Id$

#include "QueryCondition.h"
#include "Subscriber.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_QueryCondition_i::RTI_QueryCondition_i (DDSQueryCondition *sc)
        : qc_ (sc)
      {
      }

      // Implementation skeleton destructor
      RTI_QueryCondition_i::~RTI_QueryCondition_i (void)
      {
      }

      ::CORBA::Boolean
      RTI_QueryCondition_i::get_trigger_value (void)
      {
        return this->qc_->get_trigger_value ();
      }

      ::DDS::SampleStateMask
      RTI_QueryCondition_i::get_sample_state_mask (void)
      {
        return this->qc_->get_sample_state_mask ();
      }

      ::DDS::ViewStateMask
      RTI_QueryCondition_i::get_view_state_mask (void)
      {
        return this->qc_->get_view_state_mask ();
      }

      ::DDS::InstanceStateMask
      RTI_QueryCondition_i::get_instance_state_mask (void)
      {
        return this->qc_->get_instance_state_mask ();
      }

      ::DDS::DataReader_ptr
      RTI_QueryCondition_i::get_datareader (void)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      char *
      RTI_QueryCondition_i::get_query_expression (void)
      {
        return CORBA::string_dup (this->qc_->get_query_expression ());
      }

      ::DDS::ReturnCode_t
      RTI_QueryCondition_i::get_query_parameters (
        ::DDS::StringSeq & query_parameters)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_QueryCondition_i::set_query_parameters (
        const ::DDS::StringSeq & query_parameters)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      DDSQueryCondition *
      RTI_QueryCondition_i::get_query_condition (void)
      {
        return this->qc_;
      }
    }
  }
}

