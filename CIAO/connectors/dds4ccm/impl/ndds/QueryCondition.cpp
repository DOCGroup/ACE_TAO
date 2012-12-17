// $Id$

#include "dds4ccm/impl/ndds/QueryCondition.h"
#include "dds4ccm/impl/ndds/convertors/StringSeq.h"
#include "dds4ccm/impl/logger/Log_Macros.h"
#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_QueryCondition_i::DDS_QueryCondition_i (
      ::DDSQueryCondition * qc,
      ::DDS::DataReader_ptr dr)
      : rti_entity_ (qc),
        dr_ (::DDS::DataReader::_duplicate (dr))
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_QueryCondition_i::DDS_QueryCondition_i");
    }

    DDS_QueryCondition_i::~DDS_QueryCondition_i (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_QueryCondition_i::~DDS_QueryCondition_i");
    }

    ::CORBA::Boolean
    DDS_QueryCondition_i::get_trigger_value (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_QueryCondition_i::get_trigger_value");

      return this->rti_entity ()->get_trigger_value ();
    }

    ::DDS::SampleStateMask
    DDS_QueryCondition_i::get_sample_state_mask (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_QueryCondition_i::get_sample_state_mask");

      return this->rti_entity ()->get_sample_state_mask ();
    }

    ::DDS::ViewStateMask
    DDS_QueryCondition_i::get_view_state_mask (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_QueryCondition_i::get_view_state_mask");

      return this->rti_entity ()->get_view_state_mask ();
    }

    ::DDS::InstanceStateMask
    DDS_QueryCondition_i::get_instance_state_mask (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_QueryCondition_i::get_instance_state_mask");

      return this->rti_entity ()->get_instance_state_mask ();
    }

    ::DDS::DataReader_ptr
    DDS_QueryCondition_i::get_datareader (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_QueryCondition_i::get_datareader");

      return ::DDS::DataReader::_duplicate (this->dr_.in ());
    }

    char *
    DDS_QueryCondition_i::get_query_expression (void)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_QueryCondition_i::get_query_expression");

      return ::CORBA::string_dup (this->rti_entity ()->get_query_expression ());
    }

    ::DDS::ReturnCode_t
    DDS_QueryCondition_i::get_query_parameters (
      ::DDS::StringSeq & query_parameters)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_QueryCondition_i::get_query_parameters");

      DDS_StringSeq parameters;
      ::DDS::ReturnCode_t const retval =
        this->rti_entity ()->get_query_parameters (parameters);
      query_parameters <<= parameters;
      return retval;
    }

    ::DDS::ReturnCode_t
    DDS_QueryCondition_i::set_query_parameters (
      const ::DDS::StringSeq & query_parameters)
    {
      DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_QueryCondition_i::set_query_parameters");

      DDS_StringSeq parameters;
      parameters <<= query_parameters;

      return this->rti_entity ()->set_query_parameters (parameters);
    }

    DDSQueryCondition *
    DDS_QueryCondition_i::get_rti_entity (void)
    {
      return this->rti_entity_;
    }

    void
    DDS_QueryCondition_i::set_rti_entity (DDSQueryCondition *entity)
    {
      this->rti_entity_ = entity;
    }

    DDSQueryCondition *
    DDS_QueryCondition_i::rti_entity (void)
    {
      if (!this->rti_entity_)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                        "DDS_QueryCondition_i::rti_entity - "
                        "Throwing BAD_INV_ORDER.\n"));
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->rti_entity_;
    }
  }
}
