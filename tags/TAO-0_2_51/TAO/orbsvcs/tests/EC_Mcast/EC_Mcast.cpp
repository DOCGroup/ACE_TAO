// $Id$

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Sched_Params.h"
#include "ace/Read_Buffer.h"

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Runtime_Scheduler.h"
#include "orbsvcs/Event/Event_Channel.h"
#include "orbsvcs/Event/Module_Factory.h"
#include "EC_Mcast.h"

#if !defined (__ACE_INLINE__)
#include "EC_Mcast.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(EC_Mcast, EC_Mcast, "$Id$")

ECM_Driver::ECM_Driver (void)
  : event_period_ (250000),
    event_count_ (100),
    config_filename_ (0),
    pid_filename_ (0),
    local_federations_count_ (0),
    all_federations_count_ (0)
{
}

int
ECM_Driver::run (int argc, char* argv[])
{
  TAO_TRY
    {
      this->orb_ =
        CORBA::ORB_init (argc, argv, "", TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA");
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

     if (this->parse_config_file ())
       return 1;

      ACE_DEBUG ((LM_DEBUG,
                  "Execution parameters:\n"
                  "  event period = <%d> (usecs)\n"
                  "  event count = <%d>\n"
                  "  config file name = <%s>\n"
                  "  pid file name = <%s>\n",

                  this->event_period_,
                  this->event_count_,

                  this->config_filename_?this->config_filename_:"nil",
                  this->pid_filename_?this->pid_filename_:"nil") );

      int i;
      for (i = 0; i < this->local_federations_count_; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "    name = <%s>\n"
                      "    port = <%d>\n"
                      "    supplier types:\n",
                      this->local_federations_[i]->name ()?this->local_federations_[i]->name ():"nil",
                      this->local_federations_[i]->mcast_port ()));
          int j;
          for (j = 0;
               j < this->local_federations_[i]->supplier_types ();
               ++j)
            {

              ACE_DEBUG ((LM_DEBUG,
                          "    name = <%s>\n"
                          "    ipadd = <%x>\n",
                          this->local_federations_[i]->supplier_name (j),
                          this->local_federations_[i]->supplier_ipaddr (j)));
            }
          ACE_DEBUG ((LM_DEBUG,
                      "    consumer types:\n"));
          for (j = 0;
               j < this->local_federations_[i]->consumer_types ();
               ++j)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "    name = <%s>\n"
                          "    ipadd = <%x>\n",
                          this->local_federations_[i]->consumer_name (j),
                          this->local_federations_[i]->consumer_ipaddr (j)));
            }
        }

      if (this->pid_filename_ != 0)
        {
          FILE* pid = ACE_OS::fopen (this->pid_filename_, "w");
          if (pid != 0)
            {
              ACE_OS::fprintf (pid, "%d\n", ACE_OS::getpid ());
              ACE_OS::fclose (pid);
            }
        }

#if 0
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
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) main thr_setprio failed\n"));
        }
