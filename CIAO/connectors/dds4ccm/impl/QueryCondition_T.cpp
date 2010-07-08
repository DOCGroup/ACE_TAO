// $Id$

#include "QueryCondition_T.h"
#include "Subscriber_T.h"

#include "ndds/StringSeq.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "dds4ccm/impl/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    CCM_DDS_QueryCondition_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::CCM_DDS_QueryCondition_T (DDSQueryCondition * qc)
      : impl_ (qc)
    {
      DDS4CCM_TRACE ("CCM_DDS_QueryCondition_T::CCM_DDS_QueryCondition_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    CCM_DDS_QueryCondition_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~CCM_DDS_QueryCondition_T (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_QueryCondition_T::~CCM_DDS_QueryCondition_T");
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::CORBA::Boolean
    CCM_DDS_QueryCondition_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_trigger_value (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_QueryCondition_T::get_trigger_value");

      return this->impl ()->get_trigger_value ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::SampleStateMask
    CCM_DDS_QueryCondition_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_sample_state_mask (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_QueryCondition_T::get_sample_state_mask");

      return this->impl ()->get_sample_state_mask ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ViewStateMask
    CCM_DDS_QueryCondition_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_view_state_mask (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_QueryCondition_T::get_view_state_mask");

      return this->impl ()->get_view_state_mask ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::InstanceStateMask
    CCM_DDS_QueryCondition_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_instance_state_mask (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_QueryCondition_T::get_instance_state_mask");

      return this->impl ()->get_instance_state_mask ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::DataReader_ptr
    CCM_DDS_QueryCondition_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_datareader (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_QueryCondition_T::get_datareader");

      ::DDS::DataReader_var dds_reader = ::DDS::DataReader::_nil ();
      DDSDataReader* reader = this->impl ()->get_datareader ();
      if (reader)
        {
          ACE_NEW_THROW_EX (dds_reader,
                            DataReader_type (reader),
                            CORBA::NO_MEMORY ());
        }
      return dds_reader._retn ();
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    char *
    CCM_DDS_QueryCondition_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_query_expression (void)
    {
      DDS4CCM_TRACE ("CCM_DDS_QueryCondition_T::get_query_expression");

      return ::CORBA::string_dup (this->impl ()->get_query_expression ());
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_QueryCondition_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_query_parameters (
      ::DDS::StringSeq & query_parameters)
    {
      DDS4CCM_TRACE ("CCM_DDS_QueryCondition_T::get_query_parameters");

      DDS_StringSeq parameters;
      ::DDS::ReturnCode_t const retval =
        this->impl ()->get_query_parameters (parameters);
      query_parameters <<= parameters;
      return retval;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    ::DDS::ReturnCode_t
    CCM_DDS_QueryCondition_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_query_parameters (
      const ::DDS::StringSeq & query_parameters)
    {
      DDS4CCM_TRACE ("CCM_DDS_QueryCondition_T::set_query_parameters");

      DDS_StringSeq parameters;
      parameters <<= query_parameters;

      return this->impl ()->set_query_parameters (parameters);
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    DDSQueryCondition *
    CCM_DDS_QueryCondition_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::get_impl (void)
    {
      return this->impl_;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    void
    CCM_DDS_QueryCondition_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::set_impl (DDSQueryCondition * dw)
    {
      this->impl_ = dw;
    }

    template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
    DDSQueryCondition *
    CCM_DDS_QueryCondition_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::impl (void)
    {
      if (!this->impl_)
        {
          throw ::CORBA::BAD_INV_ORDER ();
        }
      return this->impl_;
    }
  }
}

