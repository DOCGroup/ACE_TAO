// $Id$

#include "Task_Client.h"
#include "Timer.h"
#include "ace/Stats.h"
#include "tao/TAO_Internal.h"
#include "ace/Barrier.h"
#include "ace/Thread_Semaphore.h"
#include "ace/OS_NS_unistd.h"

#if defined (ACE_HAS_QUANTIFY)
# include "quantify.h"
#endif /* ACE_HAS_QUANTIFY */

inline
ACE_UINT32
ACE_round (ACE_timer_t t)
{
  return static_cast<ACE_UINT32> (t);
}

Task_State::Task_State (void)
  : barrier_ (0),
    key_ ("Cubit"),
    loop_count_ (1000),
    thread_count_ (2),
    latency_ (0),
    ave_latency_ (0),
    datatype_ (CB_OCTET),
    thread_per_rate_ (0),
    global_jitter_array_ (0),
    count_ (0),
    shutdown_ (0),
    oneway_ (0),
    one_ior_ (0),
    one_to_n_test_ (0),
    context_switch_test_ (0),
    iors_ (0),
    iors_count_ (0),
    ior_file_ (0),
    granularity_ (1),
    use_utilization_test_ (0),
    high_priority_loop_count_ (0),
    semaphore_ (0),
    use_multiple_priority_ (0),
    ready_ (0),
    ready_cnd_ (ready_mtx_),
    remote_invocations_ (1),
    util_test_time_ (0)
{
}

