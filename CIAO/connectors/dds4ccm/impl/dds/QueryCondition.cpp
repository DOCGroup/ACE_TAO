// $Id$

#include "QueryCondition.h"
#include "Subscriber.h"
#include "DataReader.h"

#include "ndds/StringSeq.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    CCM_DDS_QueryCondition_i::CCM_DDS_QueryCondition_i (DDSQueryCondition * qc)
      : impl_ (qc)
    {
    }

    CCM_DDS_QueryCondition_i::~CCM_DDS_QueryCondition_i (void)
    {
    }

    ::CORBA::Boolean
    CCM_DDS_QueryCondition_i::get_trigger_value (void)
    {
      return this->impl ()->get_trigger_value ();
    }

    ::DDS::SampleStateMask
    CCM_DDS_QueryCondition_i::get_sample_state_mask (void)
    {
      return this->impl ()->get_sample_state_mask ();
    }

    ::DDS::ViewStateMask
    CCM_DDS_QueryCondition_i::get_view_state_mask (void)
    {
      return this->impl ()->get_view_state_mask ();
    }

    ::DDS::InstanceStateMask
    CCM_DDS_QueryCondition_i::get_instance_state_mask (void)
    {
      return this->impl ()->get_instance_state_mask ();
    }

    ::DDS::DataReader_ptr
    CCM_DDS_QueryCondition_i::get_datareader (void)
    {
      ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      ::DDSDataReader* reader = this->impl ()->get_datareader ();
      if (reader)
        {
          ACE_NEW_THROW_EX (dds_reader,
                            CCM_DDS_DataReader_i (reader),
                            CORBA::NO_MEMORY ());
        }
#else
      ::DDS::DataReader_var reader = this->impl ()->get_datareader ();
      if (!CORBA::is_nil (reader.in ()))
        {
          ACE_NEW_THROW_EX (dds_reader,
                            CCM_DDS_DataReader_i (reader.in ()),
                            CORBA::NO_MEMORY ());
        }
#endif
      return dds_reader._retn ();
    }

    char *
    CCM_DDS_QueryCondition_i::get_query_expression (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      return ::CORBA::string_dup (this->impl ()->get_query_expression ());
#else
      return this->impl ()->get_query_expression ();
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_QueryCondition_i::get_query_parameters (
      ::DDS::StringSeq & query_parameters)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_StringSeq parameters;
      ::DDS::ReturnCode_t const retval = this->impl ()->get_query_parameters (parameters);
      query_parameters <<= parameters;
      return retval;
#else
      return this->impl ()->get_query_parameters (query_parameters);
#endif
    }

    ::DDS::ReturnCode_t
    CCM_DDS_QueryCondition_i::set_query_parameters (
      const ::DDS::StringSeq & query_parameters)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_StringSeq parameters;
      parameters <<= query_parameters;
      return this->impl ()->set_query_parameters (parameters);
#else
      return this->impl ()->set_query_parameters (query_parameters);
#endif
    }

    DDSQueryCondition *
    CCM_DDS_QueryCondition_i::get_impl (void)
    {
      return this->impl_;
    }

    void
    CCM_DDS_QueryCondition_i::set_impl (DDSQueryCondition * dw)
    {
      this->impl_ = dw;
    }

    DDSQueryCondition *
    CCM_DDS_QueryCondition_i::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}

