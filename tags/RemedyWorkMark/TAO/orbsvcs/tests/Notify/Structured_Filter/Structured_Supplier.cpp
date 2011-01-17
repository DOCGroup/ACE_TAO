// $Id$

// A Standard simple supplier.
// All filtering is done on the consumer side.

#include "Notify_StructuredPushSupplier.h"
#include "goS.h"
#include "Notify_Test_Client.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_strings.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_unistd.h"

static const ACE_TCHAR *ior_file = ACE_TEXT ("supplier.ior");
static TAO_Notify_Tests_StructuredPushSupplier* supplier = 0;
static int num_events = 90;

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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
  {
    case 'o':
      ior_file = get_opts.optarg;
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
        "usage:  %s "
        "-o <iorfile>"
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

int
send_event (int id)
{
  CosNotification::StructuredEvent event;

  event.header.fixed_header.event_type.domain_name =
    CORBA::string_dup ("TAO Test Suite");
  event.header.fixed_header.event_type.type_name =
    CORBA::string_dup ("Filtered Structured Event Notification Svc test");

  event.header.fixed_header.event_name = CORBA::string_dup ("test");
  event.header.variable_header.length (1);

  event.filterable_data.length (3);
  event.filterable_data[0].name = CORBA::string_dup ("id");
  event.filterable_data[0].value <<= static_cast<CORBA::ULong>(id);
  event.filterable_data[1].name = CORBA::string_dup ("group");
  event.filterable_data[1].value <<= static_cast<CORBA::ULong>(id % 3);
  event.filterable_data[2].name = CORBA::string_dup ("type");
  // Divide by 3 first so that the type and group aren't synched
  event.filterable_data[2].value <<= static_cast<CORBA::ULong>(id / 3 % 3);

  try
  {
    ACE_DEBUG((LM_DEBUG, "+"));

    supplier->send_event (event);
  }
  catch (const CORBA::Exception& e)
  {
    e._tao_print_exception ("\nError: Supplier: ");
  }

  return 0;
}

static void create_supplier (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                             PortableServer::POA_ptr poa)
{
  ACE_NEW_THROW_EX (supplier,
    TAO_Notify_Tests_StructuredPushSupplier (),
    CORBA::NO_MEMORY ());

  supplier->init (poa);

  supplier->connect (admin);
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Auto_Ptr< sig_i > sig_impl;
  try
  {
    Notify_Test_Client client;
    int status = client.init (argc, argv);
    ACE_UNUSED_ARG(status);
    ACE_ASSERT(status == 0);

    CosNotifyChannelAdmin::EventChannel_var ec =
      client.create_event_channel ("MyEventChannel", 0);

    ACE_ASSERT(! CORBA::is_nil(ec.in()));

    CORBA::ORB_ptr orb = client.orb ();

    sig_impl.reset( new sig_i( orb ) );
    sig_var sig = sig_impl->_this ();

    CosNotifyChannelAdmin::SupplierAdmin_var admin =
      create_supplieradmin (ec.in ());
    ACE_ASSERT(! CORBA::is_nil (admin.in ()));

    create_supplier (admin.in (), client.root_poa ());

    if (ior_file != 0)
    {
      CORBA::String_var ior =
        client.orb ()->object_to_string (sig.in ());

      FILE *output_file= ACE_OS::fopen (ACE_TEXT_ALWAYS_CHAR(ior_file), "w");
      ACE_ASSERT (output_file != 0);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);
    }

    ACE_DEBUG((LM_DEBUG, "Supplier ready...\n"));

    sig_impl->wait_for_startup();

    ACE_DEBUG((LM_DEBUG, "Supplier sending %d events...\n", num_events));
    for (int i = 0; i < num_events; ++i)
    {
      ACE_DEBUG((LM_DEBUG, "+"));
      send_event (i);
    }
    ACE_DEBUG((LM_DEBUG, "\nSupplier sent %d events.\n", num_events));

    sig_impl->wait_for_completion();

    ACE_OS::unlink (ior_file);

    ec->destroy();

    return 0;
  }
  catch (const CORBA::Exception& e)
  {
    e._tao_print_exception ("Error: Supplier: ");
  }

  return 1;
}
