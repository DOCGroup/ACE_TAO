// -*- C++ -*-

#include "DDSTopicDataReaderListenerImpl.h"
#include "DDSTopicTypeSupportC.h"
#include "DDSTopicTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <ace/streams.h>


// Implementation skeleton constructor
DDSTopicDataReaderListenerImpl::DDSTopicDataReaderListenerImpl()
{
}

// Implementation skeleton destructor
DDSTopicDataReaderListenerImpl::~DDSTopicDataReaderListenerImpl ()
{
}

void DDSTopicDataReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader)
  throw (CORBA::SystemException)
{
  try {
    DDSTopicDataReader_var topic_reader = DDSTopicDataReader::_narrow(reader);
    if (CORBA::is_nil (topic_reader.in ())) {
      cerr << "DDSTopicDataReaderListenerImpl::on_data_available: _narrow failed." << endl;
      ACE_OS::exit(1);
    }

    DDSTopic topic;
    DDS::SampleInfo si ;
    DDS::ReturnCode_t status = topic_reader->take_next_sample(topic, si) ;

    if (status == DDS::RETCODE_OK) {
      cout << "Topic name: " << topic.id.in() << " and seq.no.: " 
	   << topic.sequence_number << endl;
    } else if (status == DDS::RETCODE_NO_DATA) {
      cerr << "ERROR: reader received DDS::RETCODE_NO_DATA!" << endl;
    } else {
      cerr << "ERROR: read DDSTopic: Error: " <<  status << endl;
    }
  } catch (CORBA::Exception& e) {
    cerr << "Exception caught in read:" << endl << e << endl;
    ACE_OS::exit(1);
  }
}

void DDSTopicDataReaderListenerImpl::on_requested_deadline_missed (
    DDS::DataReader_ptr,
    const DDS::RequestedDeadlineMissedStatus &)
  throw (CORBA::SystemException)
{
  cerr << "DDSTopicDataReaderListenerImpl::on_requested_deadline_missed" << endl;
}

void DDSTopicDataReaderListenerImpl::on_requested_incompatible_qos (
    DDS::DataReader_ptr,
    const DDS::RequestedIncompatibleQosStatus &)
  throw (CORBA::SystemException)
{
  cerr << "DDSTopicDataReaderListenerImpl::on_requested_incompatible_qos" << endl;
}

void DDSTopicDataReaderListenerImpl::on_liveliness_changed (
    DDS::DataReader_ptr,
    const DDS::LivelinessChangedStatus &)
  throw (CORBA::SystemException)
{
  cerr << "DDSTopicDataReaderListenerImpl::on_liveliness_changed" << endl;
}

void DDSTopicDataReaderListenerImpl::on_subscription_match (
    DDS::DataReader_ptr,
    const DDS::SubscriptionMatchStatus &)
  throw (CORBA::SystemException)
{
  cerr << "DDSTopicDataReaderListenerImpl::on_subscription_match" << endl;
}

void DDSTopicDataReaderListenerImpl::on_sample_rejected(
    DDS::DataReader_ptr,
    const DDS::SampleRejectedStatus&)
  throw (CORBA::SystemException)
{
  cerr << "DDSTopicDataReaderListenerImpl::on_sample_rejected" << endl;
}

void DDSTopicDataReaderListenerImpl::on_sample_lost(
  DDS::DataReader_ptr,
  const DDS::SampleLostStatus&)
  throw (CORBA::SystemException)
{
  cerr << "DDSTopicDatabReaderListenerImpl::on_sample_lost" << endl;
}
