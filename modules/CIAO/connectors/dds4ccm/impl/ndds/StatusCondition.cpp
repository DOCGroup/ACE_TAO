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
      // Implementation skeleton constructor
      RTI_StatusCondition_i::RTI_StatusCondition_i (DDSStatusCondition *sc)
        : sc_ (sc)
      {
      }

      // Implementation skeleton destructor
      RTI_StatusCondition_i::~RTI_StatusCondition_i (void)
      {
      }

      ::CORBA::Boolean
      RTI_StatusCondition_i::get_trigger_value (void)
      {
        return this->sc_->get_trigger_value ();
      }

      ::DDS::StatusMask
      RTI_StatusCondition_i::get_enabled_statuses (void)
      {
        return this->sc_->get_enabled_statuses ();
      }

      ::DDS::ReturnCode_t
      RTI_StatusCondition_i::set_enabled_statuses (
        ::DDS::StatusMask mask)
      {
        return this->sc_->set_enabled_statuses (mask);
      }

      ::DDS::Entity_ptr
      RTI_StatusCondition_i::get_entity (void)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      DDSStatusCondition *
      RTI_StatusCondition_i::get_status_condition (void)
      {
        return this->sc_;
      }
    }
  }
}

