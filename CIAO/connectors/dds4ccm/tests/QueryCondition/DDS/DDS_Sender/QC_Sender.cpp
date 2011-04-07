// $Id$

#include <stdio.h>
#include <iostream>

using namespace std;

#include "Base.h"
#include "BaseSupport.h"

#define LIBRARY_NAME  "QueryCondition_Library"
#define PROFILE_NAME  "QueryCondition_Profile"

const int number_of_iterations_ = 3;

const int number_of_keys_ = 2;

int last_iteration_ = 0;
int run_ = 0;

void sleep_now (int sleeptime)
{
#ifdef RTI_WIN32
        Sleep(sleeptime * 1000);
#else
        sleep(sleeptime);
#endif
}

void write (DDSDataWriter * writer)
{
  QueryConditionTestDataWriter* typed_writer = 0;
  typed_writer = QueryConditionTestDataWriter::narrow (writer);
  for (int iter_key = 1; iter_key < number_of_keys_ + 1; ++iter_key)
    {
      char key[7];
      QueryConditionTest new_key;
      sprintf (key, "KEY_%d", iter_key);
      new_key.symbol = DDS_String_dup(key);
      for (int iter = 1;
          iter < number_of_iterations_ + 1;
          ++iter)
        {
          new_key.iteration = iter + last_iteration_;
          DDS_ReturnCode_t retcode = typed_writer->write (new_key, DDS_HANDLE_NIL);
          if (retcode != DDS_RETCODE_OK)
            {
              cout << "ERROR writing symbol <"
                  << key
                  << "> - iteration <"
                  << new_key.iteration
                  << ">"
                  << endl;
            }
        }
      cout << "Sender: Written samples with symbol <"
            << key
            << "> and iteration <"
            << last_iteration_ + 1
            << "> through <"
            << new_key.iteration
            << ">"
            << endl;
    }
  ++run_;
  last_iteration_ = number_of_iterations_ * run_;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
    DDS_ReturnCode_t    retcode;
    DDSTopic*           topic = 0;
    DDSDataWriter*      data_writer = 0;
    DDSPublisher*       publisher = 0;
    int                 main_result = 1; /* error by default */

//     NDDS_Config_LogVerbosity n_verbosity =
//       static_cast <NDDS_Config_LogVerbosity> (3);
//     NDDSConfigLogger::get_instance()->set_verbosity (n_verbosity);
    const char * env_domain_id = 0;
    if (argc > 1)
      {
        env_domain_id = argv[1];
      }
    if (!env_domain_id)
      {
        printf ("Environment variable DEFAULT_DOMAIN_ID not set "
                "=> setting it to 2\n");
        env_domain_id = "2";
      }
    else
      printf ("Domain ID set to %s\n", env_domain_id);

    const int domain_id = atoi (env_domain_id);

    DDSDomainParticipant *participant = DDSDomainParticipantFactory::get_instance()->
        create_participant_with_profile (domain_id,    /* Domain ID */
                                         LIBRARY_NAME, /* QoS */
                                         PROFILE_NAME,
                                         0,            /* Listener */
                                         DDS_STATUS_MASK_NONE);
    if (!participant) {
      cerr << "SENDER: Error creating participant" << endl;
      goto clean_exit;
    }

    retcode = QueryConditionTestTypeSupport::register_type(
      participant, QueryConditionTestTypeSupport::get_type_name ());

    if (retcode != DDS_RETCODE_OK) {
      cerr << "SENDER: Error registering type" << endl;
      goto clean_exit;
    }

    topic = participant->create_topic_with_profile (
                                      "QC",                   /* Topic name*/
                                      QueryConditionTestTypeSupport::get_type_name (), /* Type name */
                                      LIBRARY_NAME,       /* QoS */
                                      PROFILE_NAME,
                                      0,                  /* Listener  */
                                      DDS_STATUS_MASK_NONE);
    if (!topic) {
      cerr << "SENDER: Error creating topic" << endl;
      goto clean_exit;
    }

    publisher = participant->create_publisher_with_profile (LIBRARY_NAME,
                                                            PROFILE_NAME,
                                                            0,
                                                            0);
    if (!publisher) {
      cerr << "SENDER: Error creating publisher" << endl;
      goto clean_exit;
    }

    data_writer = publisher->create_datawriter_with_profile (
                                               topic,
                                               LIBRARY_NAME, /* QoS */
                                               PROFILE_NAME,
                                               0,            /* Listener */
                                               DDS_STATUS_MASK_NONE);
    if (!data_writer) {
      cerr << "SENDER: Error creating data writer" << endl;
      goto clean_exit;
    }

    write (data_writer);
    sleep_now (8);
    write (data_writer);
    sleep_now (8);
    write (data_writer);
    sleep_now (30);

    main_result = 0;
clean_exit:
    cout << "SENDER: Exiting ..." << endl;
    if (participant) {
        retcode = participant->delete_contained_entities();
        if (retcode != DDS_RETCODE_OK) {
            main_result = 1;
        }
        retcode = DDSDomainParticipantFactory::get_instance()->
                        delete_participant(participant);
        if (retcode != DDS_RETCODE_OK) {
            main_result = 1;
        }
    }
    return main_result;
}

