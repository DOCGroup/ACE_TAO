// $Id$

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Sched_Params.h"

#include "tao/Timeprobe.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Event/Event_Channel.h"
#include "ECT_Supplier.h"

ACE_RCSID(EC_Throughput, ECT_Supplier, "$Id$")

int
main (int argc, char *argv [])
{
  ECTS_Driver driver;
  return driver.run (argc, argv);
}

// ****************************************************************

ECTS_Driver::ECTS_Driver (void)
  : n_suppliers_ (1),
    burst_count_ (10),
    burst_size_ (100),
    event_size_ (128),
    burst_pause_ (100),
    event_a_ (ACE_ES_EVENT_UNDEFINED),
    event_b_ (ACE_ES_EVENT_UNDEFINED + 1),
    pid_file_name_ (0)
{
}



int
ECTS_Driver::run (int argc, char* argv[])
{
  TAO_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->parse_args (argc, argv))
        return 1;

      ACE_DEBUG ((LM_DEBUG,
                  "Execution parameters:\n"
                  "  suppliers = <%d>\n"
                  "  burst count = <%d>\n"
                  "  burst size = <%d>\n"
                  "  event size = <%d>\n"
                  "  burst size = <%d>\n"
                  "  supplier Event A = <%d>\n"
                  "  supplier Event B = <%d>\n"
                  "  pid file name = <%s>\n",

                  this->n_suppliers_,
                  this->burst_count_,
                  this->burst_size_,
                  this->event_size_,
                  this->burst_pause_,
                  this->event_a_,
                  this->event_b_,

                  this->pid_file_name_?this->pid_file_name_:"nil") );

      if (this->pid_file_name_ != 0)
        {
          FILE* pid = ACE_OS::fopen (this->pid_file_name_, "w");
          if (pid != 0)
            {
              ACE_OS::fprintf (pid, "%d\n", ACE_OS::getpid ());
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
        orb->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (ACE_Scheduler_Factory::use_config (naming_context.in ()) == -1)
        return -1;

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup ("EventService");

      CORBA::Object_var ec_obj =
        naming_context->resolve (name, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventChannelAdmin::EventChannel_var channel;
      if (CORBA::is_nil (ec_obj.in ()))
        channel = RtecEventChannelAdmin::EventChannel::_nil ();
      else
        channel = RtecEventChannelAdmin::EventChannel::_narrow (ec_obj.in (),
                                                                TAO_TRY_ENV);
      TAO_CHECK_ENV;

      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->connect_suppliers (channel.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "connected supplier(s)\n"));

      this->activate_suppliers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "suppliers are active\n"));

      // Wait for the supplier threads...
      if (ACE_Thread_Manager::instance ()->wait () == -1)
        {
          ACE_ERROR ((LM_ERROR, "Thread_Manager wait failed\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "suppliers finished\n"));

      this->dump_results ();

      this->disconnect_suppliers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // @@ Deactivate the suppliers (as CORBA Objects?)
    }
  TAO_CATCH (CORBA::SystemException, sys_ex)
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("NON SYS EX");
    }
  TAO_ENDTRY;
  return 0;
}

void
ECTS_Driver::connect_suppliers (RtecEventChannelAdmin::EventChannel_ptr channel,
                           CORBA::Environment &_env)
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "supplier_%02.2d", i);

      ACE_NEW (this->suppliers_[i], Test_Supplier (this));

      this->suppliers_[i]->connect (buf,
                                    this->burst_count_,
                                    this->burst_size_,
                                    this->event_size_,
                                    this->burst_pause_,
                                    this->event_a_,
                                    this->event_b_,
                                    channel,
                                    _env);
      if (_env.exception () != 0) return;
    }
}

void
ECTS_Driver::activate_suppliers (CORBA::Environment &)
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->suppliers_[i]->activate ();
    }
}

void
ECTS_Driver::disconnect_suppliers (CORBA::Environment &_env)
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      this->suppliers_[i]->disconnect (_env);
      if (_env.exception () != 0) return;
    }
}

void
ECTS_Driver::dump_results (void)
{
  for (int i = 0; i < this->n_suppliers_; ++i)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "supplier_%02.2d", i);

      this->suppliers_[i]->dump_results (buf);
    }
}

