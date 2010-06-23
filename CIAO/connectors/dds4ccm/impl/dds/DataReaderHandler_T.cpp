// $Id$

#include "dds4ccm/impl/dds/Utils.h"
#include "dds4ccm/impl/dds/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DataReaderHandler_T<DDS_TYPE, CCM_TYPE>::DataReaderHandler_T (
            typename CIAO::DDS4CCM::DataReaderListener_T<DDS_TYPE, CCM_TYPE>* drl,
            ::DDS::DataReader_ptr dr)
      : drl_ (drl),
        dr_ (::DDS::DataReader::_duplicate (dr))
{
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DataReaderHandler_T<DDS_TYPE, CCM_TYPE>::~DataReaderHandler_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
int
CIAO::DDS4CCM::DataReaderHandler_T<DDS_TYPE, CCM_TYPE>::handle_exception (ACE_HANDLE)
{
  try
    {
      this->drl_->on_data_available_i (this->dr_.in ());
    }
  catch (...)
    {
    }
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DataReaderStateHandler_T<DDS_TYPE, CCM_TYPE>::DataReaderStateHandler_T (
            typename CIAO::DDS4CCM::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>* drl,
            ::DDS::DataReader_ptr dr)
      : drl_ (drl),
        dr_ (::DDS::DataReader::_duplicate (dr))
{
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::DataReaderStateHandler_T<DDS_TYPE, CCM_TYPE>::~DataReaderStateHandler_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
int
CIAO::DDS4CCM::DataReaderStateHandler_T<DDS_TYPE, CCM_TYPE>::handle_exception (ACE_HANDLE)
{
  try
    {
      this->drl_->on_data_available_i (this->dr_.in ());
    }
  catch (...)
    {
    }
  return 0;
}
