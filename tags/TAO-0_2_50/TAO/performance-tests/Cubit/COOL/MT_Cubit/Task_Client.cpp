// $Id$

#include "Task_Client.h"

ACE_RCSID(MT_Cubit, Task_Client, "$Id$")

Task_State::Task_State (int argc, char **argv)
  : start_count_ (0),
    loop_count_ (5),
    thread_count_ (5),
    base_port_ (5000),
    datatype_ (CB_OCTET),
    argc_ (argc),
    argv_ (argv),
    thread_per_rate_ (0),
    global_jitter_array_ (0),
    use_chorus_ipc_ (0),
    grain_ (1)
{
  int c;
  int datatype;

  // defaults
  ACE_OS::strcpy (server_host_, "localhost");
  ior_header_ = ACE_OS::strdup ("cool-tcp");
  ACE_Get_Opt opts (argc, argv, "Hh:n:t:p:d:rIg:");

  while ((c = opts ()) != -1)
    switch (c) {
    case 'g':
      grain_ = ACE_OS::atoi (opts.optarg);
      if (grain_ < 1)
        grain_ = 1;
      break;
    case 'I':
      use_chorus_ipc_ = 1;
      break;
    case 'r':
      thread_per_rate_ = 1;
      break;
    case 'd':
      datatype = ACE_OS::atoi (opts.optarg);
      switch (datatype)
        {
        case CB_OCTET:
          ACE_DEBUG ((LM_DEBUG, "Testing Octets\n"));
          datatype_ = CB_OCTET;
          break;
        case CB_LONG:
          ACE_DEBUG ((LM_DEBUG, "Testing Longs\n"));
          datatype_ = CB_LONG;
          break;
        case CB_STRUCT:
          ACE_DEBUG ((LM_DEBUG, "Testing Structs\n"));
          datatype_ = CB_STRUCT;
          break;
        case CB_SHORT:
        default:
          ACE_DEBUG ((LM_DEBUG, "Testing Shorts\n"));
          datatype_ = CB_SHORT;
          break;
        }
      continue;
    case 'h':
      ACE_OS::strcpy (server_host_, opts.optarg);
      continue;
    case 'p':
      base_port_ = ACE_OS::atoi (opts.optarg);
      continue;
    case 'n':                   // loop count
      loop_count_ = (u_int) ACE_OS::atoi (opts.optarg);
      continue;
    case 't':
      thread_count_ = (u_int) ACE_OS::atoi (opts.optarg);
      continue;
    case 'H':
      ACE_DEBUG ((LM_DEBUG, "usage:  %s"
                  "[-d datatype Octet=0, Short=1, Long=2, Struct=3]"
                  " [-n num_calls]"
                  " [-h server_hostname]"
                  " [-p server_port_num]"
                  " [-t num_threads]"
                  " [-I Use Chorus IPC. (For Chorus ClassiX *only*) ]"
                  " [-g granularity_of_request_timing]"
                  "\n", argv [0]));
      continue;
    }

  if (use_chorus_ipc_ == 1)
    {
      ior_header_ = ACE_OS::strdup ("cool-chorus");
      ACE_OS::strcpy (server_host_, "");
    }

  // thread_count_ + 1 because there is one utilization thread also
  // wanting to begin at the same time the clients begin..
  ACE_NEW (barrier_, ACE_Barrier (thread_count_ + 1));
  ACE_NEW (latency_, double [thread_count_]);
  ACE_NEW (global_jitter_array_, double *[thread_count_]);
}

Client::Client (Task_State *ts)
  : ACE_Task<ACE_MT_SYNCH> (ACE_Thread_Manager::instance ()),
    ts_ (ts)
{
}

void
Client::put_latency (double *jitter, double latency, u_int thread_id)
{
  ACE_MT (ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, ts_->lock_));

  ts_->latency_[thread_id] = latency;
  ts_->global_jitter_array_ [thread_id] = jitter;

#if defined (ACE_LACKS_FLOATING_POINT)
  ACE_DEBUG ((LM_DEBUG,
              "(%t) My latency was %u\n",
              latency));
#else
  ACE_DEBUG ((LM_DEBUG,
              "(%t) My latency was %f\n",
              latency));
#endif /* ! ACE_LACKS_FLOATING_POINT */
}