#endif /* 0 */

      ACE_Config_Scheduler scheduler_impl;
      RtecScheduler::Scheduler_var scheduler =
        scheduler_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::String_var str =
        this->orb_->object_to_string (scheduler.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG, "The (local) scheduler IOR is <%s>\n",
                  str.in ()));

      if (ACE_Scheduler_Factory::server (scheduler.in ()) == -1)
        return -1;

      // Create the EventService implementation, but don't start its
      // internal threads.
      TAO_Reactive_Module_Factory module_factory;
      ACE_EventChannel ec_impl (0,
                                ACE_DEFAULT_EVENT_CHANNEL_TYPE,
                                &module_factory);

      // Register Event_Service with the Naming Service.
      RtecEventChannelAdmin::EventChannel_var ec =
        ec_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      str = this->orb_->object_to_string (ec.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "The (local) EC IOR is <%s>\n", str.in ()));

      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventChannelAdmin::EventChannel_var local_ec =
        ec_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: local EC objref ready\n"));

      this->open_federations (local_ec.in (),
                              scheduler.in (),
                              TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: open_federations done\n"));

      this->open_senders (local_ec.in (),
                          scheduler.in (),
                          TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: open_senders done\n"));

      this->open_receivers (local_ec.in (),
                            scheduler.in (),
                            TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: open_receivers done\n"));

      this->activate_federations (local_ec.in (),
                                  scheduler.in (),
                                  TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: activate_federations done\n"));

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: activate the  EC\n"));

      // Create the EC internal threads
      ec_impl.activate ();

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: running the test\n"));
      if (this->orb_->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);

      this->dump_results ();

      this->close_receivers (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      this->close_senders (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->close_federations (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: shutdown the EC\n"));
      ec_impl.shutdown ();

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: shutdown grace period\n"));

      ACE_Time_Value tv (5, 0);
      if (this->orb_->run (&tv) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "orb->run"), -1);

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
ECM_Driver::federation_has_shutdown (ECM_Local_Federation *federation,
                                     CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG, "Federation <%s> shuting down\n",
              federation->name ()));
  this->federations_running_--;
  if (this->federations_running_ <= 0)
    this->orb_->shutdown ();
}

void
ECM_Driver::open_federations (RtecEventChannelAdmin::EventChannel_ptr ec,
                              RtecScheduler::Scheduler_ptr scheduler,
                              CORBA::Environment &TAO_IN_ENV)
{
  for (int i = 0; i < this->local_federations_count_; ++i)
    {
      this->local_federations_[i]->open (this->event_count_,
                                         this->event_period_,
                                         ec, scheduler, TAO_IN_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
    }
}

void
ECM_Driver::activate_federations (RtecEventChannelAdmin::EventChannel_ptr ec,
                                  RtecScheduler::Scheduler_ptr scheduler,
                                  CORBA::Environment &TAO_IN_ENV)
{
  this->federations_running_ = this->local_federations_count_;
  for (int i = 0; i < this->local_federations_count_; ++i)
    {
      this->local_federations_[i]->activate (this->event_period_,
                                             ec, scheduler, TAO_IN_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
    }
}

void
ECM_Driver::close_federations (CORBA::Environment &TAO_IN_ENV)
{
  for (int i = 0; i < this->local_federations_count_; ++i)
    {
      this->local_federations_[i]->close (TAO_IN_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
    }
}

void
ECM_Driver::open_senders (RtecEventChannelAdmin::EventChannel_ptr ec,
                          RtecScheduler::Scheduler_ptr scheduler,
                          CORBA::Environment &TAO_IN_ENV)
{
  if (this->endpoint_.dgram ().open (ACE_Addr::sap_any) == -1)
    {
      // @@ TODO throw an application specific exception.
      TAO_IN_ENV.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_NO));
      return;
    }
  ACE_INET_Addr ignore_from;
  this->endpoint_.dgram ().get_local_addr (ignore_from);
  ACE_DEBUG ((LM_DEBUG, "ECM_Driver::open_senders - "
              "local endpoint = (%u:%d)\n",
              ignore_from.get_ip_address (),
              ignore_from.get_port_number ()));
  for (int i = 0; i < this->all_federations_count_; ++i)
    {
      this->all_federations_[i]->open (&this->endpoint_,
                                       ec,
                                       scheduler,
                                       TAO_IN_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
    }
}

void
ECM_Driver::close_senders (CORBA::Environment &TAO_IN_ENV)
{
  for (int i = 0; i < this->all_federations_count_; ++i)
    {
      this->all_federations_[i]->close (TAO_IN_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
    }
  this->endpoint_.dgram ().close ();
}

void
ECM_Driver::open_receivers (RtecEventChannelAdmin::EventChannel_ptr ec,
                            RtecScheduler::Scheduler_ptr scheduler,
                            CORBA::Environment &TAO_IN_ENV)
{
  for (int i = 0; i < this->local_federations_count_; ++i)
    {
      this->local_federations_[i]->open_receiver (ec,
                                                  scheduler,
                                                  &this->endpoint_,
                                                  TAO_IN_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
    }
}

void
ECM_Driver::close_receivers (CORBA::Environment &TAO_IN_ENV)
{
  for (int i = 0; i < this->local_federations_count_; ++i)
    {
      this->local_federations_[i]->close_receiver (TAO_IN_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
    }
}

void
ECM_Driver::dump_results (void)
{
  for (int i = 0; i < this->local_federations_count_; ++i)
    {
      this->local_federations_[i]->dump_results ();
    }
}


// ****************************************************************

int
ECM_Driver::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "l:p:c:n:t:f:");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'p':
          this->pid_filename_ = get_opt.optarg;
          break;

        case 'c':
          this->config_filename_ = get_opt.optarg;
          break;

        case 't':
          this->event_period_ = ACE_OS::atoi (get_opt.optarg);
          break;

        case 'n':
          this->event_count_ = ACE_OS::atoi (get_opt.optarg);
          break;

        case 'f':
          {
            char* aux;
            int i = 0;
            for (char* arg = ACE_OS::strtok_r (get_opt.optarg, ",", &aux);
                 arg != 0 && i < ECM_Driver::MAX_LOCAL_FEDERATIONS;
                 arg = ACE_OS::strtok_r (0, ",", &aux), ++i)
              {
                this->local_names_[i] = arg;
              }
            this->local_federations_count_ = i;
          }
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "[ORB options] "
                      "-n <event_count> "
                      "-t <event_period> "
                      "-l <localname> "
                      "-p <pid file name> "
                      "-c <config file name> "
                      "-f federation,federation,... "
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  if (this->event_count_ < 0
      || this->event_count_ >= ECM_Driver::MAX_EVENTS)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s: event count (%d) is out of range, "
                  "reset to default (%d)\n",
                  argv[0], this->event_count_,
                  100));
      this->event_count_ = 100;
    }

  return 0;
}

int
ECM_Driver::parse_config_file (void)
{
  FILE* cfg = 0;
  if (this->config_filename_ != 0)
    cfg = ACE_OS::fopen (this->config_filename_, "r");
  else
    cfg = stdin;

  if (cfg == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "cannot open config file <%s>\n",
                         this->config_filename_), -1);
    }

  int s = fscanf (cfg, "%d", &this->all_federations_count_);
  if (s == 0 || s == EOF)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "problem reading federation count\n"), -1);
    }
  // ACE_DEBUG ((LM_DEBUG,
  // "total federations = %d\n",
  // this->all_federations_count_));
  for (int i = 0; i < this->all_federations_count_; ++i)
    {
      if (this->skip_blanks (cfg, "reading federation name"))
        return -1;
      ACE_Read_Buffer reader(cfg);
      char* buf = reader.read (' ', ' ', '\0');
      char* name = CORBA::string_dup (buf);
      reader.alloc()->free (buf);


      int port;
      if (this->skip_blanks (cfg, "reading federation port number"))
        return -1;
      fscanf (cfg, "%d", &port);
      CORBA::UShort mcast_port = ACE_static_cast(CORBA::UShort, port);

      int ns, nc;
      if (this->skip_blanks (cfg, "reading supplier count"))
        return -1;
      s = fscanf (cfg, "%d", &ns);
      if (s == 0 || s == EOF)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "problem reading supplier count (%d)\n",
                             i), -1);
        }
      if (this->skip_blanks (cfg, "reading constumer count"))
        return -1;
      s = fscanf (cfg, "%d", &nc);
      if (s == 0 || s == EOF)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "problem reading consumer count (%d)\n",
                             i), -1);
        }
      // ACE_DEBUG ((LM_DEBUG, "i = %d <%s> <%d> <%d> <%d>\n",
      // i, name, mcast_port, ns, nc));

      char** supplier_names;
      char** consumer_names;
      ACE_NEW_RETURN (supplier_names, char*[ns], -1);
      ACE_NEW_RETURN (consumer_names, char*[nc], -1);

      if (this->parse_name_list (cfg, ns, supplier_names,
                                 "reading supplier list"))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "error parsing supplier list for <%s>\n",
                             name), -1);
        }

      if (this->parse_name_list (cfg, nc, consumer_names,
                                 "reading consumer list"))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "error parsing consumer list for <%s>\n",
                             name), -1);
        }

      ACE_NEW_RETURN (this->all_federations_[i],
                      ECM_Federation (name, mcast_port,
                                      ns, supplier_names,
                                      nc, consumer_names), -1);
    }
  ACE_OS::fclose (cfg);

  for (int j = 0; j < this->local_federations_count_; ++j)
    {
      int k = 0;
      for (; k < this->all_federations_count_; ++k)
        {
          if (ACE_OS::strcmp (this->local_names_[j],
                              this->all_federations_[k]->name ()) == 0)
            {
              ACE_NEW_RETURN (this->local_federations_[j],
                              ECM_Local_Federation (this->all_federations_[k],
                                                    this),
                              -1);
              break;
            }
        }
      if (k == this->all_federations_count_)
        ACE_ERROR ((LM_ERROR,
                    "Cannot find federations <%s>\n",
                    this->local_names_[j]));
    }

  return 0;
}

