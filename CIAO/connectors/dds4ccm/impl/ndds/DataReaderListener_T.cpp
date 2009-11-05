// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"

template <typename NDDS_TYPE, typename RAWLISTENER>
CIAO::DDS4CCM::RTI::DataReaderHandler_T<NDDS_TYPE, RAWLISTENER>::DataReaderHandler_T (
typename RAWLISTENER::_ptr_type listen,
typename NDDS_TYPE::data_reader * reader)
      : listener_ (RAWLISTENER::_duplicate (listen)),
        reader_ (reader)
{
}

template <typename NDDS_TYPE, typename RAWLISTENER>
CIAO::DDS4CCM::RTI::DataReaderHandler_T<NDDS_TYPE, RAWLISTENER>::~DataReaderHandler_T (void)
{
}

template <typename NDDS_TYPE, typename RAWLISTENER>
int
CIAO::DDS4CCM::RTI::DataReaderHandler_T<NDDS_TYPE, RAWLISTENER>::handle_exception (ACE_HANDLE)
{
  // Loop until there are messages available in the queue 
  for(;;) 
    {
      typename NDDS_TYPE::value_type instance;
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
template <typename NDDS_TYPE, typename RAWLISTENER, typename PORTSTATUSLISTENER, typename CONNECTORSTATUSLISTENER>
CIAO::DDS4CCM::RTI::DataReaderListener_T<NDDS_TYPE, RAWLISTENER, PORTSTATUSLISTENER, CONNECTORSTATUSLISTENER>::DataReaderListener_T (

      typename RAWLISTENER::_ptr_type listen, 
      typename PORTSTATUSLISTENER::_ptr_type psl, 
      typename CONNECTORSTATUSLISTENER::_ptr_type csl, 
       ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> &enabled,
      CORBA::ORB_ptr orb)
      : listener_ (RAWLISTENER::_duplicate (listen)),
        portlistener_ (PORTSTATUSLISTENER::_duplicate (psl)),
        connectorstatuslistener_ (CONNECTORSTATUSLISTENER::_duplicate (csl)),
        orb_ (CORBA::ORB::_duplicate (orb)),
        enable_ (enabled)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::DataReaderListener_T::DataReaderListener_T");
}

// Implementation skeleton destructor
template <typename NDDS_TYPE, typename RAWLISTENER, typename PORTSTATUSLISTENER, typename CONNECTORSTATUSLISTENER>
CIAO::DDS4CCM::RTI::DataReaderListener_T<NDDS_TYPE, RAWLISTENER, PORTSTATUSLISTENER, CONNECTORSTATUSLISTENER>::~DataReaderListener_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::DataReaderListener_T::~DataReaderListener_T");
}

template <typename NDDS_TYPE, typename RAWLISTENER, typename PORTSTATUSLISTENER, typename CONNECTORSTATUSLISTENER>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<NDDS_TYPE, RAWLISTENER, PORTSTATUSLISTENER, CONNECTORSTATUSLISTENER>::on_data_available( 
                                            ::DDS::DataReader *rdr)
{
  if (!this->enable_.value ())
    return;

  ::CIAO::DDS4CCM::RTI::RTI_DataReader_i* rd = dynamic_cast < ::CIAO::DDS4CCM::RTI::RTI_DataReader_i*>(rdr);
  typename NDDS_TYPE::data_reader * reader = dynamic_cast< typename NDDS_TYPE::data_reader * > ((rd->get_datareader ()));
  
  if (!reader) {
    /* In this specific case, this will never fail */
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("DataReaderListener_T::narrow failed.\n")));
    return;
  }
  DataReaderHandler_T<NDDS_TYPE, RAWLISTENER>* rh = new  DataReaderHandler_T<NDDS_TYPE, RAWLISTENER>(this->listener_, reader);
  orb_->orb_core ()->reactor ()->notify (rh);
}

template <typename NDDS_TYPE, typename RAWLISTENER, typename PORTSTATUSLISTENER, typename CONNECTORSTATUSLISTENER>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<NDDS_TYPE, RAWLISTENER, PORTSTATUSLISTENER, CONNECTORSTATUSLISTENER>::on_requested_deadline_missed (
                                              ::DDS::DataReader_ptr the_reader,
                                               const ::DDS::RequestedDeadlineMissedStatus & status)
{
  this->portlistener_->on_requested_deadline_missed (the_reader, status);
}

template <typename NDDS_TYPE, typename RAWLISTENER, typename PORTSTATUSLISTENER, typename CONNECTORSTATUSLISTENER>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<NDDS_TYPE, RAWLISTENER, PORTSTATUSLISTENER, CONNECTORSTATUSLISTENER>::on_sample_lost (
                                  ::DDS::DataReader_ptr the_reader,
                                  const ::DDS::SampleLostStatus & status)
{
  this->portlistener_->on_sample_lost (the_reader, status);
}

template <typename NDDS_TYPE, typename RAWLISTENER, typename PORTSTATUSLISTENER, typename CONNECTORSTATUSLISTENER>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<NDDS_TYPE, RAWLISTENER, PORTSTATUSLISTENER, CONNECTORSTATUSLISTENER>::on_requested_incompatible_qos (
                                ::DDS::DataReader_ptr the_reader,
                                const ::DDS::RequestedIncompatibleQosStatus & status)
{
  this->connectorstatuslistener_->on_requested_incompatible_qos (the_reader, status);
}
