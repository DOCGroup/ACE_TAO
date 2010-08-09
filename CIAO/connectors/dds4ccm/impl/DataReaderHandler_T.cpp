// $Id$

#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DataReaderHandler_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::DataReaderHandler_T (
            typename CIAO::DDS4CCM::DataReaderListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>* drl,
            ::DDS::DataReader_ptr dr)
      : drl_ (drl),
        dr_ (::DDS::DataReader::_duplicate (dr))
{
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DataReaderHandler_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~DataReaderHandler_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
int
CIAO::DDS4CCM::DataReaderHandler_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::handle_exception (ACE_HANDLE)
{
  try
    {
      this->drl_->on_data_available_i (this->dr_.in ());
    }
  catch (const ::CORBA::Exception& ex)
    {
      DDS4CCM_PRINT_CORBA_EXCEPTION (
                              DDS4CCM_LOG_LEVEL_ERROR,
                              ex,
                              "DataReaderHandler_T::handle_exception");
    }
  catch (...)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
        "CIAO::DDS4CCM::DataReaderHandler_T::handle_exception - "
        "Unexpected exception caught\n"));
    }
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DataReaderStateHandler_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::DataReaderStateHandler_T (
            typename CIAO::DDS4CCM::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>* drl,
            ::DDS::DataReader_ptr dr)
      : drl_ (drl),
        dr_ (::DDS::DataReader::_duplicate (dr))
{
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DataReaderStateHandler_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~DataReaderStateHandler_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
int
CIAO::DDS4CCM::DataReaderStateHandler_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::handle_exception (ACE_HANDLE)
{
  try
    {
      this->drl_->on_data_available_i (this->dr_.in ());
    }
  catch (const ::CORBA::Exception& ex)
    {
      DDS4CCM_PRINT_CORBA_EXCEPTION (
                              DDS4CCM_LOG_LEVEL_ERROR,
                              ex,
                              "DataReaderStateHandler_T::handle_exception");
    }
  catch (...)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, CLINFO
        "CIAO::DDS4CCM::DataReaderStateHandler_T::handle_exception - "
        "Unexpected exception caught\n"));
    }
  return 0;
}
