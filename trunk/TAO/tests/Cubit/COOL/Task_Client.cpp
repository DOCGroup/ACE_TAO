#include "Task_Client.h"

Client::Client (char *hostname, unsigned int n_threads, unsigned int loop_count)
  : ACE_Task<ACE_MT_SYNCH> (ACE_Thread_Manager::instance ()),
    hostname_ (hostname),
    start_count_ (0),
    loop_count_ (loop_count)
{
  barrier_ = new ACE_Barrier (n_threads);
  latency_ = new long [n_threads];
  if (this->activate (THR_BOUND | THR_NEW_LWP, n_threads) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));
}

void
Client::put_latency (long latency, unsigned int thread_id) {
  lock_.acquire ();
  latency_[thread_id] = latency;
  ACE_DEBUG ((LM_DEBUG, "(%t) My latency was %d\n", latency));
  lock_.release ();
}

double
Client::get_high_priority_latency () {
  return (double) latency_[0] / 1000;
}

double
Client::get_low_priority_latency () {
  double l = 0;
  for (int i = 1; i < start_count_; i++)
    l += (double) latency_[i];
  return l / (double)(1000 * (start_count_ - 1));
}

int
Client::svc (void) 
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread created\n"));
  unsigned int thread_id;
  Cubit_ptr cb;
  lock_.acquire ();
  {
    char server_name[1024];
    thread_id = start_count_;
    start_count_++;
    if (thread_id == 0) {
      strcpy (server_name, "HP_Cubit");
      ACE_DEBUG ((LM_DEBUG, "(%t) Im the high priority client.\n"));
    }
    else {
      strcpy (server_name, "LP_Cubit");
      ACE_DEBUG ((LM_DEBUG, "(%t) Im a low priority client.\n"));
    }    
    
    //
    // Initialise client's binding to an
    // arbitrary cubit server (at some host)
    //
    CORBA_Environment env;
    COOL_NamingService_var naming = thisCapsule->naming_service (env);
    CORBA_Object_ptr obj;
    naming->import ("Cubit", obj, env);
   
    if (env.exception ()) {
      ACE_ERROR ((LM_ERROR, "naming->import failed..\n"));
      return -1;
    }

    cb = Cubit::_narrow (obj);
    CORBA_release (obj);
    
    ACE_DEBUG ((LM_DEBUG, "(%t) Binding succeeded\n"));
  }
  lock_.release ();
  
  ACE_DEBUG ((LM_DEBUG, "(%t) Waiting for other threads to finish binding..\n"));
  barrier_->wait ();
  ACE_DEBUG ((LM_DEBUG, "(%t) Everyone's done, here I go!!\n"));


  run_tests (cb, loop_count_, thread_id);
  return 0;
}

void
Client::run_tests (Cubit_ptr cb, int loop_count, unsigned int thread_id) 
{
    //
  // Make the calls in a loop.
  //
  unsigned i;
  unsigned call_count, error_count;

  call_count = 0;
  error_count = 0;

  ACE_Time_Value before, after;

  before = ACE_OS::gettimeofday();
  
  //
  // Cube an octet.
  //
  
  for (i = 0; i < loop_count; i++)
    {

      call_count++;

      CORBA::Octet arg_octet = func (i), ret_octet;

      TRY {
      ret_octet = cb->cube_octet (arg_octet);
      } 
      CATCHANY {
        cerr << "Call failed: " << IT_X;
        error_count++;
      } 
      ENDTRY;
      arg_octet = arg_octet * arg_octet * arg_octet;
      if (arg_octet != ret_octet) {
        ACE_OS::printf ("** cube_octet(%d)  (--> %d)\n", arg_octet , ret_octet);
        error_count++;
      }
    
  
  //
  // Cube a short.
  //
      call_count++;

      CORBA::Short arg_short = func (i), ret_short;

      TRY {
      ret_short = cb->cube_short (arg_short);
      } 
      CATCHANY {
        cerr << "Call failed: " << IT_X;
        error_count++;
      } 
      ENDTRY;
      arg_short = arg_short * arg_short * arg_short;
      if (arg_short != ret_short) {
        ACE_OS::printf ("** cube_short(%d)  (--> %d)\n", arg_short , ret_short);
        error_count++;
      }

  //
  // Cube a long.
  //
  
      call_count++;

      CORBA::Long arg_long = func (i), ret_long;

      TRY {
      ret_long = cb->cube_long (arg_long);
      } 
      CATCHANY {
        cerr << "Call failed: " << IT_X;
      } 
      ENDTRY;
      arg_long = arg_long * arg_long * arg_long;
      if (arg_long != ret_long) {
        ACE_OS::printf ("** cube_long(%d)  (--> %d)\n", arg_long , ret_long);
        error_count++;
      }


      //
      // Cube a "struct" ...
      //
      Cubit::Many	arg_struct, ret_struct;
      
       call_count++;
       
       arg_struct.l = func (i);
       arg_struct.s = func (i);
       arg_struct.o = func (i);
       
       TRY {
         ret_struct = cb->cube_struct (arg_struct);
       }
       CATCHANY {
         cerr << "Call failed: " << IT_X;
         error_count++;
       } 
       ENDTRY;
       arg_struct.l = arg_struct.l  * arg_struct.l  * arg_struct.l ;
       arg_struct.s = arg_struct.s  * arg_struct.s  * arg_struct.s ;
       arg_struct.o = arg_struct.o  * arg_struct.o  * arg_struct.o ;
       
       if (arg_struct.l  != ret_struct.l 
           || arg_struct.s  != ret_struct.s 
           || arg_struct.o  != ret_struct.o )
         {
           cerr << "** cube_struct ERROR\n";
           error_count++;
         }
    }
    
  
  after = ACE_OS::gettimeofday();
  
  if (call_count > 0) 
    {
      if (error_count == 0)
        {
          ACE_Time_Value diff = after - before;
          unsigned long	us = diff.sec() * 1000 * 1000 + diff.usec();
          
          us /= call_count;
          
          if (us > 0) {
//             ACE_OS::printf ("cube average call ACE_OS::time\t= %ld.%.03ldms, \t"
//                             "%ld calls/second\n",
//                             us / 1000, us % 1000,
//                             1000000L / us);
            this->put_latency (us, thread_id);
          }
      
        }
      //      ACE_OS::printf ("%d calls, %d errors\n", call_count, error_count);
    }
}

