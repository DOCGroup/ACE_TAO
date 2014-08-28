// $Id$

#include <stdio.h>
#include <iostream>

using namespace std;

#include "Base.h"
#include "BaseSupport.h"

#include "ace/OS_main.h"
#include "ace/OS_NS_stdlib.h"

#define LIBRARY_NAME  "QueryCondition_Library"
#define PROFILE_NAME  "QueryConditionProfile"

bool shutdown_flag = false;
long num_samples = 0;
long received_samples = 0;
long expected_samples_run1 = 1 * 2;    //only sample 2 should be received of 2 keys
long expected_samples_run2 = 1 * 2;   //only sample 5 should be received o 2 keys
long expected_samples_run3 = (9 - 2) * 2; //all samples 1-9 except sample 2 and 5 of both keys.

DDSWaitSet* ws_ = new DDSWaitSet ();
DDS_Duration_t dur_ = {9, 0};

void sleep_now (int sleeptime)
{
#ifdef RTI_WIN32
        Sleep(sleeptime * 1000);
#else
        sleep(sleeptime);
#endif
}

void check_iter (const QueryConditionTest & sample,
                   const DDS_SampleInfo & readinfo,
                   const int & run)
{
  bool error = false;
  if (run == 1)
    {
      //only sample 2 should be received
      error = sample.iteration <= 1 && sample.iteration >= 3;
    }
  else if (run == 2)
    {
      //only samples 8 should be received
      error = sample.iteration <= 4 && sample.iteration >= 6;
    }
  else if (run == 3)
    {
      //all other unread samples.
      error = sample.iteration == 2;
      error |= sample.iteration ==5;
    }

  error
    ?
    cerr << "ERROR Received: key <"
        << sample.symbol
        << "> - iteration <"
        << sample.iteration
        << ">"
        << endl
    :
    cout << "Received: key <"
        << sample.symbol
        << "> - iteration <"
        << sample.iteration
        << "> - instance status <"
        << readinfo.instance_state
        << "> - view status <"
        << readinfo.view_state
        << "> - sample status <"
        << readinfo.sample_state
        << ">"
        << endl;
}


