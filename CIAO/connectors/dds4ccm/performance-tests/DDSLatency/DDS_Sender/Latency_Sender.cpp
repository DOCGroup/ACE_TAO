// $Id$

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "tao/ORB_Core.h"
#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"
#include "ace/Env_Value_T.h"
#include "Latency_Base.h"
#include "Latency_BaseSupport.h"
#include "Latency_BasePlugin.h"
#include "ace/Tokenizer_T.h"

#include <ndds/ndds_namespace_cpp.h>

//typedefs
typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Long > Atomic_Long;
typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::Boolean> Atomic_Boolean;

//forward declarations
class WriteTicker;

//Global variables
CORBA::UShort iterations_ = 1000;
CORBA::UShort datalen_ = 100;
CORBA::UShort datalen_idx_ = 0;
CORBA::UShort nr_of_runs_ = 10;
CORBA::UShort sleep_ = 2;
Atomic_Long  tv_total_ = 0;
Atomic_Long  tv_max_ = 0;
Atomic_Long  tv_min_ = 0;
CORBA::UShort count_ = 0;
CORBA::UShort number_of_msg_ = 0;
Atomic_Boolean received_ = false;
Atomic_Long seq_num_ = 0;
CORBA::Double sigma_duration_squared_;
ACE_UINT64 start_time_ = 0;
ACE_UINT64 start_time_test_ = 0;
ACE_UINT64 end_time_test_ = 0;

CORBA::Long * duration_times_;
CORBA::Short * datalen_range_;
CORBA::Long clock_overhead_;

LatencyTest * instance_ = 0;

LatencyTestDataWriter * test_data_writer_ = 0;

const char * lib_name_  = 0;
const char * prof_name_ = 0;

CORBA::UShort domain_id_ = 0;
CORBA::Boolean both_read_write_ = false;

ACE_Reactor * reactor_ = 0;

WriteTicker * ticker_ = 0;

/* The listener of events and data from the middleware */
class HelloListener: public DDSDataReaderListener {
public:
  void on_data_available(DDSDataReader *reader);
};
/* The dummy listener of events and data from the middleware */
class DummyListener: public DDSDataReaderListener {
};

class WriteTicker :public ACE_Event_Handler
{
  public:
    WriteTicker (void);
    int handle_timeout (const ACE_Time_Value &, const void *);
};

void
split_qos (const char * qos)
{
  char* buf = const_cast <char *> (qos);
  ACE_Tokenizer_T<char> tok (buf);
  tok.delimiter_replace ('#', 0);
  for (char *p = tok.next (); p; p = tok.next ())
    {
      if (!lib_name_)
        {
          lib_name_ = p;
        }
      else if (!prof_name_)
        {
          prof_name_ = p;
        }
    }
  ACE_DEBUG ((LM_DEBUG, "Sender : Found QoS profile %C %C\n",
    lib_name_,
    prof_name_));
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("b:d:i:s:q:O"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
          case 'd':
            domain_id_  = ACE_OS::atoi (get_opts.opt_arg ());
            break;
          case 'i':
            iterations_ = ACE_OS::atoi (get_opts.opt_arg ());
            break;
          case 's':
            sleep_ = ACE_OS::atoi (get_opts.opt_arg ());
            break;
          case 'b':
            both_read_write_  = true;
            break;
          case 'q':
            {
               const char * qos = get_opts.opt_arg ();
               split_qos (qos);
            }
            break;
          case '?':
          default:
            ACE_ERROR_RETURN ((LM_ERROR,
                                "usage:\n\n"
                                "  -d <domain_id >\n"
                                "  -i <iterations >\n"
                                "  -s <sleep>\n"
                                "  -q <QoS profile>\n"
                                "  -b " 
                                "\n"),
                              -1);
        }
    }
  // Indicates sucessful parsing of the command line
  return 0;
}

void
calculate_clock_overhead (void)
{
  int num_of_loops_clock = 320;
  ACE_UINT64 begin_time;
  ACE_UINT64 clock_roundtrip_time;
  ACE_High_Res_Timer::gettimeofday_hr ().to_usec (begin_time);
  for (int i = 0; i < num_of_loops_clock; ++i)
    {
      ACE_High_Res_Timer::gettimeofday_hr ().to_usec (clock_roundtrip_time);
    }
  ACE_UINT64 total_time =  clock_roundtrip_time - begin_time;
  clock_overhead_ = (long)(total_time /num_of_loops_clock);
}

