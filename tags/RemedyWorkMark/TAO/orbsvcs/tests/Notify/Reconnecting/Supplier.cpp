//$Id$

#include "Supplier.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "tao/debug.h"
#include "tao/PortableServer/PortableServerC.h"
#include "tao/TimeBaseC.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_strings.h"



static const char NOTIFY_FACTORY_NAME[] = "NotifyEventChannelFactory";
static const char NAMING_SERVICE_NAME[] = "NameService";

///////////////////////////
// StructuredPushSupplier_i

void
StructuredPushSupplier_i::subscription_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
  )
{
  ACE_UNUSED_ARG (added);
  ACE_UNUSED_ARG (removed);
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P,%t) Supplier StructuredPushSupplier received subscription change\n")
    ));
}


void
StructuredPushSupplier_i::disconnect_structured_push_supplier (
  )
{
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P,%t) Supplier StructuredPushSupplier received disconnect\n")
    ));
}

///////////////////////////
// SequencePushSupplier_i

void
SequencePushSupplier_i::subscription_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
  )
{
  ACE_UNUSED_ARG (added);
  ACE_UNUSED_ARG (removed);
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P,%t) Supplier SequencePushSupplier received subscription change\n")
    ));
}


void
SequencePushSupplier_i::disconnect_sequence_push_supplier (
  )
{
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P,%t) Supplier SequencePushSupplier received disconnect\n")
    ));
}

///////////////////////////
// AnyPushSupplier_i

void
AnyPushSupplier_i::subscription_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
  )
{
  ACE_UNUSED_ARG (added);
  ACE_UNUSED_ARG (removed);
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P,%t) Supplier AnyPushSupplier received subscription change\n")
    ));
}


void
AnyPushSupplier_i::disconnect_push_supplier (
  )
{
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P,%t) Suppleir AnyPushSupplier received disconnect\n")
    ));
}

/////////////////////////
// ReconnectionCallback_i

ReconnectionCallback_i::ReconnectionCallback_i (Supplier_Main & supplier_main)
  : supplier_main_ (supplier_main)
  , id_is_valid_ (false)
  , reconnect_count_ (0)
{
}

size_t
ReconnectionCallback_i::reconnect_count () const
{
  return this->reconnect_count_;
}

void
ReconnectionCallback_i::reconnect (CORBA::Object_ptr reconnection)
{
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P|%t) Supplier received reconnection request\n")
    ));
  this->ecf_ = CosNotifyChannelAdmin::EventChannelFactory::_narrow (reconnection);
  if (!CORBA::is_nil (this->ecf_.in ()))
  {
    this->supplier_main_.reconnect (this->ecf_.in ());
    this->reconnect_count_ += 1;
  }
  else
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Supplier reconnection request failed: wrong object type\n")
      ));
  }
}

CORBA::Boolean
ReconnectionCallback_i::is_alive (void)
{
  return true;
}

ReconnectionCallback_i::~ReconnectionCallback_i ()
{
  // normally you would disconnect from registry here, but
  // to simulate a failure, we exit without cleaning up
  // if the fini method is not called.
}


void
ReconnectionCallback_i::fini (void)
{
  if (this->id_is_valid_)
  {
    NotifyExt::ReconnectionRegistry_var registry =
      NotifyExt::ReconnectionRegistry::_narrow (this->ecf_.in ());

    registry->unregister_callback (this->callback_id_);
    this->id_is_valid_ = false;
  }
}

void
ReconnectionCallback_i::init (
  PortableServer::POA_ptr poa,
  CosNotifyChannelAdmin::EventChannelFactory_ptr ecf)
{
  this->ecf_ = CosNotifyChannelAdmin::EventChannelFactory::_duplicate (ecf);
  PortableServer::ObjectId_var reconnection_callback_id =
    poa->activate_object (this);

  CORBA::Object_var obj =
    poa->id_to_reference (reconnection_callback_id.in ());

  NotifyExt::ReconnectionCallback_var callback =
    NotifyExt::ReconnectionCallback::_narrow (obj.in ());

  NotifyExt::ReconnectionRegistry_var registry =
    NotifyExt::ReconnectionRegistry::_narrow (ecf);

  this->callback_id_ = registry->register_callback (callback.in ());
  this->id_is_valid_ = true;
}



