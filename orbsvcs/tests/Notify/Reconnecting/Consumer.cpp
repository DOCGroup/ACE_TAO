//$Id$

#include "Consumer.h"
#include "tao/debug.h"
#include "tao/PortableServer/PortableServerC.h"
#include "tao/TimeBaseC.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_stdio.h"
#include "tao/AnyTypeCode/Any.h"



static const char NOTIFY_FACTORY_NAME[] = "NotifyEventChannelFactory";
static const char NAMING_SERVICE_NAME[] = "NameService";

static const size_t minus_one = static_cast<size_t>(-1);

///////////////////////////
// StructuredPushConsumer_i

StructuredPushConsumer_i::StructuredPushConsumer_i ()
  : verbose_ (false)
  , received_ (0)
  , expect_ (10)
  , fail_ (0)
  , may_discard_ (0)
  , serial_number_ (0)
  , exceptions_thrown_ (0)
  , problem_ (false)
  , connected_ (false)
{
}

size_t
StructuredPushConsumer_i::received () const
{
  return this->received_;
}

bool
StructuredPushConsumer_i::has_problem () const
{
  return this->problem_;
}

void
StructuredPushConsumer_i::set_connected (bool flag)
{
  this->connected_ = flag;
}

bool
StructuredPushConsumer_i::is_connected () const
{
  return this->connected_;
}

void
StructuredPushConsumer_i::set_expectations (size_t expected, size_t fail, size_t serial_number, bool verbose)
{
  this->expect_ = expected;
  this->fail_ = fail;
  this->serial_number_ = serial_number;
  this->verbose_ = verbose;
}

void
StructuredPushConsumer_i::offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
  )
{
  ACE_UNUSED_ARG (added);
  ACE_UNUSED_ARG (removed);
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P,%t) StructuredPushConsumer offered change\n")
    ));
}

void
StructuredPushConsumer_i::check_serial_number (CORBA::ULong seq)
{
  if (this->serial_number_ == minus_one)
  {
    if (this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Structured Consumer received event %d.\n"),
        static_cast<int>(seq)
        ));
    }
  }
  else
  {
    if (seq == this->serial_number_)
    {
      if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Structured Consumer received correct event %d.\n"),
          static_cast<int>(seq)
          ));
      }
    }
    else if ((seq - this->serial_number_) <= this->may_discard_)
    {
      if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Structured Consumer: service received event %d; Notify discarded %d*.\n"),
          static_cast<int>(seq),
          static_cast<int>(this->serial_number_)
          ));
      }
      this->received_ += seq - this->serial_number_;
    }
    else
    {
      this->problem_ = true;
      ACE_DEBUG ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Structured Consumer received notifications out of sequence.  Expecting %d received %d.\n"),
        static_cast<int>(this->serial_number_),
        static_cast<int>(seq)
        ));
    }
    this->serial_number_ = seq + 1;
  }
  may_discard_ = 0;
}


void
StructuredPushConsumer_i::push_structured_event (
        const CosNotification::StructuredEvent & notification)
{
  this->received_ += 1;
  if (this->received_ == this->expect_ + 1)
  {
    this->problem_ = true;
    ACE_DEBUG ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Structured Consumer received more than  the %d events that were expected.\n"),
      this->expect_
      ));
  }
  if (this->fail_ != 0 && ((this->received_ + this->exceptions_thrown_) % this->fail_) == 0)
  {
    this->received_ -= 1;
    this->exceptions_thrown_ += 1;
    this->may_discard_ = 1;
    ACE_DEBUG ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Structured Consumer failing at event # %d.\n"),
      static_cast<int>(this->received_)
      ));
    throw CORBA::UNKNOWN();
  }
  if (notification.filterable_data.length () > 0)
  {
    if (0 == ACE_OS::strcmp (notification.filterable_data[0].name, "serial_number"))
    {
      const CORBA::Any & data = notification.filterable_data[0].value;

      CORBA::ULong seq = 0;
      if (data >>= seq)
      {
        check_serial_number (seq);
      }
      else
      {
        this->problem_ = true;
        ACE_DEBUG ((LM_ERROR,
          ACE_TEXT ("(%P|%t) Structured Consumer received unexpected data type for serial_number.\n")
          ));
      }
    }
    else
    {
      this->problem_ = true;
      ACE_DEBUG ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Structured Consumer received wrong filterable data name.%s. Expecting serial_number\n"),
        static_cast<const char *>(notification.filterable_data[0].name)
        ));
    }
  }
  else if (0 == ACE_OS::strcmp (notification.header.fixed_header.event_type.type_name, "%ANY") &&
           0 == ACE_OS::strcmp (notification.header.fixed_header.event_type.domain_name, ""))
  {
    const CORBA::Any * any;
    if (notification.remainder_of_body >>= any)
    {
      // this is a wrapped "Any"
      CORBA::ULong seq = 0;
      if ((*any) >>= seq)
      {
        check_serial_number (seq);
      }
      else
      {
        this->problem_ = true;
        ACE_DEBUG ((LM_ERROR,
          ACE_TEXT ("(%P|%t) Structured Consumer received Any with unknown data type\n")
          ));
      }
    }
    else
    {
      this->problem_ = true;
      ACE_DEBUG ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Structured Consumer received remainder_of_body with unknown data type\n")
        ));
    }
  }
  else
  {
    this->problem_ = true;
    ACE_DEBUG ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Structured Consumer: no filterable data.\n")
      ));
  }
  this->may_discard_ = 0;
}

