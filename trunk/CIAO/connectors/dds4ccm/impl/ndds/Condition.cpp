// $Id$

#include "Condition.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

CIAO::NDDS::DDS_Condition_i::DDS_Condition_i (DDSCondition * condition)
  : impl_ (condition)
{
  DDS4CCM_TRACE ("CIAO::NDDS::DDS_Condition_i::DDS_Condition_i");
}

CIAO::NDDS::DDS_Condition_i::~DDS_Condition_i (void)
{
  DDS4CCM_TRACE ("CIAO::NDDS::DDS_Condition_i::~DDS_Condition_i");
}

::CORBA::Boolean
CIAO::NDDS::DDS_Condition_i::get_trigger_value (void)
{
  DDS4CCM_TRACE ("CIAO::NDDS::DDS_Condition_i::get_trigger_value");

  return this->impl ()->get_trigger_value ();
}

DDSCondition *
CIAO::NDDS::DDS_Condition_i::get_rti_entity (void)
{
  return this->impl_;
}

void
CIAO::NDDS::DDS_Condition_i::set_rti_entity (DDSCondition * cond)
{
  this->impl_ = cond;
}

DDSCondition *
CIAO::NDDS::DDS_Condition_i::impl (void)
{
  if (!this->impl_)
    {
      throw ::CORBA::BAD_INV_ORDER ();
    }
  return this->impl_;
}