int
Task_State::parse_args (int argc,ACE_TCHAR *argv[])
{
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("mu:n:t:d:rxof:g:1cl"));
  int c;

  while ((c = opts ()) != -1)
    switch (c) {
    case 'g':
      granularity_ = ACE_OS::atoi (opts.opt_arg ());
      if (granularity_ < 1)
        granularity_ = 1;
      break;
    case 'l':
      remote_invocations_ = 0;
      break;
    case 'c':
      context_switch_test_ = 1;
      break;
    case 'm':
      use_multiple_priority_ = 1;
      break;
    case '1':
      one_to_n_test_ = 1;
      break;
    case 'u':
      use_utilization_test_ = 1;
      loop_count_ = ACE_OS::atoi (opts.opt_arg ());
      break;
    case 'f':
      ior_file_ = ACE_OS::strdup (opts.opt_arg ());
      break;
    case 'o':
      oneway_ = 1;
      break;
    case 'x':
      shutdown_ = 1;
      break;
    case 'r':
      thread_per_rate_ = 1;
      break;
    case 'd':
      {
        int datatype = ACE_OS::atoi (opts.opt_arg ());
        switch (datatype)
          {
          case CB_OCTET:
            ACE_DEBUG ((LM_DEBUG,
                        "Testing Octets\n"));
            datatype_ = CB_OCTET;
            break;
          case CB_LONG:
            ACE_DEBUG ((LM_DEBUG,
                        "Testing Longs\n"));
            datatype_ = CB_LONG;
            break;
          case CB_STRUCT:
            ACE_DEBUG ((LM_DEBUG,
                        "Testing Structs\n"));
            datatype_ = CB_STRUCT;
            break;
          case CB_SHORT:
          default:
            ACE_DEBUG ((LM_DEBUG,
                        "Testing Shorts\n"));
            datatype_ = CB_SHORT;
            break;
          }
      }
      continue;
    case 'n':                   // loop count
      loop_count_ = (u_int) ACE_OS::atoi (opts.opt_arg ());
      continue;
    case 't':
      thread_count_ = (u_int) ACE_OS::atoi (opts.opt_arg ());
      continue;
    case '?':
    default:
      ACE_DEBUG ((LM_DEBUG, "usage:  %s\t"
                  "[<ORB OPTIONS>]        // ORB options, e.g., \"-ORBobjrefstyle url\"                               \n\t\t\t"
                  "[-d <datatype>]        // what datatype to use for calls:  Octet=0, Short=1, Long=2, Struct=3      \n\t\t\t"
                  "[-n <num_calls>]       // number of CORBA calls to make.                                           \n\t\t\t"
                  "[-t <num_of_clients>]  // number of client threads to create                                       \n\t\t\t"
                  "[-f <ior_file>]        // specify the file from which we read the object references (iors), if any.\n\t\t\t"
                  "[-r]                   // run thread-per-rate test.                                                \n\t\t\t"
                  "[-o]                   // makes client use oneway calls.  By default, twoway calls are used.       \n\t\t\t"
                  "[-x]                   // makes a call to servant to shutdown                                      \n\t\t\t"
                  "[-u <requests> ]       // run the client utilization test for a number of <requests>               \n\t\t\t"
                  "[-1]                   // run the one-to-n test.                                                   \n\t\t\t"
                  "[-g <granularity>]     // choose the granularity of the timing of CORBA calls                      \n\t\t\t"
                  "[-c]                   // run the number of context switches test.                                 \n\t\t\t"
                  "[-l]                   // use direct function calls, as opposed to CORBA requests.  ONLY to be used with -u option.\n\t\t\t"
                  "[-m]                   // use multiple priorities for the low priority clients.                   \n"
                  ,argv [0]));
      return -1;
    }

  if (thread_per_rate_ == 1)
    thread_count_ = THREAD_PER_RATE_OBJS;

  if (use_utilization_test_ == 1)
    {
      thread_count_ = 1;
      shutdown_ = 1;
      datatype_ = CB_OCTET;
    }

  // Allocate the array of character pointers.
  ACE_NEW_RETURN (iors_,
                  char *[thread_count_],
                  -1);

  if (ior_file_ != 0)
    {
      FILE *ior_file =
        ACE_OS::fopen (ior_file_, "r");

      if (ior_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Task_State::parse_args; "
                           "unable to open IOR file \"%s\"\n",
                           ior_file_),
                          -1);
      char buf[BUFSIZ];
      u_int i;

      for (i = 0;
           ACE_OS::fgets (buf, BUFSIZ, ior_file) != 0
             && i < thread_count_;
           i++)
        {
          ACE_DEBUG ((LM_DEBUG,
                      buf));
          int j = ACE_OS::strlen (buf);

          // This overwrites the '\n' that was read from the file.
          buf[j - 1] = 0;
          iors_[i] = ACE_OS::strdup (buf);
        }

      this->iors_count_ = i;
      ACE_OS::fclose (ior_file);
    }

  // thread_count_ + 2 because there is one utilization thread also
  // wanting to begin at the same time the clients begin && the main
  // thread wants to know when clients will start running to get
  // accurate context switch numbers.

  if (thread_per_rate_ == 0)
    {
      if (use_utilization_test_ == 1)
        // If we are to use the utilization test, include it in the
        // barrier count.  See description of this variable in header
        // file.
        ACE_NEW_RETURN (barrier_,
                        ACE_Barrier (thread_count_ + 2),
                        -1);
      else
        ACE_NEW_RETURN (barrier_,
                        ACE_Barrier (thread_count_ + 1),
                        -1);
    }
  else
    ACE_NEW_RETURN (this->barrier_,
                    ACE_Barrier (thread_count_),
                    -1);
  ACE_NEW_RETURN (this->semaphore_,
                  ACE_SYNCH_SEMAPHORE (0),
                  -1);
  ACE_NEW_RETURN (this->latency_,
                  ACE_timer_t [thread_count_],
                  -1);
  ACE_NEW_RETURN (this->global_jitter_array_,
                  JITTER_ARRAY *[this->thread_count_],
                  -1);
  ACE_NEW_RETURN (this->count_,
                  u_int [thread_count_],
                  -1);
  return 0;
}

Task_State::~Task_State (void)
{
  int i;

  if (this->ior_file_ != 0)
    ACE_OS::free (this->ior_file_);

  // Delete the strduped memory.
  for (i = 0; i < this->iors_count_; i++)
    ACE_OS::free (this->iors_ [i]);

  delete [] this->iors_;
  // Delete the barrier.

  delete this->barrier_;
  delete this->semaphore_;
  delete [] this->latency_;
  delete [] this->ave_latency_;
  delete [] this->global_jitter_array_;
  delete [] this->count_;
}

Client::Client (ACE_Thread_Manager *thread_manager,
                Task_State *ts,
                int argc,
                ACE_TCHAR **argv,
                u_int id)
  : ACE_Task<ACE_SYNCH> (thread_manager),
    cubit_impl_ (CORBA::ORB::_nil (),
                 PortableServer::POA::_nil ()),
    ts_ (ts),
    num_ (0),
    id_ (id),
    call_count_ (0),
    error_count_ (0),
    my_jitter_array_ (0),
    timer_ (0),
    frequency_ (0),
    latency_ (0),
    argc_ (argc),
    argv_ (argv)
{
}

