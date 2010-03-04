// $Id$

#include "ReadCondition.h"
#include "DataReader.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_ReadCondition_i::CCM_DDS_ReadCondition_i (DDSReadCondition * rc)
      : impl_ (rc)
    {
    }

    CCM_DDS_ReadCondition_i::~CCM_DDS_ReadCondition_i (void)
    {
    }

    ::CORBA::Boolean
    CCM_DDS_ReadCondition_i::get_trigger_value (void)
    {
      return this->impl ()->get_trigger_value ();
    }

    ::DDS::SampleStateMask
    CCM_DDS_ReadCondition_i::get_sample_state_mask (void)
    {
      return this->impl ()->get_sample_state_mask ();
    }

    ::DDS::ViewStateMask
    CCM_DDS_ReadCondition_i::get_view_state_mask (void)
    {
      return this->impl ()->get_view_state_mask ();
    }

    ::DDS::InstanceStateMask
    CCM_DDS_ReadCondition_i::get_instance_state_mask (void)
    {
      return this->impl ()->get_instance_state_mask ();
    }

    ::DDS::DataReader_ptr
    CCM_DDS_ReadCondition_i::get_datareader (void)
    {
      ::DDS::DataReader_var retval = ::DDS::DataReader::_nil ();
      ::DDSDataReader* rd = this->impl ()->get_datareader ();
      if (rd)
        {
          ACE_NEW_THROW_EX (retval,
                            CCM_DDS_DataReader_i (rd),
                            CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }

    DDSReadCondition *
    CCM_DDS_ReadCondition_i::get_impl (void)
    {
      return this->impl_;
    }

    void
    CCM_DDS_ReadCondition_i::set_impl (DDSReadCondition * rc)
    {
      this->impl_ = rc;
    }

    DDSReadCondition *
    CCM_DDS_ReadCondition_i::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}