void
stop (void)
{
  if (ticker_)
    {
      reactor_->cancel_timer (ticker_);
      delete ticker_;
      ticker_ = 0;
    }
}

void
init_values (void)
{
  duration_times_ = new CORBA::Long[iterations_];
  datalen_range_ = new CORBA::Short[nr_of_runs_];
  int start = 16;
  for(int i = 0; i < nr_of_runs_; i++)
    {
      datalen_range_[i] = start;
      start = 2 * start;
    }

  datalen_ = datalen_range_[0];

  // make instances of Topic
  instance_->seq_num = 0;
  instance_->ping = 0;
  instance_->data.length (datalen_);
  calculate_clock_overhead ();
}

void
record_time (ACE_UINT64  receive_time)
{
  ACE_UINT64 interval = receive_time  - start_time_;
  ++count_;
  long duration = static_cast <CORBA::Long> (interval) - clock_overhead_;
  if (count_ > iterations_)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: Internal error while getting more "
                  "messages back as expected.\n"));
    }
  else
    {
      duration_times_[count_-1] = duration;
      sigma_duration_squared_ += (double)duration * (double)duration;
      tv_total_ += duration;
      if (duration > tv_max_.value () || (tv_max_.value () == 0L))
        {
          tv_max_ = duration;
        }
      if (duration < tv_min_.value () || (tv_min_.value () == 0L))
        {
          tv_min_ = duration;
        }
    }
}

void
reset_results (void)
{
  count_ = 0;
  duration_times_ = new CORBA::Long[iterations_];
  tv_total_ = 0L;
  tv_max_ = 0L;
  tv_min_ = 0L;
  number_of_msg_ = 0;
  received_ = false;
  seq_num_ = 0;
  sigma_duration_squared_ = 0;
}

static int compare_two_longs (const void * long1, const void * long2)
{
  return (*(CORBA::Long*)long1 - *(CORBA::Long*)long2);
}

void
calc_results()
{
  // Sort all duration times.
  qsort(duration_times_,
        count_,
        sizeof(CORBA::Long),
        compare_two_longs);

  // Show latency_50_percentile, latency_90_percentile,
  // latency_99_percentile and latency_99.99_percentile.
  // For example duration_times[per50] is the median i.e. 50% of the
  // samples have a latency time  <=  duration_times[per50]
  int per50 = count_/2;
  int per90 = (int)(count_ * 0.90);
  int per99 = (int)(count_ * 0.990);
  int per9999 = (int)(count_ * 0.9999);

  double avg = 0;
  double roundtrip_time_std = 0;
  if (count_ > 0)
    {
      avg = tv_total_.value () / count_;
      // Calculate standard deviation.
      roundtrip_time_std  = sqrt(
        (sigma_duration_squared_ / (double)count_) -
        (avg * avg));
    }

  // Show values as float, in order to be comparable with RTI performance test.
  if (count_ > 0)
    {
      if (datalen_idx_ == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
            "Collecting statistics on %d samples per message size.\n"
            "This is the roundtrip time, *not* the one-way-latency\n"
            "bytes ,stdev us,ave us, min us, 50%% us, 90%% us, 99%% us, 99.99%%,"
            " max us\n"
            "------,-------,-------,-------,-------,-------,-------,-------,"
            "-------\n", count_));
        }
      ACE_DEBUG ((LM_DEBUG,
        "%6d,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f\n",
        datalen_,
        roundtrip_time_std,
        avg,
        (double)tv_min_.value (),
        (double)duration_times_[per50-1],
        (double)duration_times_[per90-1],
        (double)duration_times_[per99-1],
        (double)duration_times_[per9999-1],
        (double)tv_max_.value ()));
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "SUMMARY SENDER latency time:\n "
                          "No samples reveived back.\n"));
    }
}