Client::~Client (void)
{
  delete this->my_jitter_array_;
  delete this->timer_;
}

int
Client::func (u_int i)
{
  return i - 117;
}

void
Client::put_latency (JITTER_ARRAY *jitter,
                     ACE_timer_t latency,
                     u_int thread_id,
                     u_int count)
{
  ACE_MT (ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->ts_->lock_));

  this->ts_->latency_[thread_id] = latency;
  this->ts_->global_jitter_array_[thread_id] = jitter;
  this->ts_->count_[thread_id] = count;

  ACE_DEBUG ((LM_DEBUG,
              "(%t) My latency was %A msec\n",
              latency/ACE_ONE_SECOND_IN_MSECS));
}

// Returns the latency in usecs.
ACE_timer_t
Client::get_high_priority_latency (void)
{
  return (ACE_timer_t) this->ts_->latency_ [0];
}

// Returns the latency in usecs.
ACE_timer_t
Client::get_low_priority_latency (void)
{
  if (this->ts_->thread_count_ == 1)
    return 0;

  ACE_timer_t l = 0;

  for (u_int i = 1;
       i < this->ts_->thread_count_;
       i++)
    l += (ACE_timer_t) this->ts_->latency_[i];

  // Return the average latency for the low priority threads.
  return l / (ACE_timer_t) (this->ts_->thread_count_ - 1);
}

ACE_timer_t
Client::get_latency (u_int thread_id)
{
  return static_cast<ACE_timer_t> (this->ts_->latency_ [thread_id]);
}

// Returns the jitter in usecs.
ACE_timer_t
Client::get_high_priority_jitter (void)
{
  ACE_timer_t jitter = 0.0;
  ACE_timer_t average = get_high_priority_latency ();
  u_int number_of_samples = 0;

  // Compute the standard deviation, i.e., jitter, from the values
  // stored in the global_jitter_array_.

  ACE_Stats stats;

  // We first compute the sum of the squares of the differences each
  // latency has from the average.

  JITTER_ARRAY_ITERATOR iterator  =
    this->ts_->global_jitter_array_[0]->begin ();

  // latency in usecs.
  ACE_timer_t *latency = 0;

  for (iterator.first ();
       iterator.next (latency) == 1;
       iterator.advance ())
    {
      ++number_of_samples;

      ACE_timer_t difference = *latency - average;
      jitter += difference * difference;

      if (stats.sample (ACE_round (*latency)) == -1)
        ACE_DEBUG ((LM_DEBUG, "Error: stats.sample returned -1\n"));

    }

  // Return the square root of the sum of the differences computed
  // above, i.e., jitter.

  ACE_DEBUG ((LM_DEBUG,
              "high priority jitter (%u samples):\n", number_of_samples));

  ACE_DEBUG ((LM_DEBUG,"Latency stats (time in usec)\n"));
  stats.print_summary (1, 1, stderr);

  return sqrt (jitter / (number_of_samples - 1));
}

// Returns the jitter in usecs.

ACE_timer_t
Client::get_low_priority_jitter (void)
{
  if (this->ts_->thread_count_ == 1)
    return 0;

  ACE_timer_t jitter = 0.0;
  ACE_timer_t average = get_low_priority_latency ();
  u_int number_of_samples = 0;

  // Compute the standard deviation, i.e., jitter, from the values
  // stored in the global_jitter_array_.

  ACE_Stats stats;

  // We first compute the sum of the squares of the differences each
  // latency has from the average.

  for (u_int j = 1;
       j < this->ts_->thread_count_;
       j++)
    {
      ACE_DEBUG ((LM_DEBUG, "count: %u\n", ts_->count_[j]));

      JITTER_ARRAY_ITERATOR iterator =
        this->ts_->global_jitter_array_ [j]->begin ();

      ACE_timer_t number_of_calls =
        this->ts_->count_ [j] / this->ts_->granularity_;

      ACE_timer_t *latency = 0;

      u_int i = 0;

      for (iterator.first ();
           i < number_of_calls && iterator.next (latency) == 1;
           iterator.advance ())
        {
          ++number_of_samples;
          ACE_timer_t difference = *latency - average;
          jitter += difference * difference;
          stats.sample (ACE_round (*latency));
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              "low priority jitter (%u samples):\n", number_of_samples));
  ACE_DEBUG ((LM_DEBUG,"Latency stats (time in usec)\n"));
  stats.print_summary (1, 1, stderr);

  // Return the square root of the sum of the differences computed
  // above, i.e. jitter.
  return sqrt (jitter / (number_of_samples - 1));
}

