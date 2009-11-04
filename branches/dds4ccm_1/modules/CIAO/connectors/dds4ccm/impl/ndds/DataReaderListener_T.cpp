// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"


template <typename NDDS_TYPE, typename RAWLISTENER>
CIAO::DDS4CCM::RTI::DataReaderHandler_T<NDDS_TYPE, RAWLISTENER>::DataReaderHandler_T (
typename RAWLISTENER::_ptr_type listen,
typename NDDS_TYPE::data_reader * reader)
      : listener_ (RAWLISTENER::_duplicate (listen)),
        reader_ (reader)
{
}

template <typename NDDS_TYPE, typename RAWLISTENER>
CIAO::DDS4CCM::RTI::DataReaderHandler_T<NDDS_TYPE, RAWLISTENER>::~DataReaderHandler_T ()
{
  printf ("############### ~DataReaderHandler_T before wait\n");
  this->wait ();
  printf ("############### ~DataReaderHandler_T after wait\n");
}

template <typename NDDS_TYPE, typename RAWLISTENER>
int
CIAO::DDS4CCM::RTI::DataReaderHandler_T<NDDS_TYPE, RAWLISTENER>::open (void *)
{
  this->reactor (&this->ar_);
  return this->activate ();
}

template <typename NDDS_TYPE, typename RAWLISTENER>
int
CIAO::DDS4CCM::RTI::DataReaderHandler_T<NDDS_TYPE, RAWLISTENER>::svc (void)
{
  //start transmitting data to the sender.
  printf ("############### svc\n");
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Waiting for events\n")));
  this->ar_.owner (ACE_Thread::self ());
  this->ar_.handle_events ();
  printf ("############### svc Start handling samples\n");
  /* Loop until there are messages available in the queue */
  for(;;) 
    {
      typename NDDS_TYPE::value_type instance;
      ::DDS_SampleInfo sampleinfo;
      ::DDS::ReturnCode_t const result  = this->reader_->take_next_sample(instance,
                                                                  sampleinfo);
      printf ("############### svc Samples taken\n");
      if (result == DDS_RETCODE_NO_DATA) 
        {
          printf ("no more samples\n");
          break;
        } 
      else if (result != DDS_RETCODE_OK) 
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to take data from data reader, error %d.\n"), result));
          return 1;
        }
      if (sampleinfo.valid_data) 
        {
          printf ("got valid data\n");
          ::CCM_DDS::ReadInfo empty;
          listener_->on_data (instance, empty);
        }
    }
  printf ("############### svc END LOOP\n");
  return 0;
}

// Implementation skeleton constructor
template <typename NDDS_TYPE, typename RAWLISTENER, typename PORTSTATUSLISTENER>
CIAO::DDS4CCM::RTI::DataReaderListener_T<NDDS_TYPE, RAWLISTENER, PORTSTATUSLISTENER>::DataReaderListener_T (
typename RAWLISTENER::_ptr_type listen, typename PORTSTATUSLISTENER::_ptr_type psl, ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> &enabled)
      : listener_ (RAWLISTENER::_duplicate (listen)),
        portlistener_ (PORTSTATUSLISTENER::_duplicate (psl)),
        enable_ (enabled)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::DataReaderListener_T::DataReaderListener_T");
}

// Implementation skeleton destructor
template <typename NDDS_TYPE, typename RAWLISTENER, typename PORTSTATUSLISTENER>
CIAO::DDS4CCM::RTI::DataReaderListener_T<NDDS_TYPE, RAWLISTENER, PORTSTATUSLISTENER>::~DataReaderListener_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::DataReaderListener_T::~DataReaderListener_T");
}

template <typename NDDS_TYPE, typename RAWLISTENER, typename PORTSTATUSLISTENER>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<NDDS_TYPE, RAWLISTENER, PORTSTATUSLISTENER>::on_data_available( ::DDS::DataReader *rdr)
{
  printf ("*** on data available\n");
  if (!this->enable_.value ())
    return;

  ::CIAO::DDS4CCM::RTI::RTI_DataReader_i* rd = dynamic_cast < ::CIAO::DDS4CCM::RTI::RTI_DataReader_i*>(rdr);
  typename NDDS_TYPE::data_reader * reader = dynamic_cast< typename NDDS_TYPE::data_reader * > ((rd->get_datareader ()));
  
  if (!reader) {
    /* In this specific case, this will never fail */
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("DataReaderListener_T::narrow failed.\n")));
    return;
  }
  printf ("############### Create DataReaderHandler_T\n");
  DataReaderHandler_T<NDDS_TYPE, RAWLISTENER>* rh = new  DataReaderHandler_T<NDDS_TYPE, RAWLISTENER>(this->listener_, reader);
  if (rh->open () == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to open reactor thread.\n")));
      return;
    }
  printf ("############### Notify reactor\n");
  rh->reactor ()->notify ();
  printf ("############### Reactor notified\n");
}

template <typename NDDS_TYPE, typename RAWLISTENER, typename PORTSTATUSLISTENER>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<NDDS_TYPE, RAWLISTENER, PORTSTATUSLISTENER>::on_requested_deadline_missed (::DDS::DataReader_ptr the_reader,
                                               const ::DDS::RequestedDeadlineMissedStatus & status)
{
  this->portlistener_->on_requested_deadline_missed (the_reader, status);
}

template <typename NDDS_TYPE, typename RAWLISTENER, typename PORTSTATUSLISTENER>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<NDDS_TYPE, RAWLISTENER, PORTSTATUSLISTENER>::on_sample_lost (::DDS::DataReader_ptr the_reader,
                                 const ::DDS::SampleLostStatus & status)
{
  this->portlistener_->on_sample_lost (the_reader, status);
}