void
write_one (void)
{
  if ((number_of_msg_ == 0) && (datalen_idx_ == 0))
    {
      ACE_High_Res_Timer::gettimeofday_hr ().to_usec (start_time_test_);
    }
  // First message sent always, next messages only as previous sent message
  // is received back.
  if ((number_of_msg_ == 0) || received_.value())
    {
      // All messages send, stop timer.
      if ((iterations_ != 0) &&
          (number_of_msg_ >= iterations_ ))
        {
          if (datalen_idx_ >= (nr_of_runs_ - 1))
            {
              stop();
              calc_results();
              ACE_High_Res_Timer::gettimeofday_hr ().to_usec (end_time_test_);
              ACE_Reactor::instance ()->end_reactor_event_loop ();
            }
          else
            {
              calc_results();
              reset_results();
              ++datalen_idx_;
              datalen_ = datalen_range_[datalen_idx_];
              instance_->data.length (datalen_);
            }
        }
      else
        {
          try
            {
              // Send messages with indicator (ping = 1L) so that subscriber knows
              // that this message has to sent back.
              instance_->ping = 1L;
              instance_->seq_num =  number_of_msg_;
              // Keep last sent seq_num, to control if message is sent back.
              seq_num_ = number_of_msg_;
              received_ = false;
              ACE_High_Res_Timer::gettimeofday_hr ().to_usec (start_time_);
              test_data_writer_->write (*instance_, DDS_HANDLE_NIL);
            }
          catch (const CORBA::Exception& )
            {
              ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                          ACE_TEXT ("while writing sample with sequence_number <%u>.\n"),
                          instance_->seq_num));
            }
          ++number_of_msg_;
        }
    }
}

ACE_Reactor *
reactor_used (void)
{
  return ACE_Reactor::instance ();
}

void start (void)
{
  reactor_ = reactor_used();
  ticker_ = new WriteTicker();

  // This->sleep_ is in ms
  unsigned int sec = sleep_/1000;
  unsigned int usec = (sleep_ % 1000) * 1000;
  (void) ACE_High_Res_Timer::global_scale_factor ();
  reactor_->timer_queue()->gettimeofday (&ACE_High_Res_Timer::gettimeofday_hr);
  if (reactor_->schedule_timer (
                  ticker_,
                  0,
                  ACE_Time_Value (5, 0),
                  ACE_Time_Value (sec, usec)) == -1)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("start : ")
                            ACE_TEXT ("Error scheduling timer")));
    }
  reactor_->run_reactor_event_loop ();
}

void
read (LatencyTest & an_instance, ACE_UINT64 receive_time)
{
  if (an_instance.seq_num == seq_num_.value ())
    {
      record_time( receive_time);
      received_ = true;
    }
}

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
  DDS_ReturnCode_t retcode;
  ::DDS::DataReader * data_reader = 0;
  ::DDS::DataReader * dum_data_reader = 0;

  HelloListener listener;
  DummyListener dum_listener;
  const char * type_name = 0;
  int main_result = 1; /* error by default */

  ::DDS::Topic * receive_topic = 0;
  ::DDS::Topic * send_topic = 0;
  ::DDS::DataWriter * data_writer = 0;
  ::DDS::DataWriter * dum_data_writer = 0;

  ACE_Env_Value<int> id (ACE_TEXT("DDS4CCM_DEFAULT_DOMAIN_ID"), domain_id_);
  domain_id_ = id;

  if (parse_args (argc, argv) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Error arguments.\n")));
      return 1;
    }

  /* Create the domain participant */
  DDSDomainParticipant * participant =
                          DDSDomainParticipantFactory::get_instance()->
                          create_participant_with_profile(
                              domain_id_,
                              lib_name_,
                              prof_name_,
                              0,
                              DDS_STATUS_MASK_NONE);
  if (!participant)
    {
      ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Sender : Unable to create domain participant.\n")));
      goto clean_exit;
    }

  /* Register type before creating topic */
  type_name = LatencyTestTypeSupport::get_type_name();
  retcode = LatencyTestTypeSupport::register_type (participant,
                                                   type_name);
  if (retcode != DDS_RETCODE_OK)
    {
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
    if (!receive_topic) {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create topic.\n")));
        goto clean_exit;
    }
    /* Create the data writer using the default publisher */
    data_writer = participant->create_datawriter_with_profile(
                                                          send_topic,
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
                                                send_topic,
                                                lib_name_,
                                                prof_name_,
                                                &dum_listener,
                                                DDS_DATA_AVAILABLE_STATUS);

      if (!dum_data_reader )
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create dummy data reader.\n")));
          goto clean_exit;
        }
      }

    data_reader = participant->create_datareader_with_profile(
                                                receive_topic,
                                                lib_name_,
                                                prof_name_,
                                                &listener,
                                                DDS_DATA_AVAILABLE_STATUS);
    if (!data_reader)
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
                                   receive_topic,
                                    lib_name_,
                                    prof_name_,
                                    0,
                                    DDS_STATUS_MASK_NONE);
        if (!dum_data_writer)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create dummy data writer.\n")));
            goto clean_exit;
          }
      }

    /* Create data sample for writing */
    instance_ = LatencyTestTypeSupport::create_data ();
    if (instance_ == 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create data sample.\n")));
        goto clean_exit;
      }

    init_values();

    test_data_writer_ = LatencyTestDataWriter::narrow (data_writer);
    if (!test_data_writer_)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("LatencyTestDataWriter_narrow failed.\n")));
        goto clean_exit;
      }

    // Sleep a couple seconds to allow discovery to happen
    ACE_OS::sleep (5);

    // handle writing of messages
    start();

    /* --- Clean Up --- */
    ACE_OS::sleep (5);
    main_result = 0;