ACE_timer_t
Client::get_jitter (u_int id)
{
  ACE_timer_t jitter = 0.0;
  ACE_timer_t average = get_latency (id);
  u_int number_of_samples = 0;

  // Compute the standard deviation, i.e., jitter, from the values
  // stored in the global_jitter_array_.

  ACE_Stats stats;

  // We first compute the sum of the squares of the differences each
  // latency has from the average.

  JITTER_ARRAY_ITERATOR iterator =
    this->ts_->global_jitter_array_ [id]->begin ();

  ACE_timer_t number_of_calls =
    this->ts_->count_[id] / this->ts_->granularity_;

  ACE_timer_t *latency = 0;

  u_int i = 0;

  for (iterator.first ();
       i < number_of_calls && iterator.next (latency) == 1;
       i ++,iterator.advance ())
    {
      ++number_of_samples;
      ACE_timer_t difference = *latency - average;
      jitter += difference * difference;
      stats.sample (ACE_round (*latency));
    }

  ACE_DEBUG ((LM_DEBUG,
             "jitter for thread id %u:\n", id));
  ACE_DEBUG ((LM_DEBUG,"Latency stats (time in usec)\n"));
  stats.print_summary (1, 1, stderr);

  // Return the square root of the sum of the differences computed
  // above, i.e. jitter.
  return sqrt (jitter / (number_of_samples - 1));
}

void
Client::find_frequency (void)
{
    if (this->ts_->thread_per_rate_ == 0)
      {
        if (this->id_ == 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%t) I'm the high priority client, my id is %d.\n",
                        this->id_));
            this->frequency_ = CB_HIGH_PRIORITY_RATE;
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%t) I'm a low priority client, my id is %d.\n",
                        this->id_));
            this->frequency_ = CB_LOW_PRIORITY_RATE;
          }
      }
    else
      switch (this->id_)
        {
        case CB_20HZ_CONSUMER:
          this->frequency_ = CB_20HZ_CONSUMER_RATE;
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) I'm a %u Hz frequency client, "
                      "my id is %u.\n",
                      CB_20HZ_CONSUMER_RATE,
                      this->id_));
          break;
        case CB_10HZ_CONSUMER:
          this->frequency_ = CB_10HZ_CONSUMER_RATE;
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) I'm a %u Hz frequency client, "
                      "my id is %u.\n",
                      CB_10HZ_CONSUMER_RATE,
                      this->id_));
          break;
        case CB_5HZ_CONSUMER:
          this->frequency_ = CB_5HZ_CONSUMER_RATE;
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) I'm a %u Hz frequency client, "
                      "my id is %u.\n",
                      CB_5HZ_CONSUMER_RATE,
                      this->id_));
          break;
        case CB_1HZ_CONSUMER:
          this->frequency_ = CB_1HZ_CONSUMER_RATE;
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) I'm a %u Hz frequency client, "
                      "my id is %u.\n",
                      CB_1HZ_CONSUMER_RATE,
                      this->id_));
          break;
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) Invalid Thread ID!!!!\n",
                      this->id_));
        }
}

CORBA::ORB_ptr
Client::init_orb (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "I'm thread %t\n"));


  // Convert the argv vector into a string.
  ACE_ARGV tmp_args (this->argv_);
  ACE_TCHAR tmp_buf[BUFSIZ];

  ACE_OS::strcpy (tmp_buf,
                  tmp_args.buf ());
  // Add the argument.
  ACE_OS::strcat (tmp_buf,
                  ACE_TEXT(" -ORBRcvSock 32768 ")
                  ACE_TEXT(" -ORBSndSock 32768 "));

  ACE_DEBUG ((LM_DEBUG,
              tmp_buf));

  // Convert back to argv vector style.
  ACE_ARGV tmp_args2 (tmp_buf);
  int argc = tmp_args2.argc ();
  ACE_TCHAR **argv = tmp_args2.argv ();

  char orbid[64];
  ACE_OS::sprintf (orbid, "orb:%d", this->id_);
  CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                        argv,
                                        orbid);

  if (this->id_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "parsing the arguments\n"));

      int result = this->ts_->parse_args (argc,
                                          argv);
      if (result != 0)
        return CORBA::ORB::_nil ();

      ACE_DEBUG ((LM_DEBUG,
                  "(%t)Arguments parsed successfully\n"));

      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ready_mon,
                        this->ts_->ready_mtx_,
                        CORBA::ORB::_nil ());
      this->ts_->ready_ = 1;
      this->ts_->ready_cnd_.broadcast ();
      ready_mon.release ();
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%t) ORB_init success\n"));
  return orb._retn ();
}