double
Client::get_high_priority_latency (void)
{
  return (double) ts_->latency_ [0];
}

double
Client::get_low_priority_latency (void)
{
  double l = 0;

  for (u_int i = 1; i < ts_->start_count_; i++)
    l += (double) ts_->latency_[i];
  return ts_->start_count_ > 1? l / (double) (ts_->start_count_ - 1) : 0;
}

int
Client::get_latency (u_int thread_id)
{
  return ts_->latency_ [thread_id];
}

double
Client::get_high_priority_jitter (void)
{
  double jitter = 0;
  double average = get_high_priority_latency ();

  // Compute the standard deviation (i.e. jitter) from the values
  // stored in the global_jitter_array_.

  // we first compute the sum of the squares of the differences
  // each latency has from the average
  for (u_int i = 0; i < ts_->loop_count_; i ++)
    {
      double difference =
        ts_->global_jitter_array_ [0][i] - average;
      jitter += difference * difference;
    }

  // Return the square root of the sum of the differences computed
  // above, i.e. jitter.
  return sqrt (jitter);
}

double
Client::get_low_priority_jitter (void)
{
  double jitter = 0;
  double average = get_low_priority_latency ();

  // Compute the standard deviation (i.e. jitter) from the values
  // stored in the global_jitter_array_.

  // We first compute the sum of the squares of the differences each
  // latency has from the average.
  for (u_int j = 1; j < ts_->start_count_; j ++)
      for (u_int i = 0; i < ts_->loop_count_; i ++)
        {
          double difference = ts_->global_jitter_array_ [j][i] - average;
          jitter += difference * difference;
        }

  // Return the square root of the sum of the differences computed
  // above, i.e. jitter.
  return sqrt (jitter);
}

