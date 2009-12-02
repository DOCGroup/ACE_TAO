// $Id$

#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"


template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::RTI::DataReaderHandler_T<DDS_TYPE, CCM_TYPE>::DataReaderHandler_T (
            typename CCM_TYPE::listener_type::_ptr_type listener,
            typename DDS_TYPE::data_reader * reader)
      : reader_ (reader),
        listener_ (CCM_TYPE::listener_type::_duplicate (listener))
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
      // Loop until there are messages available in the queue
      for(;;)
        {
          typename DDS_TYPE::value_type instance;
          ::DDS_SampleInfo sampleinfo;
          ::DDS::ReturnCode_t const result = this->reader_->take_next_sample(instance,
                                                                      sampleinfo);
          if (result == DDS_RETCODE_NO_DATA)
              break;
          else if (result != DDS_RETCODE_OK)
            {
              CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Unable to take data from data reader, error %d.\n"), result));
              return 1;
            }
          if (sampleinfo.valid_data)
            {
              CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("DataReaderHandler_T : found valid data\n")));
              ::CCM_DDS::ReadInfo readinfo;
              readinfo <<= sampleinfo;
              listener_->on_one_data (instance, readinfo);
            }
        }
    }
  catch (...)
    {
    }
  return 0;
}