void
StructuredPushConsumer_i::disconnect_structured_push_consumer (
  )
{
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P,%t) StructuredPushConsumer received disconnect\n")
    ));
}

///////////////////////////
// SequencePushConsumer_i

SequencePushConsumer_i::SequencePushConsumer_i ()
  : verbose_ (false)
  , received_ (0)
  , expect_ (10)
  , fail_ (0)
  , may_discard_ (0)
  , serial_number_ (0)
  , exceptions_thrown_ (0)
  , problem_ (false)
  , connected_ (false)
{
}

size_t
SequencePushConsumer_i::received () const
{
  return this->received_;
}

bool
SequencePushConsumer_i::has_problem () const
{
  return this->problem_;
}

void
SequencePushConsumer_i::set_connected (bool flag)
{
  this->connected_ = flag;
}

bool
SequencePushConsumer_i::is_connected () const
{
  return this->connected_;
}

void
SequencePushConsumer_i::set_expectations (size_t expected, size_t fail,size_t serial_number, bool verbose)
{
  this->expect_ = expected;
  this->fail_ = fail;
  this->serial_number_ = serial_number;
  this->verbose_ = verbose;
}

void
SequencePushConsumer_i::offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
  )
{
  ACE_UNUSED_ARG (added);
  ACE_UNUSED_ARG (removed);
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P,%t) SequencePushConsumer offered subscription change\n")
    ));
}

void
SequencePushConsumer_i::check_serial_number (CORBA::ULong seq)
{
  if (this->serial_number_ == minus_one)
  {
    if (this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Sequence Consumer received event %d.\n"),
        static_cast<int>(seq)
        ));
    }
  }
  else
  {
    if (seq == this->serial_number_)
    {
      if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Sequence Consumer received correct event %d.\n"),
          static_cast<int>(seq)
          ));
      }
    }
    else if ((seq - this->serial_number_) <= this->may_discard_)
    {
      if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Sequence Consumer: service received event %d; Notify discarded %d*.\n"),
          static_cast<int>(seq),
          static_cast<int>(this->serial_number_)
          ));
      }
      this->received_ += seq - this->serial_number_;
    }
    else
    {
      this->problem_ = true;
      ACE_DEBUG ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Sequence Consumer received notifications out of sequence.  Expecting %d received %d.\n"),
        static_cast<int>(this->serial_number_),
        static_cast<int>(seq)
        ));
    }
    this->serial_number_ = seq + 1;
  }
  may_discard_ = 0;
}

