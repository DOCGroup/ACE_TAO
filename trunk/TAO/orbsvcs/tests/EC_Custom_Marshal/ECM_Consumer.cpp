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
#include "ECM_Consumer.h"
#include "ECM_Data.h"

ACE_RCSID(EC_Custom_Marshal, ECM_Consumer, "$Id$")

int
main (int argc, char *argv [])
{
  Driver driver;
  return driver.run (argc, argv);
}

// ****************************************************************

Driver::Driver (void)
  : n_consumers_ (1),
    event_count_ (100),
    event_a_ (ACE_ES_EVENT_UNDEFINED),
    event_b_ (ACE_ES_EVENT_UNDEFINED + 1),
    pid_file_name_ (0),
    recv_count_ (0)
{
}



int
Driver::run (int argc, char* argv[])
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
                  "  consumers = <%d>\n"
                  "  event count = <%d>\n"
                  "  supplier Event A = <%d>\n"
                  "  supplier Event B = <%d>\n"
                  "  pid file name = <%s>\n",

                  this->n_consumers_,
                  this->event_count_,
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

      this->connect_consumers (channel.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "connected consumer(s)\n"));

      ACE_DEBUG ((LM_DEBUG, "running the test\n"));
      if (orb->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);
      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      this->disconnect_consumers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      channel->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
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
Driver::push_consumer (void* /* consumer_cookie */,
                       ACE_hrtime_t /* arrival */,
                       const RtecEventComm::EventSet& events,
                       CORBA::Environment &TAO_IN_ENV)
{
  // int ID =
  //   (ACE_reinterpret_cast(Test_Consumer**,consumer_cookie)
  //    - this->consumers_);
  //
  // ACE_DEBUG ((LM_DEBUG, "(%t) events received by consumer %d\n", ID));

  if (events.length () == 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }

  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->recv_count_mutex_);

  this->recv_count_ += events.length ();

  int x = this->event_count_ / 10;
  if (this->recv_count_ % x == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
		  "ECM_Consumer (%P|%t): %d events received\n",
		  this->recv_count_));
    }

  if (this->recv_count_ >= this->event_count_)
    {
      TAO_ORB_Core_instance ()->orb ()->shutdown ();
    }

  // ACE_DEBUG ((LM_DEBUG, "%d event(s)\n", events.length ()));

  for (u_int i = 0; i < events.length (); ++i)
    {
      const RtecEventComm::Event& e = events[i];

      if (e.data.payload.mb () == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "No data in event[%d]\n", i));
          continue;
        }

      // @@ TODO this is a little messy, infortunately we have to
      // extract the first byte to determine the byte order, the CDR
      // cannot do it for us because in certain cases the byte order
      // is not in the encapsulation. Maybe we need another
      // constructor for the InputCDR streams (but there are too many
      // already!)?

      // Note that there is no copying
      int byte_order = e.data.payload[0];

      ACE_Message_Block* mb =
        ACE_Message_Block::duplicate (e.data.payload.mb ());
      mb->rd_ptr (1); // skip the byte order

      TAO_InputCDR cdr (mb, byte_order);

      ECM_IDLData::Info info;
      cdr.decode (ECM_IDLData::_tc_Info, &info, 0, TAO_IN_ENV);
      if (TAO_IN_ENV.exception () != 0) return;

      ECM_Data other;
      cdr >> other;

      if (!cdr.good_bit ())
	ACE_ERROR ((LM_ERROR, "Problem demarshalling C++ data\n"));

      CORBA::ULong n = info.trajectory.length ();
      // ACE_DEBUG ((LM_DEBUG, "Payload contains <%d> elements\n", n));
      // ACE_DEBUG ((LM_DEBUG, "Inventory <%s> contains <%d> elements\n",
      // other.description.in (),
      // other.inventory.current_size ()));

      for (CORBA::ULong j = 0; j < n; ++j)
        {
          ECM_IDLData::Point& p = info.trajectory[j];
          if (p.x != j || p.y != j*j)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "invalid data in trajectory[%d] = (%f,%f)\n",
                          j, p.x, p.y));
            }
        }
    }
}

