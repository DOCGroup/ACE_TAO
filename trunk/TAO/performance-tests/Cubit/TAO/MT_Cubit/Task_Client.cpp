// $Id$

#include "Task_Client.h"

Task_State::Task_State (int argc, char **argv)
  : key_ ("Cubit"),
    start_count_ (0),
    loop_count_ (1000),
    thread_count_ (2),
    datatype_ (CB_OCTET),
    argc_ (argc),
    argv_ (argv),
    thread_per_rate_ (0),
    global_jitter_array_ (0),
    factory_ior_ (0),
    shutdown_ (0),
    oneway_ (0),
    use_name_service_ (1),
    ior_file_ (0),
    granularity_ (1),
    use_utilization_test_ (0)
{
  ACE_Get_Opt opts (argc, argv, "usn:t:d:rk:xof:g:");
  int c;
  int datatype;

  while ((c = opts ()) != -1)
    switch (c) {
    case 'g':
      granularity_ = ACE_OS::atoi (opts.optarg);
      if (granularity_ < 1)
        granularity_ = 1;
      break;
    case 'u':
      use_utilization_test_ = 1;
      break;
    case 's':
      use_name_service_ = 0;
      break;
    case 'k':
      factory_ior_ = ACE_OS::strdup (opts.optarg);
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
                  " [-k factory_ior_key]"
                  " [-f ior_file]"
                  " [-x] // makes a call to servant to shutdown"
                  " [-o] // makes client use oneway calls instead"
                  " [-s] // makes client *NOT* use the name service"
                  " [-g granularity_of_request_timing]"
                  "\n", argv [0]));
    }

  // allocate the array of character pointers.
  ACE_NEW (iors_,
           char *[thread_count_]);

  if (ior_file_ != 0)
    {
      FILE *ior_file = ACE_OS::fopen (ior_file_, "r");
      char buf[BUFSIZ];
      int i = 0;
      int j = 0;

      while (ACE_OS::fgets (buf, BUFSIZ, ior_file) != 0)
        {
          j = ACE_OS::strlen (buf);
          buf[j - 1] = 0;  // this is to delete the "\n" that was read from the file.
          iors_[i] = ACE_OS::strdup (buf);
          i++;
        }

      ACE_OS::fclose (ior_file);
    }

  // thread_count_ + 2 because there is one utilization thread also
  // wanting to begin at the same time the clients begin && the main
  // thread wants to know when clients will start running to get
  // accurate context switch numbers.

  if (use_utilization_test_ == 1)
    // If we are to use the utilization test, include it in the
    // barrier count.  See description of this variable in header
    // file.
    {
      ACE_NEW (barrier_,
               ACE_Barrier (thread_count_ + 2));
    }
  else
    {
      ACE_NEW (barrier_,
               ACE_Barrier (thread_count_ + 1));
    }

  ACE_NEW (latency_,
           double [thread_count_]);
  ACE_NEW (global_jitter_array_,
           double *[thread_count_]);
}

Client::Client (ACE_Thread_Manager &thread_manager, Task_State *ts, u_int id)
  : ACE_MT (ACE_Task<ACE_MT_SYNCH> (&thread_manager)),
    ts_ (ts),
    id_ (id)
{
}

void
Client::put_latency (double *jitter,
                     double latency,
                     u_int thread_id)
{
  ACE_MT (ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, ts_->lock_));

  ts_->latency_[thread_id] = latency;
  ts_->global_jitter_array_[thread_id] = jitter;

#if defined (ACE_LACKS_FLOATING_POINT)
  ACE_DEBUG ((LM_DEBUG,
              "(%t) My latency was %u msec\n",
              latency));
