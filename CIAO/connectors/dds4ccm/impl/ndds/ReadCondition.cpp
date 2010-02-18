// $Id$

#include "ReadCondition.h"
#include "DataReader.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      RTI_ReadCondition_i::RTI_ReadCondition_i (void)
        : impl_ (0)
      {
      }

      RTI_ReadCondition_i::~RTI_ReadCondition_i (void)
      {
      }

      ::CORBA::Boolean
      RTI_ReadCondition_i::get_trigger_value (void)
      {
        return this->impl ()->get_trigger_value ();
      }

      ::DDS::SampleStateMask
      RTI_ReadCondition_i::get_sample_state_mask (void)
      {
        return this->impl ()->get_sample_state_mask ();
      }

      ::DDS::ViewStateMask
      RTI_ReadCondition_i::get_view_state_mask (void)
      {
        return this->impl ()->get_view_state_mask ();
      }

      ::DDS::InstanceStateMask
      RTI_ReadCondition_i::get_instance_state_mask (void)
      {
        return this->impl ()->get_instance_state_mask ();
      }

      ::DDS::DataReader_ptr
      RTI_ReadCondition_i::get_datareader (void)
      {
        ::DDS::DataReader_var retval = ::DDS::DataReader::_nil ();
        ACE_NEW_THROW_EX (retval,
                          RTI_DataReader_i (),
                          CORBA::NO_MEMORY ());
        DDSDataReader* rd = this->impl ()->get_datareader ();
        RTI_DataReader_i *rti_dr = dynamic_cast < RTI_DataReader_i *>(retval.in ());
        rti_dr->set_impl (rd);

        return retval._retn ();
      }

      DDSReadCondition *
      RTI_ReadCondition_i::get_impl (void)
      {
        return this->impl_;
      }

      void
      RTI_ReadCondition_i::set_impl (DDSReadCondition * rc)
      {
        this->impl_ = rc;
      }

      DDSReadCondition *
      RTI_ReadCondition_i::impl (void)
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