/////////////////
// Supplier_Main

Supplier_Main::Supplier_Main ()
  : verbose_ (false)
  , mode_ (MODE_ANY)
  , send_ (10)
  , use_naming_service_ (true)
  , serial_number_ (0)
  , disconnect_on_exit_ (false)
  , id_file_ (ACE_TEXT ("supplier.ids"))
  , pause_ (0)
  , ec_id_ (0)
  , sa_id_(0)
  , structured_proxy_id_(0)
  , sequence_proxy_id_(0)
  , any_proxy_id_(0)
  , reconnection_callback_ (*this)
  , reconnecting_ (false)
{
}

Supplier_Main::~Supplier_Main ()
{
}

int
Supplier_Main::parse_args (int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  int narg = 1;
  bool corba_arg = false;
  while (narg < argc && result == 0)
  {
    int consumed = parse_single_arg (argc - narg, &argv[narg]);
    if ( consumed > 0)
    {
      narg += consumed;
      corba_arg = false;
    }
    else if (ACE_OS::strncmp (argv[narg], ACE_TEXT("-ORB"), 4) == 0)
    {
      corba_arg = true;
    }
    else if (corba_arg)
    {
      // previous argument was a ORB arg.
      // current argument is unrecognized
      // assume the ORB eats this arg
      narg += 1;
      corba_arg = false;
    }
    else
    {
      ACE_OS::fprintf(stderr, "Unrecognized argument: %s\n", argv[narg]);
      usage (stderr);
      result = -1;
    }
  }
  return result;
}

int
Supplier_Main::parse_single_arg (int argc, ACE_TCHAR *argv[])
{
  int consumed = 0;
  if (ACE_OS::strcasecmp (argv[0], ACE_TEXT ("-v")) == 0)
  {
    this->verbose_ = true;
    consumed = 1;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT ("-any")) == 0)
  {
    this->mode_ = MODE_ANY;
    consumed = 1;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT ("-structured")) == 0)
  {
    this->mode_ = MODE_STRUCTURED;
    consumed = 1;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT ("-sequence")) == 0)
  {
    this->mode_ = MODE_SEQUENCE;
    consumed = 1;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT ("-channel")) == 0)
  {
    this->channel_file_= argv[1];
    consumed = 2;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT ("-send")) == 0 && argc > 1)
  {
    this->send_ = ACE_OS::atoi (argv[1]);
    consumed = 2;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT ("-pause")) == 0 && argc > 1)
  {
    this->pause_ = ACE_OS::atoi (argv[1]);
    consumed = 2;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT ("-serial_number")) == 0)
  {
    this->serial_number_= ACE_OS::atoi (argv[1]);
    consumed = 2;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT ("-nonamesvc")) == 0)
  {
    this->use_naming_service_ = false;
    consumed = 1;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT("-disconnect")) == 0)
  {
    this->disconnect_on_exit_ = true;
    consumed = 1;
  }

  return consumed;
}

void Supplier_Main::usage(FILE * log)const
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_OS::fputs (
      ACE_TEXT ("usage\n")
      ACE_TEXT ("  -channel filename Where to find a channel number.\n")
      ACE_TEXT ("  -any or -structured or -sequence\n")
      ACE_TEXT ("                    What type of event to send (pick one, default is -any)\n")
      ACE_TEXT ("  -send n           How many events of each type to send.\n")
      ACE_TEXT ("  -pause n          Pause after sending n events. Write to file \"Supplier.paused\"\n")
      ACE_TEXT ("  -serial_number n  What serial number to start with.\n")
      ACE_TEXT ("  -v                Verbose output.\n")
      ACE_TEXT ("  -disconnect       Disconnect from channel on exit (prevents reconnect.)\n")
      ACE_TEXT ("  -nonamesvc        Don't use the name service to find EventChannelFactory\n")
      , log);
  //FUZZ: enable check_for_lack_ACE_OS
}

