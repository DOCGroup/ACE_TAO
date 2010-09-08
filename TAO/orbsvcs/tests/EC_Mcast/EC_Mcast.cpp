// $Id$

#include "EC_Mcast.h"

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Time_Utilities.h"

#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

#include "tao/ORB_Core.h"

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Sched_Params.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_ctype.h"

#if !defined (__ACE_INLINE__)
#include "EC_Mcast.inl"
#endif /* __ACE_INLINE__ */

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
ECM_Driver::run (int argc, ACE_TCHAR* argv[])
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

                  this->config_filename_?this->config_filename_:ACE_TEXT("nil"),
                  this->pid_filename_?this->pid_filename_:ACE_TEXT("nil")) );

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
              ACE_OS::fprintf (pid, "%ld\n",
                               static_cast<long> (ACE_OS::getpid ()));
              ACE_OS::fclose (pid);
            }
        }

      TAO_EC_Event_Channel_Attributes attr (root_poa.in (),
                                            root_poa.in ());
      TAO_EC_Event_Channel ec_impl (attr);

      // Register Event_Service with the Naming Service.
      PortableServer::ObjectId_var id =
        root_poa->activate_object (&ec_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow (object.in ());

      CORBA::String_var str =
        this->orb_->object_to_string (ec.in ());

      ACE_DEBUG ((LM_DEBUG, "The (local) EC IOR is <%s>\n", str.in ()));

      poa_manager->activate ();

      ec_impl.activate ();

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: local EC objref ready\n"));

      this->open_federations (ec.in ());

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: open_federations done\n"));

      this->open_senders (ec.in ());

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: open_senders done\n"));

      this->open_receivers (ec.in ());

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: open_receivers done\n"));

      this->activate_federations (ec.in ());

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: activate_federations done\n"));

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: running the test\n"));
      while (this->federations_running_ > 0)
        this->orb_->perform_work ();

      this->dump_results ();

      this->close_receivers ();
      this->close_senders ();

      this->close_federations ();

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: shutdown the EC\n"));

      ec_impl.shutdown ();

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
ECM_Driver::federation_has_shutdown (ECM_Local_Federation *federation)
{
  ACE_DEBUG ((LM_DEBUG, "Federation <%s> shuting down\n",
              federation->name ()));
  if (this->federations_running_ > 0)
    this->federations_running_--;
}

void
ECM_Driver::open_federations (RtecEventChannelAdmin::EventChannel_ptr ec)
{
  for (int i = 0; i < this->local_federations_count_; ++i)
    {
      this->local_federations_[i]->open (this->event_count_,
                                         ec);
    }
}

void
ECM_Driver::activate_federations (RtecEventChannelAdmin::EventChannel_ptr ec)
{
  this->federations_running_ = this->local_federations_count_;
  RtecEventComm::Time interval = this->event_period_;
  interval *= 10;
  for (int i = 0; i < this->local_federations_count_; ++i)
    {
      this->local_federations_[i]->activate (ec,
                                             interval);
    }
}

void
ECM_Driver::close_federations (void)
{
  for (int i = 0; i < this->local_federations_count_; ++i)
    {
      this->local_federations_[i]->close ();
    }
}

void
ECM_Driver::open_senders (RtecEventChannelAdmin::EventChannel_ptr ec)
{
  ACE_INET_Addr ignore_from;
  this->endpoint_.dgram ().get_local_addr (ignore_from);

  if (this->endpoint_.dgram ().open (ACE_Addr::sap_any,
                                     ignore_from.get_type()) == -1)
    {
      // @@ TODO throw an application specific exception.
      throw CORBA::COMM_FAILURE ();
    }

  ACE_TCHAR buffer[256];
  ignore_from.addr_to_string (buffer,256);
  ACE_DEBUG ((LM_DEBUG, "(%P) ECM_Driver::open_senders - "
              "local endpoint = (%s)\n",
              ACE_TEXT_ALWAYS_CHAR (buffer)));
  for (int i = 0; i < this->all_federations_count_; ++i)
    {
      TAO_ECG_UDP_Out_Endpoint* clone;
      ACE_NEW (clone,
               TAO_ECG_UDP_Out_Endpoint (this->endpoint_));

      this->all_federations_[i]->open (clone,
                                       ec);
    }
}

void
ECM_Driver::close_senders (void)
{
  for (int i = 0; i < this->all_federations_count_; ++i)
    {
      this->all_federations_[i]->close ();
    }
  this->endpoint_.dgram ().close ();
}

void
ECM_Driver::open_receivers (RtecEventChannelAdmin::EventChannel_ptr ec)
{
  for (int i = 0; i < this->local_federations_count_; ++i)
    {
      TAO_ECG_Refcounted_Endpoint endpoint;
      TAO_ECG_UDP_Out_Endpoint* clone = 0;
      ACE_NEW (clone,
               TAO_ECG_UDP_Out_Endpoint (this->endpoint_));
      endpoint.reset (clone);

      this->local_federations_[i]->open_receiver (ec,
                                                  endpoint);
    }
}

void
ECM_Driver::close_receivers (void)
{
  for (int i = 0; i < this->local_federations_count_; ++i)
    {
      this->local_federations_[i]->close_receiver ();
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
ECM_Driver::parse_args (int argc, ACE_TCHAR *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("l:p:c:n:t:f:"));
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'p':
          this->pid_filename_ = get_opt.opt_arg ();
          break;

        case 'c':
          this->config_filename_ = get_opt.opt_arg ();
          break;

        case 't':
          this->event_period_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'n':
          this->event_count_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'f':
          {
            char* aux;
            int i = 0;
            for (char* arg = ACE_OS::strtok_r (ACE_TEXT_ALWAYS_CHAR(get_opt.opt_arg ()), ",", &aux);
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
      s = fscanf (cfg, "%d", &port);
      if (s == 0 || s == EOF)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "problem reading federation port number\n"), -1);
        }

      CORBA::UShort mcast_port = static_cast<CORBA::UShort> (port);

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
  while (ACE_OS::ace_isspace (c = ACE_OS::fgetc (file)))
    {
      // No action.
    }

  if (c == EOF)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Unexpected EOF in config file while %s\n",
                         error_msg),
                        -1);
    }

  ACE_OS::ungetc (c, file);
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
  sender_ = TAO_ECG_UDP_Sender::create (true);

  ACE_NEW (this->supplier_ipaddr_, CORBA::ULong[this->supplier_types_]);
  ACE_NEW (this->consumer_ipaddr_, CORBA::ULong[this->consumer_types_]);

  int i;
  for (i = 0; i < this->supplier_types_; ++i)
    {
      ACE_INET_Addr addr (u_short(0), this->supplier_names_[i], AF_INET);
      this->supplier_ipaddr_[i] = addr.get_ip_address ();
    }
  for (i = 0; i < this->consumer_types_; ++i)
    {
      ACE_INET_Addr addr (u_short(0), this->consumer_names_[i], AF_INET);
      this->consumer_ipaddr_[i] = addr.get_ip_address ();
    }
}

