// $Id$

#include "ReadCondition_T.h"

template <typename DDS_TYPE>
CIAO::DDS4CCM::DDS_ReadCondition_T<DDS_TYPE>::DDS_ReadCondition_T (
  DDSReadCondition * rc)
  : impl_ (rc)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_ReadCondition_T::DDS_ReadCondition_T");
}

template <typename DDS_TYPE>
CIAO::DDS4CCM::DDS_ReadCondition_T<DDS_TYPE>::~DDS_ReadCondition_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_ReadCondition_T::~DDS_ReadCondition_T");
}

template <typename DDS_TYPE>
::CORBA::Boolean
CIAO::DDS4CCM::DDS_ReadCondition_T<DDS_TYPE>::get_trigger_value (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_ReadCondition_T::get_trigger_value");

  return this->impl ()->get_trigger_value ();
}

template <typename DDS_TYPE>
::DDS::SampleStateMask
CIAO::DDS4CCM::DDS_ReadCondition_T<DDS_TYPE>::get_sample_state_mask (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_ReadCondition_T::get_sample_state_mask");

  return this->impl ()->get_sample_state_mask ();
}

template <typename DDS_TYPE>
::DDS::ViewStateMask
CIAO::DDS4CCM::DDS_ReadCondition_T<DDS_TYPE>::get_view_state_mask (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_ReadCondition_T::get_view_state_mask");

  return this->impl ()->get_view_state_mask ();
}

template <typename DDS_TYPE>
::DDS::InstanceStateMask
CIAO::DDS4CCM::DDS_ReadCondition_T<DDS_TYPE>::get_instance_state_mask (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_ReadCondition_T::get_instance_state_mask");

  return this->impl ()->get_instance_state_mask ();
}

template <typename DDS_TYPE>
::DDS::DataReader_ptr
CIAO::DDS4CCM::DDS_ReadCondition_T<DDS_TYPE>::get_datareader (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_ReadCondition_T::get_datareader");

  ::DDS::DataReader_var retval;
  DDSDataReader* rd = this->impl ()->get_datareader ();
  if (rd)
    {
      ACE_NEW_THROW_EX (retval,
                        DataReader_type (rd),
                        ::CORBA::NO_MEMORY ());
    }
  return retval._retn ();
}

template <typename DDS_TYPE>
DDSReadCondition *
CIAO::DDS4CCM::DDS_ReadCondition_T<DDS_TYPE>::get_impl (void)
{
  return this->impl_;
}

template <typename DDS_TYPE>
void
CIAO::DDS4CCM::DDS_ReadCondition_T<DDS_TYPE>::set_impl (
  DDSReadCondition * rc)
{
  this->impl_ = rc;
}

template <typename DDS_TYPE>
DDSReadCondition *
CIAO::DDS4CCM::DDS_ReadCondition_T<DDS_TYPE>::impl (void)
{
  if (!this->impl_)
    {
      throw ::CORBA::BAD_INV_ORDER ();
    }
  return this->impl_;
}