void
SequencePushConsumer_i::push_structured_events (
      const CosNotification::EventBatch & notifications
    )
{
  CORBA::ULong batch_size = notifications.length();
  if (this->verbose_)
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Sequence Consumer received batch of %d events.\n"),
      static_cast<int>(batch_size)
      ));
  }
  for (CORBA::ULong nevent = 0; nevent < batch_size; ++nevent)
  {
    this->received_ += 1;
    if (this->received_ == this->expect_ + 1)
    {
      this->problem_ = true;
      ACE_DEBUG ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Sequence Consumer received more than  the %d events that were expected.\n"),
        this->expect_
        ));
    }
    if (this->fail_ != 0 && ((this->received_ + this->exceptions_thrown_) % this->fail_) == 0)
    {
      this->received_ -= 1;
      this->exceptions_thrown_ += 1;
      this->may_discard_ = batch_size;
      ACE_DEBUG ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Sequence Consumer failing at event # %d. Expecting %d duplicates.\n"),
        static_cast<int>(this->received_),
        static_cast<int>(nevent + 1)
        ));
      throw CORBA::UNKNOWN();
    }
    const CosNotification::StructuredEvent & notification = notifications[nevent];
    if (notification.filterable_data.length () > 0)
    {
      if (0 == ACE_OS::strcmp (notification.filterable_data[0].name, "serial_number"))
      {
        const CORBA::Any & data = notification.filterable_data[0].value;

        CORBA::ULong seq = 0;
        if (data >>= seq)
        {
          check_serial_number (seq);
        }
        else
        {
          this->problem_ = true;
          ACE_DEBUG ((LM_ERROR,
            ACE_TEXT ("(%P|%t) Sequence Consumer received unexpected data type for serial_number.\n")
            ));
        }
      }
      else
      {
        this->problem_ = true;
        ACE_DEBUG ((LM_ERROR,
          ACE_TEXT ("(%P|%t) Sequence Consumer received wrong filterable data name.%s. Expecting serial_number\n"),
          static_cast<const char *>(notification.filterable_data[0].name)
          ));
      }
    }
    else if (0 == ACE_OS::strcmp (notification.header.fixed_header.event_type.type_name, "%ANY") &&
             0 == ACE_OS::strcmp (notification.header.fixed_header.event_type.domain_name, ""))
    {
      CORBA::ULong seq = 0;
      if (notification.remainder_of_body >>= seq)
      {
        check_serial_number (seq);
      }
      else
      {
        this->problem_ = true;
        ACE_DEBUG ((LM_ERROR,
          ACE_TEXT ("(%P|%t) Sequence Consumer received Any with unknown data type\n")
          ));
      }
    }
    else
    {
      this->problem_ = true;
      ACE_DEBUG ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Sequence Consumer received no filterable data\n")
        ));
    }
  }
}

void
SequencePushConsumer_i::disconnect_sequence_push_consumer (
  )
{
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P,%t) SequencePushConsumer received disconnect\n")
    ));
}

///////////////////////////
// AnyPushConsumer_i

AnyPushConsumer_i::AnyPushConsumer_i ()
  : verbose_ (false)
  , received_ (0)
  , expect_ (10)
  , fail_ (0)
  , may_discard_ (0)
  , serial_number_ (0)
  , exceptions_thrown_ (0)
  , problem_ (false)
  , connected_ (false)
{
}

size_t
AnyPushConsumer_i::received () const
{
  return this->received_;
}

bool
AnyPushConsumer_i::has_problem () const
{
  return this->problem_;
}

void
AnyPushConsumer_i::set_connected (bool flag)
{
  this->connected_ = flag;
}

bool
AnyPushConsumer_i::is_connected () const
{
  return this->connected_;
}

void
AnyPushConsumer_i::set_expectations (size_t expected, size_t fail, size_t serial_number, bool verbose)
{
  this->expect_ = expected;
  this->fail_ = fail;
  this->serial_number_ = serial_number;
  this->verbose_ = verbose;
}

void
AnyPushConsumer_i::offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
  )
{
  ACE_UNUSED_ARG (added);
  ACE_UNUSED_ARG (removed);
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P,%t) SequencePushConsumer offered subscription change\n")
    ));
}

void
AnyPushConsumer_i::check_serial_number (CORBA::ULong seq)
{
  if (this->serial_number_ == minus_one)
  {
    if (this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Any Consumer received event %d.\n"),
        static_cast<int>(seq)
        ));
    }
  }
  else
  {
    if (seq == this->serial_number_)
    {
      if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Any Consumer received correct event %d.\n"),
          static_cast<int>(seq)
          ));
      }
    }
    else if ((seq - this->serial_number_) <= this->may_discard_)
    {
      if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Any Consumer: service received event %d; Notify discarded %d*.\n"),
          static_cast<int>(seq),
          static_cast<int>(this->serial_number_)
          ));
      }
      this->received_ += seq - this->serial_number_;
    }
    else
    {
      this->problem_ = true;
      ACE_DEBUG ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Any Consumer received notifications out of sequence.  Expecting %d received %d.\n"),
        static_cast<int>(this->serial_number_),
        static_cast<int>(seq)
        ));
    }
    this->serial_number_ = seq + 1;
  }
  may_discard_ = 0;
}

