// $Id$

#include "Task_Client.h"

int stats(int data[], int n)
{
    int i, j, key, sum, mean;

    if( n < 2 )
        return 0;

    /* sort the samples */
    for(j=1; j<n; j++) {
        key = data[j];
        i = j - 1;
        while( i >= 0 && data[i] > key ) {
            data[i+1] = data[i];
            i--;
        }
        data[i+1] = key;
    }

    sum = 0;
    for(i=0; i<n; i++) {
        sum += data[i];
    }

    if( (n%2) == 0 )
        mean = (data[n/2-1] + data[n/2]) / 2;
    else
        mean = data[n/2];

    return sum/n;
}

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
    factory_ior_ (0),
    shutdown_ (0),
    oneway_ (0),
    use_name_service_ (1),
    ior_file_ (0)
{
  server_host_ = ACE_OS::strdup ("localhost");
  ACE_Get_Opt opts (argc, argv, "sh:n:t:p:d:rk:xof:1:2:3:");
  int c;
  int datatype;

  while ((c = opts ()) != -1)
    switch (c) {
    case 's':
      use_name_service_ = 0;
      break;
    case 'k':
      factory_ior_ = ACE_OS::strdup (opts.optarg);
      break;
    case 'f':
      ior_file_ = ACE_OS::strdup (opts.optarg);
      break;
    case '1':
      iors_[0] =  ACE_OS::strdup (opts.optarg);
      break;
    case '2':
      iors_[1] =  ACE_OS::strdup (opts.optarg);
      break;
    case '3':
      iors_[2] =  ACE_OS::strdup (opts.optarg);
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
          datatype_ = CB_SHORT;
          ACE_DEBUG ((LM_DEBUG, "Testing Shorts\n"));
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
    case '?':
    default:
      ACE_DEBUG ((LM_DEBUG, "usage:  %s"
                  "[-d datatype Octet=0, Short=1, Long=2, Struct=3]"
                  " [-n num_calls]"
                  " [-h server_hostname]"
                  " [-p server_port_num]"
                  " [-t num_threads]"
                  " [-k factory_ior_key]"
                  " [-f ior_file]"
                  " [-x] // makes a call to servant to shutdown"
                  " [-o] // makes client use oneway calls instead"
                  " [-s] // makes client *NOT* use the name service"
                  "\n", argv [0]));
    }

  if (ior_file_ != 0)
    {
      FILE *iorFile = fopen (ior_file_, "r");
      char buf[BUFSIZ];
      int i = 0, j=0;
      while (ACE_OS::fgets (buf, BUFSIZ, iorFile) != 0)
	{ j=ACE_OS::strlen(buf); buf[j-1]=0;iors_[i] = ACE_OS::strdup (buf); printf("ior[%d]=\"%s\"\n",i,iors_[i]); i++;  }
      fclose (iorFile);
    }

  // thread_count_ + 1 because there is one utilization thread also
  // wanting to begin at the same time the clients begin..
  ACE_NEW (barrier_, ACE_Barrier (thread_count_ )); //+ 1));
  ACE_NEW (latency_, double [thread_count_]);
  ACE_NEW (global_jitter_array_, double *[thread_count_]);
  ACE_NEW (ave_latency_, int [thread_count_]);
}

Client::Client (Task_State *ts)
  :
#if defined (ACE_HAS_THREADS)
  ACE_Task<ACE_MT_SYNCH> (ACE_Thread_Manager::instance ()),
#endif /* ACE_HAS_THREADS */
  ts_ (ts)
{

}

void
Client::put_ave_latency (int ave_latency, u_int thread_id)
{
#if defined (ACE_HAS_THREADS)
  ts_->lock_.acquire ();
#endif /* ACE_HAS_THREADS */

  ts_->ave_latency_[thread_id] = ave_latency;

#if defined (ACE_HAS_THREADS)
  ts_->lock_.release ();
#endif /* ACE_HAS_THREADS */
}

