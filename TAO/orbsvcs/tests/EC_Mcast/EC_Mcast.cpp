#include "EC_Mcast.h"

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Time_Utilities.h"

#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

#include "tao/ORB_Core.h"

#include "ace/Get_Opt.h"
#include "ace/Malloc_Base.h"
#include "ace/Sched_Params.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_ctype.h"

#include <memory>
#include <utility>

struct ECM_Read_Buffer_Deleter
{
  ACE_Allocator* allocator;

  void operator() (char* buffer) const
  {
    this->allocator->free (buffer);
  }
};

#if !defined (__ACE_INLINE__)
#include "EC_Mcast.inl"
#endif /* __ACE_INLINE__ */

int
ECM_Driver::run (int argc, ACE_TCHAR* argv[])
{
  try
    {
      this->orb_ =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        this->orb_->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      if (this->parse_args (argc, argv))
        return 1;

      if (this->parse_config_file ())
        return 1;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Execution parameters:\n"
                            "  event period = <%d> (usecs)\n"
                            "  event count = <%d>\n"
                            "  config file name = <%s>\n"
                            "  pid file name = <%s>\n"),

                  this->event_period_,
                  this->event_count_,

                  this->config_filename_.empty () ? ACE_TEXT ("nil") : this->config_filename_.c_str (),
                  this->pid_filename_.empty () ? ACE_TEXT ("nil") : this->pid_filename_.c_str ()));

      for (size_t i {}; i < this->local_federations_count_; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "    name = <%s>\n"
                      "    port = <%d>\n"
                      "    supplier types:\n",
                      this->local_federations_[i]->name (),
                      this->local_federations_[i]->mcast_port ()));
          for (size_t j {}; j < this->local_federations_[i]->supplier_types (); ++j)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "    name = <%s>\n"
                          "    ipadd = <%x>\n",
                          this->local_federations_[i]->supplier_name (j),
                          this->local_federations_[i]->supplier_ipaddr (j)));
            }
          ACE_DEBUG ((LM_DEBUG,
                      "    consumer types:\n"));
          for (size_t j {}; j < this->local_federations_[i]->consumer_types (); ++j)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "    name = <%s>\n"
                          "    ipadd = <%x>\n",
                          this->local_federations_[i]->consumer_name (j),
                          this->local_federations_[i]->consumer_ipaddr (j)));
            }
        }

      if (!this->pid_filename_.empty ())
        {
          std::unique_ptr<FILE, int (*) (FILE *)> pid_file (
            ACE_OS::fopen (this->pid_filename_.c_str (), "w"), ACE_OS::fclose);
          if (pid_file)
            {
              ACE_OS::fprintf (pid_file.get (), "%ld\n",
                               static_cast<long> (ACE_OS::getpid ()));
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
      while (this->federations_running_.load (std::memory_order_relaxed) > 0)
        this->orb_->perform_work ();

      this->dump_results ();

      this->close_receivers ();
      this->close_senders ();

      this->close_federations ();

      ACE_DEBUG ((LM_DEBUG, "EC_Mcast: shutdown the EC\n"));

      ec_impl.shutdown ();
      root_poa->deactivate_object (id.in ());
    }
  catch (CORBA::SystemException const& sys_ex)
    {
      sys_ex._tao_print_exception ("SYS_EX");
      return 1;
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("NON SYS EX");
      return 1;
    }
  return 0;
}

void
ECM_Driver::federation_has_shutdown (ECM_Local_Federation *federation)
{
  ACE_DEBUG ((LM_DEBUG, "Federation <%s> shuting down\n",
              federation->name ()));
  CORBA::ULong count = this->federations_running_.load (std::memory_order_relaxed);
  while (count > 0)
    {
      if (this->federations_running_.compare_exchange_weak (
            count,
            count - 1,
            std::memory_order_relaxed))
        break;
    }
}

void
ECM_Driver::open_federations (RtecEventChannelAdmin::EventChannel_ptr ec)
{
  for (size_t i {}; i < this->local_federations_count_; ++i)
    {
      this->local_federations_[i]->open (this->event_count_, ec);
    }
}