int Supplier_Main::init (int argc, ACE_TCHAR *argv[])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  if (0 != this->parse_args (argc, argv))
  {
    return -1;
  }

  CORBA::Object_ptr poa_object  =
    this->orb_->resolve_initial_references("RootPOA");

  if (CORBA::is_nil (poa_object))
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")));
    return -1;
  }

  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object);

  PortableServer::POAManager_var poa_manager =
    root_poa_->the_POAManager ();

  poa_manager->activate ();

  if (this->use_naming_service_ )
  {
    this->find_notify_factory ();
  }
  else
  {
    int ok = resolve_notify_factory ();
    if (!ok)
    {
      return -1;
    }
  }

  this->reconnecting_ = load_ids ();

  init_event_channel ();

  init_supplier_admin ();

  switch (this->mode_)
  {
    case MODE_STRUCTURED:
    {
      init_structured_proxy_consumer ();
      break;
    }
    case MODE_SEQUENCE:
    {
      init_sequence_proxy_consumer ();
      break;
    }
    case MODE_ANY:
    {

      init_any_proxy_consumer ();
      break;
    }
    default:
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Supplier: Unknown event push mode.\n")
        ));
      break;
    }
  }
  this->reconnection_callback_.init (
    this->root_poa_.in (),
    this->ecf_.in ());

  save_ids ();
  return 0;
}
void
Supplier_Main::save_ids()
{
  FILE *idf =
    ACE_OS::fopen (this->id_file_.c_str (), "w");

  if (idf != 0)
  {
    int endflag = 12345;
    int imode = static_cast<int>(this->mode_);
    ACE_OS::fprintf (idf,
      "%d,%d,%d,%d,%d,%d,%d,\n",
      static_cast<int> (imode),
      static_cast<int> (ec_id_),
      static_cast<int> (sa_id_),
      static_cast<int> (structured_proxy_id_),
      static_cast<int> (sequence_proxy_id_),
      static_cast<int> (any_proxy_id_),
      static_cast<int> (endflag) );
    ACE_OS::fclose (idf);
  }
}

bool
Supplier_Main::load_ids()
{
  bool ok = false;
  FILE *idf =
    ACE_OS::fopen (this->id_file_.c_str (), "r");

  if (idf != 0)
  {
    int field = 0;

    char buffer[100] = ""; // because ACE fgets doesn't put a null if the file is empty
    ACE_OS::fgets (buffer, sizeof(buffer), idf);
    ACE_OS::fclose (idf);
    char * pb = buffer;
    while (!ok && *pb != 0)
    {
      char * eb = ACE_OS::strchr (pb, ',');
      char * nb = eb + 1;
      if (eb == 0)
      {
        eb = pb + ACE_OS::strlen (pb);
        nb = eb;
      }
      *eb = 0;
      if (pb < eb)
      {
        int value = ACE_OS::atoi(pb);
        switch (++field)
        {
        case 1:
          this->mode_ = static_cast<Mode_T> (value);
          break;
        case 2:
          this->ec_id_ = value;
          break;
        case 3:
          this->sa_id_ = value;
          break;
        case 4:
          this->structured_proxy_id_ = value;
          break;
        case 5:
          this->sequence_proxy_id_ = value;
          break;
        case 6:
          this->any_proxy_id_ = value;
          break;
        case 7:
          ok = value == 12345;
          break;
        default:
          ACE_OS::fprintf (stderr, ACE_TEXT ("Supplier: Warning: too many fields in saved id file.\n"));
          ok = false;
          break;
        }
      }
      pb = nb;
    }
  }
  return ok;
}

void
Supplier_Main::reconnect (
    CosNotifyChannelAdmin::EventChannelFactory_ptr dest_factory)
{
  this->ecf_ = CosNotifyChannelAdmin::EventChannelFactory::_duplicate (dest_factory);
  this->reconnecting_ = true;
  init_event_channel ();

  init_supplier_admin ();

  switch (this->mode_)
  {
    case MODE_STRUCTURED:
    {
      init_structured_proxy_consumer ();
      break;
    }
    case MODE_SEQUENCE:
    {
      init_sequence_proxy_consumer ();
      break;
    }
    case MODE_ANY:
    {
      init_any_proxy_consumer ();
      break;
    }
  }
}