void
Client::put_latency (double *jitter, double latency, u_int thread_id)
{
#if defined (ACE_HAS_THREADS)
  ts_->lock_.acquire ();
#endif /* ACE_HAS_THREADS */

  ts_->latency_[thread_id] = latency;
#if defined (ACE_LACKS_FLOATING_POINT)
  ACE_DEBUG ((LM_DEBUG, "(%t) My latency was %u\n", latency));
#else
  ACE_DEBUG ((LM_DEBUG, "(%t) My latency was %f\n", latency));
#endif /* ! ACE_LACKS_FLOATING_POINT */
  ts_->global_jitter_array_ [thread_id] = jitter;

#if defined (ACE_HAS_THREADS)
  ts_->lock_.release ();
#endif /* ACE_HAS_THREADS */
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
  return ts_->ave_latency_ [thread_id];
}

double
Client::get_high_priority_jitter () {
  double jitter = 0;
  double average = get_high_priority_latency ();
  for (u_int i = 0; i < ts_->loop_count_; i ++)
    {
      double difference = ts_->global_jitter_array_ [0][i] - average;
      jitter += difference * difference;
    }
// @@ This is a *hack*, we need to implement sqrt without double's.
#if defined (ACE_LACKS_FLOATING_POINT)
  return 1;
#else  /* ! ACE_LACKS_FLOATING_POINT */
  return sqrt (jitter / (double) (ts_->loop_count_ - 1));
#endif /* ! ACE_LACKS_FLOATING_POINT */
}

double
Client::get_low_priority_jitter () {
  double jitter = 0;
  double average = get_low_priority_latency ();
  for (u_int j = 1; j < ts_->start_count_; j ++)
      for (u_int i = 0; i < ts_->loop_count_; i ++)
        {
          double difference = ts_->global_jitter_array_ [j][i] - average;
          jitter += difference * difference;
        }
  return (double) (jitter / ((ts_->loop_count_* (ts_->start_count_ - 1)) - 1));
}

