// $Id$

#include "pbroker/corba/xpsorb.h"
#include "pbroker/corba/xpsboa.h"
#include "pbroker/corba/boad/boafull.h"   // will force usage of "Full" BOA API
#include "Task_Client.h"

ACE_RCSID(client, Task_Client, "$Id$")

Task_State::Task_State (int argc, char **argv)
  : start_count_ (0),
    loop_count_ (5),
    thread_count_ (5),
    base_port_ (5000),
    datatype_ (CB_OCTET),
    argc_ (argc),
    argv_ (argv)
{
  ACE_OS::strcpy (server_host_, "localhost");
  ACE_Get_Opt opts (argc, argv, "h:n:t:p:d:");
  int c;
  int datatype;

  while ((c = opts ()) != -1)
    switch (c) {
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
    case 'n':			// loop count
      loop_count_ = (unsigned) ACE_OS::atoi (opts.optarg);
      continue;
    case 't':
      thread_count_ = (unsigned) ACE_OS::atoi (opts.optarg);
      continue;
    case '?':
    default:
      ACE_OS::fprintf (stderr, "usage:  %s"
                       "[-d datatype Octet=0, Short=1, Long=2, Struct=3]"
                       " [-n num_calls]"
                       " [-h server_hostname]"
                       " [-p server_port_num]"
                       " [-t num_threads]"
                       "\n", argv [0]
                       );
    }
  barrier_ = new ACE_Barrier (thread_count_);
  latency_ = new double [thread_count_];
}

Client::Client (Task_State *ts)
  : ACE_Task<ACE_MT_SYNCH> (ACE_Thread_Manager::instance ()),
    ts_ (ts)
{
}

void
Client::put_latency (double latency, u_int thread_id) {
  ts_->lock_.acquire ();
  ts_->latency_[thread_id] = latency;
  ACE_DEBUG ((LM_DEBUG, "(%t) My latency was %f\n", latency));
  ts_->lock_.release ();
}

double
Client::get_high_priority_latency () {
  return (double) ts_->latency_ [0];
}

double
Client::get_low_priority_latency () {
  double l = 0;
  for (u_int i = 1; i < ts_->start_count_; i++)
    l += (double) ts_->latency_[i];
  return l / (double) (ts_->start_count_ - 1);
}

