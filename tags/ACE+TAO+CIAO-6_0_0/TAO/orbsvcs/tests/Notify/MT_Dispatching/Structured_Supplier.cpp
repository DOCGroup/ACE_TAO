// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"

#include "tao/ORB_Core.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "Notify_StructuredPushSupplier.h"
#include "goS.h"

#include "Notify_Test_Client.h"

#include "ace/OS_NS_unistd.h"

// ******************************************************************
// Data Section
// ******************************************************************

static const unsigned int supplier_max = 32;
static TAO_Notify_Tests_StructuredPushSupplier* suppliers[supplier_max] = {0};
static unsigned int supplier_count = 1;
static int event_count = 1;
static const ACE_TCHAR *ior_output_file = ACE_TEXT ("supplier.ior");

// ******************************************************************
// Subroutine Section
// ******************************************************************
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

class Consumer_Client : public Notify_Test_Client
{
public:
  virtual int parse_args (int argc, ACE_TCHAR *argv[]);
};


int
Consumer_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:e:fc:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
  {
    case 'c':
      supplier_count = ACE_OS::atoi (get_opts.optarg);
      if (supplier_count > supplier_max)
      {
        supplier_count = supplier_max;
      }
      break;

    case 'e':
      event_count = ACE_OS::atoi (get_opts.optarg);
      break;

    case 'o':
      ior_output_file = get_opts.optarg;
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
        "usage:  %s "
        "-o <iorfile> -e <# of events>"
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
SendEvent (int id)
{
  ACE_UNUSED_ARG(id);
  CosNotification::StructuredEvent event;

  event.header.fixed_header.event_type.domain_name = CORBA::string_dup ("a");
  event.header.fixed_header.event_type.type_name = CORBA::string_dup ("b");
  event.header.fixed_header.event_name = CORBA::string_dup ("test");

  try
  {
    for (unsigned int i = 0; i < supplier_count; i++)
    {
      suppliers[i]->send_event (event);
    }
  }
  catch (const CORBA::Exception& e)
  {
    e._tao_print_exception ("Error: ");
  }
}

static void
create_suppliers (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                  Notify_Test_Client* client)
{
  for (unsigned int i = 0; i < supplier_count; i++)
  {
    ACE_NEW_THROW_EX (suppliers[i],
      TAO_Notify_Tests_StructuredPushSupplier (),
      CORBA::NO_MEMORY ());

    suppliers[i]->init (client->root_poa ());

    suppliers[i]->connect (admin);
  }
}

static void
disconnect_suppliers (void)
{
  for (unsigned int i = 0; i < supplier_count; ++i)
  {
    suppliers[i]->disconnect ();
  }
}

// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Auto_Ptr< sig_i > sig_impl;
  try
  {
    Consumer_Client client;
    int status = client.init (argc, argv);
    ACE_UNUSED_ARG(status);
    ACE_ASSERT(status == 0);

    CosNotifyChannelAdmin::EventChannel_var ec =
      client.create_event_channel ("MyEventChannel", 0);

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
    create_suppliers (admin.in (), &client);

    sig_impl->wait_for_startup();

    ACE_DEBUG((LM_DEBUG, "%i supplier(s) sending %d events...\n", supplier_count, event_count));
    for (int i = 0; i < event_count; ++i)
    {
      ACE_DEBUG((LM_DEBUG, "+"));
      SendEvent (i);
    }
    ACE_DEBUG((LM_DEBUG, "\nEach Supplier sent %d events.\n", event_count));

    sig_impl->wait_for_completion();

    ACE_OS::unlink (ior_output_file);

    disconnect_suppliers();

    ec->destroy();

    return 0;
  }
  catch (const CORBA::Exception& e)
  {
    e._tao_print_exception ("Error: ");
  }

  return 1;
}
