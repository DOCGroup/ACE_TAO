// $Id$

#include "ace/OS_main.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Date_Time.h"
#include "ace/SString.h"

#include <ndds/ndds_namespace_cpp.h>

/* By default DDS::String type manage strings up to 1k */
#define MAX_STRING_SIZE         1024

bool shutdown_flag = false;
long num_samples = 0;
long received_samples = 0;

/* The listener of events and data from the middleware */
class HelloListener: public ::DDS::DataReaderListener {
public:
  void on_data_available(::DDS::DataReader *reader);
};


int ACE_TMAIN(int argc, ACE_TCHAR** argv) {
  HelloListener listener;
  int           main_result = 1; /* error by default */
  ::DDS::Topic * topic = 0;
  ::DDS::DataReader *data_reader = 0;

  if (argc == 2)
    {
      char *end = 0;
      num_samples = ACE_OS::strtol (argv[1], &end, 10);

      if (end == argv[1] && num_samples < 0)
        {
          ACE_ERROR ((LM_ERROR, "Error: provided argument not a valid integer over zero\n"));
          return -1;
        }
    }

  /* Create the domain participant on domain ID 0 */
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
  data_reader = participant->create_datareader(
                                               topic,
                                               DDS_DATAREADER_QOS_DEFAULT,    /* QoS */
                                               &listener,                      /* Listener */
                                               DDS_DATA_AVAILABLE_STATUS);
  if (!data_reader) {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create data reader.\n")));
    goto clean_exit;
  }

  /* --- Sleep During Asynchronous Reception ---------------------------- */

  /* This thread sleeps forever. When a sample is received, RTI Data
   * Distribution Service will call the on_data_available_callback function.
   */
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Ready to read data.\n")));
  for (;;) {
    ACE_OS::sleep (1);
    if(shutdown_flag){
      break;
    }
  }

  /* --- Clean Up ------------------------------------------------------- */

  main_result = 0;
 clean_exit:
  ::DDS::ReturnCode_t retcode;
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


/* This method gets called back by DDS when one or more data samples have been
 * received.
 */
void HelloListener::on_data_available(::DDS::DataReader *reader) {
  /* Perform a safe type-cast from a generic data reader into a
   * specific data reader for the type "DDS::String"
   */
  ::DDS::StringDataReader * string_reader = ::DDS::StringDataReader::narrow(reader);
  if (!string_reader) {
    /* In this specific case, this will never fail */
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("::DDS::StringDataReader::narrow failed.\n")));
    return;
  }

  /* Loop until there are messages available in the queue */
  char                  sample[MAX_STRING_SIZE];
  char *ptr_sample = &sample[0];
  for(;;) {
    ::DDS::SampleInfo        info;
    ::DDS::ReturnCode_t retcode = string_reader->take_next_sample(
                                                                  ptr_sample,
                                                                  info);
    if (retcode == DDS_RETCODE_NO_DATA) {
      /* No more samples */
      break;
    } else if (retcode != DDS_RETCODE_OK) {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to take data from data reader, error %d.\n"), retcode));
      return;
    }
    if (info.valid_data) {
      // Valid (this isn't just a lifecycle sample): print it
      ++received_samples;
      ACE_CString rec (sample);
      ACE_Date_Time now;
      int sec_rec = ACE_OS::atoi (rec.substr (0, 2).c_str() );
      if (sec_rec > 0)
        {
          int usec_rec = ACE_OS::atoi (rec.substr (3, 6).c_str ());
          if (sec_rec != now.second ())
            usec_rec += 10000000;
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("%C duration <%d>\n"), sample, now.microsec () - usec_rec));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("%C\n"), sample));
        }
      if(received_samples == num_samples || ACE_OS::strlen(sample) == 0){
        shutdown_flag = true;
      }
    }
  }
}