#else
  ACE_DEBUG ((LM_DEBUG,
              "(%t) My latency was %f msec\n",
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

  for (u_int i = 1; i < ts_->thread_count_; i++)
    l += (double) ts_->latency_[i];

  return ts_->thread_count_ > 1? l / (double) (ts_->thread_count_ - 1) : 0;
}

u_int
Client::get_latency (u_int thread_id)
{
  return ACE_static_cast (u_int, ts_->latency_ [thread_id]);
}

double
Client::get_high_priority_jitter (void)
{
  double jitter = 0.0;
  double average = get_high_priority_latency ();
  double number_of_samples = ts_->loop_count_ / ts_->granularity_;

  // Compute the standard deviation (i.e. jitter) from the values
  // stored in the global_jitter_array_.

  // we first compute the sum of the squares of the differences
  // each latency has from the average
  for (u_int i = 0; i < ts_->loop_count_ / ts_->granularity_; i ++)
    {
      double difference =
        ts_->global_jitter_array_ [0][i] - average;
      jitter += difference * difference;
    }

  // Return the square root of the sum of the differences computed
  // above, i.e. jitter.
  return sqrt (jitter / (number_of_samples - 1));
}

double
Client::get_low_priority_jitter (void)
{
  double jitter = 0.0;
  double average = get_low_priority_latency ();
  double number_of_samples = (ts_->thread_count_ - 1) * (ts_->loop_count_ / ts_->granularity_);

  // Compute the standard deviation (i.e. jitter) from the values
  // stored in the global_jitter_array_.

  // We first compute the sum of the squares of the differences each
  // latency has from the average.
  for (u_int j = 1; j < ts_->thread_count_; j ++)
    for (u_int i = 0; i < ts_->loop_count_ / ts_->granularity_; i ++)
      {
        double difference =
          ts_->global_jitter_array_[j][i] - average;
        jitter += difference * difference;
      }

  // Return the square root of the sum of the differences computed
  // above, i.e. jitter.
  return sqrt (jitter / (number_of_samples - 1));
}

int
Client::svc (void)
{
  Cubit_ptr cb = 0;
  CORBA::ORB_var orb;
  CORBA::Object_var objref (0);
  CORBA::Object_var naming_obj (0);
  CORBA::Environment env;

  double frequency = 0.0;

  /// Add "-ORBobjrefstyle url" argument to the argv vector for the
  //orb to / use a URL style to represent the ior.

  // Convert the argv vector into a string.
  ACE_ARGV tmp_args (ts_->argv_);
  char tmp_buf[BUFSIZ];

  ACE_OS::strcpy (tmp_buf,
                  tmp_args.buf ());

  // Add the argument.
  ACE_OS::strcat (tmp_buf,
                  " -ORBobjrefstyle url ");

  // Convert back to argv vector style.
  ACE_ARGV tmp_args2 (tmp_buf);
  int argc = tmp_args2.argc ();

  char *const *argv = tmp_args2.argv ();

  u_int naming_success = CORBA::B_FALSE;

  orb = CORBA::ORB_init (argc,
                         argv,
                         "internet",
                         env);

  if (env.exception () != 0)
    {
      env.print_exception ("ORB_init()\n");
      return -1;
    }

  if (ts_->use_name_service_ != 0)
    {
      naming_obj =
        orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR ((LM_ERROR,
                    " (%P|%t) Unable to resolve the Name Service.\n"));
      else
        this->naming_context_ =
          CosNaming::NamingContext::_narrow (naming_obj.in (), env);
    }

  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, ts_->lock_, -1));

    if (ts_->thread_per_rate_ == 0)
      {
        if (this->id_ == 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%t) I'm the high priority client, my id is %d.\n",
                        this->id_));
            frequency = CB_HIGH_PRIORITY_RATE;
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%t) I'm a low priority client, my id is %d.\n",
                        this->id_));
            frequency = CB_LOW_PRIORITY_RATE;
          }
      }
    else
      switch (this->id_)
        {
        case CB_40HZ_CONSUMER:
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) I'm the high priority client, "
                      "my id is %d.\n", this->id_));
          frequency = CB_40HZ_CONSUMER_RATE;
          break;
        case CB_20HZ_CONSUMER:
          ACE_DEBUG ((LM_DEBUG, "(%t) I'm the high priority client, "
                      "my id is %d.\n", this->id_));
          frequency = CB_20HZ_CONSUMER_RATE;
          break;
        case CB_10HZ_CONSUMER:
          ACE_DEBUG ((LM_DEBUG, "(%t) I'm the high priority client, "
                      "my id is %d.\n", this->id_));
          frequency = CB_10HZ_CONSUMER_RATE;
          break;
        case CB_5HZ_CONSUMER:
          ACE_DEBUG ((LM_DEBUG, "(%t) I'm the high priority client, "
                      "my id is %d.\n", this->id_));
          frequency = CB_5HZ_CONSUMER_RATE;
          break;
        case CB_1HZ_CONSUMER:
          ACE_DEBUG ((LM_DEBUG, "(%t) I'm the high priority client, "
                      "my id is %d.\n", this->id_));
          frequency = CB_1HZ_CONSUMER_RATE;
          break;
        default:
          ACE_DEBUG ((LM_DEBUG, "(%t) Invalid Thread ID!!!!\n", this->id_));
        }

    TAO_TRY
      {
        // if the naming service was resolved successsfully ...
        if (!CORBA::is_nil (this->naming_context_.in ()))
          {
            ACE_DEBUG ((LM_DEBUG,
                        " (%t) ----- Using the NameService resolve() method to get cubit objects -----\n"));

            // Construct the key for the name service lookup.
            CosNaming::Name mt_cubit_context_name (1);
            mt_cubit_context_name.length (1);
            mt_cubit_context_name[0].id = CORBA::string_dup ("MT_Cubit");

            objref =
              this->naming_context_->resolve (mt_cubit_context_name,
                                              TAO_TRY_ENV);
            TAO_CHECK_ENV;

            this->mt_cubit_context_ =
              CosNaming::NamingContext::_narrow (objref.in (),
                                                 TAO_TRY_ENV);
            TAO_CHECK_ENV;

            char *buffer;
            int l = ACE_OS::strlen (ts_->key_) + 3;
            ACE_NEW_RETURN (buffer,
                            char[l],
                            -1);

            ACE_OS::sprintf (buffer,
                             "%s%02d",
                             (char *) ts_->key_,
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
                TAO_TRY_ENV.print_exception ("Attempt to resolve() a cubit object using the name service Failed!\n");
              }
            else
              {
                naming_success = CORBA::B_TRUE;
                ACE_DEBUG ((LM_DEBUG,
                            " (%t) Cubit object resolved to the name \"%s\".\n",
                            buffer));
              }
          }

        if (naming_success == CORBA::B_FALSE && ts_->factory_ior_ != 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                        " (%t) ----- Using the factory IOR method to get cubit objects -----\n"));

            objref =
              orb->string_to_object (ts_->factory_ior_, TAO_TRY_ENV);
            TAO_CHECK_ENV;

            if (CORBA::is_nil (objref.in ()))
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "%s:  must identify non-null target objref\n",
                                 ts_->argv_ [0]),
                                1);

            // Narrow the CORBA::Object reference to the stub object,
            // checking the type along the way using _is_a.
            Cubit_Factory_var cb_factory =
              Cubit_Factory::_narrow (objref.in (),
                                      TAO_TRY_ENV);
            TAO_CHECK_ENV;

            if (CORBA::is_nil (cb_factory.in ()))
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Create cubit factory failed\n"),
                                1);

            ACE_DEBUG ((LM_DEBUG,
                        "(%t) >>> Factory binding succeeded\n"));


            char * tmp_ior = cb_factory->create_cubit (this->id_,
                                                       TAO_TRY_ENV);
            TAO_CHECK_ENV;

            if (tmp_ior == 0)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "create_cubit() returned a null pointer!\n"),
                                 -1);

            char *my_ior = ACE_OS::strdup (tmp_ior);

            TAO_CHECK_ENV;

            objref = orb->string_to_object (my_ior,
                                            TAO_TRY_ENV);
            TAO_CHECK_ENV;
          }
        else
          {
            char *my_ior = ts_->iors_[this->id_];

            if (my_ior == 0)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Must specify valid factory ior key with -k option,"
                                 " naming service, or ior filename\n"),
                                -1);

            objref = orb->string_to_object (my_ior,
                                            TAO_TRY_ENV);
            TAO_CHECK_ENV;
          }

        if (CORBA::is_nil (objref.in ()))
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%t) string_to_object or NameService->resolve() Failed!\n"),
                            -1);

        // Narrow the CORBA::Object reference to the stub object,
        // checking the type along the way using _is_a.
        cb = Cubit::_narrow (objref.in (),
                             TAO_TRY_ENV);
        TAO_CHECK_ENV;

        if (CORBA::is_nil (cb))
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Create cubit failed\n"),
                            1);

        ACE_DEBUG ((LM_DEBUG,
                    "(%t) Binding succeeded\n"));

        CORBA::String_var str =
          orb->object_to_string (cb, TAO_TRY_ENV);
        TAO_CHECK_ENV;

        ACE_DEBUG ((LM_DEBUG,
                    "(%t) CUBIT OBJECT connected <%s>\n",
                    str.in ()));

        ACE_DEBUG ((LM_DEBUG,
                    "(%t) Waiting for other threads to "
                    "finish binding..\n"));
      }
    TAO_CATCHANY
      {
        TAO_TRY_ENV.print_exception ("get_object");
        return 1;
      }
    TAO_ENDTRY;
  }

  // Wait for all the client threads to be initialized before going
  // any further.
  ts_->barrier_->wait ();
  ACE_DEBUG ((LM_DEBUG,
              "(%t) Everyone's done, here I go!!\n"));

  if (ts_->oneway_ == 1)
    ACE_DEBUG ((LM_DEBUG,
                "(%t) **** USING ONEWAY CALLS ****\n"));

  // Perform the tests.
  int result = this->run_tests (cb,
                                ts_->loop_count_,
                                this->id_,
                                ts_->datatype_,
                                frequency);

  if (result == -1)
    return -1;

  if (ts_->shutdown_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) CALLING SHUTDOWN() ON THE SERVANT\n"));
      cb->shutdown (env);
      if (env.exception () != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Shutdown of the server failed!\n"));
          env.print_exception ("shutdown() call failed.\n");
        }
    }

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
  u_int call_count = 0;
  u_int error_count = 0;
  double *my_jitter_array;

  ACE_NEW_RETURN (my_jitter_array,
                  double [loop_count],
                  -1);

  double latency = 0;
  double sleep_time = (1 / frequency) * (1000 * 1000) * ts_->granularity_; // usec
  double delta = 0;

