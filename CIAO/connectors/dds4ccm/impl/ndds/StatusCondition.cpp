// $Id$

#include "StatusCondition.h"
#include "Subscriber_T.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    DDS_StatusCondition_i::DDS_StatusCondition_i (
      DDSStatusCondition * sc) :
      impl_ (sc)
    {
    }

    DDS_StatusCondition_i::~DDS_StatusCondition_i (void)
    {
    }

    ::CORBA::Boolean
    DDS_StatusCondition_i::get_trigger_value (void)
    {
      return this->impl ()->get_trigger_value ();
    }

    ::DDS::StatusMask
    DDS_StatusCondition_i::get_enabled_statuses (void)
    {
      return this->impl ()->get_enabled_statuses ();
    }

    ::DDS::ReturnCode_t
    DDS_StatusCondition_i::set_enabled_statuses (
      ::DDS::StatusMask mask)
    {
      return this->impl ()->set_enabled_statuses (mask);
    }

    ::DDS::Entity_ptr
    DDS_StatusCondition_i::get_entity (void)
    {
      throw ::CORBA::NO_IMPLEMENT ();
    }

    DDSStatusCondition *
    DDS_StatusCondition_i::get_impl (void)
    {
      return this->impl_;
    }

    void
    DDS_StatusCondition_i::set_impl (DDSStatusCondition * sc)
    {
      this->impl_ = sc;
    }

    DDSStatusCondition *
    DDS_StatusCondition_i::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}

