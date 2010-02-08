// $Id$

#include "StatusCondition.h"
#include "Subscriber.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      RTI_StatusCondition_i::RTI_StatusCondition_i (void)
        : impl_ (0)
      {
      }

      RTI_StatusCondition_i::~RTI_StatusCondition_i (void)
      {
      }

      ::CORBA::Boolean
      RTI_StatusCondition_i::get_trigger_value (void)
      {
        return this->impl ()->get_trigger_value ();
      }

      ::DDS::StatusMask
      RTI_StatusCondition_i::get_enabled_statuses (void)
      {
        return this->impl ()->get_enabled_statuses ();
      }

      ::DDS::ReturnCode_t
      RTI_StatusCondition_i::set_enabled_statuses (
        ::DDS::StatusMask mask)
      {
        return this->impl ()->set_enabled_statuses (mask);
      }

      ::DDS::Entity_ptr
      RTI_StatusCondition_i::get_entity (void)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      DDSStatusCondition *
      RTI_StatusCondition_i::get_impl (void)
      {
        return this->impl_;
      }

      void
      RTI_StatusCondition_i::set_impl (DDSStatusCondition * sc)
      {
        this->impl_ = sc;
      }

      DDSStatusCondition *
      RTI_StatusCondition_i::impl (void)
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