int
Client::get_cubit (CORBA::ORB_ptr orb)
{
  char *my_ior =
    this->ts_->use_utilization_test_ == 1
    ? this->ts_->one_ior_
    : this->ts_->iors_[this->id_];

  // If we are running the "1 to n" test make sure all low
  // priority clients use only 1 low priority servant.
  if (this->id_ > 0
      && this->ts_->one_to_n_test_ == 1)
    my_ior = this->ts_->iors_[1];

  if (my_ior == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Must specify valid ior filename with -f option\n"),
                      -1);

  CORBA::Object_var objref =
    orb->string_to_object (my_ior);

  if (CORBA::is_nil (objref.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%t) string_to_object Failed!\n"),
                      -1);

  // Narrow the CORBA::Object reference to the stub object,
  // checking the type along the way using _is_a.
  this->cubit_ = Cubit::_narrow (objref.in ());

  if (CORBA::is_nil (this->cubit_))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Create cubit failed\n"),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Binding succeeded\n"));

  CORBA::String_var str =
    orb->object_to_string (this->cubit_);

  ACE_DEBUG ((LM_DEBUG,
              "(%t) CUBIT OBJECT connected to <%C>\n",
              str.in ()));

  return 0;
}

int
Client::svc (void)
{
  try
    {
      // Initialize the ORB.
      CORBA::ORB_var orb = this->init_orb ();

      // Find the frequency of CORBA requests based on thread id.
      this->find_frequency ();

      // Get the cubit object from the file.
      int r = this->get_cubit (orb.in ());
      if (r != 0)
        return r;

      ACE_DEBUG ((LM_DEBUG,
                  "(%t) Waiting for other threads to "
                  "finish binding..\n"));

      // Wait for all the client threads to be initialized before going
      // any further.
      this->ts_->barrier_->wait ();
      ACE_DEBUG ((LM_DEBUG,
                  "(%t; %D) Everyone's done, here I go!!\n"));
      if (this->ts_->oneway_ == 1)
        ACE_DEBUG ((LM_DEBUG,
                    "(%t) **** USING ONEWAY CALLS ****\n"));

      // Perform the tests.
      int result = this->run_tests ();
      if (result != 0)
        return result;

      // release the semaphore
      if (this->ts_->thread_per_rate_ == 1
          && this->id_ == this->ts_->thread_count_ - 1)
        this->ts_->semaphore_->release (this->ts_->thread_count_ - 1);
      else
        this->ts_->semaphore_->release ();

      // shutdown the server if necessary.
      if (this->ts_->shutdown_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) CALLING SHUTDOWN() ON THE SERVANT\n"));
          this->cubit_->shutdown ();
        }

      CORBA::release (this->cubit_);
      this->cubit_ = 0;

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Task_Client::svc()");
    }

  // To avoid a memPartFree on VxWorks.  It will leak memory, though.
  ACE_THR_FUNC_RETURN status = 0;

  if (thr_mgr ())
    thr_mgr ()->exit (status, 1);
  else
    ACE_OS::thr_exit (status);

  return 0;
}

int
Client::cube_octet (void)
{
  try
    {
      this->call_count_++;
      // Cube an octet.
      CORBA::Octet arg_octet = func (this->num_);
      CORBA::Octet ret_octet = 0;

      START_QUANTIFY;

      if (this->ts_->use_utilization_test_ == 1 && this->ts_->remote_invocations_ == 0)
        ret_octet = this->cubit_impl_.cube_octet (arg_octet);
      else
        ret_octet = this->cubit_->cube_octet (arg_octet);

      STOP_QUANTIFY;

      // Perform the cube operation.
      arg_octet = arg_octet * arg_octet * arg_octet;

      if (arg_octet != ret_octet)
        {
          this->error_count_++;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "** cube_octet (%d) (--> %d)\n",
                             arg_octet,
                             ret_octet),
                            -1);
        }

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("call to cube_octet()\n");
      return -1;
    }
  return 0;
}