ECM_Federation::~ECM_Federation (void)
{
  delete[] this->consumer_ipaddr_;
  delete[] this->supplier_ipaddr_;
}

void
ECM_Federation::open (TAO_ECG_UDP_Out_Endpoint *endpoint,
                      RtecEventChannelAdmin::EventChannel_ptr ec)
{
  RtecUDPAdmin::AddrServer_var addr_server =
    this->addr_server ();

  TAO_ECG_Refcounted_Endpoint ref_endpoint (endpoint);

  this->sender_->init (ec,
                       addr_server.in (),
                       ref_endpoint);

  // @@ TODO Make this a parameter....
  this->sender_->mtu (64);

  // The worst case execution time is far less than 2
  // milliseconds, but that is a safe estimate....
  ACE_Time_Value tv (0, 2000);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);

  ACE_ConsumerQOS_Factory qos;
  qos.start_disjunction_group ();
  for (int i = 0; i < this->consumer_types (); ++i)
    {
      qos.insert_type (this->consumer_ipaddr (i), 0);
    }
  RtecEventChannelAdmin::ConsumerQOS qos_copy = qos.get_ConsumerQOS ();
  this->sender_->connect (qos_copy);
}

void
ECM_Federation::close (void)
{
  this->sender_->shutdown ();
}

RtecUDPAdmin::AddrServer_ptr
ECM_Federation::addr_server (void)
{
  return this->addr_server_._this ();
}

// ****************************************************************

ECM_Supplier::ECM_Supplier (ECM_Local_Federation* federation)
  :  federation_ (federation),
     consumer_ (this)
{
}

