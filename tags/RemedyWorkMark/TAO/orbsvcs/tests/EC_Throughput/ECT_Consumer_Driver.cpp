// $Id$

#include "ECT_Consumer_Driver.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Time_Utilities.h"

#include "tao/Timeprobe.h"
#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_unistd.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ECT_Consumer_Driver driver;
  return driver.run (argc, argv);
}

// ****************************************************************

ECT_Consumer_Driver::ECT_Consumer_Driver (void)
  : n_consumers_ (1),
    n_suppliers_ (1),
    type_start_ (ACE_ES_EVENT_UNDEFINED),
    type_count_ (1),
    stall_length_(0),
    shutdown_event_channel_ (1),
    pid_file_name_ (0),
    active_count_ (0)
{
}

ECT_Consumer_Driver::~ECT_Consumer_Driver (void)
{
}

int
ECT_Consumer_Driver::run (int argc, ACE_TCHAR* argv[])
{
  try
    {
      this->orb_ =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (this->parse_args (argc, argv))
        return 1;

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Execution parameters:\n"
                      "  consumers = <%d>\n"
                      "  suppliers = <%d>\n"
                      "  type start = <%d>\n"
                      "  type count = <%d>\n"
                      "  pid file name = <%s>\n",

                      this->n_consumers_,
                      this->n_suppliers_,
                      this->type_start_,
                      this->type_count_,

                      this->pid_file_name_?this->pid_file_name_:ACE_TEXT("nil")) );
        }

      if (this->pid_file_name_ != 0)
        {
          FILE* pid = ACE_OS::fopen (this->pid_file_name_, "w");
          if (pid != 0)
            {
              ACE_OS::fprintf (pid, "%ld\n",
                               static_cast<long> (ACE_OS::getpid ()));
              ACE_OS::fclose (pid);
            }
        }

      int min_priority =
        ACE_Sched_Params::priority_min (ACE_SCHED_FIFO);
        // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

      if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                                  min_priority,
                                                  ACE_SCOPE_PROCESS)) != 0)
        {
          if (ACE_OS::last_error () == EPERM)
            ACE_DEBUG ((LM_DEBUG,
                        "%s: user is not superuser, "
                        "so remain in time-sharing class\n", argv[0]));
          else
            ACE_ERROR ((LM_ERROR,
                        "%s: ACE_OS::sched_params failed\n", argv[0]));
        }

      if (ACE_OS::thr_setprio (min_priority) == -1)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) main thr_setprio failed,"
                      "no real-time features\n"));
        }

      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup ("ScheduleService");

      CORBA::Object_var sched_obj =
        naming_context->resolve (schedule_name);
      if (CORBA::is_nil (sched_obj.in ()))
        return 1;
      RtecScheduler::Scheduler_var scheduler =
        RtecScheduler::Scheduler::_narrow (sched_obj.in ());

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup ("EventService");

      CORBA::Object_var ec_obj =
        naming_context->resolve (name);

      RtecEventChannelAdmin::EventChannel_var channel;
      if (CORBA::is_nil (ec_obj.in ()))
        channel = RtecEventChannelAdmin::EventChannel::_nil ();
      else
        channel = RtecEventChannelAdmin::EventChannel::_narrow (ec_obj.in ());

      poa_manager->activate ();

      this->connect_consumers (scheduler.in (), channel.in ());

      ACE_DEBUG ((LM_DEBUG, "connected consumer(s)\n"));

      ACE_DEBUG ((LM_DEBUG, "running the test\n"));
      for (;;)
        {
          ACE_Time_Value tv (1, 0);
          this->orb_->perform_work (tv);
          ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 1);
          if (this->active_count_ <= 0)
            break;
        }
      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      this->dump_results ();

      this->disconnect_consumers ();

      if (this->shutdown_event_channel_ != 0)
        {
          channel->destroy ();
        }

      root_poa->destroy (1, 1);

      this->orb_->destroy ();
    }
  catch (const CORBA::SystemException& sys_ex)
    {
      sys_ex._tao_print_exception ("SYS_EX");
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("NON SYS EX");
    }
  return 0;
}

void
ECT_Consumer_Driver::shutdown_consumer (void*)
{
  // int ID =
  //   (reinterpret_cast<Test_Consumer**> (consumer_cookie)
  //    - this->consumers_);
  //
  // ACE_DEBUG ((LM_DEBUG, "(%t) events received by consumer %d\n", ID));

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->active_count_--;
}

void
ECT_Consumer_Driver::connect_consumers
     (RtecScheduler::Scheduler_ptr scheduler,
      RtecEventChannelAdmin::EventChannel_ptr channel)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
    this->active_count_ = this->n_consumers_;
  }
  for (int i = 0; i < this->n_consumers_; ++i)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "consumer_%02d", i);

      ACE_NEW (this->consumers_[i],
               Test_Consumer (this,
                              this->consumers_ + i,
                              this->n_suppliers_,
                              this->stall_length_));

      this->consumers_[i]->connect (scheduler,
                                    buf,
                                    this->type_start_,
                                    this->type_count_,
                                    channel);
    }
}

void
ECT_Consumer_Driver::dump_results (void)
{
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();

  ACE_Throughput_Stats throughput;
  for (int i = 0; i < this->n_consumers_; ++i)
    {
      ACE_TCHAR buf[BUFSIZ];
      ACE_OS::sprintf (buf, ACE_TEXT("consumer_%02d"), i);

      this->consumers_[i]->dump_results (buf, gsf);
      this->consumers_[i]->accumulate (throughput);
    }
  throughput.dump_results (ACE_TEXT("ECT_Consumer/totals"), gsf);
}

void
ECT_Consumer_Driver::disconnect_consumers (void)
{
  for (int i = 0; i < this->n_consumers_; ++i)
    {
      this->consumers_[i]->disconnect ();

      delete this->consumers_[i];
      this->consumers_[i] = 0;
    }
}

int
ECT_Consumer_Driver::parse_args (int argc, ACE_TCHAR *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("xdc:s:h:p:o:"));
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'x':
          this->shutdown_event_channel_ = 0;
          break;

        case 'c':
          this->n_consumers_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 's':
          this->n_suppliers_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'h':
          {
            char* aux;
                char* arg = ACE_OS::strtok_r (ACE_TEXT_ALWAYS_CHAR(get_opt.opt_arg ()), ",", &aux);

            this->type_start_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->type_count_ = ACE_OS::atoi (arg);
          }
          break;

        case 'p':
          this->pid_file_name_ = get_opt.opt_arg ();
          break;

        case 'o':
          this->stall_length_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "[ORB options] "
                      "-d -x "
                      "-c <n_consumers> "
                      "-s <n_suppliers> "
                      "-h <type_start,type_count> "
                      "-p <pid file name> "
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  if (this->n_suppliers_ <= 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: number of suppliers (%d) is out of range, "
                  "reset to default (%d)\n",
                  argv[0], this->n_suppliers_,
                  1));
      this->n_suppliers_ = 1;
    }

  if (this->n_consumers_ <= 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: number of consumers or "
                         "suppliers out of range\n", argv[0]), -1);
    }

  if (this->type_count_ <= 0)
    {
      this->type_count_ = 1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: number of event types "
                         "suppliers out of range, reset to default (1)\n",
                         argv[0]), -1);
    }

  return 0;
}
