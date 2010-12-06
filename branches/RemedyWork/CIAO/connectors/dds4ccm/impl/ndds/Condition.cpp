// $Id$

#include "ndds/ndds_cpp.h"
#include "dds4ccm/idl/dds4ccm_BaseC.h"
#include "dds4ccm/impl/ndds/Condition.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_Condition_i::DDS_Condition_i (DDSCondition * condition)
      : rti_entity_ (condition)
    {
      DDS4CCM_TRACE ("DDS_Condition_i::DDS_Condition_i");
    }

    DDS_Condition_i::~DDS_Condition_i (void)
    {
      DDS4CCM_TRACE ("DDS_Condition_i::~DDS_Condition_i");
    }

    ::CORBA::Boolean
    DDS_Condition_i::get_trigger_value (void)
    {
      DDS4CCM_TRACE ("DDS_Condition_i::get_trigger_value");

      return this->rti_entity ()->get_trigger_value ();
    }

    DDSCondition *
    DDS_Condition_i::get_rti_entity (void)
    {
      return this->rti_entity_;
    }

    void
    DDS_Condition_i::set_rti_entity (DDSCondition * cond)
    {
      this->rti_entity_ = cond;
    }

    DDSCondition *
    DDS_Condition_i::rti_entity (void)
    {
      if (!this->rti_entity_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->rti_entity_;
    }
  }
}