#if defined (CHORUS)
  int pstartTime = 0;
  int pstopTime = 0;
#endif /* CHORUS */
  double real_time = 0.0;

#if defined (USE_QUANTIFY)
  quantify_stop_recording_data();
  quantify_clear_data ();
#endif /* USE_QUANTIFY */

  // Make the calls in a loop.

  ACE_High_Res_Timer * timer_ = 0;
  for (i = 0; i < loop_count; i++)
    {
      // Elapsed time will be in microseconds.
      ACE_Time_Value delta_t;

      if ( (i % ts_->granularity_) == 0)
        {
          if (ts_->use_utilization_test_ == 0)
            {
              ACE_Time_Value tv (0,
                                 (u_long) ((sleep_time - delta) < 0
                                           ? 0
                                           : (sleep_time - delta)));
              ACE_OS::sleep (tv);
            }

#if defined (CHORUS)
          pstartTime = pccTime1Get();
#else /* CHORUS */
          ACE_NEW_RETURN (timer_,
                          ACE_High_Res_Timer,
                          -1);
          timer_->start ();
#endif /* !CHORUS */
        }

      if (ts_->oneway_ == 0)
        {
          switch (datatype)
            {
            case CB_OCTET:
              {
                // Cube an octet.
                CORBA::Octet arg_octet = func (i), ret_octet = 0;

#if defined (USE_QUANTIFY)
                /* start recording quantify data from here */
                quantify_start_recording_data ();
#endif /* USE_QUANTIFY */
                //ACE_ERROR (( LM_ERROR, "in {%t} i=%d\n", i));
                ret_octet = cb->cube_octet (arg_octet, env);
                //ACE_ERROR (( LM_ERROR, "out {%t} i=%d\n", i));

#if defined (USE_QUANTIFY)
                quantify_stop_recording_data();
#endif /* USE_QUANTIFY */

                if (env.exception () != 0)
                  {
                    env.print_exception ("call to cube_octet()\n");
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "%s:Call failed\n",
                                       env.exception ()),
                                      2);
                  }

                arg_octet = arg_octet * arg_octet * arg_octet;

                if (arg_octet != ret_octet)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                                "** cube_octet(%d)  (--> %d)\n",
                                arg_octet,
                                ret_octet));
                    error_count++;
                  }
                call_count++;
                break;
              }
            case CB_SHORT:
              // Cube a short.
              {
                call_count++;

                CORBA::Short arg_short = func (i), ret_short;

#if defined (USE_QUANTIFY)
                // start recording quantify data from here.
                quantify_start_recording_data ();
#endif /* USE_QUANTIFY */

                ret_short = cb->cube_short (arg_short, env);

#if defined (USE_QUANTIFY)
                quantify_stop_recording_data();
#endif /* USE_QUANTIFY */

                if (env.exception () != 0)
                  {
                    env.print_exception ("call to cube_short()\n");
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "%s:Call failed\n",
                                       env.exception ()),
                                      2);
                  }

                arg_short = arg_short * arg_short * arg_short;

                if (arg_short != ret_short)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                                "** cube_short(%d)  (--> %d)\n",
                                arg_short ,
                                ret_short));
                    error_count++;
                  }
                break;
              }
            // Cube a long.

            case CB_LONG:
              {
                call_count++;

                CORBA::Long arg_long = func (i);
                CORBA::Long ret_long;

#if defined (USE_QUANTIFY)
                /* start recording quantify data from here */
                quantify_start_recording_data ();
#endif /* USE_QUANTIFY */

                ret_long = cb->cube_long (arg_long, env);

#if defined (USE_QUANTIFY)
                quantify_stop_recording_data();
#endif /* USE_QUANTIFY */

                if (env.exception () != 0)
                  {
                    env.print_exception ("call to cube_long()\n");
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "%s:Call failed\n",
                                       env.exception ()),
                                      2);
                  }

                arg_long = arg_long * arg_long * arg_long;

                if (arg_long != ret_long)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                                "** cube_long(%d)  (--> %d)\n",
                                arg_long,
                                ret_long));
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