int
Supplier_Main::resolve_naming_service (void)
{
  // ignore redundant calls
  if (CORBA::is_nil (this->naming_context_.in ()))
  {
    CORBA::Object_var naming_obj =
      this->orb_->resolve_initial_references (NAMING_SERVICE_NAME);

    this->naming_context_ =
      CosNaming::NamingContext::_narrow (naming_obj.in ());
  }

  return !CORBA::is_nil (this->naming_context_.in ());
}

int
Supplier_Main::find_notify_factory (void)
{
  int status = this->resolve_naming_service ();
  if (status)
  {
    CosNaming::Name name (1);
    name.length (1);
    name[0].id = CORBA::string_dup (NOTIFY_FACTORY_NAME);

    CORBA::Object_var obj =
      this->naming_context_->resolve (name);

    this->ecf_ =
      CosNotifyChannelAdmin::EventChannelFactory::_narrow (
                                                      obj.in ()
                                                    );
  }
  return ! CORBA::is_nil (this->ecf_.in ());
}

int
Supplier_Main::resolve_notify_factory (void)
{
  CORBA::Object_var factory_obj =
    this->orb_->resolve_initial_references (NOTIFY_FACTORY_NAME);

  this->ecf_ =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (
      factory_obj.in ());
  return ! CORBA::is_nil (this->ecf_.in ());
}

void
Supplier_Main::init_event_channel (void)
{
  bool ok = false;
  if (this->reconnecting_)
  {
    try
    {
      this->ec_ = this->ecf_->get_event_channel (
            this->ec_id_);
      ok = ! CORBA::is_nil (this->ec_.in ());
      if (ok && this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Supplier: Reconnect to event channel %d\n"),
          static_cast<int>(this->ec_id_)
          ));
      }
    }
    catch (...)
    {
    }
  }

  // if we don't have a channel yet, and a channel id file was specified
  // try to read from it
  if (!ok && this->channel_file_.length () > 0)
  {
    FILE * chf = ACE_OS::fopen (this->channel_file_.c_str (), "r");
    if (chf != 0)
    {
      char buffer[100];
      ACE_OS::fgets (buffer, sizeof(buffer), chf);
      ACE_OS::fclose (chf);
      this->ec_id_ = ACE_OS::atoi (buffer);

      try
      {
        this->ec_ = this->ecf_->get_event_channel (
              this->ec_id_);
        ok = ! CORBA::is_nil (this->ec_.in ());
        if (ok)
        {
          if (this->verbose_)
          {
            ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Supplier: Connect to Existing event channel %d\n"),
              static_cast<int>(this->ec_id_)
              ));
          }
          // kill the channel filename so we don't overwrite the file
          this->channel_file_ = ACE_TEXT("");
        }
      }
      catch (...)
      {
      }
    }
  }

  if (!ok)
  {
    CosNotification::QoSProperties qosprops (7);
    qosprops.length (7);
    CORBA::ULong i = 0;
    qosprops[i].name = CORBA::string_dup(CosNotification::EventReliability);
    qosprops[i++].value <<= CosNotification::Persistent;
    qosprops[i].name = CORBA::string_dup(CosNotification::ConnectionReliability);
    qosprops[i++].value <<= CosNotification::Persistent; // Required, or we won't persist much
    qosprops[i].name = CORBA::string_dup(CosNotification::Priority);
    qosprops[i++].value <<= CosNotification::HighestPriority;
    qosprops[i].name = CORBA::string_dup(CosNotification::Timeout);
    qosprops[i++].value <<= (TimeBase::TimeT) 42 * 1000000; // 4.2s
    qosprops[i].name = CORBA::string_dup(CosNotification::StopTimeSupported);
    qosprops[i++].value <<= CORBA::Any::from_boolean(1);
    qosprops[i].name = CORBA::string_dup(CosNotification::MaximumBatchSize);
    qosprops[i++].value <<= (CORBA::Long) 2;
    qosprops[i].name = CORBA::string_dup(CosNotification::PacingInterval);
    qosprops[i++].value <<= (TimeBase::TimeT) 50 * 10000; // 50ms

    CosNotification::AdminProperties adminprops(4);
    adminprops.length (4);
    i = 0;
    adminprops[i].name = CORBA::string_dup(CosNotification::MaxQueueLength);
    adminprops[i++].value <<= (CORBA::Long) 1234;
    adminprops[i].name = CORBA::string_dup(CosNotification::MaxConsumers);
    adminprops[i++].value <<= (CORBA::Long) 1000;
    adminprops[i].name = CORBA::string_dup(CosNotification::MaxSuppliers);
    adminprops[i++].value <<= (CORBA::Long) 1000;
    adminprops[i].name = CORBA::string_dup(CosNotification::RejectNewEvents);
    adminprops[i++].value <<= CORBA::Any::from_boolean(1);

    ec_ = this->ecf_->create_channel (
          qosprops,
          adminprops,
          this->ec_id_);
    ok = ! CORBA::is_nil (ec_.in ());
    if (ok && this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Supplier: Create event channel %d\n"),
        static_cast<int> (this->ec_id_)
        ));
    }
  }

  // save channel id
  if (ok && this->channel_file_.length() > 0)
  {
    FILE * chf = ACE_OS::fopen (this->channel_file_.c_str (), "w");
    if (chf != 0)
    {
      ACE_OS::fprintf (chf, "%d\n", static_cast<int> (this->ec_id_));
      ACE_OS::fclose (chf);
    }
  }
}