void
AnyPushConsumer_i::push (
        const CORBA::Any & data
      )
{
  this->received_ += 1;
  if (this->received_ == this->expect_ + 1)
  {
    this->problem_ = true;
    ACE_DEBUG ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Any Consumer received more than  the %d events that were expected.\n"),
      this->expect_
      ));
  }
  if (this->fail_ != 0 && ((this->received_ + this->exceptions_thrown_) % this->fail_) == 0)
  {
    this->received_ -= 1;
    this->exceptions_thrown_ += 1;
    this->may_discard_ = 1;
    ACE_DEBUG ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Any Consumer failing at event # %d.\n"),
      static_cast<int>(this->received_)
      ));
    throw CORBA::UNKNOWN();
  }
  CORBA::ULong seq = 0;
  if (data >>= seq)
  {
    check_serial_number (seq);
  }
  else
  {
    const CosNotification::StructuredEvent * notification;
    if (data >>= notification)
    {
      if (notification->filterable_data.length () > 0)
      {
        if (0 == ACE_OS::strcmp (notification->filterable_data[0].name, "serial_number"))
        {
          const CORBA::Any & data = notification->filterable_data[0].value;

          CORBA::ULong seq = 0;
          if (data >>= seq)
          {
            check_serial_number (seq);
          }
          else
          {
            this->problem_ = true;
            ACE_DEBUG ((LM_ERROR,
              ACE_TEXT ("(%P|%t) Any Consumer received structured notification with unexpected data type for serial_number in structured event.\n")
              ));
          }
        }
        else
        {
          this->problem_ = true;
          ACE_DEBUG ((LM_ERROR,
            ACE_TEXT ("(%P|%t) Any Consumer received wrong filterable data name in structured event: %s. Expecting serial_number\n"),
            static_cast<const char *>(notification->filterable_data[0].name)
            ));
        }
      }
      else
      {
        this->problem_ = true;
        ACE_DEBUG ((LM_ERROR,
          ACE_TEXT ("(%P|%t) Any Consumer: no filterable data in structured event.\n")
          ));
      }
    }
    else
    {
      this->problem_ = true;

      ACE_DEBUG ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Any Consumer received unexpected data type.\n")
        ));
    }
  }
}

void
AnyPushConsumer_i::disconnect_push_consumer (
  )
{
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P,%t) AnyPushConsumer received disconnect\n")
    ));
}

/////////////////////////
// ReconnectionCallback_i

ReconnectionCallback_i::ReconnectionCallback_i (Consumer_Main & consumer_main)
  : consumer_main_ (consumer_main)
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
    ACE_TEXT ("(%P|%t) Consumer received reconnection request\n")
    ));
  this->ecf_ = CosNotifyChannelAdmin::EventChannelFactory::_narrow (reconnection);
  if (!CORBA::is_nil (this->ecf_.in ()))
  {
    this->consumer_main_.reconnect (this->ecf_.in ());
    this->reconnect_count_ += 1;
  }
  else
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Consumer reconnection request failed: wrong object type\n")
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
// Consumer_Main

Consumer_Main::Consumer_Main ()
  : verbose_ (false)
  , mode_ (MODE_ANY)
  , expect_ (10)
  , fail_ (0)
  , use_naming_service_ (true)
  , serial_number_ (0)
  , disconnect_on_exit_ (false)
  , structured_count_(0)
  , sequence_count_(0)
  , any_count_(0)
  , id_file_ ("consumer.ids")
  , ec_id_ (0)
  , sa_id_ (0)
  , structured_proxy_id_ (0)
  , sequence_proxy_id_ (0)
  , any_proxy_id_ (0)
  , reconnection_callback_ (*this)
  , reconnecting_ (false)
{
}

Consumer_Main::~Consumer_Main ()
{
}

int
Consumer_Main::parse_args (int argc, ACE_TCHAR *argv[])
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
      ACE_OS::fprintf (stderr, ACE_TEXT ("Unrecognized argument: %s\n"),
        argv[narg]);
      usage (stderr);
      result = -1;
    }
  }
  return result;
}

int
Consumer_Main::parse_single_arg (int argc, ACE_TCHAR *argv[])
{
  int consumed = 0;
  if (ACE_OS::strcasecmp (argv[0], ACE_TEXT("-any")) == 0)
  {
    this->mode_ = MODE_ANY;
    consumed = 1;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT("-structured")) == 0)
  {
    this->mode_ = MODE_STRUCTURED;
    consumed = 1;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT("-sequence")) == 0)
  {
    this->mode_ = MODE_SEQUENCE;
    consumed = 1;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT("-v")) == 0)
  {
    this->verbose_ = true;
    consumed = 1;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT("-channel")) == 0)
  {
    this->channel_file_= argv[1];
    consumed = 2;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT("-expect")) == 0 && argc > 1)
  {
    this->expect_ = ACE_OS::atoi (argv[1]);
    consumed = 2;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT("-fail")) == 0 && argc > 1)
  {
    this->fail_ = ACE_OS::atoi (argv[1]);
    consumed = 2;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT("-serial_number")) == 0)
  {
    this->serial_number_= ACE_OS::atoi (argv[1]);
    consumed = 2;
  }
  else if (ACE_OS::strcasecmp (argv[0], ACE_TEXT("-nonamesvc")) == 0)
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

