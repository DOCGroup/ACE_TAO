// $Id$

#include "Task_Client.h"
#include "Timer.h"
#include "ace/Stats.h"

#if defined (NO_ACE_QUANTIFY)
#include "quantify.h"
#endif /* NO_ACE_QUANTIFY */

ACE_RCSID(MT_Cubit, Task_Client, "$Id$")

Task_State::Task_State (int argc, char **argv)
  : key_ ("Cubit"),
    loop_count_ (1000),
    thread_count_ (2),
    datatype_ (CB_OCTET),
    argc_ (ACE_static_cast (u_int, argc)),
    argv_ (argv),
    thread_per_rate_ (0),
    global_jitter_array_ (0),
    shutdown_ (0),
    oneway_ (0),
    use_name_service_ (1),
    one_to_n_test_ (0),
    context_switch_test_ (0),
    ior_file_ (0),
    granularity_ (1),
    use_utilization_test_ (0),
    high_priority_loop_count_ (0),
    use_multiple_priority_ (0),
    utilization_task_started_ (0),
    util_time_ (0),
    ready_ (0),
    ready_cnd_ (ready_mtx_),
    remote_invocations_ (1),
    util_test_time_ (0)
{
}

int
Task_State::parse_args (int argc,char **argv)
{
  ACE_Get_Opt opts (argc, argv, "U:mu:sn:t:d:rxof:g:1cl");
  int c;
  int datatype;

  while ((c = opts ()) != -1)
    switch (c) {
    case 'g':
      granularity_ = ACE_OS::atoi (opts.optarg);
      if (granularity_ < 1)
        granularity_ = 1;
      break;
    case 'l':
      remote_invocations_ = 0;
      break;
    case 'c':
      context_switch_test_ = 1;
      break;
    case '1':
      one_to_n_test_ = 1;
      break;
    case 'u':
      use_utilization_test_ = 1;
      loop_count_ = util_time_ = ACE_OS::atoi (opts.optarg);
      break;
    case 's':
      use_name_service_ = 0;
      break;
    case 'f':
      ior_file_ = ACE_OS::strdup (opts.optarg);
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
      datatype = ACE_OS::atoi (opts.optarg);
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
      continue;
    case 'n':                   // loop count
      loop_count_ = (u_int) ACE_OS::atoi (opts.optarg);
      continue;
    case 't':
      thread_count_ = (u_int) ACE_OS::atoi (opts.optarg);
      continue;
    case '?':
    default:
      ACE_DEBUG ((LM_DEBUG, "usage:  %s"
                  "[-d datatype Octet=0, Short=1, Long=2, Struct=3]"
                  " [-n num_calls]"
                  " [-t num_threads]"
                  " [-f ior_file]"
                  " [-x] // makes a call to servant to shutdown"
                  " [-o] // makes client use oneway calls instead"
                  " [-s] // makes client *NOT* use the name service"
                  " [-g granularity_of_timing]"
                  "\n", this->argv_ [0]));
      return -1;
    }

  if (thread_per_rate_ == 1)
    thread_count_ = THREAD_PER_RATE_OBJS;

  if (use_utilization_test_ == 1)
    {
      thread_count_ = 1;
      shutdown_ = 1;
    }

  // Allocate the array of character pointers.
  ACE_NEW_RETURN (iors_,
                  char *[thread_count_],
                  -1);

  if (ior_file_ != 0)
    {
      FILE *ior_file = ACE_OS::fopen (ior_file_, "r");

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
                  ACE_Thread_Semaphore (0),
                  -1);
  ACE_NEW_RETURN (this->latency_,
                  ACE_timer_t [thread_count_],
                  -1);
  ACE_NEW_RETURN (this->global_jitter_array_,
                  ACE_timer_t *[thread_count_],
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

  // Delete the barrier.

  delete this->barrier_;
  delete this->semaphore_;
  delete [] this->latency_;
  delete [] this->global_jitter_array_;
  delete [] this->count_;
}

Client::Client (ACE_Thread_Manager *thread_manager,
                Task_State *ts,
                u_int id)
  : ACE_MT (ACE_Task<ACE_MT_SYNCH> (thread_manager)),
    cubit_impl_ (0),
    ts_ (ts),
    id_ (id),
    frequency_ (0),
    naming_success_ (0)
{
}

ACE_INLINE int
Client::func (u_int i)
{
  return i - 117;
}

void
Client::put_latency (ACE_timer_t *jitter,
                     ACE_timer_t latency,
                     u_int thread_id,
                     u_int count)
{
  ACE_MT (ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->ts_->lock_));

  this->ts_->latency_[thread_id] = latency;
  this->ts_->global_jitter_array_[thread_id] = jitter;
  this->ts_->count_[thread_id] = count;

  ACE_DEBUG ((LM_DEBUG,
              "(%t) My latency was %A msec\n",
              latency));
}

