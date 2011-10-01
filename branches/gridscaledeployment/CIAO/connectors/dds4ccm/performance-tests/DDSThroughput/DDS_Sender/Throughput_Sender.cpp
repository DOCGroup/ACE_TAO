// $Id$

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Env_Value_T.h"
#include "tao/ORB_Core.h"
#include "Throughput_Base.h"
#include "Throughput_BaseSupport.h"
#include "Throughput_BasePlugin.h"

#include <ndds/ndds_namespace_cpp.h>

CORBA::UShort datalen = 1024;
CORBA::UShort recover_time = 1; // in sec
CORBA::ULong start_load = 1000;
CORBA::ULong incr_load = 1000;
CORBA::ULongLong max_load = 9000;
CORBA::ULongLong number_of_msg = 0;
ACE_UINT64 duration_run_ = 10;  // in sec

CORBA::ULongLong load = 0;
ACE_UINT64 start_time = 0;

ThroughputTest *instance = 0;
ThroughputCommand *instance_cmd = 0;
::DDS::Topic *topic = 0;
::DDS::Topic *cmd_topic = 0;
::DDS::DataWriter *data_writer = 0;
::DDS::DataWriter *cmd_writer = 0;
ThroughputTestDataWriter *test_data_writer = 0;
ThroughputCommandDataWriter *cmd_data_writer = 0;

const char *lib_name = "HelloTest_Library";
const char *cmd_prof_name = "ThroughputCmdQoS";
const char *prof_name = "ThroughputQoS";
const char *part_name = "ThroughputPartQoS";