CosNotifyChannelAdmin::AdminID default_admin_id = static_cast<CosNotifyChannelAdmin::AdminID>(-1);

void
Supplier_Main::init_supplier_admin (void)
{
  bool ok = false;
  if (this->reconnecting_ && this->sa_id_ != default_admin_id)
  {
    try
    {
      this->sa_ = this->ec_->get_supplieradmin(
        this->sa_id_);
      ok = ! CORBA::is_nil (this->sa_.in ());
      if (ok && this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Supplier: Reconnect to supplier admin %d\n"),
          static_cast<int>(this->sa_id_)
          ));
      }
    }
    catch (...)
    {
    }
  }

  if (!ok)
  {
    try
    {
      this->sa_ = this->ec_->default_supplier_admin ();
      ok = ! CORBA::is_nil (this->sa_.in ());
      this->sa_id_ = default_admin_id;
      if (ok && this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Supplier: Using default supplier admin\n")
          ));
      }
      else if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Supplier: No default supplier admin\n")
          ));
      }
    }
    catch (...)
    {
    }
  }

  if (!ok)
  {
    this->sa_ = this->ec_->new_for_suppliers(
      CosNotifyChannelAdmin::OR_OP,
      this->sa_id_);
    ok = ! CORBA::is_nil (this->sa_.in ());
    if (ok && this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Supplier: Create new supplier admin %d\n"),
        static_cast<int>(this->sa_id_)
        ));
    }
  }
}

