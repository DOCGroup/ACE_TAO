// $Id$

#include <stdio.h>
#include <iostream>

using namespace std;

#include "Base.h"
#include "BaseSupport.h"

#define LIBRARY_NAME  "QueryCondition_Library"
#define PROFILE_NAME  "QueryCondition_Profile"

bool shutdown_flag = false;
long num_samples = 0;
long received_samples = 0;

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
                 const int & run)
{
  bool error = false;
  if (run == 1)
    {
      //only samples between 2 and 5 should be received
      error = sample.iteration <= 2 && sample.iteration >= 5;
    }
  else if (run == 2)
    {
      //only samples between 22 and 34 should be received
      error = sample.iteration <= 22 && sample.iteration >= 34;
    }
  else if (run == 3)
    {
      //all other unread samples.
      error = sample.iteration > 2 && sample.iteration < 5;
      error |= sample.iteration > 22 && sample.iteration < 34;
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
      QueryConditionTestRTISeq data;
      DDS_SampleInfoSeq     info_seq;
      sleep_now (3);
      for (DDS_Long i = 0; i < cond.length (); ++i)
        {
          if (run == 3)
            {
              if (cond[i] == rc)
                {
                  typed_dr->read_w_condition (data,
                                              info_seq,
                                              DDS_LENGTH_UNLIMITED,
                                              rc);
                }
              else
                {
                  cerr << "ERROR: Should be woken up on ReadCondition" << endl;
                }
            }
          else
            {
              if (cond[i] == qc)
                {
                  typed_dr->read_w_condition (data,
                                              info_seq,
                                              DDS_LENGTH_UNLIMITED,
                                              qc);
                }
              else
                {
                  cerr << "ERROR: Should be woken up on QueryCondition" << endl;
                }
             }
        }
      for (DDS_Long i = 0; i < data.length (); ++i)
        {
          if (info_seq[i].valid_data)
            {
              check_iter (data[i], run);
            }
        }
      typed_dr->return_loan (data, info_seq);
    }


  //check if all samples are still available.
  if (run == 3)
    {
      QueryConditionTestRTISeq data;
      DDS_SampleInfoSeq     info_seq;
      sleep_now (3);
      typed_dr->read (data,
                      info_seq,
                      DDS_LENGTH_UNLIMITED);
      cout << "CHECKING..... TOTAL NUMBER OF SAMPLES IN DDS : <"
            << data.length ()
            << ">"
            << endl;
      for (DDS_Long i = 0; i < data.length (); ++i)
        {
          cout << "Received: key <"
              << data[i].symbol
              << "> - iteration <"
              << data[i].iteration
              << ">"
              << endl;
        }
      if (data.length () != 300)
        {
          cerr << "ERROR: unexpected number of samples received : "
          << "expected <300> - received <"
          << data.length () << ">" << endl;
        }
      typed_dr->return_loan (data, info_seq);
    }
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
  return main_result;
}

int ACE_TMAIN (int , ACE_TCHAR *[])
{
  int                 main_result = 1; /* error by default */
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

  /* Create the domain participant on domain ID 0 */
  DDSDomainParticipant *participant = DDSDomainParticipantFactory::get_instance()->
      create_participant_with_profile (2,            /* Domain ID */
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
  if (!topic) {
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

  const char* PARAMS_RUN_1[] = {"2", "5"};
  DDS_StringSeq parameters_run_1;
  parameters_run_1.from_array (PARAMS_RUN_1, 2);

  qc = typed_dr->create_querycondition (
                                DDS_NOT_READ_SAMPLE_STATE,
                                DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                DDS_ALIVE_INSTANCE_STATE | DDS_NOT_ALIVE_INSTANCE_STATE,
                                DDS_String_dup ("iteration > %0 AND iteration < %1"),
                                parameters_run_1);
  if (!qc) {
    cerr << "RECEIVER: Error creating query condition" << endl;
    return clean_up (participant);
  }
  else
    cout << "RECEIVER: query condition created" << endl;

  ws_->attach_condition (qc);

  cout << "RECEIVER: Expecting samples with iteration 3 and 4" <<endl;

  read (data_reader, qc, rc, ++run);

  //Second run: change the parameters
  if (qc)
    {
      const char* PARAMS_RUN_2[] = {"22", "34"};
      DDS_StringSeq parameters_run_2;
      parameters_run_2.from_array (PARAMS_RUN_2, 2);
      if (qc->set_query_parameters (parameters_run_2) != DDS_RETCODE_OK)
        {
          cerr << "RECEIVER: Unable the set the new query parameters!!!" << endl;
          return clean_up (participant);
        }
    }

  cout << "RECEIVER: Expecting samples with iterations between 22 and 34" <<endl;

  read (data_reader, qc, rc, ++run);
  // Third run: Detach querycondition and create read condition instead.
  // Remove the qc from the waitset and create a readcondition and start receiving
  // samples
  if (ws_->detach_condition (qc) != DDS_RETCODE_OK)
    {
      cerr << "RECEIVER: Error detaching query condition" << endl;
      return clean_up (participant);
    }
  // Delete the query condition from the data reader
  typed_dr->delete_readcondition (qc);

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

  cout << "RECEIVER: Expecting ALL UNREAD samples" <<endl;

  read (data_reader, qc, rc, ++run);

  main_result = 0;

  return main_result;
}