int
Client::cube_short (void)
{
  try
    {
      this->call_count_++;

      CORBA::Short arg_short = func (this->num_);
      CORBA::Short ret_short;

      START_QUANTIFY;
      ret_short = this->cubit_->cube_short (arg_short);
      STOP_QUANTIFY;
      arg_short = arg_short * arg_short * arg_short;

      if (arg_short != ret_short)
        {
          this->error_count_++;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "** cube_short (%d) (--> %d)\n",
                             arg_short ,
                             ret_short),
                            -1);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("call to cube_short\n");
      return -1;
    }
  return 0;
}

int
Client::cube_long (void)
{
  try
    {
      this->call_count_++;

      CORBA::Long arg_long = func (this->num_);
      CORBA::Long ret_long;

      START_QUANTIFY;
      ret_long = this->cubit_->cube_long (arg_long);
      STOP_QUANTIFY;

      arg_long = arg_long * arg_long * arg_long;

      if (arg_long != ret_long)
        {
          this->error_count_++;
          ACE_ERROR ((LM_ERROR,
                      "** cube_long (%d) (--> %d)\n",
                      arg_long,
                      ret_long));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("call to cube_long()\n");
      return -1;
    }
  return 0;
}

int
Client::cube_struct (void)
{
  try
    {
      Cubit::Many arg_struct;
      Cubit::Many ret_struct;

      this->call_count_++;

      arg_struct.l = func (this->num_);
      arg_struct.s = func (this->num_);
      arg_struct.o = func (this->num_);

      START_QUANTIFY;
      ret_struct = this->cubit_->cube_struct (arg_struct);
      STOP_QUANTIFY;

      arg_struct.l = arg_struct.l  * arg_struct.l  * arg_struct.l ;
      arg_struct.s = arg_struct.s  * arg_struct.s  * arg_struct.s ;
      arg_struct.o = arg_struct.o  * arg_struct.o  * arg_struct.o ;

      if (arg_struct.l  != ret_struct.l
          || arg_struct.s  != ret_struct.s
          || arg_struct.o  != ret_struct.o )
        {
          this->error_count_++;
          ACE_ERROR ((LM_ERROR,
                      "**cube_struct error!\n"));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("call to cube_struct()\n");
      return -1;
    }
  return 0;
}

int
Client::make_request (void)
{
  int result;

  if (this->ts_->oneway_ == 0)
    {
      switch (this->ts_->datatype_)
        {
        case CB_OCTET:
          result = this->cube_octet ();
          break;
          // Cube a short.
        case CB_SHORT:
          result = this->cube_short ();
          break;
          // Cube a long.
        case CB_LONG:
          result = this->cube_long ();
          break;
          // Cube a "struct" ...
        case CB_STRUCT:
          result = this->cube_struct ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t); %s:%d; unexpected datatype: %d\n",
                             this->ts_->datatype_), -1);
        }
      if (result != 0)
        return result;
    }
  else
    {
      try
        {
          this->call_count_++;
          START_QUANTIFY;
          this->cubit_->noop ();
          STOP_QUANTIFY;
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("oneway call noop()\n");
          return -1;
        }
    }
  // return success.
  return 0;
}

void
Client::print_stats (void)
{
  // Perform latency stats only if we are not running the utilization
  // tests.
  if (this->call_count_ > 0
      && this->ts_->use_utilization_test_ == 0)
    {
      if (this->error_count_ == 0)
        {
          // Latency is in usecs.
          ACE_timer_t calls_per_second =
            (this->call_count_ * ACE_ONE_SECOND_IN_USECS) / this->latency_;

          // Calculate average (per-call) latency in usecs.
          this->latency_ = this->latency_/this->call_count_;

          if (this->latency_ > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) cube average call ACE_OS::time\t= %A msec, \t"
                          "%A calls/second\n",
                          this->latency_ / ACE_ONE_SECOND_IN_MSECS,
                          calls_per_second));
              this->put_latency (this->my_jitter_array_,
                                 this->latency_,
                                 this->id_,
                                 this->call_count_);
            }
          else
            {
              // Still we have to call this function to store a valid
              // array pointer.
              this->put_latency (this->my_jitter_array_,
                                 0,
                                 this->id_,
                                 this->call_count_);

              ACE_DEBUG ((LM_DEBUG,
                "*** Warning: Latency, %f, is less than or equal to zero."
                "  Precision may have been lost.\n, this->latency_"));
            }
        }
      ACE_DEBUG ((LM_DEBUG,
                  "%d calls, %d errors\n",
                  this->call_count_,
                  this->error_count_));
    }
}