int
ECM_Driver::parse_name_list (FILE* file,
                             int n,
                             char** names,
                             const char* error_msg)
{
  for (int i = 0; i < n; ++i)
    {
      if (this->skip_blanks (file, error_msg))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "error on item %d while %s\n",
                             i, error_msg), -1);
        }
      ACE_Read_Buffer tmp(file);
      char* buf = tmp.read ('\n', '\n', '\0');
      names[i] = CORBA::string_dup (buf);
      tmp.alloc ()->free (buf);
    }
  return 0;
}

int
ECM_Driver::skip_blanks (FILE* file,
                         const char* error_msg)
{
  int c;
  // Consume all the blanks.
  while (isspace (c = fgetc (file)));
  if (c == EOF)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Unexpected EOF in config file while %s\n",
                         error_msg),
                        -1);
    }
  ungetc (c, file);
  return 0;
}
// ****************************************************************

ECM_Federation::ECM_Federation (char* name,
                                CORBA::UShort mcast_port,
                                int supplier_types,
                                char** supplier_names,
                                int consumer_types,
                                char** consumer_names)
  :  name_ (name),
     mcast_port_ (mcast_port),
     supplier_types_ (supplier_types),
     supplier_names_ (supplier_names),
     consumer_types_ (consumer_types),
     consumer_names_ (consumer_names),
     addr_server_ (mcast_port)
{
  ACE_NEW (this->supplier_ipaddr_, CORBA::ULong[this->supplier_types_]);
  ACE_NEW (this->consumer_ipaddr_, CORBA::ULong[this->consumer_types_]);

  int i;
  for (i = 0; i < this->supplier_types_; ++i)
    {
      ACE_INET_Addr addr (u_short(0), this->supplier_names_[i]);
      this->supplier_ipaddr_[i] = addr.get_ip_address ();
    }
  for (i = 0; i < this->consumer_types_; ++i)
    {
      ACE_INET_Addr addr (u_short(0), this->consumer_names_[i]);
      this->consumer_ipaddr_[i] = addr.get_ip_address ();
    }
}

