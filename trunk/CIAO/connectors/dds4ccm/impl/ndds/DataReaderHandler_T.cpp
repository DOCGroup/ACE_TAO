// $Id$

#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"


template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::RTI::DataReaderHandler_T<DDS_TYPE, CCM_TYPE>::DataReaderHandler_T (
            typename CIAO::DDS4CCM::RTI::DataReaderListener_T<DDS_TYPE, CCM_TYPE>* drl,
            ::DDS::DataReader_ptr dr)
      : drl_ (drl),
        dr_ (::DDS::DataReader::_duplicate (dr))
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::RTI::DataReaderHandler_T<DDS_TYPE, CCM_TYPE>::~DataReaderHandler_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
int
CIAO::DDS4CCM::RTI::DataReaderHandler_T<DDS_TYPE, CCM_TYPE>::handle_exception (ACE_HANDLE)
{
  try
    {
      drl_->on_data_available_i (dr_.in ());
    }
  catch (...)
    {
    }
  return 0;
}
