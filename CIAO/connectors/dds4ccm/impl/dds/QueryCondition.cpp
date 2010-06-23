// $Id$

#include "QueryCondition.h"
#include "Subscriber.h"

#include "ndds/StringSeq.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/dds/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_QueryCondition_i<DDS_TYPE, CCM_TYPE>::CCM_DDS_QueryCondition_i (DDSQueryCondition * qc)
      : impl_ (qc)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    CCM_DDS_QueryCondition_i<DDS_TYPE, CCM_TYPE>::~CCM_DDS_QueryCondition_i (void)
    {
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::CORBA::Boolean
    CCM_DDS_QueryCondition_i<DDS_TYPE, CCM_TYPE>::get_trigger_value (void)
    {
      return this->impl ()->get_trigger_value ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::SampleStateMask
    CCM_DDS_QueryCondition_i<DDS_TYPE, CCM_TYPE>::get_sample_state_mask (void)
    {
      return this->impl ()->get_sample_state_mask ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ViewStateMask
    CCM_DDS_QueryCondition_i<DDS_TYPE, CCM_TYPE>::get_view_state_mask (void)
    {
      return this->impl ()->get_view_state_mask ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::InstanceStateMask
    CCM_DDS_QueryCondition_i<DDS_TYPE, CCM_TYPE>::get_instance_state_mask (void)
    {
      return this->impl ()->get_instance_state_mask ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::DataReader_ptr
    CCM_DDS_QueryCondition_i<DDS_TYPE, CCM_TYPE>::get_datareader (void)
    {
      ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
#if (CIAO_DDS4CCM_NDDS==1)
      DDSDataReader* reader = this->impl ()->get_datareader ();
      if (reader)
        {
          ACE_NEW_THROW_EX (dds_reader,
                            DataReader_type (reader),
                            CORBA::NO_MEMORY ());
        }
#else
      ::DDS::DataReader_var reader = this->impl ()->get_datareader ();
      if (! ::CORBA::is_nil (reader.in ()))
        {
          ACE_NEW_THROW_EX (dds_reader,
                            DataReader_type (reader.in ()),
                            CORBA::NO_MEMORY ());
        }
#endif
      return dds_reader._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    char *
    CCM_DDS_QueryCondition_i<DDS_TYPE, CCM_TYPE>::get_query_expression (void)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      return ::CORBA::string_dup (this->impl ()->get_query_expression ());
#else
      return this->impl ()->get_query_expression ();
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_QueryCondition_i<DDS_TYPE, CCM_TYPE>::get_query_parameters (
      ::DDS::StringSeq & query_parameters)
    {
#if (CIAO_DDS4CCM_NDDS==1)
      DDS_StringSeq parameters;
      ::DDS::ReturnCode_t const retval =
        this->impl ()->get_query_parameters (parameters);
      query_parameters <<= parameters;
      return retval;
#else
      return this->impl ()->get_query_parameters (query_parameters);
#endif
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_QueryCondition_i<DDS_TYPE, CCM_TYPE>::set_query_parameters (
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

    template <typename DDS_TYPE, typename CCM_TYPE>
    DDSQueryCondition *
    CCM_DDS_QueryCondition_i<DDS_TYPE, CCM_TYPE>::get_impl (void)
    {
      return this->impl_;
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    void
    CCM_DDS_QueryCondition_i<DDS_TYPE, CCM_TYPE>::set_impl (DDSQueryCondition * dw)
    {
      this->impl_ = dw;
    }

    template <typename DDS_TYPE, typename CCM_TYPE>
    DDSQueryCondition *
    CCM_DDS_QueryCondition_i<DDS_TYPE, CCM_TYPE>::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}