void
ECM_Federation::open (TAO_ECG_UDP_Out_Endpoint *endpoint,
                      RtecEventChannelAdmin::EventChannel_ptr ec,
                      RtecScheduler::Scheduler_ptr scheduler,
                      CORBA::Environment &TAO_IN_ENV)
{
  const int bufsize = 512;
  char buf[bufsize];
  ACE_OS::strcpy (buf, this->name ());
  ACE_OS::strcat (buf, "/sender");

  RtecUDPAdmin::AddrServer_var addr_server =
    this->addr_server (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->sender_.init (ec, scheduler,
                      buf,
                      addr_server.in (),
                      endpoint,
                      TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  // @@ TODO Make this a parameter....
  this->sender_.mtu (64);

  RtecScheduler::handle_t rt_info =
    scheduler->create (buf, TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID(TAO_IN_ENV);

  // The worst case execution time is far less than 2
  // milliseconds, but that is a safe estimate....
  ACE_Time_Value tv (0, 2000);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
  scheduler->set (rt_info,
                  RtecScheduler::VERY_HIGH_CRITICALITY,
                  time, time, time,
                  0,
                  RtecScheduler::VERY_LOW_IMPORTANCE,
                  time,
                  0,
                  RtecScheduler::OPERATION,
                  TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  ACE_ConsumerQOS_Factory qos;
  qos.start_disjunction_group ();
  for (int i = 0; i < this->consumer_types (); ++i)
    {
      qos.insert_type (this->consumer_ipaddr (i), rt_info);
    }
  RtecEventChannelAdmin::ConsumerQOS qos_copy = qos.get_ConsumerQOS ();
  this->sender_.open (qos_copy, TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
}

void
ECM_Federation::close (CORBA::Environment &TAO_IN_ENV)
{
  this->sender_.close (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
  this->sender_.shutdown (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
}

RtecUDPAdmin::AddrServer_ptr
ECM_Federation::addr_server (CORBA::Environment &env)
{
  return this->addr_server_._this (env);
}

// ****************************************************************

ECM_Supplier::ECM_Supplier (ECM_Local_Federation* federation)
  :  federation_ (federation),
     consumer_ (this)
{
}

void
ECM_Supplier::open (const char* name,
                    RtecScheduler::Period_t period,
                    RtecEventChannelAdmin::EventChannel_ptr ec,
                    RtecScheduler::Scheduler_ptr scheduler,
                    CORBA::Environment &TAO_IN_ENV)
{
  RtecScheduler::handle_t rt_info =
    scheduler->create (name, TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  // The execution times are set to reasonable values, but
  // actually they are changed on the real execution, i.e. we
  // lie to the scheduler to obtain right priorities; but we
  // don't care if the set is schedulable.
  ACE_Time_Value tv (0, 2000);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);

  scheduler->set (rt_info,
                  RtecScheduler::VERY_HIGH_CRITICALITY,
                  time, time, time,
                  period,
                  RtecScheduler::VERY_LOW_IMPORTANCE,
                  time,
                  1,
                  RtecScheduler::OPERATION,
                  TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->supplier_id_ = ACE::crc32 (name);
  ACE_DEBUG ((LM_DEBUG, "ID for <%s> is %04.4x\n", name,
              this->supplier_id_));

  ACE_SupplierQOS_Factory qos;
  for (int i = 0; i < this->federation_->supplier_types (); ++i)
    {
      qos.insert (this->supplier_id_,
                  this->federation_->supplier_ipaddr (i),
                  rt_info, 1);
    }
  qos.insert (this->supplier_id_,
              ACE_ES_EVENT_SHUTDOWN,
              rt_info, 1);

  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    ec->for_suppliers (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->consumer_proxy_ =
    supplier_admin->obtain_push_consumer (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  RtecEventComm::PushSupplier_var objref = this->_this (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->consumer_proxy_->connect_push_supplier (objref.in (),
                                                qos.get_SupplierQOS (),
                                                TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
}

void
ECM_Supplier::close (CORBA::Environment &TAO_IN_ENV)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->consumer_proxy_ = 0;
}

void
ECM_Supplier::activate (const char* name,
                        RtecScheduler::Period_t period,
                        RtecEventChannelAdmin::EventChannel_ptr ec,
                        RtecScheduler::Scheduler_ptr scheduler,
                        CORBA::Environment &TAO_IN_ENV)
{
  const int bufsize = 512;
  char buf[bufsize];
  ACE_OS::strcpy (buf, "consumer_");
  ACE_OS::strcat (buf, name);
  RtecScheduler::handle_t rt_info =
    scheduler->create (buf, TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  // The execution times are set to reasonable values, but
  // actually they are changed on the real execution, i.e. we
  // lie to the scheduler to obtain right priorities; but we
  // don't care if the set is schedulable.
  ACE_Time_Value tv (0, 2000);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
  scheduler->set (rt_info,
                  RtecScheduler::VERY_HIGH_CRITICALITY,
                  time, time, time,
                  period,
                  RtecScheduler::VERY_LOW_IMPORTANCE,
                  time,
                  1,
                  RtecScheduler::OPERATION,
                  TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  // Also connect our consumer for timeout events from the EC.
  int interval = period / 10;
  ACE_Time_Value tv_timeout (interval / ACE_ONE_SECOND_IN_USECS,
                             interval % ACE_ONE_SECOND_IN_USECS);
  TimeBase::TimeT timeout;
  ORBSVCS_Time::Time_Value_to_TimeT (timeout, tv_timeout);

  ACE_ConsumerQOS_Factory consumer_qos;
  consumer_qos.start_disjunction_group ();
  consumer_qos.insert_time (ACE_ES_EVENT_INTERVAL_TIMEOUT,
                            timeout,
                            rt_info);

  // = Connect as a consumer.
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    ec->for_consumers (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  RtecEventComm::PushConsumer_var cref =
    this->consumer_._this (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->supplier_proxy_->connect_push_consumer (cref.in (),
                                                consumer_qos.get_ConsumerQOS (),
                                                TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
}

int
ECM_Supplier::supplier_id (void) const
{
  return this->supplier_id_;
}

void
ECM_Supplier::push (const RtecEventComm::EventSet& events,
                    CORBA::Environment& TAO_IN_ENV)
{
  for (u_int i = 0; i < events.length (); ++i)
    {
      const RtecEventComm::Event& e = events[i];
      if (e.header.type != ACE_ES_EVENT_INTERVAL_TIMEOUT)
        continue;

      this->federation_->supplier_timeout (this->consumer_proxy_.in (),
                                           TAO_IN_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
    }
}

void
ECM_Supplier::disconnect_push_supplier (CORBA::Environment& TAO_IN_ENV)
{
  ACE_UNUSED_ARG (TAO_IN_ENV);

  // this->supplier_proxy_->disconnect_push_supplier (TAO_IN_ENV);
}

void
ECM_Supplier::disconnect_push_consumer (CORBA::Environment &)
{
}

// ****************************************************************

ECM_Consumer::ECM_Consumer (ECM_Local_Federation *federation)
  : federation_ (federation),
    supplier_proxy_ (0),
    consumer_admin_ (0)
{
}

void
ECM_Consumer::open (const char* name,
                    RtecEventChannelAdmin::EventChannel_ptr ec,
                    RtecScheduler::Scheduler_ptr scheduler,
                    ACE_RANDR_TYPE &seed,
                    CORBA::Environment& TAO_IN_ENV)
{
  this->rt_info_ =
    scheduler->create (name, TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID(TAO_IN_ENV);

  // The worst case execution time is far less than 2
  // milliseconds, but that is a safe estimate....
  ACE_Time_Value tv (0, 2000);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
  scheduler->set (this->rt_info_,
                  RtecScheduler::VERY_HIGH_CRITICALITY,
                  time, time, time,
                  0,
                  RtecScheduler::VERY_LOW_IMPORTANCE,
                  time,
                  0,
                  RtecScheduler::OPERATION,
                  TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  // = Connect as a consumer.
  this->consumer_admin_ = ec->for_consumers (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->connect (seed, TAO_IN_ENV);
}

void
ECM_Consumer::connect (ACE_RANDR_TYPE &seed,
                       CORBA::Environment& TAO_IN_ENV)
{
  if (CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  this->supplier_proxy_ =
    this->consumer_admin_->obtain_push_supplier (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  ACE_ConsumerQOS_Factory qos;
  qos.start_disjunction_group ();
  qos.insert_type (ACE_ES_EVENT_SHUTDOWN,
                   this->rt_info_);
  const ECM_Federation* federation = this->federation_->federation ();
  for (int i = 0; i < federation->consumer_types (); ++i)
    {
      if (ACE_OS::rand_r (seed) < RAND_MAX / 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Federation %s leaves group %s\n",
                      federation->name (),
                      federation->consumer_name (i)));
          this->federation_->subscribed_bit (i, 0);
          continue;
        }
      ACE_DEBUG ((LM_DEBUG,
                  "Federation %s joins group %s\n",
                  federation->name (),
                  federation->consumer_name (i)));
      this->federation_->subscribed_bit (i, 1);
      qos.insert_type (federation->consumer_ipaddr (i),
                       this->rt_info_);
    }

  RtecEventComm::PushConsumer_var objref = this->_this (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->supplier_proxy_->connect_push_consumer (objref.in (),
                                                qos.get_ConsumerQOS (),
                                                TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
}

void
ECM_Consumer::disconnect (CORBA::Environment& TAO_IN_ENV)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ())
      || CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
  this->supplier_proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
ECM_Consumer::close (CORBA::Environment &TAO_IN_ENV)
{
  this->disconnect (TAO_IN_ENV);
  this->consumer_admin_ =
    RtecEventChannelAdmin::ConsumerAdmin::_nil ();
}

void
ECM_Consumer::push (const RtecEventComm::EventSet& events,
                    CORBA::Environment &TAO_IN_ENV)
{
  ACE_hrtime_t arrival = ACE_OS::gethrtime ();
  this->federation_->consumer_push (arrival, events, TAO_IN_ENV);
}

void
ECM_Consumer::disconnect_push_consumer (CORBA::Environment &)
{
}

// ****************************************************************

ECM_Local_Federation::ECM_Local_Federation (ECM_Federation *federation,
                                            ECM_Driver *driver)
  :  federation_ (federation),
     driver_ (driver),
     consumer_ (this),
     supplier_ (this),
     recv_count_ (0),
     unfiltered_count_ (0),
     invalid_count_ (0),
     send_count_ (0),
     event_count_ (0),
     last_publication_change_ (0),
     last_subscription_change_ (0),
     mcast_eh_ (&receiver_),
     seed_ (0),
     subscription_change_period_ (10000),
     publication_change_period_ (10000)
{
  ACE_NEW (this->subscription_subset_,
           CORBA::Boolean[this->consumer_types ()]);
}

ECM_Local_Federation::~ECM_Local_Federation (void)
{
  delete[] this->subscription_subset_;
}

void
ECM_Local_Federation::open (int event_count,
                            RtecScheduler::Period_t period,
                            RtecEventChannelAdmin::EventChannel_ptr ec,
                            RtecScheduler::Scheduler_ptr scheduler,
                            CORBA::Environment& TAO_IN_ENV)
{
  this->event_count_ = event_count;

  const int bufsize = 512;
  char buf[bufsize];
  ACE_OS::strcpy (buf, this->federation_->name ());
  ACE_OS::strcat (buf, "/supplier");

  this->supplier_.open (buf, period, ec, scheduler, TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  ACE_OS::strcpy (buf, this->federation_->name ());
  ACE_OS::strcat (buf, "/consumer");
  this->consumer_.open (buf, ec, scheduler, this->seed_, TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->last_subscription_change_ = ACE_OS::gettimeofday ();
}

void
ECM_Local_Federation::close (CORBA::Environment &TAO_IN_ENV)
{
  this->consumer_.close (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->supplier_.close (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
}

void
ECM_Local_Federation::activate (RtecScheduler::Period_t period,
                                RtecEventChannelAdmin::EventChannel_ptr ec,
                                RtecScheduler::Scheduler_ptr scheduler,
                                CORBA::Environment& TAO_IN_ENV)
{
  this->supplier_.activate (this->federation_->name (),
                            period,
                            ec, scheduler, TAO_IN_ENV);
}

void
ECM_Local_Federation::supplier_timeout (RtecEventComm::PushConsumer_ptr consumer,
                                        CORBA::Environment &TAO_IN_ENV)
{
  RtecEventComm::EventSet sent (1);
  sent.length (1);

  RtecEventComm::Event& s = sent[0];
  s.header.source = this->supplier_.supplier_id();
  s.header.ttl = 1;

  ACE_hrtime_t t = ACE_OS::gethrtime ();
  ORBSVCS_Time::hrtime_to_TimeT (s.header.creation_time, t);
  s.header.ec_recv_time = ORBSVCS_Time::zero;
  s.header.ec_send_time = ORBSVCS_Time::zero;

  s.data.x = 0;
  s.data.y = 0;

  this->event_count_--;

  // ACE_DEBUG ((LM_DEBUG, "Federation <%s> event count <%d>\n",
  //          this->name (), this->event_count_));

  if (this->event_count_ < 0)
    {
      this->driver_->federation_has_shutdown (this, TAO_IN_ENV);
      return;
    }
  int i = this->event_count_ % this->federation_->supplier_types ();
  s.header.type = this->federation_->supplier_ipaddr (i);

  consumer->push (sent, TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  this->send_count_++;

  ACE_Time_Value delta = ACE_OS::gettimeofday () -
    this->last_subscription_change_;

  double p = double (ACE_OS::rand_r (this->seed_)) / RAND_MAX;
  double maxp = double (delta.msec ()) / this->subscription_change_period_;

  if (4 * p < maxp)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Reconfiguring federation %s: %f %f\n",
                  this->name (), p, maxp));
      this->consumer_.disconnect (TAO_IN_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
      this->consumer_.connect (this->seed_, TAO_IN_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
      this->last_subscription_change_ = ACE_OS::gettimeofday ();
    }
}

void
ECM_Local_Federation::consumer_push (ACE_hrtime_t,
                                     const RtecEventComm::EventSet &event,
                                     CORBA::Environment &TAO_IN_ENV)
{
  ACE_UNUSED_ARG (TAO_IN_ENV);

  if (event.length () == 0)
    {
      return;
    }

  for (CORBA::ULong i = 0; i < event.length (); ++i)
    {
      const RtecEventComm::Event& e = event[i];

      this->recv_count_++;

      int j = 0;
      for (; j < this->federation_->consumer_types (); ++j)
        {
          CORBA::ULong type = e.header.type;
          if (type == this->federation_->consumer_ipaddr(j))
            {
              if (this->subscribed_bit (j) == 0)
                this->unfiltered_count_++;
              break;
            }
        }
      if (j == this->federation_->consumer_types ())
        this->invalid_count_++;
    }
}

void
ECM_Local_Federation::open_receiver (RtecEventChannelAdmin::EventChannel_ptr ec,
                                     RtecScheduler::Scheduler_ptr scheduler,
                                     TAO_ECG_UDP_Out_Endpoint* ignore_from,
                                     CORBA::Environment &TAO_IN_ENV)
{
  const int bufsize = 512;
  char buf[bufsize];
  ACE_OS::strcpy (buf, this->name ());
  ACE_OS::strcat (buf, "/receiver");

  RtecUDPAdmin::AddrServer_var addr_server =
    this->federation_->addr_server (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  ACE_Reactor* reactor = TAO_ORB_Core_instance ()->reactor ();

  // @@ This should be parameters...
  ACE_Time_Value expire_interval (1, 0);
  const int max_timeouts = 5;
  this->receiver_.init (ec, scheduler,
                        buf,
                        ignore_from,
                        addr_server.in (),
                        reactor, expire_interval, max_timeouts,
                        TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  RtecScheduler::handle_t rt_info =
    scheduler->create (buf, TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID(TAO_IN_ENV);

  // The worst case execution time is far less than 2
  // milliseconds, but that is a safe estimate....
  ACE_Time_Value tv (0, 2000);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
  scheduler->set (rt_info,
                  RtecScheduler::VERY_HIGH_CRITICALITY,
                  time, time, time,
                  0,
                  RtecScheduler::VERY_LOW_IMPORTANCE,
                  time,
                  1,
                  RtecScheduler::OPERATION,
                  TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  RtecEventComm::EventSourceID source = ACE::crc32 (buf);

  this->mcast_eh_.reactor (reactor);

  this->mcast_eh_.open (ec, TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

  ACE_SupplierQOS_Factory qos;
  for (int i = 0; i < this->consumer_types (); ++i)
    {
      qos.insert (source,
                  this->consumer_ipaddr (i),
                  rt_info, 1);
    }

  RtecEventChannelAdmin::SupplierQOS qos_copy =
    qos.get_SupplierQOS ();
  this->receiver_.open (qos_copy, TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);


}

void
ECM_Local_Federation::close_receiver (CORBA::Environment &TAO_IN_ENV)
{
  this->receiver_.close (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
  this->receiver_.shutdown (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
  this->mcast_eh_.close (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);
}

void
ECM_Local_Federation::dump_results (void) const
{
  double unfiltered_ratio = 0;
  if (this->recv_count_ != 0)
    unfiltered_ratio = double(this->unfiltered_count_)/this->recv_count_;
  double invalid_ratio = 0;
  if (this->recv_count_ != 0)
    invalid_ratio = double(this->invalid_count_)/this->recv_count_;

  ACE_DEBUG ((LM_DEBUG,
              "Federation: %s\n"
              "    events received: %d\n"
              "    unfiltered events received: %d\n"
              "                         ratio: %f\n"
              "    invalid events received: %d\n"
              "                      ratio: %f\n"
              "    events sent: %d\n",
              this->name (),
              this->recv_count_,
              this->unfiltered_count_,
              unfiltered_ratio,
              this->invalid_count_,
              invalid_ratio,
              this->send_count_));
}

void
ECM_Local_Federation::subscribed_bit (int i, CORBA::Boolean x)
{
  if (i > this->consumer_types ())
    return;
  this->subscription_subset_[i] = x;
}

CORBA::Boolean
ECM_Local_Federation::subscribed_bit (int i) const
{
  if (i > this->consumer_types ())
    return 0;
  return this->subscription_subset_[i];
}


// ****************************************************************

int
main (int argc, char *argv [])
{
  ECM_Driver driver;
  return driver.run (argc, argv);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_PushConsumer_Adapter<ECM_Supplier>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_PushConsumer_Adapter<ECM_Supplier>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