ACE_timer_t
Client::get_high_priority_latency (void)
{
  return (ACE_timer_t) this->ts_->latency_ [0];
}

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
  return ACE_static_cast (ACE_timer_t,
                          this->ts_->latency_ [thread_id]);
}

ACE_timer_t
Client::get_high_priority_jitter (void)
{
  ACE_timer_t jitter = 0.0;
  ACE_timer_t average = get_high_priority_latency ();
  ACE_timer_t number_of_samples =
    this->ts_->high_priority_loop_count_ / this->ts_->granularity_;

  // Compute the standard deviation (i.e. jitter) from the values
  // stored in the global_jitter_array_.

  ACE_Stats stats;

  // We first compute the sum of the squares of the differences each
  // latency has from the average.

  for (u_int i = 0; i < number_of_samples; i ++)
    {
      ACE_timer_t difference =
        this->ts_->global_jitter_array_ [0][i] - average;

      jitter += difference * difference;

      stats.sample ((ACE_UINT32) (this->ts_->global_jitter_array_ [0][i] * 1000 + 0.5));
    }

  // Return the square root of the sum of the differences computed
  // above, i.e. jitter.

  ACE_DEBUG ((LM_DEBUG,
              "high priority jitter:\n"));

  stats.print_summary (3, 1000, stderr);

  return sqrt (jitter / (number_of_samples - 1));
}

ACE_timer_t
Client::get_low_priority_jitter (void)
{
  if (this->ts_->thread_count_ == 1)
    return 0;

  ACE_timer_t jitter = 0.0;
  ACE_timer_t average = get_low_priority_latency ();
  ACE_timer_t number_of_samples = 0;

  // Compute the standard deviation (i.e. jitter) from the values
  // stored in the global_jitter_array_.

  ACE_Stats stats;

  // We first compute the sum of the squares of the differences each
  // latency has from the average.
  for (u_int j = 1;
       j < this->ts_->thread_count_;
       j++)
    {
      number_of_samples += this->ts_->count_[j];

      for (u_int i = 0;
           i < this->ts_->count_[j] / this->ts_->granularity_;
           i ++)
        {
          ACE_timer_t difference =
            this->ts_->global_jitter_array_[j][i] - average;

          jitter += difference * difference;

          stats.sample ((ACE_UINT32) (this->ts_->global_jitter_array_ [j][i] * 1000 + 0.5));
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              "low priority jitter:\n"));

  stats.print_summary (3, 1000, stderr);

  // Return the square root of the sum of the differences computed
  // above, i.e. jitter.
  return sqrt (jitter / (number_of_samples - 1));
}