int
Client::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread created\n"));
  u_int thread_id;
  Cubit_ptr cb = 0;
  CORBA::ORB_var orb;
  CORBA::Object_var objref (0);
  CORBA::Object_var naming_obj (0);
  CORBA::Environment env;
  char ior [1024];
  double frequency = 0.0;

  /// Add "-ORBobjrefstyle url" argument to the argv vector for the orb to
  ///   use a URL style to represent the ior.

  // convert the argv vector into a string.
  ACE_ARGV tmp_args (ts_->argv_);
  char tmp_buf[BUFSIZ];
  strcpy (tmp_buf, tmp_args.buf ());
  // Add the argument
  strcat (tmp_buf, " -ORBobjrefstyle url ");
  // convert back to argv vector style.
  ACE_ARGV tmp_args2(tmp_buf);
  int argc = tmp_args2.argc ();
  char* const *argv = tmp_args2.argv ();

  u_int naming_success = CORBA::B_FALSE;

  orb = CORBA::ORB_init (argc, argv,
                         "internet", env);

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
        {
          this->naming_context_ =
            CosNaming::NamingContext::_narrow (naming_obj.in (), env);
        }
    }

  {
#if defined (ACE_HAS_THREADS)
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, ts_->lock_, -1);
#endif /* ACE_HAS_THREADS */

    thread_id = ts_->start_count_;
    ts_->start_count_++;

    if (ts_->thread_per_rate_ == 0)
      {
        if (thread_id == 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%t) Im the high priority client, my id is %d.\n",
                        thread_id));
            frequency = CB_HIGH_PRIORITY_RATE;
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%t) Im a low priority client, my id is %d\n",
                        thread_id));
            frequency = CB_LOW_PRIORITY_RATE;
          }
      }
    else
      {
        switch (thread_id)
          {
          case CB_40HZ_CONSUMER:
            ACE_DEBUG ((LM_DEBUG,
                        "(%t) Im the high priority client, "
                        "my id is %d.\n", thread_id));
            frequency = CB_40HZ_CONSUMER_RATE;
            break;
          case CB_20HZ_CONSUMER:
            ACE_DEBUG ((LM_DEBUG, "(%t) Im the high priority client, "
                        "my id is %d.\n", thread_id));
            frequency = CB_20HZ_CONSUMER_RATE;
            break;
          case CB_10HZ_CONSUMER:
            ACE_DEBUG ((LM_DEBUG, "(%t) Im the high priority client, "
                        "my id is %d.\n", thread_id));
            frequency = CB_10HZ_CONSUMER_RATE;
            break;
          case CB_5HZ_CONSUMER:
            ACE_DEBUG ((LM_DEBUG, "(%t) Im the high priority client, "
                        "my id is %d.\n", thread_id));
            frequency = CB_5HZ_CONSUMER_RATE;
            break;
          case CB_1HZ_CONSUMER:
            ACE_DEBUG ((LM_DEBUG, "(%t) Im the high priority client, "
                        "my id is %d.\n", thread_id));
            frequency = CB_1HZ_CONSUMER_RATE;
            break;
          default:
            ACE_DEBUG ((LM_DEBUG, "(%t) Invalid Thread ID.\n", thread_id));
          }
      }

    TAO_TRY
      {
        // if the naming service was resolved successsfully ...
        if (!CORBA::is_nil (this->naming_context_.in ()))
          {
            ACE_DEBUG ((LM_DEBUG, " (%t) ----- Using the NameService resolve() method to get cubit objects -----\n"));

            // Construct the key for the name service lookup.
            CosNaming::Name mt_cubit_context_name (1);
            mt_cubit_context_name.length (1);
            mt_cubit_context_name[0].id = CORBA::string_dup ("MT_Cubit");

            objref =
              this->naming_context_->resolve (mt_cubit_context_name,
                                              TAO_TRY_ENV);
            TAO_CHECK_ENV;

            this->mt_cubit_context_ =
              CosNaming::NamingContext::_narrow (objref.in (), TAO_TRY_ENV);
            TAO_CHECK_ENV;

            char* buffer;
            int l = ACE_OS::strlen (key) + 3;
            ACE_NEW_RETURN (buffer, char[l], -1);

            ACE_OS::sprintf (buffer, "%s%02d",
                             (char *) key, thread_id);

            // Construct the key for the name service lookup.
            CosNaming::Name cubit_name (1);
            cubit_name.length (1);
            cubit_name[0].id = CORBA::string_dup (buffer);

            objref = this->mt_cubit_context_->resolve (cubit_name, TAO_TRY_ENV);

            if ( (TAO_TRY_ENV.exception () != 0) || CORBA::is_nil (objref.in ()) )
              {
                ACE_DEBUG ((LM_DEBUG, " (%t) resolve() returned nil\n"));
                TAO_TRY_ENV.print_exception ("Attempt to resolve() a cubit object using the name service Failed!\n");
              }
            else
              {
                naming_success = CORBA::B_TRUE;
                ACE_DEBUG ((LM_DEBUG, " (%t) Cubit object resolved to the name \"%s\".\n", buffer));
              }
          }

        if ( (naming_success == CORBA::B_FALSE) && (ts_->factory_ior_ != 0) )
          {
            ACE_DEBUG ((LM_DEBUG, " (%t) ----- Using the factory IOR method to get cubit objects -----\n"));

            objref =
              orb->string_to_object (ts_->factory_ior_, TAO_TRY_ENV);
            TAO_CHECK_ENV;

            if (CORBA::is_nil (objref.in ()))
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "%s:  must identify non-null target objref\n",
                                 ts_->argv_ [0]),
                                1);

            // Narrow the CORBA::Object reference to the stub object, checking
            // the type along the way using _is_a.
            Cubit_Factory_var cb_factory = Cubit_Factory::_narrow (objref.in (), TAO_TRY_ENV);
            TAO_CHECK_ENV;

            if (CORBA::is_nil (cb_factory.in ()))
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Create cubit factory failed\n"), 1);

            ACE_DEBUG ((LM_DEBUG, "(%t) >>> Factory binding succeeded\n"));

            char * my_ior = ACE_OS::strdup (cb_factory->create_cubit (thread_id, TAO_TRY_ENV));
            TAO_CHECK_ENV;

            objref = orb->string_to_object (my_ior,
                                            TAO_TRY_ENV);
            TAO_CHECK_ENV;
          }
        else
          {
            char * my_ior = ts_->iors_[thread_id];
            if (my_ior == 0)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Must specify valid factory ior key with -k option, naming service, or ior filename\n"), -1);

            objref = orb->string_to_object (my_ior,
                                            TAO_TRY_ENV);
            TAO_CHECK_ENV;
          }

        if (CORBA::is_nil (objref.in ()))
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%t) string_to_object or NameService->resolve() Failed!\n"),
                            -1);

        // Narrow the CORBA::Object reference to the stub object, checking
        // the type along the way using _is_a.
        cb = Cubit::_narrow (objref.in (), TAO_TRY_ENV);
        TAO_CHECK_ENV;

        if (CORBA::is_nil (cb))
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Create cubit failed\n"), 1);

        ACE_DEBUG ((LM_DEBUG, "(%t) Binding succeeded\n"));

        CORBA::String_var str = orb->object_to_string (cb, TAO_TRY_ENV);
        TAO_CHECK_ENV;

        ACE_DEBUG ((LM_DEBUG,
                    "(%t) CUBIT OBJECT connected <%s>\n", str.in ()));

        ACE_DEBUG ((LM_DEBUG, "(%t) Waiting for other threads to "
                    "finish binding..\n"));
      }
    TAO_CATCHANY
      {
        TAO_TRY_ENV.print_exception ("get_object");
        return 1;
      }
    TAO_ENDTRY;
  }

  ts_->barrier_->wait ();
  ACE_DEBUG ((LM_DEBUG, "(%t) Everyone's done, here I go!!\n"));

  if (ts_->oneway_ == 1)
    ACE_DEBUG ((LM_DEBUG, "(%t) **** USING ONEWAY CALLS ****\n"));

  this->run_tests (cb, ts_->loop_count_, thread_id,
                   ts_->datatype_, frequency);

  if (ts_->shutdown_)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) CALLING SHUTDOWN() ON THE SERVANT\n"));
      cb->shutdown (env);
      if (env.exception () != 0)
        {
          ACE_ERROR ((LM_ERROR, "Shutdown of the server failed!\n"));
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
  u_int call_count = 0, error_count = 0;
  double *my_jitter_array;
  ACE_NEW_RETURN (my_jitter_array, double [ts_->loop_count_], 1);

  double latency = 0;
  double sleep_time = (1/frequency) * (1000 * 1000);
  double delta = 0;

#if defined (USE_QUANTIFY)
  quantify_stop_recording_data();
  quantify_clear_data ();
#endif

  // Make the calls in a loop.

  for (i = 0; i < loop_count; i++)
    {
      //ACE_Profile_Timer timer; Use high resolution timer instead
      ACE_High_Res_Timer timer_;
      ACE_Time_Value tv (0, (long int) (sleep_time - delta));
      ACE_OS::sleep (tv);

      ACE_Time_Value delta_t; /* elapsed time will be in microseconds */

      timer_.start (); //ACE_OS::ACE_HRTIMER_START); not using sysBench when CHORUS defined

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
#endif
                ret_octet = cb->cube_octet (arg_octet, env);

#if defined (USE_QUANTIFY)
            quantify_stop_recording_data();
#endif

                if (env.exception () != 0)
                  {
                    env.print_exception ("call to cube_octet()\n");
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

#if defined (USE_QUANTIFY)
            /* start recording quantify data from here */
            quantify_start_recording_data ();
#endif

                ret_short = cb->cube_short (arg_short, env);

#if defined (USE_QUANTIFY)
            quantify_stop_recording_data();
#endif

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

#if defined (USE_QUANTIFY)
            /* start recording quantify data from here */
            quantify_start_recording_data ();
#endif

                ret_long = cb->cube_long (arg_long, env);

#if defined (USE_QUANTIFY)
            quantify_stop_recording_data();
#endif

                if (env.exception () != 0)
                  {
                    env.print_exception ("call to cube_long()\n");
                    ACE_ERROR_RETURN ((LM_ERROR,"%s:Call failed\n", env.exception ()), 2);
                  }

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

#if defined (USE_QUANTIFY)
            /* start recording quantify data from here */
            quantify_start_recording_data ();
#endif

                ret_struct = cb->cube_struct (arg_struct, env);

#if defined (USE_QUANTIFY)
            quantify_stop_recording_data();
#endif

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
            }
        }
      else
        {
          call_count++;
#if defined (USE_QUANTIFY)
            /* start recording quantify data from here */
            quantify_start_recording_data ();
#endif
          cb->noop (env);
#if defined (USE_QUANTIFY)
            quantify_stop_recording_data();
#endif
          if (env.exception () != 0)
            {
              env.print_exception ("oneway call noop()\n");
              ACE_ERROR_RETURN ((LM_ERROR,"(%t) noop() call failed\n"), 2);
            }
        }

      timer_.stop (); //ACE_OS::ACE_HRTIMER_STOP); not using sysBench when CHORUS defined
      timer_.elapsed_time (delta_t);

      ACE_Profile_Timer::ACE_Elapsed_Time et;

      // Store the time in usecs.
      et.real_time = delta_t.sec () * ACE_ONE_SECOND_IN_USECS  +
        delta_t.usec ();

      //      ACE_DEBUG ((LM_DEBUG, "   (%t) elapsed time= %u, latency=%u\n",et.real_time,latency));
#if !defined (ACE_HAS_PRUSAGE_T) && !defined (ACE_HAS_GETRUSAGE) && defined (ACE_LACKS_FLOATING_POINT)
      delta = ((40 * fabs (et.real_time) / 100) + (60 * delta / 100)); // pow(10,6)
      latency += et.real_time;
      my_jitter_array [i] = et.real_time; // in units of milliseconds.
#else
      delta = ((40 * fabs (et.real_time * (1000 * 1000)) / 100) + (60 * delta / 100)); // pow(10,6)
      latency += et.real_time;
      my_jitter_array [i] = et.real_time * 1000;
#endif
    }

  if (call_count > 0)
    {
      if (error_count == 0)
        {
#if defined (ACE_LACKS_FLOATING_POINT)
          double tmp = latency / ACE_ONE_SECOND_IN_USECS;
          //      printf("latency=%u, tmp=%u, call_count=%u, \n", latency, tmp, call_count);
          double calls_per_second = call_count / tmp;
#endif /* ACE_LACKS_FLOATING_POINT */

          latency /= call_count;

          if (latency > 0)
            {
#if defined (ACE_LACKS_FLOATING_POINT)
              ACE_DEBUG ((LM_DEBUG, "(%P|%t) cube average call ACE_OS::time\t= %u usec, \t"
                              "%u calls/second\n",
                              latency,
                              calls_per_second));

              this->put_latency (my_jitter_array, latency, thread_id);
#else
              ACE_DEBUG ((LM_DEBUG, "(%P|%t) cube average call ACE_OS::time\t= %f msec, \t"
                              "%f calls/second\n",
                              latency * 1000,
                              1 / latency));
              this->put_latency (my_jitter_array, latency * 1000, thread_id);
#endif /* ! ACE_LACKS_FLOATING_POINT */
            }
        }
      ACE_DEBUG ((LM_DEBUG, "%d calls, %d errors\n", call_count, error_count));
    }

  // cb->please_exit (env);
  return 0;
}
