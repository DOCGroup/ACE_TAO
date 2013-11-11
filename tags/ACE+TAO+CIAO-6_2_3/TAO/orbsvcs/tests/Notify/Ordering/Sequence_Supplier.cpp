// $Id$
#include "Notify_SequencePushSupplier.h"
#include "goS.h"
#include "Notify_Test_Client.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/TimeBaseC.h"

#include "tao/ORB_Core.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_strings.h"
#include "ace/Auto_Ptr.h"

static TAO_Notify_Tests_SequencePushSupplier* supplier_1 = 0;
static CORBA::Short order_policy = CosNotification::FifoOrder;
static int num_events = 30;
static const ACE_TCHAR *ior_output_file = ACE_TEXT ("supplier.ior");
static const int BATCH_SIZE = 5;

class sig_i : public POA_sig
{
public:
  sig_i(CORBA::ORB_ptr orb)
    : orb_(orb)
    , started_(false)
  {
  }

  void go (void)
  {
    started_ = true;
  }

  void done (void)
  {
    started_ = false;
  }

  void wait_for_startup()
  {
    while (! started_) {
      ACE_Time_Value tv(0, 100 * 1000); // 100ms
      orb_->run(tv);
    }
  }

  void wait_for_completion()
  {
    while (started_) {
      ACE_Time_Value tv(0, 100 * 1000); // 100ms
      orb_->run(tv);
    }
  }

private:
  CORBA::ORB_ptr orb_;
  bool started_;
};

class Supplier_Client : public Notify_Test_Client
{
public:
  virtual int parse_args (int argc, ACE_TCHAR *argv[]);
};

int
Supplier_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:e:d:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
  {
    case 'd':
      {
        const ACE_TCHAR *order = get_opts.optarg;
        if (ACE_OS::strcasecmp (order, ACE_TEXT ("fifo")) == 0)
        {
          order_policy = CosNotification::FifoOrder;
        }
        else if (ACE_OS::strcasecmp (order, ACE_TEXT ("priority")) == 0)
        {
          order_policy = CosNotification::PriorityOrder;
        }
        else if (ACE_OS::strcasecmp (order, ACE_TEXT ("deadline")) == 0)
        {
          order_policy = CosNotification::DeadlineOrder;
#if !defined (ACE_HAS_TIMED_MESSAGE_BLOCKS)
          ACE_ERROR_RETURN ((LM_ERROR,
            "This order policy requires timed message "
            "blocks.\nPlease #define "
            "ACE_HAS_TIMED_MESSAGE_BLOCKS in your "
            "config.h\n"), -1);
#endif
        }
        else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
            "Unknown order policy: %s\n",
            order_policy),
            -1);
        }
        break;
      }

    case 'e':
      num_events = ACE_OS::atoi (get_opts.optarg);
      break;

    case 'o':
      ior_output_file = get_opts.optarg;
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
        "usage:  %s "
        "-o <iorfile> -e <# of events> -d"
        "\n",
        argv [0]),
        -1);
  }

  // Indicates successful parsing of the command line
  return 0;
}

static CosNotifyChannelAdmin::SupplierAdmin_ptr
create_supplieradmin (CosNotifyChannelAdmin::EventChannel_ptr ec)
{
  CosNotifyChannelAdmin::AdminID adminid = 0;
  CosNotifyChannelAdmin::SupplierAdmin_var admin =
    ec->new_for_suppliers (CosNotifyChannelAdmin::AND_OP,
    adminid);


  return CosNotifyChannelAdmin::SupplierAdmin::_duplicate (admin.in ());
}


static void
SendBatch (int batch_id)
{
  CosNotification::EventBatch events;
  events.length(BATCH_SIZE);
  for (CORBA::Long i = 0; i < BATCH_SIZE; ++i)
  {
    int id = batch_id * BATCH_SIZE + i;

    CosNotification::StructuredEvent event;

    event.header.fixed_header.event_type.domain_name = CORBA::string_dup ("a");
    event.header.fixed_header.event_type.type_name = CORBA::string_dup ("b");
    event.header.fixed_header.event_name = CORBA::string_dup ("c");

    event.header.variable_header.length (3);
    event.header.variable_header[0].name =
      CORBA::string_dup ("id");
    event.header.variable_header[0].value <<= (CORBA::Long)id;
    event.header.variable_header[1].name =
      CORBA::string_dup (CosNotification::Priority);
    event.header.variable_header[1].value <<= (CORBA::Short)(id);
    event.header.variable_header[2].name =
      CORBA::string_dup (CosNotification::Timeout);
    event.header.variable_header[2].value <<= (TimeBase::TimeT) (num_events - id);

    events[i] = event;
  }
  supplier_1->send_events (events);
}

static void
create_suppliers (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                  PortableServer::POA_ptr poa)
{
  ACE_NEW_THROW_EX (supplier_1,
    TAO_Notify_Tests_SequencePushSupplier (),
    CORBA::NO_MEMORY ());

  supplier_1->init (poa);

  supplier_1->connect (admin);
}


// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Auto_Ptr< sig_i > sig_impl;
  try
  {
    Supplier_Client client;
    int status = client.init (argc, argv);
    ACE_UNUSED_ARG(status);
    ACE_ASSERT(status == 0);

    CosNotifyChannelAdmin::EventChannel_var ec =
      client.create_event_channel ("MyEventChannel", 0);

    CosNotification::QoSProperties qos (1);
    qos.length (1);
    qos[0].name = CORBA::string_dup (CosNotification::OrderPolicy);
    qos[0].value <<= order_policy;
    ec->set_qos (qos);

    CORBA::ORB_ptr orb = client.orb ();

    sig_impl.reset( new sig_i( orb ) );
    sig_var sig = sig_impl->_this ();

    CORBA::String_var ior =
      orb->object_to_string (sig.in ());

    if (ior_output_file != 0)
    {
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
        "Cannot open output file %s for "
        "writing IOR: %C",
        ior_output_file,
        ior.in ()),
        1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);
    }

    CosNotifyChannelAdmin::SupplierAdmin_var admin =
      create_supplieradmin (ec.in ());
    ACE_ASSERT(!CORBA::is_nil (admin.in ()));
    create_suppliers (admin.in (), client.root_poa ());

    sig_impl->wait_for_startup();

    ACE_DEBUG((LM_DEBUG, "1 supplier sending %d events...\n", num_events));
    for (int i = 0; i < num_events / BATCH_SIZE; ++i)
    {
      ACE_DEBUG((LM_DEBUG, "+"));
      SendBatch (i);
    }
    ACE_DEBUG((LM_DEBUG, "\nSupplier sent %d events.\n", num_events));

    sig_impl->wait_for_completion();

    ACE_OS::unlink (ior_output_file);

    supplier_1->disconnect();

    ec->destroy();

    return 0;
  }
  catch (const CORBA::Exception& e)
  {
    e._tao_print_exception ("Error: ");
  }

  return 1;
}
