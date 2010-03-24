#include "ace/Get_Opt.h"
#include "tao/ORB_Core.h"
#include "ace/High_Res_Timer.h"
#include "Latency_Base.h"
#include "Latency_BaseSupport.h"
#include "Latency_BasePlugin.h"

#include <ndds/ndds_namespace_cpp.h>

bool shutdown_flag = false;

CORBA::LongLong count_ = 0; // total count of all received messages

LatencyTest *instance =0;
::DDS::Topic *topic = 0;
::DDS::DataWriter *data_writer = 0;
LatencyTestDataWriter *test_data_writer = 0;


const char *lib_name = "HelloTest_Library";
const char *prof_name = "LatencyQoS";
const char *part_name = "LatencyPartQoS";

CORBA::UShort domain_id = 0;

  int
  parse_args (int argc, ACE_TCHAR *argv[])
  {
    ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("d:O"));
    int c;
  
    while ((c = get_opts ()) != -1)
      switch (c)
        {
       case 'd':
          domain_id  = ACE_OS::atoi (get_opts.opt_arg ());
          break;
       case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s "
                             "-d <domain_id >"
                             "\n",
                             argv [0]),
                            -1);
        }
    // Indicates sucessful parsing of the command line
    return 0;
  }

/* The listener of events and data from the middleware */
class HelloListener: public ::DDS::DataReaderListener {
public:
  void on_data_available(::DDS::DataReader *reader);
};


  void
  write_back (LatencyTest & an_instance)
  {
     ++count_;
    // Change ping , so this receiever doesn't sent this message back again
    an_instance.ping = 0L;
    try
      {
        test_data_writer->write(an_instance, ::DDS::HANDLE_NIL);
      }
     catch (const CORBA::Exception& ex)
          {
            ex._tao_print_exception ("Exception caught:");
             ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                         ACE_TEXT ("while writing ping back.\n")));
          }

  }

  int ACE_TMAIN(int argc, ACE_TCHAR** argv)
  {
    ::DDS::ReturnCode_t retcode;
    HelloListener listener;
    ::DDS::DataReader *data_reader = 0;

    int main_result = 1; /* error by default */ 
    if (parse_args (argc, argv) != 0)
      return 1;

    /* Create the domain participant on domain ID 0 */
    ::DDS::DomainParticipant *participant = 
               ::DDS::DomainParticipantFactory::get_instance()->
        create_participant_with_profile(
                           domain_id,          /* Domain ID */
                           lib_name,
                           part_name,           /* QoS */
                           0,                   /* Listener */
                           DDS_STATUS_MASK_NONE);
    if (!participant) 
      {
        ACE_ERROR ((LM_ERROR, 
                    ACE_TEXT ("Unable to create domain participant.\n")));
        goto clean_exit;
      }
    /* Register type before creating topic */
    const char * type_name = LatencyTestTypeSupport::get_type_name();
    retcode = LatencyTestTypeSupport::register_type(participant, type_name);
      if (retcode != DDS_RETCODE_OK) 
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Unable to register topic type.\n")));
          goto clean_exit;
        }
    topic = participant->create_topic(
                        "Test data",            /* Topic name*/
                        type_name,               /* Type name */
                        DDS_TOPIC_QOS_DEFAULT,   /* Topic QoS */
                        0,                       /* Listener  */
                        DDS_STATUS_MASK_NONE);
    if (!topic)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create topic.\n")));
      goto clean_exit;
    }
   
    /* Create the data reader using the default publisher */
    data_reader = participant->create_datareader_with_profile(
                                                 topic,
                                                 lib_name,
                                                 prof_name,    /* QoS */
                                                 &listener,    /* Listener */
                                                 DDS_DATA_AVAILABLE_STATUS);
                                              
    if (!data_reader )
      {
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create data reader.\n")));
         goto clean_exit;
      }
/* Create the data writer using the default publisher */
    data_writer = participant->create_datawriter_with_profile(
                                topic,
                                lib_name,
                                prof_name,             /* QoS */
                                0,                     /* Listener */
                                DDS_STATUS_MASK_NONE);
    if (!data_writer)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create data writer.\n")));
        goto clean_exit;
      }
     test_data_writer = LatencyTestDataWriter::narrow(data_writer);
    if (!test_data_writer )
      {
        ACE_ERROR ((LM_ERROR, 
                    ACE_TEXT ("DDS_StringDataWriter_narrow failed.\n")));
        goto clean_exit;
     }

    /* --- Sleep During Asynchronous Reception ---------------------------- */

    /* This thread sleeps forever. When a sample is received, RTI Data
     * Distribution Service will call the on_data_available_callback function.
    */
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ready to read data.\n")));
    for (;;) 
    {
      ACE_OS::sleep (1);
      if(shutdown_flag)
        {
          break;
        }
    }

    /* --- Clean Up ------------------------------------------------------- */

    main_result = 0;
clean_exit:

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Exiting.")));
    if (count_ > 0)
      {
        ACE_DEBUG((LM_DEBUG, "SUMMARY RECEIVER:\n "
                             "Number of samples received: %u \n",
                             count_));
      }
    else
      {
        ACE_DEBUG((LM_DEBUG, "SUMMARY RECEIVER:\n "
                           "No samples received\n "));
      }
    if (participant)
      {
        retcode = participant->delete_contained_entities();
        if (retcode != DDS_RETCODE_OK)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Deletion failed.\n")));
            main_result = 1;
          }
        retcode = ::DDS::DomainParticipantFactory::get_instance()->
        delete_participant(participant);
        if (retcode != DDS_RETCODE_OK)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Deletion failed.\n")));
            main_result = 1;
          }
      }

    return main_result;
  }

  /* This method gets called back by DDS when one or more data samples
   *  have been received.
   */
  void HelloListener::on_data_available(::DDS::DataReader *reader)
  {
    /* Perform a safe type-cast from a generic data reader into a
     * specific data reader for the type "LatencyTestDataReader"
     */

     LatencyTestDataReader * test_reader = 
                             LatencyTestDataReader::narrow(reader);
    LatencyTest *instance = new LatencyTest;
    if (!test_reader)
    {
      /* In this specific case, this will never fail */
      ACE_ERROR ((LM_ERROR, 
                  ACE_TEXT ("::DDS::StringDataReader::narrow failed.\n")));
      return;
    }

    /* Loop until there are messages available in the queue */
     for(;;) 
       {
         ::DDS::SampleInfo        info;
         ::DDS::ReturnCode_t retcode = test_reader->take_next_sample(*instance,
                                                                     info);
         if (retcode == DDS_RETCODE_NO_DATA) 
           {
             /*  No more samples */
             break;
           }
         else if (retcode != DDS_RETCODE_OK) 
           {
             ACE_ERROR ((LM_ERROR, 
                         ACE_TEXT ("Unable to take data from data reader,"
                                   " error %d.\n"), 
                                   retcode));
             return;
           }
         if (info.valid_data) 
           {
               write_back(*instance);
           }
        }
  }
 