void
Supplier_Main::init_structured_proxy_consumer (void)
{
  bool ok = false;
  CosNotifyChannelAdmin::ProxyConsumer_var proxy;
  if (this->reconnecting_)
  {
    try
    {
      proxy = this->sa_->get_proxy_consumer (
                          this->structured_proxy_id_
                        );
      ok = ! CORBA::is_nil (proxy.in ());
      if (ok && this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Supplier: Reconnect to proxy supplier %d\n"),
          static_cast<int>(this->structured_proxy_id_)
          ));
      }
    }
    catch (...)
    {
    }
  }

  if (!ok)
  {
    proxy = this->sa_->obtain_notification_push_consumer(
        CosNotifyChannelAdmin::STRUCTURED_EVENT,
        this->structured_proxy_id_);
    ok = ! CORBA::is_nil (proxy.in ());
    if (ok && this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Supplier: Create new proxy %d\n"),
        static_cast<int>(this->structured_proxy_id_)
        ));
    }
  }
  this->structured_proxy_push_consumer_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow(proxy.in ());

  if (CORBA::is_nil (this->structured_proxy_push_consumer_.in ()))
  {
    ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) init_structured_proxy_consumer received nil ProxyConsumer\n")
        ));
    throw CORBA::OBJECT_NOT_EXIST ();
  }
  if (CORBA::is_nil (this->structured_push_supplier_ref_.in ()))
  {
    PortableServer::ObjectId_var push_supplier_id =
      this->root_poa_->activate_object (
        &(this->structured_push_supplier_));

    CORBA::Object_var obj =
      this->root_poa_->id_to_reference (push_supplier_id.in ());

    this->structured_push_supplier_ref_ =
      CosNotifyComm::StructuredPushSupplier::_narrow (obj.in ());
  }
  if (CORBA::is_nil (structured_push_supplier_ref_.in ()))
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Supplier: Received wrong type of push consumer proxy %d\n"),
        static_cast<int>(this->structured_proxy_id_)
      ));

    throw CORBA::UNKNOWN();
  }

  this->structured_proxy_push_consumer_->connect_structured_push_supplier (
    structured_push_supplier_ref_.in ());
}

void
Supplier_Main::init_sequence_proxy_consumer (void)
{
  bool ok = false;
  CosNotifyChannelAdmin::ProxyConsumer_var proxy;
  if (this->reconnecting_)
  {
    try
    {
      proxy = this->sa_->get_proxy_consumer(
        this->sequence_proxy_id_);
      ok = ! CORBA::is_nil (proxy.in ());
      if (ok && this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Supplier: Reconnect to proxy %d\n"),
          static_cast<int>(this->sequence_proxy_id_)
          ));
      }
    }
    catch (...)
    {
    }
  }

  if (!ok)
  {
    proxy = this->sa_->obtain_notification_push_consumer(
        CosNotifyChannelAdmin::SEQUENCE_EVENT,
        this->sequence_proxy_id_);
    ok = ! CORBA::is_nil (proxy.in ());
    if (ok && this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Supplier: Create new proxy %d\n"),
          static_cast<int>(this->sequence_proxy_id_)
        ));
    }
  }
  this->sequence_proxy_push_consumer_ =
    CosNotifyChannelAdmin::SequenceProxyPushConsumer::_narrow(proxy.in ());

  if (CORBA::is_nil (this->sequence_proxy_push_consumer_.in ()))
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Supplier: Received wrong type of push consumer proxy %d\n"),
        static_cast<int>(this->sequence_proxy_id_)
      ));
    throw CORBA::UNKNOWN();
  }

  if (CORBA::is_nil (this->sequence_push_supplier_ref_.in ()))
  {
    PortableServer::ObjectId_var push_supplier_id =
      this->root_poa_->activate_object (
        &(this->sequence_push_supplier_));

    CORBA::Object_var obj =
      this->root_poa_->id_to_reference (push_supplier_id.in ());

    this->sequence_push_supplier_ref_ =
      CosNotifyComm::SequencePushSupplier::_narrow (obj.in ());
  }
  if (CORBA::is_nil (sequence_push_supplier_ref_.in ()))
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Supplier: Received wrong type of push consumer proxy %d\n"),
        static_cast<int>(this->sequence_proxy_id_)
      ));
    throw CORBA::UNKNOWN();
  }

  this->sequence_proxy_push_consumer_->connect_sequence_push_supplier (
    sequence_push_supplier_ref_.in ());
}

