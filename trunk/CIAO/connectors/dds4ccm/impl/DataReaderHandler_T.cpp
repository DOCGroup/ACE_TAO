// $Id$

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename CCM_TYPE, typename TYPED_READER, typename SEQ_TYPE>
CIAO::DDS4CCM::DataReaderHandler_T<CCM_TYPE, TYPED_READER, SEQ_TYPE>::DataReaderHandler_T (
            DataReaderListener_type* drl,
            ::DDS::DataReader_ptr dr)
      : drl_ (drl)
      , dr_ (::DDS::DataReader::_duplicate (dr))
{
  // Add a reference, this we're using the DataReaderListener_type
  // again. This should prevent preliminary destruction of the listener
  // while the reactor queue still got messages to handle.
  this->drl_->_add_ref ();
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

template <typename CCM_TYPE, typename TYPED_READER, typename SEQ_TYPE>
CIAO::DDS4CCM::DataReaderHandler_T<CCM_TYPE, TYPED_READER, SEQ_TYPE>::~DataReaderHandler_T (void)
{
  this->drl_->_remove_ref ();
}

template <typename CCM_TYPE, typename TYPED_READER, typename SEQ_TYPE>
int
CIAO::DDS4CCM::DataReaderHandler_T<CCM_TYPE, TYPED_READER, SEQ_TYPE>::handle_exception (ACE_HANDLE)
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
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
        "DataReaderHandler_T::handle_exception - "
        "Unexpected exception caught\n"));
    }
  return 0;
}

template <typename CCM_TYPE, typename TYPED_READER, typename SEQ_TYPE>
CIAO::DDS4CCM::DataReaderStateHandler_T<CCM_TYPE, TYPED_READER, SEQ_TYPE>::DataReaderStateHandler_T (
            DataReaderStateListener_type* drl,
            ::DDS::DataReader_ptr dr)
      : drl_ (drl)
      , dr_ (::DDS::DataReader::_duplicate (dr))
{
  this->drl_->_add_ref ();
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

template <typename CCM_TYPE, typename TYPED_READER, typename SEQ_TYPE>
CIAO::DDS4CCM::DataReaderStateHandler_T<CCM_TYPE, TYPED_READER, SEQ_TYPE>::~DataReaderStateHandler_T (void)
{
  this->drl_->_remove_ref ();
}

template <typename CCM_TYPE, typename TYPED_READER, typename SEQ_TYPE>
int
CIAO::DDS4CCM::DataReaderStateHandler_T<CCM_TYPE, TYPED_READER, SEQ_TYPE>::handle_exception (ACE_HANDLE)
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
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
        "DataReaderStateHandler_T::handle_exception - "
        "Unexpected exception caught\n"));
    }
  return 0;
}