ACE_timer_t
Client::get_jitter (u_int id)
{
  ACE_timer_t jitter = 0.0;
  ACE_timer_t average = get_latency (id);
  ACE_timer_t number_of_samples = this->ts_->count_[id]  / this->ts_->granularity_;

  // Compute the standard deviation (i.e. jitter) from the values
  // stored in the global_jitter_array_.

  ACE_Stats stats;

  // We first compute the sum of the squares of the differences each
  // latency has from the average.

  for (u_int i = 0;
       i < this->ts_->count_[id] / this->ts_->granularity_;
       i ++)
    {
      ACE_timer_t difference =
        this->ts_->global_jitter_array_[id][i] - average;

      jitter += difference * difference;

      stats.sample ((ACE_UINT32) (this->ts_->global_jitter_array_ [id][i] * 1000 + 0.5));
    }

  ACE_DEBUG ((LM_DEBUG,
             "jitter for thread id %d:\n", id));

  stats.print_summary (3, 1000, stderr);

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

int
Client::init_orb (void)
{
  TAO_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
              "I'm thread %t\n"));

  // Add "-ORBobjrefstyle url" argument to the argv vector for the orb
  // to / use a URL style to represent the ior.

  // Convert the argv vector into a string.
  ACE_ARGV tmp_args (this->ts_->argv_);
  char tmp_buf[BUFSIZ];

  ACE_OS::strcpy (tmp_buf,
                  tmp_args.buf ());
  // Add the argument.
  ACE_OS::strcat (tmp_buf,
                  " -ORBobjrefstyle url "
                  " -ORBrcvsock 32768 "
                  " -ORBsndsock 32768 ");

  ACE_DEBUG ((LM_DEBUG,
              tmp_buf));

  // Convert back to argv vector style.
  ACE_ARGV tmp_args2 (tmp_buf);
  int argc = tmp_args2.argc ();
  char **argv = tmp_args2.argv ();

  this->orb_ = CORBA::ORB_init (argc,
                         argv,
                         "internet",
                         TAO_TRY_ENV);
  TAO_CHECK_ENV;
  if (this->id_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "parsing the arguments\n"));

      int result = this->ts_->parse_args (argc,
					  argv);
      if (result < 0)
        return -1;

      ACE_DEBUG ((LM_DEBUG,
                  "(%t)Arguments parsed successfully\n"));

      ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ready_mon, this->ts_->ready_mtx_, 1));
      this->ts_->ready_ = 1;
      this->ts_->ready_cnd_.broadcast ();
      ready_mon.release ();
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%t) ORB_init success\n"));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Client::Orb_init ()");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

int
Client::get_cubit_from_naming (void)
{
  CORBA::Object_var objref (0);
  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->ts_->lock_, -1));
  TAO_TRY
    {
      // Initialize the naming services.
      if (my_name_client_.init (this->orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);

      // If the naming service was resolved successsfully ...
      if (!CORBA::is_nil (this->my_name_client_.get_context ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      " (%t) ----- Using the NameService resolve() method"
                      " to get cubit objects -----\n"));

          // Construct the key for the name service lookup.
          CosNaming::Name mt_cubit_context_name (1);
          mt_cubit_context_name.length (1);
          mt_cubit_context_name[0].id =
            CORBA::string_dup ("MT_Cubit");

          objref =
            this->my_name_client_->resolve (mt_cubit_context_name,
                                            TAO_TRY_ENV);
          TAO_CHECK_ENV;

          this->mt_cubit_context_ =
            CosNaming::NamingContext::_narrow (objref.in (),
                                               TAO_TRY_ENV);
          TAO_CHECK_ENV;

          char *buffer;
          int l = ACE_OS::strlen (this->ts_->key_) + 3;
          ACE_NEW_RETURN (buffer,
                          char[l],
                          -1);

          ACE_OS::sprintf (buffer,
                           "%s%02d",
                           (char *) this->ts_->key_,
                           this->id_);

          // Construct the key for the name service lookup.
          CosNaming::Name cubit_name (1);
          cubit_name.length (1);
          cubit_name[0].id = CORBA::string_dup (buffer);

          objref = this->mt_cubit_context_->resolve (cubit_name,
                                                     TAO_TRY_ENV);

          if (TAO_TRY_ENV.exception () != 0
              || CORBA::is_nil (objref.in ()))
            {
              ACE_DEBUG ((LM_DEBUG,
                          " (%t) resolve() returned nil\n"));
              TAO_TRY_ENV.print_exception 
                ("Attempt to resolve() a cubit object"
                 "using the name service Failed!\n");
            }
          else
            {
              this->naming_success_ = 1;
              ACE_DEBUG ((LM_DEBUG,
                          " (%t) Cubit object resolved to the name \"%s\".\n",
                          buffer));
            }
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Client::get_cubit_from_naming");
      return 1;
    }
  TAO_ENDTRY;
  return 0;
}