clean_exit:
    char * read_write_str;
    if (both_read_write_)
      {
        read_write_str = "Used a extra dummy reader and writer per topic.";
      }
    else
      {
        read_write_str = "Used a reader for one topic and a writer for other topic.";
      }

    if((nr_of_runs_ -1) != datalen_idx_)
      {
        ACE_DEBUG ((LM_DEBUG, "SUMMARY SENDER : %u of %u runs completed.\n"
                            " Number of messages sent of last run (%u): %u\n"
                            "%C\n\n",
                            datalen_idx_,
                            nr_of_runs_,
                            datalen_idx_ + 1,
                            number_of_msg_,
                            read_write_str));
      }
    else
      {
        ACE_UINT64 test_time_usec = end_time_test_ - start_time_test_;

        double sec = (double)test_time_usec / (1000 * 1000);
        ACE_DEBUG ((LM_DEBUG, "TEST successful, number of runs (%u) of "
                              "%u messages in %3.3f seconds.\n"
                               "%C\n\n",
                              nr_of_runs_,
                              number_of_msg_,
                              sec,
                              read_write_str));
      }
    if (participant)
      {
        retcode = participant->delete_contained_entities ();
        if (retcode != DDS_RETCODE_OK)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Deletion failed.\n")));
            main_result = 1;
          }
        retcode = DDSDomainParticipantFactory::get_instance()->
                        delete_participant (participant);
        if (retcode != DDS_RETCODE_OK)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Deletion failed.\n")));
            main_result = 1;
          }
    }
    return main_result;
}

void HelloListener::on_data_available(DDSDataReader *reader)
{
  LatencyTestDataReader * test_reader =
                            LatencyTestDataReader::narrow (reader);
  LatencyTest *instance = new LatencyTest;
  if (!test_reader)
    {
      /* In this specific case, this will never fail */
      ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("LatencyTestDataReader::narrow failed.\n")));
      return;
    }

  /* Loop until there are messages available in the queue */
  for(;;)
    {
      DDS_SampleInfo info;
      DDS_ReturnCode_t retcode = test_reader->take_next_sample (*instance,
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
          ACE_UINT64 receive_time = 0;

          // Only interested in messages received with a latency_ping = 0
          // (messages sent back by receiver)
          if (instance->ping == 0)
            {
              ACE_High_Res_Timer::gettimeofday_hr ().to_usec ( receive_time);
              read(*instance, receive_time);
            }
        }
    }
}

//============================================================
// WriteTickerHandler
//============================================================
WriteTicker::WriteTicker ()
{
}

int
WriteTicker::handle_timeout (const ACE_Time_Value &, const void *)
{
  write_one();
  return 0;
}
