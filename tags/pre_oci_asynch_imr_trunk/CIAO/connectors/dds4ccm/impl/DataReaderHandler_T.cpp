// $Id$

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename LISTENER>
CIAO::DDS4CCM::DataReaderHandler_T<LISTENER>::DataReaderHandler_T (
            LISTENER* drl,
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

template <typename LISTENER>
CIAO::DDS4CCM::DataReaderHandler_T<LISTENER>::~DataReaderHandler_T (void)
{
  this->drl_->_remove_ref ();
}

template <typename LISTENER>
int
CIAO::DDS4CCM::DataReaderHandler_T<LISTENER>::handle_exception (ACE_HANDLE)
{
  try
    {
      this->drl_->on_data_available_i (this->dr_.in ());
    }
  catch (const ::CORBA::BAD_INV_ORDER& ex)
    {
      DDS4CCM_PRINT_DEBUG_CORBA_EXCEPTION (
                              DDS4CCM_LOG_LEVEL_ACTION,
                              ex,
                              "DataReaderHandler_T::handle_exception");
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