void
ECM_Driver::activate_federations (RtecEventChannelAdmin::EventChannel_ptr ec)
{
  this->federations_running_.store (static_cast<CORBA::ULong> (this->local_federations_count_),
                                    std::memory_order_relaxed);
  RtecEventComm::Time interval = this->event_period_;
  interval *= 10;
  for (size_t i {}; i < this->local_federations_count_; ++i)
    {
      this->local_federations_[i]->activate (ec, interval);
    }
}

void
ECM_Driver::close_federations ()
{
  for (size_t i {}; i < this->local_federations_count_; ++i)
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
  for (size_t i {}; i < this->all_federations_count_; ++i)
    {
      TAO_ECG_UDP_Out_Endpoint* clone {};
      ACE_NEW (clone,
               TAO_ECG_UDP_Out_Endpoint (this->endpoint_));

      // ECM_Federation::open places the endpoint in a reference-counted owner.
      this->all_federations_[i]->open (clone, ec);
    }
}

void
ECM_Driver::close_senders ()
{
  for (size_t i {}; i < this->all_federations_count_; ++i)
    {
      this->all_federations_[i]->close ();
    }
  this->endpoint_.dgram ().close ();
}

void
ECM_Driver::open_receivers (RtecEventChannelAdmin::EventChannel_ptr ec)
{
  for (size_t i {}; i < this->local_federations_count_; ++i)
    {
      TAO_ECG_Refcounted_Endpoint endpoint;
      TAO_ECG_UDP_Out_Endpoint* clone {};
      ACE_NEW (clone,
               TAO_ECG_UDP_Out_Endpoint (this->endpoint_));
      endpoint.reset (clone);

      this->local_federations_[i]->open_receiver (ec,
                                                  endpoint);
    }
}

void
ECM_Driver::close_receivers ()
{
  for (size_t i {}; i < this->local_federations_count_; ++i)
    {
      this->local_federations_[i]->close_receiver ();
    }
}

void
ECM_Driver::dump_results ()
{
  for (size_t i {}; i < this->local_federations_count_; ++i)
    {
      this->local_federations_[i]->dump_results ();
    }
}

// ****************************************************************