void Consumer_Main::usage(FILE * out)const
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_OS::fputs (
    ACE_TEXT ("usage\n")
    ACE_TEXT ("  -channel filename Where to find a channel number.\n")
    ACE_TEXT ("  -any or -structured or -sequence\n")
    ACE_TEXT ("                    What type of event to send (pick one, default is -any)\n")
    ACE_TEXT ("  -expect n         How many events of each type are expected.\n")
    ACE_TEXT ("  -fail n           Throw an exception every n events.\n")
    ACE_TEXT ("  -serial_number n  What serial number to start with\n")
    ACE_TEXT ("                    or -1 to disable serial number checking.\n")
    ACE_TEXT ("  -v                Verbose output.\n")
    ACE_TEXT ("  -disconnect       Disconnect from channel on exit (prevents reconnect.)\n")
    ACE_TEXT ("  -nonamesvc        Don't use the name service to find EventChannelFactory\n")
    , out);
  //FUZZ: enable check_for_lack_ACE_OS
}

int Consumer_Main::init (int argc, ACE_TCHAR *argv[])
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
                " (%P|%t) Unable to initialize the POA.\n"));
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

  init_consumer_admin ();

  switch (this->mode_)
  {
    case MODE_STRUCTURED:
    {
      if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P,%t) Consumer: Listening for %d structured events. Failing every %d events.\n"),
          static_cast<int>(this->expect_),
          static_cast<int>(this->fail_)
          ));
      }
      this->structured_push_consumer_.set_expectations (this->expect_, this->fail_, this->serial_number_, this->verbose_);
      init_structured_proxy_supplier ();
      break;
    }
    case MODE_SEQUENCE:
    {
      if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P,%t) Consumer: Listening for %d sequence events. Failing every %d events.\n"),
          static_cast<int>(this->expect_),
          static_cast<int>(this->fail_)
          ));
      }
      this->sequence_push_consumer_.set_expectations (this->expect_, this->fail_, this->serial_number_, this->verbose_);
      init_sequence_proxy_supplier ();
      break;
    }
    case MODE_ANY:
    {
      if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P,%t) Consumer: Listening for %d CORBA::Any events. Failing every %d events.\n"),
          static_cast<int>(this->expect_),
          static_cast<int>(this->fail_)
          ));
      }
      this->any_push_consumer_.set_expectations (this->expect_, this->fail_, this->serial_number_, this->verbose_);
      init_any_proxy_supplier ();
      break;
    }
    default:
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Consumer: Unknown listening mode.\n")
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
Consumer_Main::save_ids()
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
Consumer_Main::load_ids()
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
          ACE_OS::fprintf (stderr, ACE_TEXT ("Consumer: Warning: too many fields in saved id file.\n"));
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
Consumer_Main::reconnect (
    CosNotifyChannelAdmin::EventChannelFactory_ptr dest_factory)
{
  if (this->verbose_)
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Consumer begin ReconnectRegistry based reconnection\n")
      ));
  }
  this->ecf_ = CosNotifyChannelAdmin::EventChannelFactory::_duplicate (dest_factory);
  this->reconnecting_ = true;
  init_event_channel ();

  init_consumer_admin ();

  switch (this->mode_)
  {
    case MODE_STRUCTURED:
    {
      init_structured_proxy_supplier ();
      break;
    }

    case MODE_SEQUENCE:
    {
      init_sequence_proxy_supplier ();
      break;
    }

    case MODE_ANY                                                                                            :
    {
      init_any_proxy_supplier ();
      break;
    }
  }
  if (this->verbose_)
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Consumer ReconnectRegistry based reconnection complete\n")
      ));
  }
}


int
Consumer_Main::resolve_naming_service (void)
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
Consumer_Main::find_notify_factory (void)
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
Consumer_Main::resolve_notify_factory (void)
{
  CORBA::Object_var factory_obj =
    this->orb_->resolve_initial_references (NOTIFY_FACTORY_NAME);

  this->ecf_ =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (
      factory_obj.in ());
  return ! CORBA::is_nil (this->ecf_.in ());
}

