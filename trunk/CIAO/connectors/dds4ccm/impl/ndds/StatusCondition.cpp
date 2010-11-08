// $Id$

#include "dds4ccm/impl/ndds/StatusCondition.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_StatusCondition_i::DDS_StatusCondition_i (
      ::DDSStatusCondition * sc,
      ::DDS::Entity_ptr entity) :
      rti_entity_ (sc),
      entity_ (::DDS::Entity::_duplicate (entity))
    {
    }

    DDS_StatusCondition_i::~DDS_StatusCondition_i (void)
    {
    }

    ::CORBA::Boolean
    DDS_StatusCondition_i::get_trigger_value (void)
    {
      return this->rti_entity ()->get_trigger_value ();
    }

    ::DDS::StatusMask
    DDS_StatusCondition_i::get_enabled_statuses (void)
    {
      return this->rti_entity ()->get_enabled_statuses ();
    }

    ::DDS::ReturnCode_t
    DDS_StatusCondition_i::set_enabled_statuses (
      ::DDS::StatusMask mask)
    {
      return this->rti_entity ()->set_enabled_statuses (mask);
    }

    ::DDS::Entity_ptr
    DDS_StatusCondition_i::get_entity (void)
    {
      return ::DDS::Entity::_duplicate (this->entity_.in ());
    }

    ::DDSStatusCondition *
    DDS_StatusCondition_i::get_rti_entity (void)
    {
      return this->rti_entity_;
    }

    ::DDSStatusCondition *
    DDS_StatusCondition_i::rti_entity (void)
    {
      if (!this->rti_entity_)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "DDS_StatusCondition_i::rti_entity - "
                        "Throwing BAD_INV_ORDER.\n"));
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->rti_entity_;
    }
  }
}

