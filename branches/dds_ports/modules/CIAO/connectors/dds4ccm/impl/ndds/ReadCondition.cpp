// $Id$

#include "ReadCondition.h"
#include "DataReader.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_ReadCondition_i::RTI_ReadCondition_i (DDSReadCondition *rc)
        : rc_ (rc)
      {
      }

      // Implementation skeleton destructor
      RTI_ReadCondition_i::~RTI_ReadCondition_i (void)
      {
      }

      ::CORBA::Boolean
      RTI_ReadCondition_i::get_trigger_value (void)
      {
        return this->rc_->get_trigger_value ();
      }

      ::DDS::SampleStateMask
      RTI_ReadCondition_i::get_sample_state_mask (void)
      {
        return this->rc_->get_sample_state_mask ();
      }

      ::DDS::ViewStateMask
      RTI_ReadCondition_i::get_view_state_mask (void)
      {
        return this->rc_->get_view_state_mask ();
      }

      ::DDS::InstanceStateMask
      RTI_ReadCondition_i::get_instance_state_mask (void)
      {
        return this->rc_->get_instance_state_mask ();
      }

      ::DDS::DataReader_ptr
      RTI_ReadCondition_i::get_datareader (void)
      {
        DDSDataReader* rd = this->rc_->get_datareader ();
        ::DDS::DataReader_var retval = new RTI_DataReader_i (rd);
        return retval._retn ();
      }

      DDSReadCondition *
      RTI_ReadCondition_i::get_readcondition (void)
      {
        return this->rc_;
      }
    }
  }
}