void
Consumer_Main::init_event_channel (void)
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
          ACE_TEXT ("(%P|%t) Consumer: Reconnect to event channel %d\n"),
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
              ACE_TEXT ("(%P|%t) Consumer: Connect to Existing event channel %d\n"),
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
    qosprops[i++].value <<= (TimeBase::TimeT) 42 * 10000; // 42ms
    qosprops[i].name = CORBA::string_dup(CosNotification::StopTimeSupported);
    qosprops[i++].value <<= CORBA::Any::from_boolean(1);
    qosprops[i].name = CORBA::string_dup(CosNotification::MaximumBatchSize);
    qosprops[i++].value <<= (CORBA::Long)2;
    qosprops[i].name = CORBA::string_dup(CosNotification::PacingInterval);
    qosprops[i++].value <<= (TimeBase::TimeT) 50 * 10000; // 50ms

    CosNotification::AdminProperties adminprops(4);
    adminprops.length (4);
    i = 0;
    adminprops[i].name = CORBA::string_dup(CosNotification::MaxQueueLength);
    adminprops[i++].value <<= (CORBA::Long) 1234;
    adminprops[i].name = CORBA::string_dup(CosNotification::MaxSuppliers);
    adminprops[i++].value <<= (CORBA::Long) 1000;
    adminprops[i].name = CORBA::string_dup(CosNotification::MaxConsumers);
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
        ACE_TEXT ("(%P|%t) Consumer: Create event channel %d\n"),
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
Consumer_Main::init_consumer_admin (void)
{
  bool ok = false;
  if (this->reconnecting_)
  {
    if (this->sa_id_ == default_admin_id)
    {
      try
      {
        this->sa_ = this->ec_->default_consumer_admin ();
        ok = ! CORBA::is_nil (this->sa_.in ());
        this->sa_id_ = default_admin_id;
        if (ok && this->verbose_)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("(%P|%t) Consumer: Using default consumer admin\n")
            ));
        }
        else if (this->verbose_)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("(%P|%t) Consumer: No default consumer admin\n")
            ));
        }
      }
      catch (...)
      {
      }
    }
    else // not default admin
    {
      try
      {
        this->sa_ = this->ec_->get_consumeradmin(
          this->sa_id_);
        ok = ! CORBA::is_nil (this->sa_.in ());
        if (ok && this->verbose_)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("(%P|%t) Consumer: Reconnect to consumer admin %d\n"),
            static_cast<int>(this->sa_id_)
            ));
        }
      }
      catch (...)
      {
      }
    }
  }
  else // !reconnecting
  {
    try
    {
      this->sa_ = this->ec_->default_consumer_admin ();
      ok = ! CORBA::is_nil (this->sa_.in ());
      this->sa_id_ = default_admin_id;
      if (ok && this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Consumer: Using default consumer admin\n")
          ));
      }
      else if (this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Consumer: No default consumer admin\n")
          ));
      }
    }
    catch (...)
    {
    }

    if (!ok)
    {
      this->sa_ = this->ec_->new_for_consumers(
        CosNotifyChannelAdmin::OR_OP,
        this->sa_id_);
      ok = ! CORBA::is_nil (this->sa_.in ());

      // temporary: be sure we can set qos properties here
      if (ok)
      {
        CosNotification::QoSProperties qosprops(2);
        CORBA::ULong i = 0;
        qosprops.length(2);

        qosprops[i].name = CORBA::string_dup(CosNotification::EventReliability);
        qosprops[i++].value <<= CosNotification::Persistent;
        qosprops[i].name = CORBA::string_dup(CosNotification::ConnectionReliability);
        qosprops[i++].value <<= CosNotification::Persistent; // Required, or we won't persist much
        qosprops.length(i);
        this->sa_->set_qos (qosprops);
      }

      if (ok && this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Consumer: Create new consumer admin %d\n"),
          static_cast<int>(this->sa_id_)
          ));
      }
    }
  }
  if (!ok)
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Consumer: connect to consumer admin failed %d\n"),
      static_cast<int>(this->sa_id_)
      ));
  }
}
void
Consumer_Main::init_structured_proxy_supplier (void)
{
  bool ok = false;
  CosNotifyChannelAdmin::ProxySupplier_var proxy;
  if (this->reconnecting_)
  {
    try
    {
      proxy = this->sa_->get_proxy_supplier (
                           this->structured_proxy_id_
                        );
      ok = ! CORBA::is_nil (proxy.in ());
      if (this->verbose_)
      {
        if (ok)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("(%P|%t) Consumer: Reconnect to proxy supplier %d\n"),
            static_cast<int>(this->structured_proxy_id_)
            ));
        }
        else
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("(%P|%t) Consumer: Unable to reconnect to proxy supplier %d\n"),
            static_cast<int>(this->structured_proxy_id_)
            ));
        }
      }
    }
    catch (...)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Consumer: Unable to reconnect to proxy supplier %d\n"),
        static_cast<int>(this->structured_proxy_id_)
        ));
    }
  }

  if (!ok)
  {
    proxy = this->sa_->obtain_notification_push_supplier(
        CosNotifyChannelAdmin::STRUCTURED_EVENT,
        this->structured_proxy_id_);
    ok = ! CORBA::is_nil (proxy.in ());
    if (ok && this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Consumer: Create new structured proxy %d\n"),
        static_cast<int>(this->structured_proxy_id_)
        ));
    }
  }
  this->structured_proxy_push_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow(proxy.in ());

  if (CORBA::is_nil (this->structured_proxy_push_supplier_.in ()))
  {
    ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) init_structured_proxy_supplier received nil ProxySupplier\n")
        ));
    throw CORBA::OBJECT_NOT_EXIST ();
  }

  if (CORBA::is_nil (this->structured_push_consumer_ref_.in ()))
  {
    PortableServer::ObjectId_var push_consumer_id =
      this->root_poa_->activate_object (
        &(this->structured_push_consumer_));

    CORBA::Object_var obj =
      this->root_poa_->id_to_reference (push_consumer_id.in ());

    this->structured_push_consumer_ref_ =
      CosNotifyComm::StructuredPushConsumer::_narrow (obj.in ());
  }

  if (CORBA::is_nil (this->structured_push_consumer_ref_.in ()))
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Consumer: Error activating structured push consumer\n")
      ));

    throw CORBA::BAD_PARAM();
  }

  this->structured_proxy_push_supplier_->connect_structured_push_consumer (
    this->structured_push_consumer_ref_.in ());
  this->structured_push_consumer_.set_connected(true);
}

