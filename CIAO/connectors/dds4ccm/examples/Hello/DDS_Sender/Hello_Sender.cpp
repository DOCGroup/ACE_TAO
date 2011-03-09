// $Id$

#include "ace/OS_main.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Date_Time.h"
#include "ace/SString.h"
#include <ndds/ndds_namespace_cpp.h>

int number_of_iterations = 100;
bool log_time = false;
const char* send_string = "This is a DDS sender";

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:s:t"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 's':
        send_string = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ());
        break;
      case 'i':
        number_of_iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 't':
        log_time = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s "
                           "-s <send string>"
                           "-i <number of iterations>"
                           "-t log timing"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int ACE_TMAIN(int argc, ACE_TCHAR* argv[]) {
    ::DDS::ReturnCode_t retcode;
    ::DDS::StringDataWriter *string_writer = 0;
    ::DDS::Topic *topic = 0;
    ::DDS::DataWriter *data_writer = 0;
    int              main_result = 1; /* error by default */

    if (parse_args (argc, argv) != 0)
      return 1;

    ACE_Time_Value tv (0, 1000); //1 msec
    /* Create the domain participant */
    ::DDS::DomainParticipant *participant = ::DDS::DomainParticipantFactory::get_instance()->
                        create_participant(
                                0,                              /* Domain ID */
                                DDS_PARTICIPANT_QOS_DEFAULT,    /* QoS */
                                0,                           /* Listener */
                                DDS_STATUS_MASK_NONE);
    if (!participant) {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create domain participant.\n")));
        goto clean_exit;
    }

    /* Create the topic "Hello, World" for the String type */
    topic = participant->create_topic(
                        "Hello, World",                        /* Topic name*/
                        ::DDS::StringTypeSupport::get_type_name(), /* Type name */
                        DDS_TOPIC_QOS_DEFAULT,                 /* Topic QoS */
                        0,                                  /* Listener  */
                        DDS_STATUS_MASK_NONE);
    if (!topic) {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create topic.\n")));
        goto clean_exit;
    }

    /* Create the data writer using the default publisher */
    data_writer = participant->create_datawriter(
                                topic,
                                DDS_DATAWRITER_QOS_DEFAULT,     /* QoS */
                                0,                           /* Listener */
                                DDS_STATUS_MASK_NONE);
    if (!data_writer) {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create data writer.\n")));
        goto clean_exit;
    }

    /* Perform a safe type-cast from a generic data writer into a
     * specific data writer for the type "DDS::String"
     */
    string_writer = ::DDS::StringDataWriter::narrow(data_writer);
    if (!string_writer) {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("DDS_StringDataWriter_narrow failed.\n")));
        /* In this specific case, this will never fail */
        goto clean_exit;
    }

    // Sleep a couple seconds to allow discovery to happen
    ACE_OS::sleep (1);

    /* --- Write Data ----------------------------------------------------- */

    for (int i = 0; i < number_of_iterations; i++)
      {
        ACE_TCHAR timestamp[16];
        ACE_CString msg (send_string);
        ACE_CString ret;
        ACE_Date_Time now;
        ACE_OS::sprintf (timestamp,
                         "%02d.%d",
                         static_cast<int> (now.second()),
                         static_cast<int> (now.microsec ()));
        ret.set (timestamp);
        ret = ret + " " + msg;
        retcode = string_writer->write(
                            ret.c_str (),
                            DDS_HANDLE_NIL);
        if (retcode != DDS_RETCODE_OK)
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Write failed: %d.\n"), retcode));
        //ACE_ERROR ((LM_ERROR, ACE_TEXT ("%C.\n"), ret.c_str()));
        ACE_OS::sleep (tv);
    }

    /* --- Clean Up ------------------------------------------------------- */

    main_result = 0;
clean_exit:
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Exiting.")));
    if (participant) {
        retcode = participant->delete_contained_entities();
        if (retcode != DDS_RETCODE_OK) {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Deletion failed.\n")));
            main_result = 1;
        }
        retcode = ::DDS::DomainParticipantFactory::get_instance()->
                        delete_participant(participant);
        if (retcode != DDS_RETCODE_OK) {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Deletion failed.\n")));
            main_result = 1;
        }
    }
    return main_result;
}