int
Client::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) Thread created\n"));

  u_int thread_id;
  Cubit_ptr cb;
  char ior [1024];
  double frequency;
  CORBA::ORB_ptr orb_ptr;

  {
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, ts_->lock_, -1);

    thread_id = ts_->start_count_;
    ts_->start_count_++;

    if (ts_->thread_per_rate_ == 0)
      {
        if (thread_id == 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%t) Im the high priority client, my id is %d.\n",
                        thread_id));
            ACE_OS::sprintf (ior,
                             "%s://%s:%d",
                             ts_->ior_header_,
                             ts_->server_host_,
                             ts_->base_port_);
            frequency = CB_HIGH_PRIORITY_RATE;
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%t) Im a low priority client, my id is %d\n",
                        thread_id));
            ACE_OS::sprintf (ior,
                             "%s://%s:%d",
                             ts_->ior_header_,
                             ts_->server_host_,
                             ts_->base_port_);// + thread_id);
            frequency = CB_LOW_PRIORITY_RATE;
          }
      }
    else
      {
        switch (thread_id)
          {
/////////////////////////////////////
// THIS NEEDS TO BE UPDATED AS ABOVE.
/////////////////////////////////////
          case CB_40HZ_CONSUMER:
            ACE_DEBUG ((LM_DEBUG, "(%t) Im the high priority client, my id is %d.\n", thread_id));
            ::sprintf (ior, "cool-tcp://%s:%d", ts_->server_host_, ts_->base_port_);
            frequency = CB_40HZ_CONSUMER_RATE;
            break;
          case CB_20HZ_CONSUMER:
            ACE_DEBUG ((LM_DEBUG, "(%t) Im the high priority client, my id is %d.\n", thread_id));
            ::sprintf (ior, "cool-tcp://%s:%d", ts_->server_host_, ts_->base_port_);//+1);
            frequency = CB_20HZ_CONSUMER_RATE;
            break;
          case CB_10HZ_CONSUMER:
            ACE_DEBUG ((LM_DEBUG, "(%t) Im the high priority client, my id is %d.\n", thread_id));
            ::sprintf (ior, "cool-tcp://%s:%d", ts_->server_host_, ts_->base_port_);//+2);
            frequency = CB_10HZ_CONSUMER_RATE;
            break;
          case CB_5HZ_CONSUMER:
            ACE_DEBUG ((LM_DEBUG, "(%t) Im the high priority client, my id is %d.\n", thread_id));
            ::sprintf (ior, "cool-tcp://%s:%d", ts_->server_host_, ts_->base_port_);//+3);
            frequency = CB_5HZ_CONSUMER_RATE;
            break;
          case CB_1HZ_CONSUMER:
            ACE_DEBUG ((LM_DEBUG, "(%t) Im the high priority client, my id is %d.\n", thread_id));
            ::sprintf (ior, "cool-tcp://%s:%d", ts_->server_host_, ts_->base_port_);//+4);
            frequency = CB_1HZ_CONSUMER_RATE;
            break;
          default:
            ACE_DEBUG ((LM_DEBUG, "(%t) Invalid Thread ID.\n", thread_id));
          }
      } /* else */

    ACE_DEBUG ((LM_DEBUG, "Using ior = %s\n", ior));

    CORBA::Object_ptr objref = CORBA::Object::_nil ();
    CORBA::Environment env;

    orb_ptr = CORBA::ORB_init (ts_->argc_, ts_->argv_, 0, env);

    if (env.exception () != 0)
        ACE_ERROR_RETURN ((LM_ERROR,"%s:ORB initialization", env.exception ()), 2);

    //    CORBA_BOA_ptr oa_ptr = orb_ptr->OA_init (ts_->argc_, ts_->argv_, 0, env);
    //
    //    if (oa_ptr == 0)
    //      ACE_ERROR_RETURN ((LM_ERROR,
    //                         " (%P|%t) Unable to initialize the POA.\n"),
    //                        1);


    //
    // Initialize client's binding to an
    // arbitrary cubit server (at some host)
    //
    COOL::EOABindingData bindingData (ior);

    cb = Cubit::_bind(bindingData, env);

    //    objref = orb_ptr->string_to_object ((CORBA::String) ior, env);

    //    if (env.exception () != 0)
    //      ACE_ERROR_RETURN ((LM_ERROR,
    //                         "%s:string2object failed. Supply valid IOR with the -O option\n",
    //                         env.exception ()), 2);

    //    if (CORBA::is_nil (objref) == CORBA_TRUE)
    //      ACE_ERROR_RETURN ((LM_ERROR,
    //                         "%s:  must identify non-null target objref\n",
    //                         ts_->argv_ [0]),
    //                        1);

    // Narrow the CORBA::Object reference to the stub object, checking
    // the type along the way using _is_a.
    //    cb = Cubit::_narrow (objref);

   if ( (CORBA::is_nil (cb) == CORBA_TRUE) || (env.exception () != 0))
     {
       ACE_ERROR_RETURN ((LM_ERROR, "%s:Create cubit failed\n", env.exception ()), 1);
     }
    ACE_DEBUG ((LM_DEBUG, "(%t) Binding succeeded\n"));
    ACE_DEBUG ((LM_DEBUG, "(%t) Cb == %x\n", cb));

    CORBA::String str;

    str = orb_ptr->object_to_string (cb, env);

    if (env.exception () != 0)
      ACE_ERROR_RETURN ((LM_ERROR, "object_to_string %s\n", env.exception ()), -1);

    ACE_OS::puts ((char *) str);
    ACE_OS::fflush (stdout);
    ACE_DEBUG ((LM_DEBUG, "Object Created at: '%ul'", cb));
    ACE_DEBUG ((LM_DEBUG, "connected to object '%s'", str));
  }

  ACE_DEBUG ((LM_DEBUG, "(%t) Waiting for other threads to finish binding..\n"));
  ts_->barrier_->wait ();
  ACE_DEBUG ((LM_DEBUG, "(%t) Everyone's done, here I go!!\n"));

  this->run_tests (cb, ts_->loop_count_, thread_id, ts_->datatype_, frequency);

  // Free resources
  //  CORBA::release (orb_ptr);

  return 0;
}