int
ECTS_Driver::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "ds:u:n:t:b:h:p:");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 's':
          this->n_suppliers_ = ACE_OS::atoi (get_opt.optarg);
          break;

        case 'u':
          this->burst_count_ = ACE_OS::atoi (get_opt.optarg);
          break;

        case 'n':
          this->burst_size_ = ACE_OS::atoi (get_opt.optarg);
          break;

        case 'b':
          this->event_size_ = ACE_OS::atoi (get_opt.optarg);
          break;

        case 't':
          this->burst_pause_ = ACE_OS::atoi (get_opt.optarg);
          break;

        case 'h':
          {
            char* aux;
                char* arg = ACE_OS::strtok_r (get_opt.optarg, ",", &aux);

            this->event_a_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
                arg = ACE_OS::strtok_r (0, ",", &aux);
            this->event_b_ = ACE_ES_EVENT_UNDEFINED + ACE_OS::atoi (arg);
          }
          break;

        case 'p':
          this->pid_file_name_ = get_opt.optarg;
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "[ORB options] "
                      "-s <nsuppliers> "
                      "-u <burst count> "
                      "-n <burst size> "
                      "-b <event payload size> "
                      "-T <burst pause (usecs)> "
                      "-h <eventa,eventb> "
                      "-p <pid file name> "
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  if (this->burst_count_ <= 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: burst count (%d) is out of range, "
                  "reset to default (%d)\n",
                  argv[0], this->burst_count_,
                  100));
      this->burst_count_ = 100;
    }

  if (this->burst_size_ <= 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: burst size (%d) is out of range, "
                  "reset to default (%d)\n",
                  argv[0], this->burst_size_,
                  10));
      this->burst_size_ = 10;
    }

  if (this->event_size_ < 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: event size (%d) is out of range, "
                  "reseting to default (%d)\n",
                  argv[0], this->event_size_,
                  128));
      this->event_size_ = 128;
    }

  if (this->n_suppliers_ <= 0)
    {
      this->n_suppliers_ = 1;
      ACE_ERROR_RETURN ((LM_DEBUG,
                         "%s: number of suppliers out of range, "
                         "reset to default (%d)\n",
                         argv[0], 1), -1);
    }

  return 0;
}

// ****************************************************************

Test_Supplier::Test_Supplier (ECTS_Driver *driver)
  :  driver_ (driver),
     supplier_ (this),
     burst_count_ (0),
     burst_size_ (0),
     event_size_ (0),
     burst_pause_ (0)
{
}

void
Test_Supplier::connect (const char* name,
                        int burst_count,
                        int burst_size,
                        int event_size,
                        int burst_pause,
                        int event_a,
                        int event_b,
                        RtecEventChannelAdmin::EventChannel_ptr ec,
                        CORBA::Environment &_env)
{
  this->burst_count_ = burst_count;
  this->burst_size_ = burst_size;
  this->event_size_ = event_size;
  this->burst_pause_ = burst_pause;
  this->event_a_ = event_a;
  this->event_b_ = event_b;
    
  RtecScheduler::Scheduler_ptr server =
    ACE_Scheduler_Factory::server ();

  RtecScheduler::handle_t rt_info =
    server->create (name, _env);
  TAO_CHECK_ENV_RETURN_VOID (_env);

  ACE_Time_Value tv (0, burst_pause);
  RtecScheduler::Period_t rate = tv.usec () * 10;

  // The execution times are set to reasonable values, but
  // actually they are changed on the real execution, i.e. we
  // lie to the scheduler to obtain right priorities; but we
  // don't care if the set is schedulable.
  tv.set (0, 2000);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
  server->set (rt_info,
               RtecScheduler::VERY_HIGH_CRITICALITY,
               time, time, time,
               rate,
               RtecScheduler::VERY_LOW_IMPORTANCE,
               time,
               1,
               RtecScheduler::OPERATION,
               _env);
  TAO_CHECK_ENV_RETURN_VOID (_env);

  this->supplier_id_ = ACE::crc32 (name);
  ACE_DEBUG ((LM_DEBUG, "ID for <%s> is %04.4x\n", name,
              this->supplier_id_));

  ACE_SupplierQOS_Factory qos;
  qos.insert (this->supplier_id_,
              event_a,
              rt_info, 1);
  qos.insert (this->supplier_id_,
              event_b,
              rt_info, 1);
  qos.insert (this->supplier_id_,
              ACE_ES_EVENT_SHUTDOWN,
              rt_info, 1);

  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    ec->for_suppliers (_env);
  TAO_CHECK_ENV_RETURN_VOID (_env);

  this->consumer_proxy_ =
    supplier_admin->obtain_push_consumer (_env);
  TAO_CHECK_ENV_RETURN_VOID (_env);

  RtecEventComm::PushSupplier_var objref =
    this->supplier_._this (_env);
  TAO_CHECK_ENV_RETURN_VOID (_env);

  this->consumer_proxy_->connect_push_supplier (objref.in (),
                                                qos.get_SupplierQOS (),
                                                _env);
  TAO_CHECK_ENV_RETURN_VOID (_env);
}

