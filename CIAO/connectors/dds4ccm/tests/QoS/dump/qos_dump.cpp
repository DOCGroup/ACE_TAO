// $Id$

#include "QOS_XML_Loader.h"
#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

int ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  try
    {
      DDS4CCM_debug_level = 10;

      if (argc < 1)
        {
          ACE_ERROR ((LM_ERROR,
                       ACE_TEXT ("Incorrect count of arguments. ")
                       ACE_TEXT ("Path to deployment plan has not been specified.\n")));
          return 1;
        }

      DDS4CCM::QOS_XML_Loader xml_loader;
      if (xml_loader.init (argv[1]))
        {
          ::DDS::DataWriterQos dw_qos;
          xml_loader.get_datawriter_qos (dw_qos,
                                         ACE_TEXT ("TestProfile"),
                                         "TopicName");
          //dump

          ::DDS::DataReaderQos dr_qos;
          xml_loader.get_datareader_qos (dr_qos,
                                         ACE_TEXT ("TestProfile"),
                                         "TopicName");
          //dump

          ::DDS::TopicQos tp_qos;
          xml_loader.get_topic_qos (tp_qos,
                                    ACE_TEXT ("TestProfile"),
                                    "TopicName");
          //dump

          ::DDS::PublisherQos pub_qos;
          xml_loader.get_publisher_qos (pub_qos,
                                        ACE_TEXT ("TestProfile"));
          //dump

          ::DDS::SubscriberQos sub_qos;
          xml_loader.get_subscriber_qos (sub_qos,
                                         ACE_TEXT ("TestProfile"));
          //dump

          ::DDS::DomainParticipantQos dp_qos;
          xml_loader.get_participant_qos (dp_qos,
                                          ACE_TEXT ("TestProfile"));
          //dump
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("DDS4CCM_QOS_Dump::main\n");
      return -1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unexpected exception\n")));
      return 1;
    }

  return 0;
}
