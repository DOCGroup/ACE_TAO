// -*- C++ -*-

//=============================================================================
/**
 *  @file    Receiver_Task.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 *
 */
//=============================================================================

#include "Receiver_Task.h"
#include "DDSTopicDataReaderListenerImpl.h"
#include "DDSTopicTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/SubscriberImpl.h>
#include <dds/DCPS/transport/framework/TheTransportFactory.h>

namespace CIDL_DDSReceiverImpl
{

// constant used by this publisher for transport; 
// must match transport id in config file
const OpenDDS::DCPS::TransportIdType TRANSPORT_IMPL_ID = 1;

DDS::DomainId_t DDSTOPIC_DOMAIN_ID = 467;
const char* DDSTOPIC_TYPE = "DDS Test";
const char* DDSTEST_TOPIC = "DDSTopic";

//-----------------------------------------------------------------------------

  Receiver_Task::Receiver_Task ()
  {
  }

//-----------------------------------------------------------------------------

  void
  Receiver_Task::set_context (DDSReceiver_Context* context)
  {
    this->context_ = context;
  }

//-----------------------------------------------------------------------------

  int
  Receiver_Task::svc ()
  {
    DDS::DomainParticipantFactory_var dpf = 
      DDS::DomainParticipantFactory::_nil();
    DDS::DomainParticipant_var participant = 
      DDS::DomainParticipant::_nil();

    try {
      // Initialize, and create a DomainParticipant

      dpf = TheParticipantFactory;

      participant = dpf->create_participant(
					    DDSTOPIC_DOMAIN_ID,
					    PARTICIPANT_QOS_DEFAULT,
					    DDS::DomainParticipantListener::_nil());
      
      if (CORBA::is_nil (participant.in ())) {
	cerr << "create_participant failed." << endl;
	return 1;
      }

      // Create a subscriber for the two topics
      // (SUBSCRIBER_QOS_DEFAULT is defined in Marked_Default_Qos.h)
      DDS::Subscriber_var sub =
	participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT,
                                     DDS::SubscriberListener::_nil());
      if (CORBA::is_nil (sub.in ())) {
	cerr << "create_subscriber failed." << endl;
	return 1;
      }

      // Initialize the transport; the TRANSPORT_IMPL_ID must match the
      // value in the configuration file.
      OpenDDS::DCPS::TransportImpl_rch trans_impl =
	TheTransportFactory->create_transport_impl (TRANSPORT_IMPL_ID, 
						    OpenDDS::DCPS::AUTO_CONFIG);
      

      // Attach the subscriber to the TCP transport.
      // (almost identical to the publisher)
      OpenDDS::DCPS::SubscriberImpl* sub_impl =
	OpenDDS::DCPS::reference_to_servant< OpenDDS::DCPS::SubscriberImpl >(sub.in ());
      if (0 == sub_impl) {
	cerr << "Failed to obtain subscriber servant" << endl;
	return 1;
      }

      OpenDDS::DCPS::AttachStatus status = sub_impl->attach_transport(trans_impl.in());
      if (status != OpenDDS::DCPS::ATTACH_OK) {
	std::string status_str;
	switch (status) {
        case OpenDDS::DCPS::ATTACH_BAD_TRANSPORT:
          status_str = "ATTACH_BAD_TRANSPORT";
          break;
        case OpenDDS::DCPS::ATTACH_ERROR:
          status_str = "ATTACH_ERROR";
          break;
        case OpenDDS::DCPS::ATTACH_INCOMPATIBLE_QOS:
          status_str = "ATTACH_INCOMPATIBLE_QOS";
          break;
        default:
          status_str = "Unknown Status";
          break;
	}
	cerr << "Failed to attach to the transport. Status == "
	     << status_str.c_str() << endl;
	return 1;
      }

      // Register the Ddstopic type
      DDSTopicTypeSupport_var ddstopic_servant = new DDSTopicTypeSupportImpl();
      if (DDS::RETCODE_OK != ddstopic_servant->register_type(participant.in (),
							     DDSTOPIC_TYPE)) {
	cerr << "register_type for " << DDSTOPIC_TYPE << " failed." << endl;
	return 1;
      }
      
      // Create a topic for the Ddstopic type...
      DDS::Topic_var ddstopic_topic =
	participant->create_topic (DDSTEST_TOPIC,
				   DDSTOPIC_TYPE,
				   TOPIC_QOS_DEFAULT,
				   DDS::TopicListener::_nil());
      if (CORBA::is_nil (ddstopic_topic.in ())) {
	cerr << "create_topic for " << DDSTEST_TOPIC << " failed." << endl;
	return 1;
      }
      
      // Create DataReaders and DataReaderListeners for the
      // Quote and ExchangeEvent

      DDSTopicDataReaderListenerImpl ddstopic_listener_servant;

      DDS::DataReaderListener_var ddstopic_listener =
	OpenDDS::DCPS::servant_to_reference( &ddstopic_listener_servant );

      if (CORBA::is_nil (ddstopic_listener.in ())) {
	cerr << "DDSTopic listener is nil." << endl;
	return 1;
      }

      DDS::DataReader_var ddstopic_dr = 
	sub->create_datareader(ddstopic_topic.in (),
			       DATAREADER_QOS_DEFAULT,
			       ddstopic_listener.in ());
      
      // Wait for events from the Publisher; shut down when "close" received
      while ( !this->stopped_ ) {
	cout << "DDSTopic Reveicer  sleeping for 1 sec..." << endl;
	ACE_OS::sleep(1);
      }

    } 
    catch (CORBA::Exception& e) {
      cerr << "Exception caught in main.cpp:" << endl
	   << e << endl;
      return 1;
    }

    // Cleanup
    try {
      if (!CORBA::is_nil (participant.in ())) {
	participant->delete_contained_entities();
      }
      if (!CORBA::is_nil (dpf.in ())) {
	dpf->delete_participant(participant.in ());
      }
    } catch (CORBA::Exception& e) {
      cerr << "Exception caught in cleanup." << endl << e << endl;
      return 1;
    }
    TheTransportFactory->release();
    TheServiceParticipant->shutdown ();
    return 0;
  }
  
//-----------------------------------------------------------------------------

  void
  Receiver_Task::stop ()
  {
    this->stopped_ = true;
  }

} /* namespace CIDL_DDSReceiverImpl */

//-----------------------------------------------------------------------------