int
Client::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread created\n"));
  u_int thread_id;
  Cubit_ptr cb;
  char ior [1024];
  double frequency;
  XpsORB_ptr       orb_ptr = XpsORB::_nil();
  //  XpsBOA *         oa_ptr  = XpsBOA::_nil ();
  //CORBA_Object_ptr obj_ptr = CORBA_Object::_nil();
  // CORBA::ORB_ptr orb_ptr;
  try {
    {
    ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, ts_->lock_, -1);

    thread_id = ts_->start_count_;
    ts_->start_count_++;

    if (thread_id == 0)
      {
        ACE_DEBUG ((LM_DEBUG, "(%t) Im the high priority client, my id is %d.\n", thread_id));
        ::sprintf (ior, "iiop:1.0//%s:%d/Cubit00", ts_->server_host_, ts_->base_port_);
        frequency = CB_HIGH_PRIORITY_RATE;
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "(%t) Im a low priority client, my id is %d\n", thread_id));
        ::sprintf (ior, "iiop:1.0//%s:%d/Cubit00", ts_->server_host_, ts_->base_port_ + 1);
        frequency = CB_LOW_PRIORITY_RATE;
      }

    ACE_DEBUG ((LM_DEBUG, "Using ior = %s\n", ior));

    CORBA::Object_ptr objref = CORBA::Object::_nil ();
    CORBA::Environment env;

    orb_ptr = XpsORB::init (ts_->argc_, ts_->argv_);
    //orb_ptr = CORBA::ORB_init (ts_->argc_, ts_->argv_);
    //if (env.exception () != 0) @@ put exception handling here !!
    //   ACE_ERROR_RETURN ((LM_ERROR,"%s:ORB initialization", env.exception ()), 2);

    // Retrieve a factory objref.
    objref = orb_ptr->resolve_initial_references ("Cubit");

    if (CORBA::is_nil (objref) == CORBA_TRUE)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s:  must identify non-null target objref\n",
                         ts_->argv_ [0]),
		      1);

    // Narrow the CORBA::Object reference to the stub object, checking
    // the type along the way using _is_a.
    cb = Cubit::_narrow (objref);

   if (cb == NULL) 
     {
       ACE_ERROR_RETURN ((LM_ERROR, "Create cubit failed\n"), 1);
     }
    ACE_DEBUG ((LM_DEBUG, "(%t) Binding succeeded\n"));
    ACE_DEBUG ((LM_DEBUG, "(%t) Cb == %x\n", cb));

    CORBA::String str;

    str = orb_ptr->object_to_url (cb);
    // 
    //ACE_ERROR_RETURN ((LM_ERROR, "object_to_string\n"), -1);

    ACE_OS::puts ((char *) str);
    ACE_OS::fflush (stdout);
    ACE_DEBUG ((LM_DEBUG, "Object Created at: '%ul'", cb));
    ACE_DEBUG ((LM_DEBUG, "connected to object '%s'", str));
    //    if (cb->cube_short (2, env) == 8) // dummy call.
    //      ACE_DEBUG ((LM_DEBUG, "(%t) Made successful dummy call"));
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) Waiting for other threads to finish binding..\n"));
  ts_->barrier_->wait ();
  ACE_DEBUG ((LM_DEBUG, "(%t) Everyone's done, here I go!!\n"));

  this->run_tests (cb, ts_->loop_count_, thread_id, ts_->datatype_, frequency);

  // Free resources
  CORBA::release (orb_ptr);
  }
  catch(const CORBA::SystemException& exc)
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "CORBA::SystemException caught: %s (minor: %d): client.cpp\n\n",
		  exc._repository_id (),
		  exc.minor ()));
    }
  catch(const CORBA::UserException& exc)
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "CORBA::UserException caught: %s: client.cpp\n\n",
		  exc._repository_id ()));
    }
  catch(...)
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "Exception caught: client.cpp\n\n"));
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
  unsigned i = 0;
  unsigned call_count = 0, error_count = 0;

  double latency = 0;
  double sleep_time = (1/frequency) * (1000 * 1000);
  double delta = 0;

  //
  // Make the calls in a loop.
  //
  for (i = 0; i < loop_count; i++)
    {
      ACE_Profile_Timer timer;
      ACE_Time_Value tv (0, (long int) (sleep_time - delta));
      ACE_OS::sleep (tv);
      timer.start ();

      switch (datatype)
        {
        case CB_OCTET:
          {
            // Cube an octet.
            call_count++;

            CORBA::Octet arg_octet = func (i), ret_octet = 0;

            ret_octet = cb->cube_octet (arg_octet);

            if (env.exception () != 0)
              {
                ACE_ERROR_RETURN ((LM_ERROR,"%s:Call failed\n", env.exception ()), 2);
              }
            arg_octet = arg_octet * arg_octet * arg_octet;
            if (arg_octet != ret_octet)
              {
                ACE_OS::printf ("** cube_octet(%d)  (--> %d)\n", arg_octet , ret_octet);
                error_count++;
              }
          }
        break;

        case CB_SHORT:
          // Cube a short.
          {
            call_count++;

            CORBA::Short arg_short = func (i), ret_short;

            ret_short = cb->cube_short (arg_short);

            if (env.exception () != 0) 
              ACE_ERROR_RETURN ((LM_ERROR,"%s:Call failed\n", env.exception ()), 2);

            arg_short = arg_short * arg_short * arg_short;
            if (arg_short != ret_short) {
              ACE_OS::printf ("** cube_short(%d)  (--> %d)\n", arg_short , ret_short);
              error_count++;
            }
          }
        break;
          // Cube a long.

        case CB_LONG:
          {
            call_count++;

            CORBA::Long arg_long = func (i), ret_long;

            ret_long = cb->cube_long (arg_long);

            if (env.exception () != 0)
              ACE_ERROR_RETURN ((LM_ERROR,"%s:Call failed\n", env.exception ()), 2);

            arg_long = arg_long * arg_long * arg_long;
            if (arg_long != ret_long)
              {
                ACE_OS::printf ("** cube_long(%d)  (--> %d)\n", arg_long , ret_long);
                error_count++;
              }
          }
          break;

        case CB_STRUCT:
          // Cube a "struct" ...
          {
            Cubit::Many	arg_struct, ret_struct;

            call_count++;

            arg_struct.l = func (i);
            arg_struct.s = func (i);
            arg_struct.o = func (i);

            ret_struct = cb->cube_struct (arg_struct);

            if (env.exception () != 0)
              ACE_ERROR_RETURN ((LM_ERROR,"%s:Call failed\n", env.exception ()), 2);

            arg_struct.l = arg_struct.l  * arg_struct.l  * arg_struct.l ;
            arg_struct.s = arg_struct.s  * arg_struct.s  * arg_struct.s ;
            arg_struct.o = arg_struct.o  * arg_struct.o  * arg_struct.o ;

            if (arg_struct.l  != ret_struct.l
              || arg_struct.s  != ret_struct.s
              || arg_struct.o  != ret_struct.o )
              {
                ACE_OS::printf ("**cube_struct error!\n");
                error_count++;
              }
                
          }
          break;
        }
      timer.stop();
      ACE_Profile_Timer::ACE_Elapsed_Time et;
      timer.elapsed_time (et);
      delta = ( (0.4 * fabs (et.real_time * (1000 * 1000))) + (0.6 * delta) ); // pow(10,6)
      latency += et.real_time;
    }

  if (call_count > 0)
    {
      if (error_count == 0)
        {

          latency /= call_count;

          if (latency > 0) {
            ACE_OS::printf ("cube average call ACE_OS::time\t= %f msec, \t"
                            "%f calls/second\n",
                            latency * 1000,
                            1 / latency);
            this->put_latency (latency * 1000, thread_id);
          }

        }
      ACE_OS::printf ("%d calls, %d errors\n", call_count, error_count);
    }

  //  cb->please_exit (env);
  return 0;
}