void read (DDSDataReader * dr,
           DDSQueryCondition * qc,
           DDSReadCondition * rc,
           const int & run)
{
  QueryConditionTestDataReader* typed_dr = QueryConditionTestDataReader::narrow (dr);

  DDSConditionSeq cond;

  if (ws_->wait (cond, dur_) == DDS_RETCODE_TIMEOUT)
    {
      cerr << "ERROR: wait timed out" << endl;
    }
  else
    {
      //wait a moment to allow DDS to write all samples we need here...
      QueryConditionTestSeq data;
      DDS_SampleInfoSeq     info_seq;
      sleep_now (3);
      for (DDS_Long i = 0; i < cond.length (); ++i)
        {
          if (run == 3)
            {
              sleep_now (10);
              if (cond[i] == rc)
                {
                  received_samples = 0;

                  //  **************  read all in one
                  cout << "CHECKING..... SAMPLES with read_w_condition with readcondition:" << endl;
                  typed_dr->read_w_condition (data,
                                              info_seq,
                                              DDS_LENGTH_UNLIMITED,
                                              rc);
                  for (DDS_Long y = 0; y < data.length (); ++y)
                    {
                      if (info_seq[y].valid_data)
                        {
                          check_iter (data[y], info_seq[y], run);
                          received_samples ++;
                        }
                    }
                  typed_dr->return_loan (data, info_seq);
                  if (received_samples !=  expected_samples_run3)
                    {
                      cerr << "ERROR: run  3 unexpected number of samples received : "
                           << "expected < "
                           <<  expected_samples_run3 << "> - received <"
                           << received_samples << ">" << endl;
                    }
                // end read all in one

                }
              else
                {
                  cerr << "ERROR: Should be woken up on ReadCondition" << endl;
                }
            }
          else // run 1 and 2
            {
              if (cond[i] == qc)
                {
                  received_samples = 0;
                  cout << "CHECKING..... SAMPLES with read_w_condition with querycondition:" << endl;
                  typed_dr->read_w_condition (data,
                                               info_seq,
                                              DDS_LENGTH_UNLIMITED,
                                              qc);
                  for (DDS_Long i = 0; i < data.length (); ++i)
                     {
                       if (info_seq[i].valid_data)
                         {
                           check_iter (data[i], info_seq[i], run);
                           received_samples ++;
                         }
                     }
                   typed_dr->return_loan (data, info_seq);

                  if (run == 1)
                    {
                      if (received_samples !=  expected_samples_run1)
                        {
                          cerr << "ERROR: run  1 unexpected number of samples received : "
                          << "expected < "
                          <<  expected_samples_run1 << "> - received <"
                          << received_samples << ">" << endl;
                        }
                    }
                  else
                    {
                      if (received_samples !=  expected_samples_run2)
                        {
                           cerr << "ERROR: run  2 unexpected number of samples received : "
                           << "expected < "
                           <<  expected_samples_run2 << "> - received <"
                           << received_samples << ">" << endl;
                        }
                    }
                 }
              else
                {
                  cerr << "ERROR: Should be woken up on QueryCondition" << endl;
                }
             }
        }
      if (run != 3)
        {
        }

    }

  //check if all samples are still available.
  if (run == 3)
    {
      QueryConditionTestSeq data;
      DDS_SampleInfoSeq     info_seq;
      sleep_now (3);
      typed_dr->read (data,
                      info_seq,
                      DDS_LENGTH_UNLIMITED);
      cout << "\nCHECKING..... TOTAL NUMBER OF SAMPLES IN DDS with a read : <"
             << data.length ()
             << ">"
             << endl;
      for (DDS_Long i = 0; i < data.length (); ++i)
        {
          cout << "Received: key <"
              << data[i].symbol
              << "> - iteration <"
              << data[i].iteration
              << "> - instance status <"
              << info_seq[i].instance_state
              << "> - view status <"
              << info_seq[i].view_state
              << "> - sample status <"
              << info_seq[i].sample_state
              << ">"
              << endl;
        }
      if (data.length () != 18)
        {
          cerr << "ERROR: unexpected number of samples received : "
          << "expected <18> - received <"
          << data.length () << ">" << endl;
        }
      typed_dr->return_loan (data, info_seq);
    }
    sleep_now (5);
}