ACE_timer_t
Client::calc_delta (ACE_timer_t real_time,
                    ACE_timer_t delta)
{
  ACE_timer_t new_delta = 0.4 * fabs (real_time)  +  0.6 * delta;
  return new_delta;
}

int
Client::do_test (void)
{
  ACE_timer_t delta = 0;
  u_int low_priority_client_count = this->ts_->thread_count_ - 1;
  ACE_timer_t sleep_time = // usec
    (ACE_ONE_SECOND_IN_USECS * this->ts_->granularity_)/this->frequency_;
  u_int i;
  int result = 0;

  for (i = 0;
       // keep running for loop count, OR
       i < this->ts_->loop_count_
       // keep running if we are the highest priority thread and at
       // least another lower client thread is running, OR
       || (id_ == 0 && this->ts_->thread_count_ > 1)
       // keep running if test is thread_per_rate and we're not the
       // lowest frequency thread.
       || (this->ts_->thread_per_rate_ == 1
           && id_ < (this->ts_->thread_count_ - 1));
       i++)
    {
      // Start timing a call.
      if ((i % this->ts_->granularity_) == 0 &&
           this->ts_->use_utilization_test_ == 0)
        {
          // Delay a sufficient amount of time to be able to enforce
          // the calling frequency, e.g., 20Hz, 10Hz, 5Hz, 1Hz.
          ACE_Time_Value tv (0,
                             (u_long) (sleep_time < delta
                                       ? 0
                                       : sleep_time - delta));
          ACE_OS::sleep (tv);
          this->timer_->start ();
        }
      this->num_ = i;
      // make a request to the server object depending on the datatype.
      result = this->make_request ();
      if (result != 0)
        return 2;

      // Stop the timer.
      if (i % this->ts_->granularity_ == this->ts_->granularity_ - 1
          && this->ts_->use_utilization_test_ == 0)
        {
          this->timer_->stop ();

          // Calculate time elapsed.
          ACE_timer_t real_time;
          real_time = this->timer_->get_elapsed ();
          // Recalculate delta = 0.4 * elapsed_time + 0.6 *
          // delta. This is used to adjust the sleeping time so that
          // we make calls at the required frequency.
          delta = this->calc_delta (real_time,delta);
          this->latency_ += real_time * this->ts_->granularity_;

          if ((result = this->my_jitter_array_->enqueue_tail (real_time)) != 0)
            ACE_DEBUG ((LM_DEBUG, "(%t) Error: my_jitter_array->enqueue_tail() returned %d\n", result));
        }
      if (this->ts_->thread_per_rate_ == 1
          && id_ < (this->ts_->thread_count_ - 1))
        {
          if (this->ts_->semaphore_->tryacquire () != -1)
            break;
        }
      else
        // If we are the high priority client.  If tryacquire()
        // succeeded then a client must have done a release () on it,
        // thus we decrement the client counter.
        if (id_ == 0
            && this->ts_->thread_count_ > 1)
          {
            if (this->ts_->semaphore_->tryacquire () != -1)
              {
                low_priority_client_count --;
                // If all clients are done then break out of loop.
                if (low_priority_client_count <= 0)
                  break;
              }
          }

    } /* end of for () */
  ACE_DEBUG ((LM_DEBUG, "(%t; %D) do_test executed %u iterations\n", i));

  return 0;
}

int
Client::run_tests (void)
{
  int result;
  ACE_NEW_RETURN (this->my_jitter_array_,
                  JITTER_ARRAY,
                  -1);

  ACE_NEW_RETURN (this->timer_,
                  MT_Cubit_Timer (this->ts_->granularity_),
                  -1);
  if (this->ts_->use_utilization_test_ == 1)
    this->timer_->start ();

  // Make the calls in a loop.
  result = this->do_test ();
  if (result != 0)
    return result;

  if (id_ == 0)
    this->ts_->high_priority_loop_count_ =
      this->call_count_;

  if (this->ts_->use_utilization_test_ == 1)
    {
      this->timer_->stop ();
      this->ts_->util_test_time_ = this->timer_->get_elapsed ();
    }

  // Print the latency results.
  this->print_stats ();
  return 0;
}