void
Driver::connect_consumers (RtecEventChannelAdmin::EventChannel_ptr channel,
                           CORBA::Environment &TAO_IN_ENV)
{
  for (int i = 0; i < this->n_consumers_; ++i)
    {
      char buf[BUFSIZ];
      ACE_OS::sprintf (buf, "consumer_%02.2d", i);

      ACE_NEW (this->consumers_[i],
               Test_Consumer (this, this->consumers_ + i));

      this->consumers_[i]->connect (buf,
                                    this->event_a_,
                                    this->event_b_,
                                    channel,
                                    TAO_IN_ENV);
      if (TAO_IN_ENV.exception () != 0) return;
    }
}

void
Driver::disconnect_consumers (CORBA::Environment &TAO_IN_ENV)
{
  for (int i = 0; i < this->n_consumers_; ++i)
    {
      this->consumers_[i]->disconnect (TAO_IN_ENV);
      if (TAO_IN_ENV.exception () != 0) return;
    }
}

int
Driver::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "dc:n:h:p:");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'c':
          this->n_consumers_ = ACE_OS::atoi (get_opt.optarg);
          break;

        case 'n':
          this->event_count_ = ACE_OS::atoi (get_opt.optarg);
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
                      "-s <global|local> "
                      "-a (send data in events) "
                      "-h <args> "
                      "-p <pid file name> "
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  if (this->event_count_ <= 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: event count (%d) is out of range, "
                  "reset to default (%d)\n",
                  argv[0], this->event_count_,
                  100));
      this->event_count_ = 100;
    }

  if (this->n_consumers_ <= 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: number of consumers or "
                         "suppliers out of range\n", argv[0]), -1);
    }

  return 0;
}

// ****************************************************************

Test_Consumer::Test_Consumer (Driver *driver, void *cookie)
  : driver_ (driver),
    cookie_ (cookie)
{
}

void
Test_Consumer::connect (const char* name,
                        int event_a, int event_b,
                        RtecEventChannelAdmin::EventChannel_ptr ec,
                        CORBA::Environment& TAO_IN_ENV)
{
  RtecScheduler::Scheduler_ptr server =
    ACE_Scheduler_Factory::server ();

  RtecScheduler::handle_t rt_info =
    server->create (name, TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;

  // The worst case execution time is far less than 2
  // milliseconds, but that is a safe estimate....
  ACE_Time_Value tv (0, 2000);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
  server->set (rt_info,
               RtecScheduler::VERY_HIGH_CRITICALITY,
               time, time, time,
               0,
               RtecScheduler::VERY_LOW_IMPORTANCE,
               time,
               0,
               RtecScheduler::OPERATION,
               TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;

  ACE_ConsumerQOS_Factory qos;
  qos.start_disjunction_group ();
  qos.insert_type (ACE_ES_EVENT_SHUTDOWN, rt_info);
  qos.insert_type (event_a, rt_info);
  qos.insert_type (event_b, rt_info);

  // = Connect as a consumer.
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    ec->for_consumers (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;

  RtecEventComm::PushConsumer_var objref = this->_this (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;

  this->supplier_proxy_->connect_push_consumer (objref.in (),
                                                qos.get_ConsumerQOS (),
                                                TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;
}

void
Test_Consumer::disconnect (CORBA::Environment &TAO_IN_ENV)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;

  this->supplier_proxy_ = 0;
}

void
Test_Consumer::push (const RtecEventComm::EventSet& events,
                     CORBA::Environment &TAO_IN_ENV)
{
  ACE_hrtime_t arrival = ACE_OS::gethrtime ();
  this->driver_->push_consumer (this->cookie_, arrival, events, TAO_IN_ENV);
}

void
Test_Consumer::disconnect_push_consumer (CORBA::Environment &)
{
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