int clean_up (DDSDomainParticipant * participant)
{
  DDS_ReturnCode_t    retcode = DDS_RETCODE_OK;
  int main_result = 0;

  cout << "RECEIVER: Exiting ..." << endl;
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
  delete ws_;
  DDSDomainParticipantFactory::finalize_instance ();
  return main_result;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  DDSTopic*           topic = 0;
  DDSDataReader*      data_reader = 0;
  DDSSubscriber*      sub = 0;
  DDS_ReturnCode_t    retcode = DDS_RETCODE_OK;
  QueryConditionTestDataReader* typed_dr = 0;
  DDSQueryCondition*  qc = 0;
  DDSReadCondition*   rc = 0;
  int run = 0;

//     NDDS_Config_LogVerbosity n_verbosity =
//       static_cast <NDDS_Config_LogVerbosity> (3);
//     NDDSConfigLogger::get_instance()->set_verbosity (n_verbosity);

  const ACE_TCHAR * env_domain_id = 0;
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

  const int domain_id = ACE_OS::atoi (env_domain_id);
  /* Create the domain participant on domain ID 0 */
  DDSDomainParticipant *participant = DDSDomainParticipantFactory::get_instance()->
      create_participant_with_profile (domain_id,    /* Domain ID */
                                       LIBRARY_NAME, /* QoS */
                                       PROFILE_NAME,
                                       0,            /* Listener */
                                       DDS_STATUS_MASK_NONE);
  if (!participant) {
    cerr << "RECEIVER: Error creating participant" << endl;
    return 1;
  }

  retcode = QueryConditionTestTypeSupport::register_type(
    participant, QueryConditionTestTypeSupport::get_type_name ());

  if (retcode != DDS_RETCODE_OK) {
    cerr << "RECEIVER: Error registering type" << endl;
    return clean_up (participant);
  }

  topic = participant->create_topic_with_profile (
                                    "QC",                   /* Topic name*/
                                    QueryConditionTestTypeSupport::get_type_name (), /* Type name */
                                    LIBRARY_NAME,       /* QoS */
                                    PROFILE_NAME,
                                    0,                  /* Listener  */
                                    DDS_STATUS_MASK_NONE);
  if (!topic) {
    cerr << "RECEIVER: Error creating topic" << endl;
    return clean_up (participant);
  }

  sub = participant->create_subscriber_with_profile (LIBRARY_NAME,
                                                     PROFILE_NAME,
                                                     0,
                                                     0);
  if (!sub) {
    cerr << "RECEIVER: Error creating subscriber" << endl;
    return clean_up (participant);
  }

  data_reader = sub->create_datareader_with_profile (
                                               topic,
                                               LIBRARY_NAME, /* QoS */
                                               PROFILE_NAME,
                                               0,            /* Listener */
                                               DDS_STATUS_MASK_NONE);
  if (!data_reader) {
    cerr << "RECEIVER: Error creating data reader" << endl;
    return clean_up (participant);
  }

  typed_dr = QueryConditionTestDataReader::narrow (data_reader);

  if (!typed_dr) {
    cerr << "RECEIVER: Unable to cast to a type specific data reader" << endl;
    return clean_up (participant);
  }

  const char* PARAMS_RUN_1[] = {"1", "3"};
  DDS_StringSeq parameters_run_1;
  parameters_run_1.from_array (PARAMS_RUN_1, 2);

  qc = typed_dr->create_querycondition (
                                DDS_NOT_READ_SAMPLE_STATE,
                                DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                DDS_ALIVE_INSTANCE_STATE | DDS_NOT_ALIVE_INSTANCE_STATE,
                                "iteration > %0 AND iteration < %1",
                                parameters_run_1);
  if (!qc) {
    cerr << "RECEIVER: Error creating query condition" << endl;
    return clean_up (participant);
  }
  else
    cout << "RECEIVER: query condition created : iteration > 1 AND iteration < 3" << endl;

  ws_->attach_condition (qc);

  cout << "RECEIVER: Expecting two samples (key_1 and key_2) with iteration 3." <<endl;

  read (data_reader, qc, rc, ++run);

  //Second run: change the parameters
  if (qc)
    {
      const char* PARAMS_RUN_2[] = {"4", "6"};
      DDS_StringSeq parameters_run_2;
      parameters_run_2.from_array (PARAMS_RUN_2, 2);
      if (qc->set_query_parameters (parameters_run_2) != DDS_RETCODE_OK)
        {
          cerr << "RECEIVER: Unable the set the new query parameters!!!" << endl;
          return clean_up (participant);
        }
    }
  cout << "RECEIVER: query condition changed : iteration > 4 AND iteration < 6" << endl;

  cout << "RECEIVER: Expecting two samples (key_1 and key_2) with iterations 5" <<endl;

  read (data_reader, qc, rc, ++run);
  // Third run: Detach querycondition and create read condition instead.
  // Remove the qc from the waitset and create a readcondition and start receiving
  // samples
  if (ws_->detach_condition (qc) != DDS_RETCODE_OK)
    {
      cerr << "RECEIVER: Error detaching query condition" << endl;
      return clean_up (participant);
    }
  cout << "RECEIVER: query condition deleted" << endl;
  cout << "RECEIVER: create read condition : DDS_NOT_READ_SAMPLE_STATE,"
       << "DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,"
       << "DDS_ALIVE_INSTANCE_STATE | DDS_NOT_ALIVE_INSTANCE_STATE" << endl;

  // Delete the query condition from the data reader
  typed_dr->delete_readcondition (qc);
  sleep_now(10);

  rc = typed_dr->create_readcondition (
                            DDS_NOT_READ_SAMPLE_STATE,
                            DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                            DDS_ALIVE_INSTANCE_STATE | DDS_NOT_ALIVE_INSTANCE_STATE);
  if (!rc) {
    cerr << "RECEIVER: Error creating read condition" << endl;
    return clean_up (participant);
  }

  if (ws_->attach_condition (rc) != DDS_RETCODE_OK)
    {
      cerr << "RECEIVER: Error attaching read condition" << endl;
      return clean_up (participant);
    }

  cout << "RECEIVER: Expecting ALL UNREAD samples (key_1 and key_2) with iterations between 1 and 9, except 2 and 5" <<endl;

  read (data_reader, qc, rc, ++run);

  return clean_up (participant);
}