int
Client::get_cubit (void)
{
  int result;
  CORBA::Object_var objref (0);

  TAO_TRY
    {
  if (this->ts_->use_name_service_ != 0)
    result = this->get_cubit_from_naming ();
  else
    {
      if (this->naming_success_ == 0)
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
                                 "Must specify valid factory ior key with -k option,"
                                 " naming service, or ior filename\n"),
                                -1);
            ACE_DEBUG ((LM_DEBUG,
			"(%P|%t) The ior I'm using is: \"%s\"\n",
			my_ior));

            // If we are running the "1 to n" test make sure all low
            // priority clients use only 1 low priority servant.
            if (this->id_ > 0 
                && this->ts_->one_to_n_test_ == 1)
              my_ior = this->ts_->iors_[1];

            if (my_ior == 0)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Must specify valid factory ior key with -k option,"
                                 " naming service, or ior filename\n"),
                                -1);
            objref = this->orb_->string_to_object (my_ior,
                                            TAO_TRY_ENV);
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t)  String_to_object success\n"));
            TAO_CHECK_ENV;
          }

        if (CORBA::is_nil (objref.in ()))
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%t) string_to_object or NameService->resolve() Failed!\n"),
                            -1);

        // Narrow the CORBA::Object reference to the stub object,
        // checking the type along the way using _is_a.
        this->cubit_ = Cubit::_narrow (objref.in (),
                                       TAO_TRY_ENV);
        TAO_CHECK_ENV;

        ACE_DEBUG ((LM_DEBUG,
                    "(%t) _narrow done\n"));

        if (CORBA::is_nil (this->cubit_))
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Create cubit failed\n"),
                            1);

        ACE_DEBUG ((LM_DEBUG,
                    "(%t) Binding succeeded\n"));

        CORBA::String_var str =
          this->orb_->object_to_string (this->cubit_, TAO_TRY_ENV);
        TAO_CHECK_ENV;

        ACE_DEBUG ((LM_DEBUG,
                    "(%t) CUBIT OBJECT connected <%s>\n",
                    str.in ()));
    }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Client::get_cubit");
      return 1;
    }
  TAO_ENDTRY;
  return 0;
}

