// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"

template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::RTI::DataReaderHandler_T<DDS_TYPE, CCM_TYPE>::DataReaderHandler_T (
typename CCM_TYPE::rawlistener_type::_ptr_type listen,
typename DDS_TYPE::data_reader * reader)
      : listener_ (CCM_TYPE::rawlistener_type::_duplicate (listen)),
        reader_ (reader)
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
  // Loop until there are messages available in the queue 
  for(;;) 
    {
      typename DDS_TYPE::value_type instance;
      ::DDS_SampleInfo sampleinfo;
      ::DDS::ReturnCode_t const result  = this->reader_->take_next_sample(instance,
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
          ::CCM_DDS::ReadInfo empty;
          listener_->on_data (instance, empty);
        }
    }
  return 0;
}

// Implementation skeleton constructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::RTI::DataReaderListener_T<DDS_TYPE, CCM_TYPE>::DataReaderListener_T (
      typename CCM_TYPE::context_type::_ptr_type context,
      ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> &enabled)
      : context_ (CCM_TYPE::context_type::_duplicate (context)),
        enable_ (enabled)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::DataReaderListener_T::DataReaderListener_T");
  this->info_out_portstatus_ = this->context_->get_connection_info_out_status ();
  this->info_out_connector_status_ = this->context_->get_connection_error_listener ();
  this->listener_ = this->context_->get_connection_info_out_listener ();
}

// Implementation skeleton destructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::RTI::DataReaderListener_T<DDS_TYPE, CCM_TYPE>::~DataReaderListener_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::DataReaderListener_T::~DataReaderListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<DDS_TYPE, CCM_TYPE>::on_data_available(::DDS::DataReader *rdr)
{
  if (!this->enable_.value ())
    return;

  ::CIAO::DDS4CCM::RTI::RTI_DataReader_i* rd = 
      dynamic_cast < ::CIAO::DDS4CCM::RTI::RTI_DataReader_i*>(rdr);
  typename DDS_TYPE::data_reader * reader = 
      dynamic_cast< typename DDS_TYPE::data_reader * > ((rd->get_datareader ()));
  
  if (!reader) {
    /* In this specific case, this will never fail */
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("DataReaderListener_T::narrow failed.\n")));
    return;
  }
  DataReaderHandler_T<DDS_TYPE, CCM_TYPE>* rh = 
      new  DataReaderHandler_T<DDS_TYPE, CCM_TYPE>(this->listener_, reader);
  this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->notify (rh);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<DDS_TYPE, CCM_TYPE>::on_requested_deadline_missed (
                                              ::DDS::DataReader_ptr the_reader,
                                               const ::DDS::RequestedDeadlineMissedStatus & status)
{
  this->info_out_portstatus_->on_requested_deadline_missed (the_reader, status);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<DDS_TYPE, CCM_TYPE>::on_sample_lost (
                                  ::DDS::DataReader_ptr the_reader,
                                  const ::DDS::SampleLostStatus & status)
{
  this->info_out_portstatus_->on_sample_lost (the_reader, status);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<DDS_TYPE, CCM_TYPE>::on_requested_incompatible_qos (
                                ::DDS::DataReader_ptr the_reader,
                                const ::DDS::RequestedIncompatibleQosStatus & status)
{
  this->info_out_connector_status_->on_requested_incompatible_qos (the_reader, status);
}