#if defined (USE_QUANTIFY)
                // start recording quantify data from here.
                quantify_start_recording_data ();
#endif /* USE_QUANTIFY */

                ret_struct = cb->cube_struct (arg_struct, env);

#if defined (USE_QUANTIFY)
                quantify_stop_recording_data();
#endif /* USE_QUANTIFY */

                if (env.exception () != 0)
                  {
                    env.print_exception ("call to cube_struct()\n");
                    ACE_ERROR_RETURN ((LM_ERROR,"%s:Call failed\n", env.exception ()), 2);
                  }

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
            default:
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%P|%t); %s:%d; unexpected datatype: %d\n",
                                 datatype), -1);
            }
        }
      else
        {
          call_count++;
#if defined (USE_QUANTIFY)
          // start recording quantify data from here.
          quantify_start_recording_data ();
#endif /* USE_QUANTIFY */
          cb->noop (env);
#if defined (USE_QUANTIFY)
          quantify_stop_recording_data();
#endif /* USE_QUANTIFY */
          if (env.exception () != 0)
            {
              env.print_exception ("oneway call noop()\n");
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%t) noop() call failed\n"),
                                2);
            }
        }

      if ((i % ts_->granularity_) == (ts_->granularity_ - 1))
        {
#if defined (CHORUS)
          pstopTime = pccTime1Get();
#else /* CHORUS */
          // if CHORUS is not defined just use plain timer_.stop ().
          timer_->stop ();
          timer_->elapsed_time (delta_t);
#endif /* !CHORUS */

          // Calculate time elapsed
#if defined (ACE_LACKS_FLOATING_POINT)
#   if defined (CHORUS)
          real_time = (pstopTime - pstartTime) / ts_->granularity_;
#   else /* CHORUS */
          // Store the time in usecs.
          real_time = (delta_t.sec () * ACE_ONE_SECOND_IN_USECS  +
            delta_t.usec ()) / ts_->granularity_;
#   endif /* !CHORUS */
          delta = ((40 * fabs (real_time) / 100) + (60 * delta / 100)); // pow(10,6)
          latency += real_time * ts_->granularity_;
          my_jitter_array [i/ts_->granularity_] = real_time; // in units of microseconds.
          // update the latency array, correcting the index using the granularity
#else /* ACE_LACKS_FLOATING_POINT */
          // Store the time in secs.
          real_time = (double)delta_t.sec () + (double)delta_t.usec () / (double)ACE_ONE_SECOND_IN_USECS;
          real_time /= ts_->granularity_;
          delta = ((0.4 * fabs (real_time * (1000 * 1000))) + (0.6 * delta)); // pow(10,6)
          latency += (real_time * ts_->granularity_);
          my_jitter_array [i/ts_->granularity_] = real_time * 1000;
          delete timer_;
#endif /* !ACE_LACKS_FLOATING_POINT */
        }
    }

  if (call_count > 0)
    {
      if (error_count == 0)
        {
#if defined (ACE_LACKS_FLOATING_POINT)
          double calls_per_second = (call_count * ACE_ONE_SECOND_IN_USECS) / latency;
#endif /* ACE_LACKS_FLOATING_POINT */

          latency /= call_count; // calc average latency

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
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) cube average call ACE_OS::time\t= %f msec, \t"
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
                          "*** Warning: Latency, %f, is less than or equal to zero."
                          "  Precision may have been lost.\n, latency"));
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


