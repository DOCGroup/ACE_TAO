// $Id$

#include "ReadCondition.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_ReadCondition_i<DDS_TYPE, CCM_TYPE>::CCM_DDS_ReadCondition_i (DDSReadCondition * rc)
      : impl_ (rc)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_ReadCondition_i<DDS_TYPE, CCM_TYPE>::~CCM_DDS_ReadCondition_i (void)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::CORBA::Boolean
    CCM_DDS_ReadCondition_i<DDS_TYPE, CCM_TYPE>::get_trigger_value (void)
    {
      return this->impl ()->get_trigger_value ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::SampleStateMask
    CCM_DDS_ReadCondition_i<DDS_TYPE, CCM_TYPE>::get_sample_state_mask (void)
    {
      return this->impl ()->get_sample_state_mask ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ViewStateMask
    CCM_DDS_ReadCondition_i<DDS_TYPE, CCM_TYPE>::get_view_state_mask (void)
    {
      return this->impl ()->get_view_state_mask ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::InstanceStateMask
    CCM_DDS_ReadCondition_i<DDS_TYPE, CCM_TYPE>::get_instance_state_mask (void)
    {
      return this->impl ()->get_instance_state_mask ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::DataReader_ptr
    CCM_DDS_ReadCondition_i<DDS_TYPE, CCM_TYPE>::get_datareader (void)
    {
      ::DDS::DataReader_var retval = ::DDS::DataReader::_nil ();
      DDSDataReader* rd = this->impl ()->get_datareader ();
      if (rd)
        {
          ACE_NEW_THROW_EX (retval,
                            DataReader_type (rd),
                            CORBA::NO_MEMORY ());
        }
      return retval._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    DDSReadCondition *
    CCM_DDS_ReadCondition_i<DDS_TYPE, CCM_TYPE>::get_impl (void)
    {
      return this->impl_;
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_ReadCondition_i<DDS_TYPE, CCM_TYPE>::set_impl (DDSReadCondition * rc)
    {
      this->impl_ = rc;
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    DDSReadCondition *
    CCM_DDS_ReadCondition_i<DDS_TYPE, CCM_TYPE>::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}