void
ECM_Supplier::open (const char* name,
                    RtecEventChannelAdmin::EventChannel_ptr ec)
{
  this->supplier_id_ = ACE::crc32 (name);
  ACE_DEBUG ((LM_DEBUG, "ID for <%s> is %04.4x\n", name,
              this->supplier_id_));

  ACE_SupplierQOS_Factory qos;
  for (int i = 0; i < this->federation_->supplier_types (); ++i)
    {
      qos.insert (this->supplier_id_,
                  this->federation_->supplier_ipaddr (i),
                  0, 1);
    }
  qos.insert (this->supplier_id_,
              ACE_ES_EVENT_SHUTDOWN,
              0, 1);

  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    ec->for_suppliers ();

  this->consumer_proxy_ =
    supplier_admin->obtain_push_consumer ();

  RtecEventComm::PushSupplier_var objref = this->_this ();

  this->consumer_proxy_->connect_push_supplier (objref.in (),
                                                qos.get_SupplierQOS ());
}

void
ECM_Supplier::close (void)
{
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer ();

  this->consumer_proxy_ = 0;
}

void
ECM_Supplier::activate (RtecEventChannelAdmin::EventChannel_ptr ec,
                        RtecEventComm::Time interval)
{
  ACE_ConsumerQOS_Factory consumer_qos;
  consumer_qos.start_disjunction_group ();
  consumer_qos.insert_time (ACE_ES_EVENT_INTERVAL_TIMEOUT,
                            interval,
                            0);

  // = Connect as a consumer.
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    ec->for_consumers ();

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier ();

  RtecEventComm::PushConsumer_var cref =
    this->consumer_._this ();

  this->supplier_proxy_->connect_push_consumer (cref.in (),
                                                consumer_qos.get_ConsumerQOS ());
}

int
ECM_Supplier::supplier_id (void) const
{
  return this->supplier_id_;
}

void
ECM_Supplier::push (const RtecEventComm::EventSet& events)
{
  for (u_int i = 0; i < events.length (); ++i)
    {
      const RtecEventComm::Event& e = events[i];
      if (e.header.type != ACE_ES_EVENT_INTERVAL_TIMEOUT)
        continue;

      this->federation_->supplier_timeout (this->consumer_proxy_.in ());
    }
}

void
ECM_Supplier::disconnect_push_supplier (void)
{
  // this->supplier_proxy_->disconnect_push_supplier ();
}

void
ECM_Supplier::disconnect_push_consumer (void)
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
ECM_Consumer::open (const char*,
                    RtecEventChannelAdmin::EventChannel_ptr ec,
                    ACE_RANDR_TYPE &seed)
{
  // The worst case execution time is far less than 2
  // milliseconds, but that is a safe estimate....
  ACE_Time_Value tv (0, 2000);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);

  // = Connect as a consumer.
  this->consumer_admin_ = ec->for_consumers ();

  this->connect (seed);
}