/////////////////////////////////////////////////////////////////////////
// = DESCRIPTION
//   Program that calculates context switch time between threads.
//   This test is based on the Task Context Switching measurement
//   approach described in:
//   Darren Cathey<br>
//   "RTOS Benchmarking -- All Things Considered . . ."<br>
//   <a href="http://www.realtime-info.be"><em>Real-Time Magazine</em></a>,
//      Second Quarter 1993,
//   <em>reprinted by <a href="http://www.wrs.com/artreqfm.html">Wind River
//                    Systems</a></em><p>
//   which in turn is based on Superconducting Super Collider (SSC)
//   Ping Suspend/Resume Task and Suspend/Resume Task benchmarks.
//   Laboratory benchmark
//   It measures two different times:
//   1) the time to resume a block high priority task, which does
//      nothing other than block immediately;  A lower priority task
//      resumes the high priority task, so the elapsed time includes
//      two context switches, one task suspend, and one task resume.
//   2) the time to suspend and resume a low priority task that does
//      nothing;  There is no context switching.  This time is subtracted
//      from the one described in 1) above, and the result is divided by
//      two to yield the context switch time.
//
//   Notes:
//   On Solaris 2.5.1, it appears that the lowest context switching times,
//   at least on a single-CPU machine, are obtained _without_ creating new
//   LWPs for new threads (THR_NEW_LWP).  The -n option enables the use of
//   THR_NEW_LWP for testing.
//
//   On Solaris 2.5.1, real-time threads must be bound to LWPs (using the
//   THR_BOUND flag), so that they can compete for system-wide resources.
//   In other words, if a thread is bound to an LWP, then the kernel is
//   aware of it.
//
//   On Solaris 2.5.1, a call to thr_yield () is necessary after a call
//   to thr_continue () by a low-priority task.  Without it, the high-priority
//   task doesn't preempt the low-priority task.  This happens even with a
//   10 nsec time quantum for the LWP.  Maybe it's because with this version
//   of Solaris, the scheduling policy is SCHED_OTHER.
//
//   All threads are created with the THR_DETACHED flag so that their
//   resources are released when they terminate.
//
// = CREATION DATE
//    17 January 1997
//
// = AUTHOR
//    David L. Levine
//
// ============================================================================


