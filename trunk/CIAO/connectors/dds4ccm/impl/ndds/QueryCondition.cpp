// $Id$

#include "QueryCondition.h"
#include "Subscriber.h"
#include "DataReader.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_QueryCondition_i::RTI_QueryCondition_i ()
        : impl_ (0)
      {
      }

      // Implementation skeleton destructor
      RTI_QueryCondition_i::~RTI_QueryCondition_i (void)
      {
      }

      ::CORBA::Boolean
      RTI_QueryCondition_i::get_trigger_value (void)
      {
        return this->impl ()->get_trigger_value ();
      }

      ::DDS::SampleStateMask
      RTI_QueryCondition_i::get_sample_state_mask (void)
      {
        return this->impl ()->get_sample_state_mask ();
      }

      ::DDS::ViewStateMask
      RTI_QueryCondition_i::get_view_state_mask (void)
      {
        return this->impl ()->get_view_state_mask ();
      }

      ::DDS::InstanceStateMask
      RTI_QueryCondition_i::get_instance_state_mask (void)
      {
        return this->impl ()->get_instance_state_mask ();
      }

      ::DDS::DataReader_ptr
      RTI_QueryCondition_i::get_datareader (void)
      {
        ::DDSDataReader* reader = this->impl ()->get_datareader ();
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i ();
        RTI_DataReader_i *rti_dr = dynamic_cast < RTI_DataReader_i *>(dds_reader.in ());
        rti_dr->set_impl (reader);

        return dds_reader._retn ();
      }

      char *
      RTI_QueryCondition_i::get_query_expression (void)
      {
        return ::CORBA::string_dup (this->impl ()->get_query_expression ());
      }

      ::DDS::ReturnCode_t
      RTI_QueryCondition_i::get_query_parameters (
        ::DDS::StringSeq & /*query_parameters*/)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::ReturnCode_t
      RTI_QueryCondition_i::set_query_parameters (
        const ::DDS::StringSeq & /*query_parameters*/)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      DDSQueryCondition *
      RTI_QueryCondition_i::get_impl (void)
      {
        return this->impl_;
      }

      void
      RTI_QueryCondition_i::set_impl (DDSQueryCondition * dw)
      {
        this->impl_ = dw;
      }

      DDSQueryCondition *
      RTI_QueryCondition_i::impl (void)
      {
        if (!this->impl_)
          {
            throw ::CORBA::BAD_INV_ORDER ();
          }
        return this->impl_;
      }

    }
  }
}