void
Consumer_Main::init_sequence_proxy_supplier (void)
{
  bool ok = false;
  CosNotifyChannelAdmin::ProxySupplier_var proxy;
  if (this->reconnecting_)
  {
    try
    {
      proxy = this->sa_->get_proxy_supplier(
        this->sequence_proxy_id_);
      ok = ! CORBA::is_nil (proxy.in ());
      if (ok && this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Consumer: Reconnect to proxy %d\n"),
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
    proxy = this->sa_->obtain_notification_push_supplier(
        CosNotifyChannelAdmin::SEQUENCE_EVENT,
        this->sequence_proxy_id_);
    ok = ! CORBA::is_nil (proxy.in ());

    // temporary
    if (ok)
    {
      set_proxy_qos (proxy.in ());
    }

    if (ok && this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Consumer: Create new sequence proxy %d\n"),
          static_cast<int>(this->sequence_proxy_id_)
        ));
    }
  }
  this->sequence_proxy_push_supplier_ =
    CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow(proxy.in ());

  if (CORBA::is_nil (this->sequence_proxy_push_supplier_.in ()))
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Consumer: Received wrong type of push supplier proxy %d\n"),
        static_cast<int>(this->sequence_proxy_id_)
      ));

    throw CORBA::BAD_PARAM();
  }

  if (CORBA::is_nil (this->sequence_push_consumer_ref_.in ()))
  {
    PortableServer::ObjectId_var push_consumer_id =
      this->root_poa_->activate_object (
        &(this->sequence_push_consumer_));

    CORBA::Object_var obj =
      this->root_poa_->id_to_reference (push_consumer_id.in ());

    this->sequence_push_consumer_ref_ =
      CosNotifyComm::SequencePushConsumer::_narrow (obj.in ());
  }
  if (CORBA::is_nil (this->sequence_push_consumer_ref_.in ()))
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Consumer: Error activating sequence push consumer\n")
      ));

    throw CORBA::BAD_PARAM();
  }

  this->sequence_proxy_push_supplier_->connect_sequence_push_consumer (
    this->sequence_push_consumer_ref_.in ());
  this->sequence_push_consumer_.set_connected(true);

}


void
Consumer_Main::set_proxy_qos (CosNotifyChannelAdmin::ProxySupplier_ptr proxy)
{
  // temporary: be sure we can set qos properties for proxyw
  CosNotification::QoSProperties qosprops(2);
  CORBA::ULong i = 0;
  qosprops.length(2);

  qosprops[i].name = CORBA::string_dup(CosNotification::EventReliability);
  qosprops[i++].value <<= CosNotification::Persistent;
  qosprops[i].name = CORBA::string_dup(CosNotification::ConnectionReliability);
  qosprops[i++].value <<= CosNotification::Persistent; // Required, or we won't persist much
  qosprops.length(i);
  proxy->set_qos (qosprops);
}