void
Supplier_Main::init_any_proxy_consumer (void)
{
  bool ok = false;
  CosNotifyChannelAdmin::ProxyConsumer_var proxy;
  if (this->reconnecting_)
  {
    try
    {
      proxy = this->sa_->get_proxy_consumer(
        this->any_proxy_id_);
      ok = ! CORBA::is_nil (proxy.in ());
      if (ok && this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Supplier: Reconnect to proxy %d\n"),
          static_cast<int>(this->any_proxy_id_)
          ));
      }
    }
    catch (...)
    {
    }
  }

  if (!ok)
  {
    proxy = this->sa_->obtain_notification_push_consumer(
        CosNotifyChannelAdmin::ANY_EVENT,
        this->any_proxy_id_);
    ok = ! CORBA::is_nil (proxy.in ());
    if (ok && this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Supplier: Create new proxy %d\n"),
          static_cast<int>(this->any_proxy_id_)
        ));
    }
  }
  this->any_proxy_push_consumer_ =
    CosNotifyChannelAdmin::ProxyPushConsumer::_narrow(proxy.in ());

  if (CORBA::is_nil (this->any_proxy_push_consumer_.in ()))
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Supplier: Received wrong type of push consumer proxy %d\n"),
        static_cast<int>(this->any_proxy_id_)
      ));
    throw CORBA::UNKNOWN();
  }

  if (CORBA::is_nil (this->any_push_supplier_ref_.in ()))
  {
    PortableServer::ObjectId_var push_supplier_id =
      this->root_poa_->activate_object (
        &(this->any_push_supplier_));

    CORBA::Object_var obj =
      this->root_poa_->id_to_reference (push_supplier_id.in ());

    this->any_push_supplier_ref_ =
      CosNotifyComm::PushSupplier::_narrow (obj.in ());
  }
  if (CORBA::is_nil (any_push_supplier_ref_.in ()))
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Supplier: Received wrong type of push consumer proxy %d\n"),
        static_cast<int>(this->sequence_proxy_id_)
      ));
    throw CORBA::UNKNOWN();
  }

  this->any_proxy_push_consumer_->connect_any_push_supplier (
    any_push_supplier_ref_.in ());
}

int Supplier_Main::fini (void)
{
  if (this->disconnect_on_exit_)
  {
    this->reconnection_callback_.fini ();
    if (!CORBA::is_nil (this->structured_proxy_push_consumer_.in ()))
    {
      if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Disconnecting structured\n")
          ));
      }
      this->structured_proxy_push_consumer_->disconnect_structured_push_consumer ();
    }
    if (!CORBA::is_nil (this->sequence_proxy_push_consumer_.in ()))
    {
      if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Disconnecting sequence\n")
          ));
      }
      this->sequence_proxy_push_consumer_->disconnect_sequence_push_consumer ();
    }
    if (!CORBA::is_nil (this->any_proxy_push_consumer_.in ()))
    {
      if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Disconnecting any\n")
          ));
      }
      this->any_proxy_push_consumer_->disconnect_push_consumer ();
    }
    if (!CORBA::is_nil (this->sa_.in ()) && this->sa_id_ != default_admin_id)
    {
      if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) destroy admin %d\n"),
            static_cast<int>(this->sa_id_)
          ));
      }
      this->sa_->destroy();
    }
  }
  this->orb_->shutdown ();
  return 0;
}

void Supplier_Main::send_structured_event (void)
{
 CosNotification::StructuredEvent event;

  // EventHeader.

  // FixedEventHeader.
  // EventType.
  // string.
  event.header.fixed_header.event_type.domain_name = CORBA::string_dup("*");
  // string
  event.header.fixed_header.event_type.type_name = CORBA::string_dup("*");
  // string
  event.header.fixed_header.event_name = CORBA::string_dup("reconnect_test");

  // OptionalHeaderFields.
  // PropertySeq.
  // sequence<Property>: string name, any value
  CosNotification::PropertySeq& qos =  event.header.variable_header;
  qos.length (2);
  qos[0].name = CORBA::string_dup (CosNotification::Priority);
  qos[0].value <<= CosNotification::LowestPriority;
  qos[1].name = CORBA::string_dup (CosNotification::EventReliability);
  qos[1].value <<= CosNotification::Persistent;

  // FilterableEventBody
  // PropertySeq
  // sequence<Property>: string name, any value
  event.filterable_data.length (1);
  event.filterable_data[0].name = CORBA::string_dup("serial_number");
  event.filterable_data[0].value <<= CORBA::ULong ( this->serial_number_);

  // any
  event.remainder_of_body <<= CORBA::ULong ( this->serial_number_);

  if (this->verbose_)
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P,%t) Supplier push structured event %d\n"),
      static_cast<int>(serial_number_)
      ));
  }

  this->structured_proxy_push_consumer_->push_structured_event (event);
}

