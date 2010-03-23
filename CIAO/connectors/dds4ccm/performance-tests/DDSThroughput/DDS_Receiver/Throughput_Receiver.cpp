#include "ace/Get_Opt.h"
#include "tao/ORB_Core.h"
#include "ace/High_Res_Timer.h"
#include "Throughput_Base.h"
#include "Throughput_BaseSupport.h"
#include "Throughput_BasePlugin.h"

#include <ndds/ndds_namespace_cpp.h>

bool shutdown_flag = false;

CORBA::LongLong count_ = 0; // total count of all received messages
ACE_UINT64 interval_time_ = 0;
CORBA::LongLong interval_messages_received_ = 0;
CORBA::LongLong interval_bytes_received_ = 0;
CORBA::Long interval_data_length_ = 0;
CORBA::UShort run_ = 0;
ACE_UINT64 first_time_  = 0;
CORBA::LongLong  messages_lost_ = 0;
CORBA::Boolean logres = false;
CORBA::LongLong seq_num_ = 0;
CORBA::LongLong demand_ = 0;

ThroughputTest *instance =0;
::DDS::Topic *topic = 0;
::DDS::Topic *cmd_topic = 0;

const char *lib_name = "HelloTest_Library";
const char *cmd_prof_name = "ThroughputCmdQoS";
const char *prof_name = "ThroughputQoS";
const char *part_name = "ThroughputPartQoS";

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

/* The listener of events and command data from the middleware */
class CmdListener: public ::DDS::DataReaderListener {
public:
  void on_data_available(::DDS::DataReader *reader);
};

  void
  show_results()
  {
    if ((count_ > 0) && (interval_time_ > 0))
      {
        double per_sec = (double)1000000/ interval_time_;
        double mbps = (interval_bytes_received_* per_sec)* (8.0/1000.0/1000.0);

        if(run_ == 1)
          {
             ACE_DEBUG((LM_DEBUG,
                      " bytes, demand, samples,sample/s, Mbit/s,lost samples\n"
                      "------,-------,-------,--------,-------,------------\n"
                      "%6u,%7q,%7q,%7.1f,%7.1f,%7q\n",
                      interval_data_length_,
                      demand_,
                      interval_messages_received_,
                      interval_messages_received_* per_sec,
                      mbps,
                      messages_lost_));
          }
        else
          {
            ACE_DEBUG((LM_DEBUG,"%6u,%7q,%7q,%7.1f,%7.1f,%7q\n",
                           interval_data_length_,
                           demand_,
                           interval_messages_received_,
                           interval_messages_received_* per_sec,
                           mbps,
                           messages_lost_));
          }
      }
  }

  void
  reset_results()
  {
    interval_messages_received_ = 0;
    interval_bytes_received_ = 0;
    interval_time_=0;
    seq_num_ = 0;
    messages_lost_= 0;
  }

  void
  handle_run(ThroughputCommand & an_instance)
  {
     if( an_instance.command == THROUGHPUT_COMMAND_START)
       {
         logres = true;
         reset_results();
         interval_data_length_ = an_instance.data_length;
         demand_ = an_instance.current_publisher_effort;
         ACE_High_Res_Timer::gettimeofday_hr ().to_usec (first_time_);
      }
    if( an_instance.command == THROUGHPUT_COMMAND_COMPLETE)
     {
       logres = false;
       ACE_UINT64 last_time;
       ACE_High_Res_Timer::gettimeofday_hr ().to_usec (last_time);
       interval_time_ =  (last_time  - first_time_);
       ++run_;
       show_results();
       if(an_instance.current_publisher_effort ==
          an_instance.final_publisher_effort)
         {
           shutdown_flag = true;
         }
     }
  }

  void
  record_data (ThroughputTest & an_instance)
  {
    ++count_; // total count of all received messages
    if(logres == true)
      {
        ++interval_messages_received_;
        interval_bytes_received_ += interval_data_length_;
        if (an_instance.seq_num != seq_num_)
        {
            ++messages_lost_;
            /* Reset sequence number */
            seq_num_ = an_instance.seq_num;
          }
        ++seq_num_;
      }
  }

  int ACE_TMAIN(int argc, ACE_TCHAR** argv)
  {
    ::DDS::ReturnCode_t retcode;
    HelloListener listener;
    CmdListener cmd_listener;
    int main_result = 1; /* error by default */
    ::DDS::DataReader *data_reader = 0;
    ::DDS::DataReader *cmd_data_reader = 0;
    const char * type_name_cmd = 0;
    const char * type_name = 0;
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
    type_name = ThroughputTestTypeSupport::get_type_name();
    retcode = ThroughputTestTypeSupport::register_type(participant, type_name);
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
    /* Register type before creating command topic */
    type_name_cmd = ThroughputCommandTypeSupport::get_type_name();
    retcode = ThroughputCommandTypeSupport::register_type(
        participant, type_name_cmd);
    if (retcode != DDS_RETCODE_OK)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Unable to register command topic type.\n")));
        goto clean_exit;
      }
    /* Create the topic "Hello, World" for the String type */
    cmd_topic = participant->create_topic(
                                      "Command data",           /* Topic name*/
                                      type_name_cmd,            /* Type name */
                                      DDS_TOPIC_QOS_DEFAULT,    /* Topic QoS */
                                      0,                        /* Listener  */
                                      DDS_STATUS_MASK_NONE);
    if (!cmd_topic)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create cmd_topic.\n")));
        goto clean_exit;
      }
    /* Create the data reader using the default publisher */
    data_reader = participant->create_datareader_with_profile(
                                                 topic,
                                                 lib_name,
                                                 prof_name,    /* QoS */
                                                 &listener,    /* Listener */
                                                 DDS_DATA_AVAILABLE_STATUS);
    /* Create the command data reader using the default publisher */
    cmd_data_reader = participant->create_datareader_with_profile(
                                                 cmd_topic,
                                                 lib_name,
                                                 cmd_prof_name,  /* QoS */
                                                 &cmd_listener,  /* Listener */
                                                 DDS_DATA_AVAILABLE_STATUS);
    if (!data_reader || !cmd_data_reader)
      {
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create data reader.\n")));
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
     * specific data reader for the type "ThroughputTestDataReader"
     */
    ThroughputTestDataReader * test_reader =
                             ThroughputTestDataReader::narrow(reader);
    ThroughputTest *instance = new ThroughputTest;
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
             record_data(*instance);
           }
        }
  }
 /* This method gets called back by DDS when one or more data samples
 *  have beenreceived.
 */
  void CmdListener::on_data_available(::DDS::DataReader *reader)
  {
    ThroughputCommandDataReader * cmd_reader =
                              ThroughputCommandDataReader::narrow(reader);
    ThroughputCommand *instance = new ThroughputCommand;

    if (!cmd_reader)
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
        ::DDS::ReturnCode_t retcode = cmd_reader->take_next_sample(
                                                                    *instance,
                                                                    info);
        if (retcode == DDS_RETCODE_NO_DATA)
          {
            /* No more samples */
            break;
          }
        else if (retcode != DDS_RETCODE_OK)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("Unable to take data from data reader,"
                                  " error %d.\n"), retcode));
            return;
          }
        if (info.valid_data)
          {
            handle_run(*instance);
          }
       }
  }
