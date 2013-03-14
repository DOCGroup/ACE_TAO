// $Id$

#include "dds4ccm/impl/ndds/ReadCondition.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_ReadCondition_i::DDS_ReadCondition_i (
      ::DDSReadCondition * rc,
      ::DDS::DataReader_ptr dr)
      : rti_entity_ (rc),
        dr_ (::DDS::DataReader::_duplicate (dr))
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_ReadCondition_i::DDS_ReadCondition_i");
    }

    DDS_ReadCondition_i::~DDS_ReadCondition_i (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_ReadCondition_i::~DDS_ReadCondition_i");
    }

    ::CORBA::Boolean
    DDS_ReadCondition_i::get_trigger_value (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_ReadCondition_i::get_trigger_value");

      return this->rti_entity ()->get_trigger_value ();
    }

    ::DDS::SampleStateMask
    DDS_ReadCondition_i::get_sample_state_mask (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_ReadCondition_i::get_sample_state_mask");

      return this->rti_entity ()->get_sample_state_mask ();
    }

    ::DDS::ViewStateMask
    DDS_ReadCondition_i::get_view_state_mask (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_ReadCondition_i::get_view_state_mask");

      return this->rti_entity ()->get_view_state_mask ();
    }

    ::DDS::InstanceStateMask
    DDS_ReadCondition_i::get_instance_state_mask (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_ReadCondition_i::get_instance_state_mask");

      return this->rti_entity ()->get_instance_state_mask ();
    }

    ::DDS::DataReader_ptr
    DDS_ReadCondition_i::get_datareader (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_ReadCondition_i::get_datareader");

      return ::DDS::DataReader::_duplicate (this->dr_.in ());
    }

    DDSReadCondition *
    DDS_ReadCondition_i::get_rti_entity (void)
    {
      return this->rti_entity_;
    }

    void
    DDS_ReadCondition_i::set_rti_entity (DDSReadCondition *entity)
    {
      this->rti_entity_ = entity;
    }

    DDSReadCondition *
    DDS_ReadCondition_i::rti_entity (void)
    {
      if (!this->rti_entity_)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "DDS_ReadCondition_i::rti_entity - "
                        "Throwing BAD_INV_ORDER.\n"));
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->rti_entity_;
    }
  }
}