int
Client::svc (void)
{
  int result;
  // initialize the ORB.
  result = this->init_orb ();
  if (result < 0)
    return result;
  // find the frequency of CORBA requests based on thread id.
  this->find_frequency ();
  // get the cubit object either from naming service or from the ior file.
  result = this->get_cubit ();
  if (result < 0)
    return result;
  ACE_DEBUG ((LM_DEBUG,
              "(%t) Waiting for other threads to "
              "finish binding..\n"));
  // Wait for all the client threads to be initialized before going
  // any further.
  this->ts_->barrier_->wait ();
  ACE_DEBUG ((LM_DEBUG,
              "(%t) Everyone's done, here I go!!\n"));
  if (this->ts_->oneway_ == 1)
    ACE_DEBUG ((LM_DEBUG,
                "(%t) **** USING ONEWAY CALLS ****\n"));

  // Perform the tests.
  result = this->run_tests ();
  if (result < 0)
    return result;
  // release the semaphore
  if (this->ts_->thread_per_rate_ == 1
      && this->id_ == this->ts_->thread_count_ - 1)
    this->ts_->semaphore_->release (this->ts_->thread_count_ - 1);
  else
    this->ts_->semaphore_->release ();
  // shutdown the server if necessary.
  TAO_TRY
    {
      if (this->ts_->shutdown_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) CALLING SHUTDOWN() ON THE SERVANT\n"));
          this->cubit_->shutdown (TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCHANY
    {
     ACE_ERROR ((LM_ERROR,
                      "Shutdown of the server failed!\n"));
          TAO_TRY_ENV.print_exception ("shutdown() call failed.\n");
    }
  TAO_ENDTRY;
  // Delete dynamic memory
  CORBA::release (this->cubit_);
  return 0;
}

int 
Client::cube_octet (void)
{
  TAO_TRY
    {
      // Cube an octet.
      CORBA::Octet arg_octet = func (this->num_);
      CORBA::Octet ret_octet = 0;

      START_QUANTIFY;

      if (this->ts_->remote_invocations_ == 1)
        ret_octet = this->cubit_->cube_octet (arg_octet,
                                              TAO_TRY_ENV);
      else
        ret_octet = this->cubit_impl_->cube_octet (arg_octet,
                                                   TAO_TRY_ENV);

      STOP_QUANTIFY;
      TAO_CHECK_ENV;

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
      this->call_count_++;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("call to cube_octet()\n");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: Call failed\n",
                         TAO_TRY_ENV.exception ()),
                        -1);
    }
  TAO_ENDTRY;
  return 0;
}

int
Client::cube_short (void)
{
  TAO_TRY
    {
      this->call_count_++;

      CORBA::Short arg_short = func (this->num_);
      CORBA::Short ret_short;

      START_QUANTIFY;
      ret_short = this->cubit_->cube_short (arg_short,
                                            TAO_TRY_ENV);
      STOP_QUANTIFY;
      TAO_CHECK_ENV;
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
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("call to cube_short\n");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: Call failed\n",
                         TAO_TRY_ENV.exception ()),
                        -1);
    }
  TAO_ENDTRY;
  return 0;
}

int
Client::cube_long (void)
{
  TAO_TRY
    {
      this->call_count_++;

      CORBA::Long arg_long = func (this->num_);
      CORBA::Long ret_long;

      START_QUANTIFY;
      ret_long = this->cubit_->cube_long (arg_long,
                                          TAO_TRY_ENV);
      STOP_QUANTIFY;
      TAO_CHECK_ENV;

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
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("call to cube_long()\n");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: Call failed\n",
                         TAO_TRY_ENV.exception ()),
                        -1);
    }
  TAO_ENDTRY;
  return 0;
}