int
Client::run_tests (Cubit_ptr cb,
                   u_int loop_count,
                   u_int thread_id,
                   Cubit_Datatypes datatype,
                   double frequency)
{
  CORBA::Environment env;
  u_int i = 0;
  u_int call_count = 0, error_count = 0;
  double *my_jitter_array;
  ACE_NEW_RETURN (my_jitter_array, double [ts_->loop_count_], 1);

  double latency = 0;
  double sleep_time = (1/frequency) * (1000 * 1000);
  double delta = 0;

  int pstartTime = 0;
  int pstopTime = 0;
  double real_time = 0.0;

#if defined (ACE_HAS_QUANTIFY)
  quantify_stop_recording_data();
  quantify_clear_data ();
#endif /* ACE_HAS_QUANTIFY */

  // Make the calls in a loop.
  for (i = 0; i < loop_count; i++)
    {
      ACE_High_Res_Timer timer_;
      ACE_Time_Value tv (0, (long int) (sleep_time - delta));
      ACE_OS::sleep (tv);

      // Elapsed time will be in microseconds.
      ACE_Time_Value delta_t;

#if defined (CHORUS)
      pstartTime = pccTime1Get();
#else /* CHORUS */
      timer_.start ();
#endif /* !CHORUS */

      switch (datatype)
        {
        case CB_OCTET:
          {
            // Cube an octet.
            CORBA::Octet arg_octet = func (i), ret_octet = 0;

#if defined (ACE_HAS_QUANTIFY)
                /* start recording quantify data from here */
                quantify_start_recording_data ();
#endif /* ACE_HAS_QUANTIFY */
            ret_octet = cb->cube_octet (arg_octet, env);

#if defined (ACE_HAS_QUANTIFY)
                quantify_stop_recording_data();
#endif /* ACE_HAS_QUANTIFY */

            if (env.exception () != 0)
              {
                CORBA::SystemException* ex;

                ex = CORBA::SystemException::_narrow(env.exception());
                if (ex)
                  {
                    CORBA::String_var msg = ex->message();
                    fprintf(stderr, "%s.\n", (const char*) msg);
                  }
                else
                  {
                    fprintf(stderr, "Unknown user exception.\n");
                  }
                ACE_ERROR_RETURN ((LM_ERROR,"%s:Call failed\n", env.exception ()), 2);
              }

            arg_octet = arg_octet * arg_octet * arg_octet;

            if (arg_octet != ret_octet)
              {
                ACE_DEBUG ((LM_DEBUG, "** cube_octet(%d)  (--> %d)\n", arg_octet , ret_octet));
                error_count++;
              }
            call_count++;
          }
        break;

        case CB_SHORT:
          // Cube a short.
          {
            call_count++;

            CORBA::Short arg_short = func (i), ret_short;

#if defined (ACE_HAS_QUANTIFY)
                // start recording quantify data from here.
                quantify_start_recording_data ();
#endif /* ACE_HAS_QUANTIFY */

            ret_short = cb->cube_short (arg_short, env);

#if defined (ACE_HAS_QUANTIFY)
                quantify_stop_recording_data();
#endif /* ACE_HAS_QUANTIFY */

            if (env.exception () != 0)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "%s:Call failed\n",
                                 env.exception ()),
                                2);

            arg_short = arg_short * arg_short * arg_short;

            if (arg_short != ret_short)
              {
                ACE_DEBUG ((LM_DEBUG, "** cube_short(%d)  (--> %d)\n", arg_short , ret_short));
                error_count++;
              }
            break;
          }
        // Cube a long.

        case CB_LONG:
          {
            call_count++;

            CORBA::Long arg_long = func (i), ret_long;

#if defined (ACE_HAS_QUANTIFY)
                // start recording quantify data from here.
                quantify_start_recording_data ();
#endif /* ACE_HAS_QUANTIFY */

            ret_long = cb->cube_long (arg_long, env);

#if defined (ACE_HAS_QUANTIFY)
                quantify_stop_recording_data();
#endif /* ACE_HAS_QUANTIFY */

            if (env.exception () != 0)
              ACE_ERROR_RETURN ((LM_ERROR,"%s:Call failed\n", env.exception ()), 2);

            arg_long = arg_long * arg_long * arg_long;

            if (arg_long != ret_long)
              {
                ACE_DEBUG ((LM_DEBUG, "** cube_long(%d)  (--> %d)\n", arg_long , ret_long));
                error_count++;
              }
            break;
          }

        case CB_STRUCT:
          // Cube a "struct" ...
          {
            Cubit::Many arg_struct, ret_struct;

            call_count++;

            arg_struct.l = func (i);
            arg_struct.s = func (i);
            arg_struct.o = func (i);

#if defined (ACE_HAS_QUANTIFY)
                // start recording quantify data from here.
                quantify_start_recording_data ();
#endif /* ACE_HAS_QUANTIFY */

            ret_struct = cb->cube_struct (arg_struct, env);

#if defined (ACE_HAS_QUANTIFY)
                quantify_stop_recording_data();
#endif /* ACE_HAS_QUANTIFY */

            if (env.exception () != 0)
              ACE_ERROR_RETURN ((LM_ERROR,"%s:Call failed\n", env.exception ()), 2);

            arg_struct.l = arg_struct.l  * arg_struct.l  * arg_struct.l ;
            arg_struct.s = arg_struct.s  * arg_struct.s  * arg_struct.s ;
            arg_struct.o = arg_struct.o  * arg_struct.o  * arg_struct.o ;

            if (arg_struct.l  != ret_struct.l
              || arg_struct.s  != ret_struct.s
              || arg_struct.o  != ret_struct.o )
              {
                ACE_DEBUG ((LM_DEBUG, "**cube_struct error!\n"));
                error_count++;
              }

            break;
          }
        }

      // use sysBench when CHORUS defined and option specified on command line