int
ECM_Driver::parse_args (int argc, ACE_TCHAR *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("p:c:n:t:f:"));
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
            std::basic_string<ACE_TCHAR> names (get_opt.opt_arg ());
            ACE_TCHAR* context {};
            size_t count {};
            for (ACE_TCHAR* arg = ACE_OS::strtok_r (
                   names.data (), ACE_TEXT (","), &context);
                 arg != nullptr;
                 arg = ACE_OS::strtok_r (nullptr, ACE_TEXT (","), &context))
              {
                if (count == ECM_Driver::MAX_LOCAL_FEDERATIONS)
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "too many local federations\n"), -1);
                this->local_names_[count++] = ACE_TEXT_ALWAYS_CHAR (arg);
              }
            this->local_federations_count_ = count;
          }
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "[ORB options] "
                      "-n <event_count> "
                      "-t <event_period> "
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
ECM_Driver::parse_config_file ()
{
  std::unique_ptr<FILE, int (*) (FILE*)> config_file (nullptr, ACE_OS::fclose);
  FILE* cfg = stdin;
  if (!this->config_filename_.empty ())
    {
      config_file.reset (ACE_OS::fopen (this->config_filename_.c_str (), "r"));
      cfg = config_file.get ();
    }

  if (cfg == nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("cannot open config file <%s>\n"),
                         this->config_filename_.c_str ()), -1);
    }

  int federation_count {};
  int s = fscanf (cfg, "%d", &federation_count);
  if (s != 1 || federation_count < 0 || federation_count > MAX_FEDERATIONS)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "invalid federation count\n"), -1);
    }
  this->all_federations_count_ = static_cast<size_t> (federation_count);
  for (size_t i {}; i < this->all_federations_count_; ++i)
    {
      if (this->skip_blanks (cfg, "reading federation name"))
        return -1;
      ACE_Read_Buffer reader (cfg);
      std::unique_ptr<char, ECM_Read_Buffer_Deleter> buf (
        reader.read (' ', ' ', '\0'), ECM_Read_Buffer_Deleter {reader.alloc ()});
      if (!buf)
        ACE_ERROR_RETURN ((LM_ERROR, "problem reading federation name\n"), -1);
      std::string name (buf.get ());

      int port {};
      if (this->skip_blanks (cfg, "reading federation port number"))
        return -1;
      s = fscanf (cfg, "%d", &port);
      if (s != 1 || port < 0 || port > 65535)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "problem reading federation port number\n"), -1);
        }

      CORBA::UShort const mcast_port = static_cast<CORBA::UShort> (port);

      int ns {}, nc {};
      if (this->skip_blanks (cfg, "reading supplier count"))
        return -1;
      s = fscanf (cfg, "%d", &ns);
      if (s != 1 || ns < 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "problem reading supplier count (%B)\n",
                             i), -1);
        }
      if (this->skip_blanks (cfg, "reading constumer count"))
        return -1;
      s = fscanf (cfg, "%d", &nc);
      if (s != 1 || nc < 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "problem reading consumer count (%B)\n",
                             i), -1);
        }
      if (ns == 0)
        ACE_ERROR_RETURN ((LM_ERROR, "federation requires at least one supplier\n"), -1);

      std::vector<std::string> supplier_names;
      std::vector<std::string> consumer_names;
      supplier_names.reserve (ns);
      consumer_names.reserve (nc);

      if (this->parse_name_list (cfg, ns, supplier_names,
                                 "reading supplier list"))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "error parsing supplier list for <%s>\n",
                             name.c_str ()), -1);
        }

      if (this->parse_name_list (cfg, nc, consumer_names,
                                 "reading consumer list"))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "error parsing consumer list for <%s>\n",
                             name.c_str ()), -1);
        }

      ECM_Federation* federation {};
      ACE_NEW_RETURN (federation,
                      ECM_Federation (std::move (name), mcast_port,
                                      std::move (supplier_names),
                                      std::move (consumer_names)), -1);
      this->all_federations_[i].reset (federation);
    }
  for (size_t j {}; j < this->local_federations_count_; ++j)
    {
      size_t k {};
      for (; k < this->all_federations_count_; ++k)
        {
          if (this->local_names_[j] == this->all_federations_[k]->name ())
            {
              ECM_Local_Federation* local_federation {};
              ACE_NEW_RETURN (local_federation,
                              ECM_Local_Federation (this->all_federations_[k].get (),
                                                    this), -1);
              this->local_federations_[j].reset (local_federation);
              break;
            }
        }
      if (k == this->all_federations_count_)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot find federation <%s>\n",
                           this->local_names_[j].c_str ()), -1);
    }

  return 0;
}

int
ECM_Driver::parse_name_list (FILE* file,
                             size_t count,
                             std::vector<std::string>& names,
                             char const* error_msg)
{
  for (size_t i {}; i < count; ++i)
    {
      if (this->skip_blanks (file, error_msg))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "error on item %B while %s\n",
                             i, error_msg), -1);
        }
      ACE_Read_Buffer tmp (file);
      std::unique_ptr<char, ECM_Read_Buffer_Deleter> buf (
        tmp.read ('\n', '\n', '\0'), ECM_Read_Buffer_Deleter {tmp.alloc ()});
      if (!buf)
        ACE_ERROR_RETURN ((LM_ERROR, "could not read item %B while %s\n",
                           i, error_msg), -1);
      names.emplace_back (buf.get ());
    }
  return 0;
}

int
ECM_Driver::skip_blanks (FILE* file,
                         char const* error_msg)
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