int
Client::cube_struct (void)
{
  TAO_TRY
    {
      Cubit::Many arg_struct;
      Cubit::Many ret_struct;

      this->call_count_++;

      arg_struct.l = func (this->num_);
      arg_struct.s = func (this->num_);
      arg_struct.o = func (this->num_);

      START_QUANTIFY;
      ret_struct = this->cubit_->cube_struct (arg_struct,
                                              TAO_TRY_ENV);
      STOP_QUANTIFY;
      TAO_CHECK_ENV;

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
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception 
        ("call to cube_struct()\n");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: Call failed\n", TAO_TRY_ENV.exception ()),
                        -1);
    }
  TAO_ENDTRY;
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
      if (result < 0)
        return result;
    }
  else
    {
      TAO_TRY
        {
          this->call_count_++;
          START_QUANTIFY;
          this->cubit_->noop (TAO_TRY_ENV);
          STOP_QUANTIFY;
          TAO_CHECK_ENV;
        }
      TAO_CATCHANY
        {
          TAO_TRY_ENV.print_exception ("oneway call noop()\n");
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%t) noop() call failed\n"),
                            -1);
        }
      TAO_ENDTRY;
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
          ACE_timer_t calls_per_second =
            TIME_IN_MICROSEC (this->call_count_) / this->latency_;

          // Calculate average this->latency_.
          this->latency_ = this->latency_/this->call_count_;

          if (this->latency_ > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) cube average call ACE_OS::time\t= %A usec, \t"
                          "%A calls/second\n",
                          this->latency_,
                          calls_per_second));
              this->put_latency (this->my_jitter_array_,
                                 TIME_IN_MICROSEC (this->latency_),
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

int
Client::do_test (void)
{
  u_int i;
  int result;
  ACE_timer_t delta = 0;
  u_int low_priority_client_count = this->ts_->thread_count_ - 1;
  ACE_timer_t sleep_time = // usec
    (1 / this->frequency_) * ACE_ONE_SECOND_IN_USECS * this->ts_->granularity_; 

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
          // the calling frequency (i.e., 20Hz, 10Hz, 5Hz, 1Hz).
          ACE_Time_Value tv (0,
                             (u_long) (sleep_time - delta < 0
                                       ? 0
                                       : sleep_time - delta));
          ACE_OS::sleep (tv);
          this->timer_->start ();
        }
      this->num_ = i;
      // make a request to the server object depending on the datatype.
      result = this->make_request ();

      if (result < 0)
        return 2;

      // Stop the timer.
      if ((i % this->ts_->granularity_) == this->ts_->granularity_ - 1 
          && this->ts_->use_utilization_test_ == 0)
        {
          this->timer_->stop ();

          // Calculate time elapsed.
          ACE_timer_t real_time;
          real_time = this->timer_->get_elapsed ();

          // Recalculate delta = 0.4 * elapsed_time + 0.6 *
          // delta. This is used to adjust the sleeping time so that
          // we make calls at the required frequency.
          delta = 
            (ACE_timer_t) 40 * fabs (TIME_IN_MICROSEC (real_time))
            / (ACE_timer_t) 100 + (ACE_timer_t) 60 * delta/ 100;

          this->latency_ += real_time * this->ts_->granularity_;

          this->my_jitter_array_ [i/this->ts_->granularity_] =
            TIME_IN_MICROSEC (real_time);
        } 
      if (this->ts_->thread_per_rate_ == 1 
          && id_ < (this->ts_->thread_count_ - 1))
        {
          if (this->ts_->semaphore_->tryacquire () != -1)
            break;
        }
      else
        // if We are the high priority client.
        // if tryacquire() succeeded then a client must have done a
        // release () on it, thus we decrement the client counter.
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
  return 0;
}

int
Client::run_tests (void)
{
  int result;
  // %% Naga, This has to be replaced by ACE_Array .Thanx to Sergio for the idea.
  if (id_ == 0 
      && this->ts_->thread_count_ > 1)
    ACE_NEW_RETURN (this->my_jitter_array_,
                    ACE_timer_t [(this->ts_->loop_count_/this->ts_->granularity_)*30],
                    -1);
  else
    ACE_NEW_RETURN (this->my_jitter_array_,
                    ACE_timer_t [(this->ts_->loop_count_/this->ts_->granularity_)*15],
                    -1);

  // Time to wait for utilization tests to know when to stop.
  ACE_Time_Value max_wait_time (this->ts_->util_time_, 0);
  ACE_Countdown_Time countdown (&max_wait_time);

  ACE_NEW_RETURN (this->timer_,
                  MT_Cubit_Timer (this->ts_->granularity_),
                  -1);
  if (this->ts_->use_utilization_test_ == 1)
    this->timer_->start ();

  // Make the calls in a loop.
  if ((result = this->do_test ()) != 0)
    return result;

  if (id_ == 0)
    this->ts_->high_priority_loop_count_ =
      this->call_count_;
  if (this->ts_->use_utilization_test_ == 1)
    {
      this->timer_->stop ();
      ACE_timer_t util_time = this->timer_->get_elapsed ();
      this->ts_->util_test_time_ = util_time;
    }
  this->print_stats ();
  // Delete the dynamically allocated memory
  delete [] this->my_jitter_array_;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Condition<ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# pragma instantiate ACE_Condition<ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