void
ECM_Consumer::connect (ACE_RANDR_TYPE &seed)
{
  if (CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  this->supplier_proxy_ =
    this->consumer_admin_->obtain_push_supplier ();

  ACE_ConsumerQOS_Factory qos;
  qos.start_disjunction_group ();
  qos.insert_type (ACE_ES_EVENT_SHUTDOWN, 0);
  const ECM_Federation* federation = this->federation_->federation ();
  for (int i = 0; i < federation->consumer_types (); ++i)
    {
      unsigned int x = ACE_OS::rand_r (seed);
      if (x < RAND_MAX / 2)
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
      qos.insert_type (federation->consumer_ipaddr (i), 0);
    }

  RtecEventComm::PushConsumer_var objref = this->_this ();

  this->supplier_proxy_->connect_push_consumer (objref.in (),
                                                qos.get_ConsumerQOS ());
}

void
ECM_Consumer::disconnect (void)
{
  if (CORBA::is_nil (this->supplier_proxy_.in ())
      || CORBA::is_nil (this->consumer_admin_.in ()))
    return;


  RtecEventChannelAdmin::ProxyPushSupplier_var tmp =
    this->supplier_proxy_._retn ();
  tmp->disconnect_push_supplier ();
}

void
ECM_Consumer::close (void)
{
  try
    {
      this->disconnect ();
      this->consumer_admin_ =
        RtecEventChannelAdmin::ConsumerAdmin::_nil ();
    }
  catch (const CORBA::Exception&)
    {
      this->consumer_admin_ =
        RtecEventChannelAdmin::ConsumerAdmin::_nil ();
      throw;
    }
}

void
ECM_Consumer::push (const RtecEventComm::EventSet& events)
{
  ACE_hrtime_t arrival = ACE_OS::gethrtime ();
  this->federation_->consumer_push (arrival, events);
}

void
ECM_Consumer::disconnect_push_consumer (void)
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
     mcast_eh_ (0),
     seed_ (0),
     subscription_change_period_ (10000),
     publication_change_period_ (10000)
{
  receiver_ = TAO_ECG_UDP_Receiver::create (true);

  ACE_NEW (mcast_eh_, TAO_ECG_Mcast_EH((&*receiver_)));

  ACE_NEW (this->subscription_subset_,
           CORBA::Boolean[this->consumer_types ()]);
}

ECM_Local_Federation::~ECM_Local_Federation (void)
{
  delete mcast_eh_;
  delete[] this->subscription_subset_;
}

void
ECM_Local_Federation::open (int event_count,
                            RtecEventChannelAdmin::EventChannel_ptr ec)
{
  this->event_count_ = event_count;

  const int bufsize = 512;
  char buf[bufsize];
  ACE_OS::strcpy (buf, this->federation_->name ());
  ACE_OS::strcat (buf, "/supplier");

  this->supplier_.open (buf, ec);

  ACE_OS::strcpy (buf, this->federation_->name ());
  ACE_OS::strcat (buf, "/consumer");
  this->consumer_.open (buf, ec, this->seed_);

  this->last_subscription_change_ = ACE_OS::gettimeofday ();
}

void
ECM_Local_Federation::close (void)
{
  this->consumer_.close ();

  this->supplier_.close ();
}

void
ECM_Local_Federation::activate (RtecEventChannelAdmin::EventChannel_ptr ec,
                                RtecEventComm::Time interval)
{
  this->supplier_.activate (ec, interval);
}

void
ECM_Local_Federation::supplier_timeout (RtecEventComm::PushConsumer_ptr consumer)
{
  RtecEventComm::EventSet sent (1);
  sent.length (1);

  RtecEventComm::Event& s = sent[0];
  s.header.source = this->supplier_.supplier_id();
  s.header.ttl = 1;

  ACE_hrtime_t t = ACE_OS::gethrtime ();
  ORBSVCS_Time::hrtime_to_TimeT (s.header.creation_time, t);

  this->event_count_--;

  // ACE_DEBUG ((LM_DEBUG, "Federation <%s> event count <%d>\n",
  //          this->name (), this->event_count_));

  if (this->event_count_ < 0)
    {
      this->driver_->federation_has_shutdown (this);
      return;
    }
  int i = this->event_count_ % this->federation_->supplier_types ();
  s.header.type = this->federation_->supplier_ipaddr (i);

  consumer->push (sent);

  this->send_count_++;

  ACE_Time_Value delta = ACE_OS::gettimeofday () -
    this->last_subscription_change_;

  unsigned int x = ACE_OS::rand_r (this->seed_);
  double p = double (x) / RAND_MAX;
  double maxp = double (delta.msec ()) / this->subscription_change_period_;

  if (4 * p < maxp)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Reconfiguring federation %s: %f %f [%d]\n",
                  this->name (), p, maxp, x));
      this->consumer_.disconnect ();
      this->consumer_.connect (this->seed_);
      this->last_subscription_change_ = ACE_OS::gettimeofday ();
    }
}

void
ECM_Local_Federation::consumer_push (ACE_hrtime_t,
                                     const RtecEventComm::EventSet &event)
{
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
                                     TAO_ECG_Refcounted_Endpoint ignore_from)
{
  RtecUDPAdmin::AddrServer_var addr_server =
    this->federation_->addr_server ();

  ACE_Reactor* reactor = TAO_ORB_Core_instance ()->reactor ();

  this->receiver_->init (ec,
                         ignore_from,
                         addr_server.in ());

  const int bufsize = 512;
  char buf[bufsize];
  ACE_OS::strcpy (buf, this->name ());
  ACE_OS::strcat (buf, "/receiver");

  RtecEventComm::EventSourceID source = ACE::crc32 (buf);

  this->mcast_eh_->reactor (reactor);

  this->mcast_eh_->open (ec);

  ACE_SupplierQOS_Factory qos;
  for (int i = 0; i < this->consumer_types (); ++i)
    {
      qos.insert (source,
                  this->consumer_ipaddr (i),
                  0, 1);
    }

  RtecEventChannelAdmin::SupplierQOS qos_copy =
    qos.get_SupplierQOS ();
  this->receiver_->connect (qos_copy);


}

void
ECM_Local_Federation::close_receiver (void)
{
  this->receiver_->shutdown ();
  this->mcast_eh_->shutdown ();
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

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  ECM_Driver driver;
  return driver.run (argc, argv);
}