void
Test_Supplier::disconnect (CORBA::Environment &_env)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer (_env);
  TAO_CHECK_ENV_RETURN_VOID (_env);

  this->consumer_proxy_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
}

int
Test_Supplier::svc ()
{
  TAO_TRY
    {
      ACE_Time_Value tv (0, this->burst_pause_);

      ACE_Message_Block mb (this->event_size_);
      mb.wr_ptr (this->event_size_);

      RtecEventComm::EventSet event (1);
      event.length (1);
      event[0].header.source = this->supplier_id ();
      event[0].header.ttl = 1;

      ACE_hrtime_t t = ACE_OS::gethrtime ();
      ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time, t);
      event[0].header.ec_recv_time = ORBSVCS_Time::zero;
      event[0].header.ec_send_time = ORBSVCS_Time::zero;

      event[0].data.x = 0;
      event[0].data.y = 0;

      // We use replace to minimize the copies, this should result
      // in just one memory allocation;
      event[0].data.payload.replace (this->event_size_,
                                     &mb);

      this->timer_.start ();
      for (int i = 0; i < this->burst_count_; ++i)
        {
          for (int j = 0; j < this->burst_size_; ++j)
            {
              if (j % 2 == 0)
                event[0].header.type = this->event_a_;
              else
                event[0].header.type = this->event_b_;

              // ACE_DEBUG ((LM_DEBUG, "(%t) supplier push event\n"));
              this->consumer_proxy ()->push (event, TAO_TRY_ENV);

              TAO_CHECK_ENV;
            }
          ACE_OS::sleep (tv);
        }

      // Send one event shutdown from each supplier
      event[0].header.type = ACE_ES_EVENT_SHUTDOWN;
      this->consumer_proxy ()->push(event, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      this->timer_.stop ();
      
    }
  TAO_CATCH (CORBA::SystemException, sys_ex)
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("NON SYS EX");
    }
  TAO_ENDTRY;
  return 0;
}

void
Test_Supplier::disconnect_push_supplier (CORBA::Environment& _env)
{
}

int Test_Supplier::supplier_id (void) const
{
  return this->supplier_id_;
}

RtecEventChannelAdmin::ProxyPushConsumer_ptr
Test_Supplier::consumer_proxy (void)
{
  return this->consumer_proxy_.in ();
}

void
Test_Supplier::dump_results (const char* name)
{
  ACE_Time_Value tv;
  this->timer_.elapsed_time (tv);

  int event_count = this->burst_count_ * this->burst_size_ + 1;
  double f = 1.0 / (tv.sec () + tv.usec () / 1000000.0);
  double eps = event_count * f;
  
  ACE_DEBUG ((LM_DEBUG,
              "ECT_Supplier (%s):\n"
              "    Total time: %d.%08.8d (secs.usecs)\n"
              "    Total events: %d\n"
              "    Events per second: %.3f\n",
              name,
              tv.sec (), tv.usec (),
              event_count,
              eps));
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_PushSupplier_Adapter<Test_Supplier>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_PushSupplier_Adapter<Test_Supplier>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