#if defined (CHORUS)
      if ( (loop_count % ts_->grain_) == 0)
          pstopTime = pccTime1Get();
#else /* CHORUS */
      // if CHORUS is not defined just use plain timer_.stop ().
      timer_.stop ();
      timer_.elapsed_time (delta_t);
#endif /* !CHORUS */

      // Calculate time elapsed
#if defined (ACE_LACKS_FLOATING_POINT)
#   if defined (CHORUS)
      real_time = pstopTime - pstartTime;
      my_jitter_array [i/ts_->grain_] = real_time; // in units of microseconds.
          // update the latency array, correcting the index using the granularity
#   else /* CHORUS */
      // Store the time in usecs.
      real_time = delta_t.sec () * ACE_ONE_SECOND_IN_USECS  +
        delta_t.usec ();
      my_jitter_array [i] = real_time; // in units of microseconds.
#   endif /* !CHORUS */
      delta = ((40 * fabs (real_time) / 100) + (60 * delta / 100)); // pow(10,6)
      latency += real_time;
#else /* ACE_LACKS_FLOATING_POINT */
      // Store the time in secs.
      real_time = delta_t.sec () + (double)delta_t.usec () / ACE_ONE_SECOND_IN_USECS;
      delta = ((0.4 * fabs (real_time * (1000 * 1000))) + (0.6 * delta)); // pow(10,6)
      latency += real_time;
      my_jitter_array [i] = real_time * 1000;
#endif /* !ACE_LACKS_FLOATING_POINT */
    }

  if (call_count > 0)
    {
      if (error_count == 0)
        {
#if defined (ACE_LACKS_FLOATING_POINT)
          double calls_per_second = (call_count * ACE_ONE_SECOND_IN_USECS) / latency;
#endif /* ACE_LACKS_FLOATING_POINT */

          latency /= call_count;

          if (latency > 0)
            {
#if defined (ACE_LACKS_FLOATING_POINT)
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) cube average call ACE_OS::time\t= %u usec, \t"
                          "%u calls/second\n",
                          latency,
                          calls_per_second));

              this->put_latency (my_jitter_array,
                                 latency,
                                 thread_id);
#else
              ACE_DEBUG ((LM_DEBUG, "(%P|%t) cube average call ACE_OS::time\t= %f msec, \t"
                              "%f calls/second\n",
                              latency * 1000,
                              1 / latency));
              this->put_latency (my_jitter_array,
                                 latency * 1000,
                                 thread_id);
#endif /* ! ACE_LACKS_FLOATING_POINT */
            }
          else
            {
              // still we have to call this function to store a valid array pointer.
              this->put_latency (my_jitter_array,
                                 0,
                                 thread_id);
              ACE_DEBUG ((LM_DEBUG,
                          "*** Warning: Latency is less than or equal to zero."
                          "  Precision may have been lost.\n"));
            }
        }
      ACE_DEBUG ((LM_DEBUG,
                  "%d calls, %d errors\n",
                  call_count,
                  error_count));
    }

  // cb->please_exit (env);
  return 0;
}