ECM_Federation::ECM_Federation (std::string name,
                                CORBA::UShort mcast_port,
                                std::vector<std::string> supplier_names,
                                std::vector<std::string> consumer_names)
  : name_ (std::move (name)),
    mcast_port_ (mcast_port),
    supplier_names_ (std::move (supplier_names)),
    supplier_ipaddr_ (supplier_names_.size ()),
    consumer_names_ (std::move (consumer_names)),
    consumer_ipaddr_ (consumer_names_.size ()),
    addr_server_ (mcast_port)
{
  sender_ = TAO_ECG_UDP_Sender::create (true);

  for (size_t i {}; i < this->supplier_names_.size (); ++i)
    {
      ACE_INET_Addr addr (u_short(0), this->supplier_names_[i].c_str (), AF_INET);
      this->supplier_ipaddr_[i] = addr.get_ip_address ();
    }
  for (size_t i {}; i < this->consumer_names_.size (); ++i)
    {
      ACE_INET_Addr addr (u_short(0), this->consumer_names_[i].c_str (), AF_INET);
      this->consumer_ipaddr_[i] = addr.get_ip_address ();
    }
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

  ACE_ConsumerQOS_Factory qos;
  qos.start_disjunction_group ();
  for (size_t i {}; i < this->consumer_types (); ++i)
    {
      qos.insert_type (this->consumer_ipaddr (i), 0);
    }
  RtecEventChannelAdmin::ConsumerQOS qos_copy = qos.get_ConsumerQOS ();
  this->sender_->connect (qos_copy);
}

void
ECM_Federation::close ()
{
  this->sender_->shutdown ();
}

RtecUDPAdmin::AddrServer_ptr
ECM_Federation::addr_server ()
{
  return this->addr_server_._this ();
}

// ****************************************************************

ECM_Supplier::ECM_Supplier (ECM_Local_Federation* federation)
  : federation_ (federation),
     supplier_id_ {},
     consumer_ (this)
{
}

void
ECM_Supplier::open (char const* name,
                    RtecEventChannelAdmin::EventChannel_ptr ec)
{
  this->supplier_id_ = ACE::crc32 (name);
  ACE_DEBUG ((LM_DEBUG, "ID for <%s> is %04.4x\n", name,
              this->supplier_id_));

  ACE_SupplierQOS_Factory qos;
  for (size_t i {}; i < this->federation_->supplier_types (); ++i)
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
ECM_Supplier::close ()
{
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer ();

  this->consumer_proxy_ = RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
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

  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    ec->for_consumers ();

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier ();

  RtecEventComm::PushConsumer_var cref =
    this->consumer_._this ();

  this->supplier_proxy_->connect_push_consumer (cref.in (),
                                                consumer_qos.get_ConsumerQOS ());
}

RtecEventComm::EventSourceID
ECM_Supplier::supplier_id () const
{
  return this->supplier_id_;
}

void
ECM_Supplier::push (RtecEventComm::EventSet const& events)
{
  for (u_int i = 0; i < events.length (); ++i)
    {
      RtecEventComm::Event const& e = events[i];
      if (e.header.type != ACE_ES_EVENT_INTERVAL_TIMEOUT)
        continue;

      this->federation_->supplier_timeout (this->consumer_proxy_.in ());
    }
}

void
ECM_Supplier::disconnect_push_supplier ()
{
}

void
ECM_Supplier::disconnect_push_consumer ()
{
}

// ****************************************************************

ECM_Consumer::ECM_Consumer (ECM_Local_Federation *federation)
  : federation_ (federation)
{
}

void
ECM_Consumer::open (char const*,
                    RtecEventChannelAdmin::EventChannel_ptr ec,
                    unsigned int *seed)
{
  this->consumer_admin_ = ec->for_consumers ();

  this->connect (seed);
}

void
ECM_Consumer::connect (unsigned int *seed)
{
  if (CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  this->supplier_proxy_ =
    this->consumer_admin_->obtain_push_supplier ();

  ACE_ConsumerQOS_Factory qos;
  qos.start_disjunction_group ();
  qos.insert_type (ACE_ES_EVENT_SHUTDOWN, 0);
  ECM_Federation const* federation = this->federation_->federation ();
  for (size_t i {}; i < federation->consumer_types (); ++i)
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
ECM_Consumer::disconnect ()
{
  if (CORBA::is_nil (this->supplier_proxy_.in ())
      || CORBA::is_nil (this->consumer_admin_.in ()))
    return;

  RtecEventChannelAdmin::ProxyPushSupplier_var tmp =
    this->supplier_proxy_._retn ();
  tmp->disconnect_push_supplier ();
}

void
ECM_Consumer::close ()
{
  try
    {
      this->disconnect ();
      this->consumer_admin_ =
        RtecEventChannelAdmin::ConsumerAdmin::_nil ();
    }
  catch (CORBA::Exception const&)
    {
      this->consumer_admin_ =
        RtecEventChannelAdmin::ConsumerAdmin::_nil ();
      throw;
    }
}

void
ECM_Consumer::push (RtecEventComm::EventSet const& events)
{
  ACE_hrtime_t arrival = ACE_OS::gethrtime ();
  this->federation_->consumer_push (arrival, events);
}

void
ECM_Consumer::disconnect_push_consumer ()
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
     last_subscription_change_ (0),
     mcast_eh_ {},
     seed_ (0),
     subscription_change_period_ (10000)
{
  receiver_ = TAO_ECG_UDP_Receiver::create (true);

  this->mcast_eh_ = std::make_unique<TAO_ECG_Mcast_EH> (this->receiver_.in ());
  this->subscription_subset_.resize (this->consumer_types ());
}

void
ECM_Local_Federation::open (int event_count,
                            RtecEventChannelAdmin::EventChannel_ptr ec)
{
  this->event_count_ = event_count;

  std::string name (this->federation_->name ());
  name += "/supplier";
  this->supplier_.open (name.c_str (), ec);

  name = this->federation_->name ();
  name += "/consumer";
  this->consumer_.open (name.c_str (), ec, &this->seed_);

  this->last_subscription_change_ = ACE_OS::gettimeofday ();
}

void
ECM_Local_Federation::close ()
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

  --this->event_count_;

  if (this->event_count_ < 0)
    {
      this->driver_->federation_has_shutdown (this);
      return;
    }
  size_t const i = static_cast<size_t> (this->event_count_) %
    this->federation_->supplier_types ();
  s.header.type = this->federation_->supplier_ipaddr (i);

  consumer->push (sent);

  ++this->send_count_;

  ACE_Time_Value delta = ACE_OS::gettimeofday () -
    this->last_subscription_change_;

  unsigned int x = ACE_OS::rand_r (&this->seed_);
  double p = double (x) / RAND_MAX;
  double maxp = double (delta.msec ()) / this->subscription_change_period_;

  if (4 * p < maxp)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Reconfiguring federation %s: %f %f [%d]\n",
                  this->name (), p, maxp, x));
      this->consumer_.disconnect ();
      this->consumer_.connect (&this->seed_);
      this->last_subscription_change_ = ACE_OS::gettimeofday ();
    }
}

