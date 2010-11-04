// $Id$

#include "ace/Get_Opt.h"
#include "tao/ORB_Core.h"
#include "ace/High_Res_Timer.h"
#include "ace/Env_Value_T.h"
#include "ace/Tokenizer_T.h"
#include "Latency_Base.h"
#include "Latency_BaseSupport.h"
#include "Latency_BasePlugin.h"

#include <ndds/ndds_namespace_cpp.h>

bool shutdown_flag_ = false;

CORBA::LongLong count_ = 0; // total count of all received messages

LatencyTest * instance_ =0;
LatencyTestDataWriter * test_data_writer_ = 0;

const char * lib_name_ = 0;
const char * prof_name_ = 0;

CORBA::UShort domain_id_ = 0;
CORBA::Boolean both_read_write_ = false;

void
split_qos (const char * qos)
{
  char* buf = ACE_OS::strdup (qos);
  ACE_Tokenizer_T<char> tok (buf);
  tok.delimiter_replace ('#', 0);
  for (char *p = tok.next (); p; p = tok.next ())
    {
      if (!lib_name_)
        {
          lib_name_ = ACE_OS::strdup (p);
        }
      else if (!prof_name_)
        {
          prof_name_ = ACE_OS::strdup (p);
        }
    }
  ACE_OS::free (buf);
  ACE_DEBUG ((LM_DEBUG, "Receiver : Found QoS profile %C %C\n",
    lib_name_,
    prof_name_));
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("d:b:q:O"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
          case 'b':
            both_read_write_  = true;
            break;
          case 'd':
            domain_id_  = ACE_OS::atoi (get_opts.opt_arg ());
            break;
          case 'q':
            {
              const char * qos = get_opts.opt_arg ();
              split_qos (qos);
            }
          break;
          case '?':
          default:
            printf("c = <%c>\n",c);
            ACE_ERROR_RETURN ((LM_ERROR,
                                "usage:\n\n"
                                "  -d <domain_id>\n"
                                "  -q <QoS profile>\n"
                                "  -b use both a writer and reader per topic.\n"
                                "\n"),
                              -1);
        }
    }
  // Indicates successful parsing of the command line
  return 0;
}

/* The listener of events and data from the middleware */
class HelloListener: public ::DDS::DataReaderListener {
public:
  void on_data_available(::DDS::DataReader *reader);
};

/* The dummy listener of events and data from the middleware */
class DummyListener: public ::DDS::DataReaderListener {
};

void
write_back (LatencyTest & an_instance)
{
  ++count_;

  try
    {
      test_data_writer_->write(an_instance, ::DDS::HANDLE_NIL);
    }
  catch (const ::CORBA::Exception& ex)
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
  DummyListener dum_listener;
  ::DDS::DataReader *data_reader = 0;
  ::DDS::DataReader *dum_data_reader = 0;
  const char * type_name = 0;
  ::DDS::Topic * send_topic = 0;
  ::DDS::Topic * receive_topic = 0;
  ::DDS::DataWriter * data_writer = 0;
  ::DDS::DataWriter * dum_data_writer = 0;

  ACE_Env_Value<int> id (ACE_TEXT("DDS4CCM_DEFAULT_DOMAIN_ID"), domain_id_);
  domain_id_ = id;

  int main_result = 1; /* error by default */
  if (parse_args (argc, argv) != 0)
    return 1;

  /* Create the domain participant on domain ID 0 */
  ::DDS::DomainParticipant *participant =
              ::DDS::DomainParticipantFactory::get_instance()->
                create_participant_with_profile(
                                  domain_id_,
                                  lib_name_,
                                  prof_name_,
                                  0,
                                  DDS_STATUS_MASK_NONE);
  if (!participant)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Receiver : Unable to create domain participant.\n")));
      goto clean_exit;
    }
  /* Register type before creating topic */
  type_name = LatencyTestTypeSupport::get_type_name ();
  retcode = LatencyTestTypeSupport::register_type (participant, type_name);
  if (retcode != DDS_RETCODE_OK)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Unable to register topic type.\n")));
      goto clean_exit;
    }

  send_topic = participant->create_topic_with_profile (
                      "send",
                      type_name,
                      lib_name_,
                      prof_name_,
                      0,
                      DDS_STATUS_MASK_NONE);
  if (!send_topic)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create topic.\n")));
      goto clean_exit;
    }

  receive_topic = participant->create_topic_with_profile (
                      "receive",
                      type_name,
                      lib_name_,
                      prof_name_,
                      0,
                      DDS_STATUS_MASK_NONE);
  if (!receive_topic)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create topic.\n")));
      goto clean_exit;
    }

  /* Create the data reader using the default publisher */
  data_reader = participant->create_datareader_with_profile(
                                              send_topic,
                                              lib_name_,
                                              prof_name_,
                                              &listener,
                                              DDS_DATA_AVAILABLE_STATUS);

  if (!data_reader )
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create data reader.\n")));
      goto clean_exit;
    }

  /* Create a data writer, which will not be used, but is there for
  *   compatibility with DDS4CCM latency test, where there is always a
  *  reader and a writer per connector
  */
  if (both_read_write_)
    {
    dum_data_writer = participant->create_datawriter_with_profile(
                                send_topic,
                                lib_name_,
                                prof_name_,
                                0,
                                DDS_STATUS_MASK_NONE);
    if (!dum_data_writer)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create data writer.\n")));
        goto clean_exit;
      }
    }
  /* Create the data writer using the default publisher */
  data_writer = participant->create_datawriter_with_profile(
                              receive_topic,
                              lib_name_,
                              prof_name_,
                              0,
                              DDS_STATUS_MASK_NONE);
  if (!data_writer)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create data writer.\n")));
      goto clean_exit;
    }

  /* Create a data reader, which will not be used, but is there for
  *  compatibility with DDS4CCM latency test, where there is always a
  *  reader and a writer per connector.
  */
  if (both_read_write_)
    {
      dum_data_reader = participant->create_datareader_with_profile(
                                              receive_topic,
                                              lib_name_,
                                              prof_name_,
                                              &dum_listener,
                                              DDS_DATA_AVAILABLE_STATUS);

    if (!dum_data_reader )
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create data reader.\n")));
        goto clean_exit;
      }
    }

  test_data_writer_ = LatencyTestDataWriter::narrow (data_writer);
  if (!test_data_writer_)
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
    if (shutdown_flag_)
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
      retcode = participant->delete_contained_entities ();
      if (retcode != DDS_RETCODE_OK)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Deletion failed.\n")));
          main_result = 1;
        }
      retcode = ::DDS::DomainParticipantFactory::get_instance ()->
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
                            LatencyTestDataReader::narrow (reader);
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
      ::DDS::SampleInfoSeq info;
      ::LatencyTestRTISeq sample_req;
      ::DDS::ReturnCode_t const retcode = test_reader->take(sample_req, info);
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
      for (::DDS_Long i = 0; i < sample_req.length (); ++i)
        {
          if (info[i].valid_data)
            {
              write_back(sample_req[i]);
            }
        }
      (void) test_reader->return_loan (sample_req, info);
    }
}
