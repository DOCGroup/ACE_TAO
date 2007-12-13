// -*- C++ -*-
// *******************************************************************
//
// (c) Copyright 2006, Object Computing, Inc.
// All Rights Reserved.
//
// *******************************************************************

#include "DDSTopicTypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/PublisherImpl.h>
#include <dds/DCPS/transport/framework/TheTransportFactory.h>
#include <ace/streams.h>
#include <ace/Get_Opt.h>

// constant used by this publisher for transport; 
// must match transport id in config file
const OpenDDS::DCPS::TransportIdType TRANSPORT_IMPL_ID = 1;

// constants for Stock Ddstopicr domain Id, types, and topic
DDS::DomainId_t DDSTOPIC_DOMAIN_ID = 467;
const char* DDSTOPIC_TYPE = "DDSTopic";
const char* DDSTEST_TOPIC = "DDSTopic Data";

static char* option_id = "Publisher Message";

static int parse_args(int argc, char* argv[])
{
  ACE_Get_Opt args (argc, argv, "i:");

  int c = 0;
  while ((c = args ()) != EOF)
    {
      switch (c)
	{
	case 'i' :
	  {
	    option_id = args.opt_arg ();
	    break;
	  }
	default:
	  {
	    std::cerr << "usage: publisher [-i topic_id]" << std::endl;
	    return 1;
	  }
	}
    }

  return 0;
}

int main (int argc, char *argv[]) {

  DDS::DomainParticipantFactory_var dpf = DDS::DomainParticipantFactory::_nil();
  DDS::DomainParticipant_var participant = DDS::DomainParticipant::_nil();

  try {
    // Initialize, and create a DomainParticipant

    dpf = TheParticipantFactoryWithArgs(argc, argv);

    if (parse_args (argc, argv) != 0)
      {
	return 1;
      }

    participant = dpf->create_participant(
      DDSTOPIC_DOMAIN_ID,
      PARTICIPANT_QOS_DEFAULT,
      DDS::DomainParticipantListener::_nil());

    if (CORBA::is_nil (participant.in ())) {
      cerr << "create_participant failed." << endl;
      ACE_OS::exit(1);
    }

    // Create a publisher for the two topics
    // (PUBLISHER_QOS_DEFAULT is defined in Marked_Default_Qos.h)
    DDS::Publisher_var pub =
      participant->create_publisher(PUBLISHER_QOS_DEFAULT,
                                    DDS::PublisherListener::_nil());
    if (CORBA::is_nil (pub.in ())) {
      cerr << "create_publisher failed." << endl;
      ACE_OS::exit(1);
    }

    // Initialize the transport; the TRANSPORT_IMPL_ID must match the
    // value in the configuration file.
    OpenDDS::DCPS::TransportImpl_rch trans_impl =
      TheTransportFactory->create_transport_impl (TRANSPORT_IMPL_ID, 
                                                  OpenDDS::DCPS::AUTO_CONFIG);

    // Attach the publisher to the TCP transport.
    OpenDDS::DCPS::PublisherImpl* pub_impl =
      OpenDDS::DCPS::reference_to_servant< OpenDDS::DCPS::PublisherImpl >(pub.in ());
    if (0 == pub_impl) {
      cerr << "Failed to obtain publisher servant" << endl;
      ACE_OS::exit(1);
    }
    OpenDDS::DCPS::AttachStatus status = pub_impl->attach_transport(trans_impl.in());
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
      ACE_OS::exit(1);
    }

    // Register the Ddstopic type
    DDSTopicTypeSupport_var ddstopic_servant = new DDSTopicTypeSupportImpl();
    if (DDS::RETCODE_OK != ddstopic_servant->register_type(participant.in (),
                                                        DDSTOPIC_TYPE)) {
      cerr << "register_type for " << DDSTOPIC_TYPE << " failed." << endl;
      ACE_OS::exit(1);
    }

    // Create a topic for the Ddstopic type...
    DDS::Topic_var ddstopic_topic =
      participant->create_topic (DDSTEST_TOPIC,
                                 DDSTOPIC_TYPE,
                                 TOPIC_QOS_DEFAULT,
                                 DDS::TopicListener::_nil());
    if (CORBA::is_nil (ddstopic_topic.in ())) {
      cerr << "create_topic for " << DDSTEST_TOPIC << " failed." << endl;
      ACE_OS::exit(1);
    }

    // Create a DataWriter for the Ddstopic topic
    DDS::DataWriter_var ddstopic_base_dw =
      pub->create_datawriter(ddstopic_topic.in (),
                             DATAWRITER_QOS_DEFAULT,
                             DDS::DataWriterListener::_nil());
    if (CORBA::is_nil (ddstopic_base_dw.in ())) {
      cerr << "create_datawriter for " << DDSTEST_TOPIC << " failed." << endl;
      ACE_OS::exit(1);
    }
    DDSTopicDataWriter_var ddstopic_dw
      = DDSTopicDataWriter::_narrow(ddstopic_base_dw.in());
    if (CORBA::is_nil (ddstopic_dw.in ())) {
      cerr << "DdstopicDataWriter could not be narrowed"<< endl;
      ACE_OS::exit(1);
    }

    // Register three stocks
    DDSTopic topic_template;
    topic_template.id = CORBA::string_dup("Asynch Message");
    DDS::InstanceHandle_t topic_handle = 
      ddstopic_dw->_cxx_register(topic_template);

    // Publish...

    ACE_Time_Value wait_time( 1, 0 );
    long i = 0;
    while(true)
      {
	DDSTopic ddstopic;
	ddstopic.id = CORBA::string_dup(option_id);
	ddstopic.sequence_number = i++;
	
	cout << "Writing DDSTopic [" << i << ", " << ddstopic.id << "]" << endl;

	DDS::ReturnCode_t ret = ddstopic_dw->write(ddstopic, topic_handle);
	if (ret != DDS::RETCODE_OK) {
	  ACE_ERROR ((LM_ERROR, ACE_TEXT("(%P|%t)ERROR: topic write returned %d.\n"), ret));
	}

	ACE_OS::sleep( wait_time );
      }

  } catch (CORBA::Exception& e) {
    cerr << "Exception caught in main.cpp:" << endl
         << e << endl;
    ACE_OS::exit(1);
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
    ACE_OS::exit(1);
  }
  TheTransportFactory->release();
  TheServiceParticipant->shutdown ();
  return 0;
}