Yield_Test::Yield_Test (const unsigned long iterations) :
  ACE_Task<ACE_MT_SYNCH> (),
  iterations_ (iterations),
  timer_barrier_ (3),
  timer_ ()
{
  timer_.start ();

  this->activate (THR_BOUND | THR_DETACHED |  new_lwp, 2, 0, LOW_PRIORITY);

  timer_barrier_.wait ();

  timer_.stop ();
  timer_.elapsed_microseconds (elapsed_time_);
}

Yield_Test::~Yield_Test()
{
}

int
Yield_Test::svc ()
{
  for (unsigned long i = 0; i < iterations_; ++i)
    {
      ACE_OS::thr_yield ();
    }

  timer_barrier_.wait ();

  return 0;
}

double
context_switch_time (void)
{
  const u_int iterations = 1000;
  const u_int retries = 100;
  double tmp = 0;
  // Disable LM_DEBUG
  ACE_Log_Msg::instance ()->priority_mask (ACE_LOG_MSG->priority_mask () ^
                                           LM_DEBUG);

  if (ACE_OS::sched_params (
        ACE_Sched_Params (
          ACE_SCHED_FIFO,
          ACE_Sched_Params::priority_min (ACE_SCHED_FIFO),
          ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_MAX, "context_switch_time: user is not superuser, "
                      "so remain in time-sharing class\n"));
        }
      else
        {
          ACE_OS::perror ("context_switch_time");
          ACE_OS::exit (-1);
        }
    }

  for (u_int i=0; i<100; i++)
    {
      LOW_PRIORITY = ACE_Sched_Params::priority_min (ACE_SCHED_FIFO);
      HIGH_PRIORITY = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                       LOW_PRIORITY);

      // then Yield test
      Yield_Test yield_test (iterations);
      // Wait for all tasks to exit.
      ACE_Thread_Manager::instance ()->wait ();

      tmp += (double) (yield_test.elapsed_time ()/ (ACE_UINT32) 1u) /iterations /2;
    }

  // Disable LM_DEBUG
  ACE_Log_Msg::instance ()->priority_mask (ACE_LOG_MSG->priority_mask () ^
                                           LM_DEBUG);
  return tmp/retries;
}