void Supplier_Main::send_sequence_event (void)
{
 CosNotification::EventBatch event_batch(1);
 event_batch.length (1);
 CosNotification::StructuredEvent & event = event_batch[0];

  // EventHeader.

  // FixedEventHeader.
  // EventType.
  // string.
  event.header.fixed_header.event_type.domain_name = CORBA::string_dup("*");
  // string
  event.header.fixed_header.event_type.type_name = CORBA::string_dup("*");
  // string
  event.header.fixed_header.event_name = CORBA::string_dup("reconnect_test");

  // OptionalHeaderFields.
  // PropertySeq.
  // sequence<Property>: string name, any value
  CosNotification::PropertySeq& qos =  event.header.variable_header;
  qos.length (2);
  qos[0].name = CORBA::string_dup (CosNotification::Priority);
  qos[0].value <<= CosNotification::LowestPriority;
  qos[1].name = CORBA::string_dup (CosNotification::EventReliability);
  qos[1].value <<= CosNotification::Persistent;

  // FilterableEventBody
  // PropertySeq
  // sequence<Property>: string name, any value
  event.filterable_data.length (1);
  event.filterable_data[0].name = CORBA::string_dup("serial_number");
  event.filterable_data[0].value <<= CORBA::ULong ( this->serial_number_);

  // any
  event.remainder_of_body <<= CORBA::ULong ( this->serial_number_);

  if (this->verbose_)
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P,%t) Supplier push sequence events %d\n"),
      static_cast<int>(this->serial_number_)
      ));
  }

  this->sequence_proxy_push_consumer_->push_structured_events (event_batch);
}

void Supplier_Main::send_any_event (void)
{
  CORBA::Any event;
  event <<= CORBA::ULong (this->serial_number_);

  if (this->verbose_)
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P,%t) Supplier push any event %d\n"),
      static_cast<int>(this->serial_number_)
      ));
  }

  this->any_proxy_push_consumer_->push (event);
}

int Supplier_Main::run (void)
{
  int result = 0;
  bool paused = false;
  size_t reconnections = 0;

  size_t send = 0;
  while ( send < this->send_)
  {
    ACE_Time_Value tv(0, 100 * 1000);
    orb_->run(tv);

    if (this->pause_ != 0 && send == this->pause_)
    {
      if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Supplier paused after %d events\n"),
          static_cast<int>(this->pause_)
          ));
      }
      reconnections = this->reconnection_callback_.reconnect_count ();
      FILE * pause_file = ACE_OS::fopen ("Supplier.paused", "w");
      if (pause_file != 0)
      {
        ACE_OS::fputs (ACE_TEXT ("paused\n"), pause_file);
        ACE_OS::fclose (pause_file);
      }
      paused = true;
      this->pause_ = 0;
    }
    if (paused)
    {
      if (this->reconnection_callback_.reconnect_count () != reconnections)
      {
        if (this->verbose_)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("(%P|%t) Supplier no longer paused. Next s# %d\n"),
            static_cast<int>(this->serial_number_)
            ));
        }
        paused = false;
      }
    }

    if (!paused)
    {
      switch (this->mode_)
      {
        case MODE_STRUCTURED:
        {
          send_structured_event ();
          break;
        }
        case MODE_SEQUENCE:
        {
          send_sequence_event ();
          break;
        }
        case MODE_ANY:
        {
          send_any_event ();
          break;
        }
      }
      this->serial_number_ += 1;
      send += 1;
    }
  }
  return result;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = -1;
  Supplier_Main app;
  try
  {
    result = app.init(argc, argv);

    if (result == 0)
    {
      result = app.run ();
    }
    if (result == 0)
    {
      app.fini ();
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Error: Supplier::main\t\n");
    result = -1;
  }
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P,%t) Supplier exits: code %d\n"),
    result
    ));
  return result;
}