CORBA::UShort domain_id = 0;

  int
  parse_args (int argc, ACE_TCHAR *argv[])
  {
    ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("d:l:r:s:m:i:O"));
    int c;

    while ((c = get_opts ()) != -1)
      switch (c)
        {
       case 'd':
          domain_id  = ACE_OS::atoi (get_opts.opt_arg ());
          break;
       case 'l':
          datalen  = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 'r':
          recover_time = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 's':
          start_load = ACE_OS::atol (get_opts.opt_arg ());
          break;
        case 'i':
          incr_load = ACE_OS::atol (get_opts.opt_arg ());
          break;
        case 'm':
          max_load =   ACE_OS::atol (get_opts.opt_arg ());
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s "
                             "-d <domain_id >"
                             "-l <datalen >"
                             "-r <recover_times>"
                             "-s <start_load>"
                             "-i <incr_load>"
                             "-m <max_load>"
                             "\n",
                             argv [0]),
                            -1);
        }
    // Indicates successful parsing of the command line
    return 0;
  }

  bool
  write(void)
  {
    CORBA::Boolean test_complete = false;
    load += incr_load;
    if ( load > max_load)
    {
      return false;
    }
    else
      {
        instance_cmd->command = THROUGHPUT_COMMAND_START;
        instance_cmd->data_length = datalen;
        instance_cmd->current_publisher_effort = load;
        instance_cmd->final_publisher_effort = max_load;
        try
          {
            cmd_data_writer->write(*instance_cmd, DDS_HANDLE_NIL);
          }
        catch (const ::CORBA::Exception& ex)
          {
            ex._tao_print_exception ("Exception caught:");
             ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                         ACE_TEXT ("while writing command.\n")));
          }
        test_complete = false;
        // get start time
        ACE_High_Res_Timer::gettimeofday_hr ().to_usec (start_time);
        while (!test_complete)
          {
            for (CORBA::ULongLong current_load = 0;
                 current_load < load && !test_complete;
                 ++current_load, ++instance->seq_num)
              {
                try
                  {
                     test_data_writer->write(*instance, DDS_HANDLE_NIL);
                     ++number_of_msg;
                  }
                catch (const ::CORBA::Exception&)
                  {
                     ACE_ERROR ((LM_ERROR,
                                 ACE_TEXT ("ERROR: Internal Error ")
                                 ACE_TEXT ("while updating writer "
                                            "info for <%q>.\n"),
                                   number_of_msg));
                     test_complete= true;
                   }
              }
            ACE_UINT64 end_time;
            ACE_High_Res_Timer::gettimeofday_hr ().to_usec (end_time);
            ACE_UINT64 interval = end_time - start_time;
            if(interval > (duration_run_ * 1000 * 1000))
              {
                test_complete = true;
                instance_cmd->command = THROUGHPUT_COMMAND_COMPLETE;
                cmd_data_writer->write ( *instance_cmd, DDS_HANDLE_NIL);
              }
            if (!test_complete)
              {
                ACE_OS::sleep (recover_time);
              }
          }
        return true;
      }
  }

  int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
  {
    ::DDS::ReturnCode_t retcode;
    long overhead_size = 0;
    const char * type_name_cmd = 0;
    const char * type_name = 0;

    int  main_result = 1; /* error by default */

    ACE_Env_Value<int> id (ACE_TEXT("DDS4CCM_DEFAULT_DOMAIN_ID"), domain_id);
    domain_id = id;

    if (parse_args (argc, argv) != 0)
      return 1;

     /* Create the domain participant */
    ::DDS::DomainParticipant *participant =
                             ::DDS::DomainParticipantFactory::get_instance()->
                             create_participant_with_profile(
                                domain_id,                   /* Domain ID */
                                lib_name,
                                part_name,                   /* QoS */
                                0,                           /* Listener */
                                DDS_STATUS_MASK_NONE);
    if (!participant) {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Unable to create domain participant.\n")));
        goto clean_exit;
    }

    /* Register type before creating topic */
    type_name = ThroughputTestTypeSupport::get_type_name();
    retcode = ThroughputTestTypeSupport::register_type(
                                                participant, type_name);
    if (retcode != DDS_RETCODE_OK)
      {
        goto clean_exit;
      }
    topic = participant->create_topic(
                                      "Test data",    /* Topic name*/
                                      type_name,      /* Type name */
                                      DDS_TOPIC_QOS_DEFAULT, /* Topic QoS */
                                      0,                     /* Listener  */
                                      DDS_STATUS_MASK_NONE);


    if (!topic) {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create topic.\n")));
        goto clean_exit;
    }

    /* Register type before creating topic */
    type_name_cmd = ThroughputCommandTypeSupport::get_type_name();
    retcode = ThroughputCommandTypeSupport::register_type(
        participant, type_name_cmd);
    if (retcode != DDS_RETCODE_OK)
      {
        goto clean_exit;
      }
    /* Create the topic "Command World" for the String type */
    cmd_topic = participant->create_topic("Command data",       /* Topic name*/
                                          type_name_cmd ,       /* Type name */
                                          DDS_TOPIC_QOS_DEFAULT,/* Topic QoS */
                                          0,                    /* Listener  */
                                          DDS_STATUS_MASK_NONE);
    if (!topic)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create topic.\n")));
        goto clean_exit;
      }

    /* Create the command writer using the default publisher */
    cmd_writer = participant->create_datawriter_with_profile(
                                cmd_topic,
                                lib_name,
                                cmd_prof_name,      /* QoS */
                                0,                  /* Listener */
                                DDS_STATUS_MASK_NONE);
    if (!cmd_writer)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Unable to create cmd data writer.\n")));
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

    /* Create data sample for writing */
    instance = ThroughputTestTypeSupport::create_data();
    if (instance == 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create data sample.\n")));
        goto clean_exit;
      }
    overhead_size = sizeof(CORBA::ULong) + sizeof(CORBA::ULongLong);
    instance->key = 1;
    instance->seq_num = 0;
    //instance->data.maximum (MAX_DATA_SEQUENCE_LENGTH);
    instance->data.length(datalen - overhead_size);

    /* Create data sample for writing */
    instance_cmd = ThroughputCommandTypeSupport::create_data();
    if (instance_cmd == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create command sample.\n")));
      goto clean_exit;
    }

    /* Perform a safe type-cast from a generic data writer into a
     * specific data writer for the types "ThroughputTestDataWriter"
     * and "ThroughputCommandDataWriter"
    */
    test_data_writer = ThroughputTestDataWriter::narrow(data_writer);
    cmd_data_writer =  ThroughputCommandDataWriter::narrow(cmd_writer);
    if (!test_data_writer || !cmd_data_writer)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("DDS_StringDataWriter_narrow failed.\n")));
        goto clean_exit;
     }

    // Sleep a couple seconds to allow discovery to happen
    ACE_OS::sleep (1);

    /* --- Write Data ----------------------------------------------------- */
    for(CORBA::ULong i = start_load; i < (max_load + incr_load); i+= incr_load)
    {
      write();
      ACE_OS::sleep (5);
    }

    /* --- Clean Up ------------------------------------------------------- */
    ACE_OS::sleep (5);

    main_result = 0;
clean_exit:
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Exiting.")));
    ACE_DEBUG ((LM_DEBUG, "SUMMARY SENDER number of messages sent: %Q\n",
                          (number_of_msg)));
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