void
Consumer_Main::init_any_proxy_supplier (void)
{
  bool ok = false;
  CosNotifyChannelAdmin::ProxySupplier_var proxy;
  if (this->reconnecting_)
  {
    try
    {
      proxy = this->sa_->get_proxy_supplier(
        this->any_proxy_id_);
      ok = ! CORBA::is_nil (proxy.in ());
      if (ok && this->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Consumer: Reconnect to proxy %d\n"),
          static_cast<int>(this->any_proxy_id_)
          ));
      }
      else
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t) Consumer: Get proxy supplier %d returned nil\n"),
          static_cast<int>(this->any_proxy_id_)
          ));
      }
    }
    catch (const CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Consumer: Get proxy supplier %d threw exception\n"),
        static_cast<int>(this->any_proxy_id_)
        ));
      ex._tao_print_exception (ACE_TEXT ("Error:"));
    }
    catch (...)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Error: (%P|%t) Consumer: Get proxy supplier %d threw exception\n"),
        static_cast<int>(this->any_proxy_id_)
        ));
    }
  }

  if (!ok)
  {
    proxy = this->sa_->obtain_notification_push_supplier(
        CosNotifyChannelAdmin::ANY_EVENT,
        this->any_proxy_id_);
    ok = ! CORBA::is_nil (proxy.in ());

    if (ok && this->verbose_)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t) Consumer: Create new Any proxy %d\n"),
          static_cast<int>(this->any_proxy_id_)
        ));
    }
  }
  this->any_proxy_push_supplier_ =
    CosNotifyChannelAdmin::ProxyPushSupplier::_narrow(proxy.in ());

  if (CORBA::is_nil (this->any_proxy_push_supplier_.in ()))
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Consumer: Received wrong type of push supplier proxy %d\n"),
        static_cast<int>(this->any_proxy_id_)
      ));
    throw CORBA::BAD_PARAM();
  }
  if (CORBA::is_nil (this->any_push_consumer_ref_.in ()))
  {
    PortableServer::ObjectId_var push_consumer_id =
      this->root_poa_->activate_object (
        &(this->any_push_consumer_));

    CORBA::Object_var obj =
      this->root_poa_->id_to_reference (push_consumer_id.in ());

    this->any_push_consumer_ref_ =
      CosNotifyComm::PushConsumer::_narrow (obj.in ());
  }

  if (CORBA::is_nil (this->any_push_consumer_ref_.in ()))
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("(%P|%t) Consumer:Error activating push consumer\n")
      ));

    throw CORBA::BAD_PARAM();
  }
  this->any_proxy_push_supplier_->connect_any_push_consumer (
    this->any_push_consumer_ref_.in ());

  this->any_push_consumer_.set_connected(true);
}

int Consumer_Main::fini (void)
{

  this->reconnection_callback_.fini ();

  if (this->disconnect_on_exit_)
  {
    if (!CORBA::is_nil (this->structured_proxy_push_supplier_.in ()))
    {
      this->structured_proxy_push_supplier_->disconnect_structured_push_supplier ();
    }
    if (!CORBA::is_nil (this->sequence_proxy_push_supplier_.in ()))
    {
      this->sequence_proxy_push_supplier_->disconnect_sequence_push_supplier ();
    }
    if (!CORBA::is_nil (this->any_proxy_push_supplier_.in ()))
    {
      this->any_proxy_push_supplier_->disconnect_push_supplier ();
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


int Consumer_Main::run (void)
{
  int result = 0;

  while (this->structured_push_consumer_.received () < this->expect_
    && this->sequence_push_consumer_.received () < this->expect_
    && this->any_push_consumer_.received () < this->expect_
    )
  {
    ACE_Time_Value tv(0, 100 * 1000);
    this->orb_->run(tv);
  }

  // see if consumers detected errors
  if (this->structured_push_consumer_.has_problem ()
    || this->sequence_push_consumer_.has_problem ()
    || this->any_push_consumer_.has_problem ())
  {
    result = -3;
  }
  return result;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = -1;
  Consumer_Main app;
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
    ex._tao_print_exception ("Error: Consumer::main\t\n");
    result = -1;
  }
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P,%t) Consumer exits: code %d\n"),
    result
    ));
  return result;
}