void
ECM_Local_Federation::consumer_push (ACE_hrtime_t,
                                     RtecEventComm::EventSet const& event)
{
  if (event.length () == 0)
    {
      return;
    }

  for (CORBA::ULong i = 0; i < event.length (); ++i)
    {
      RtecEventComm::Event const& e = event[i];

      ++this->recv_count_;

      size_t j {};
      for (; j < this->federation_->consumer_types (); ++j)
        {
          CORBA::ULong type = e.header.type;
          if (type == this->federation_->consumer_ipaddr(j))
            {
              if (this->subscribed_bit (j) == 0)
                ++this->unfiltered_count_;
              break;
            }
        }
      if (j == this->federation_->consumer_types ())
        ++this->invalid_count_;
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

  std::string name (this->name ());
  name += "/receiver";

  RtecEventComm::EventSourceID source = ACE::crc32 (name.c_str ());

  this->mcast_eh_->reactor (reactor);

  this->mcast_eh_->open (ec);

  ACE_SupplierQOS_Factory qos;
  for (size_t i {}; i < this->consumer_types (); ++i)
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
ECM_Local_Federation::close_receiver ()
{
  this->receiver_->shutdown ();
  this->mcast_eh_->shutdown ();
}

void
ECM_Local_Federation::dump_results () const
{
  double unfiltered_ratio {};
  if (this->recv_count_ != 0)
    unfiltered_ratio = double(this->unfiltered_count_)/this->recv_count_;
  double invalid_ratio {};
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
ECM_Local_Federation::subscribed_bit (size_t i, CORBA::Boolean x)
{
  if (i >= this->consumer_types ())
    return;
  this->subscription_subset_[i] = x;
}

CORBA::Boolean
ECM_Local_Federation::subscribed_bit (size_t i) const
{
  if (i >= this->consumer_types ())
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
