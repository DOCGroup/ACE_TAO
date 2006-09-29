// $Id$
#include "Notify_StructuredPushSupplier.h"
#include "goS.h"

#include "Notify_Test_Client.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/TimeBaseC.h"

#include "tao/ORB_Core.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Auto_Ptr.h"

static TAO_Notify_Tests_StructuredPushSupplier* supplier_1 = 0;
static CORBA::Short order_policy = CosNotification::FifoOrder;
static int num_events = 30;
static const char* ior_output_file = "supplier.ior";

class sig_i : public POA_sig
{
public:
  sig_i(CORBA::ORB_ptr orb)
    : orb_(orb)
    , started_(false)
  {
  }

  void go (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    started_ = true;
  }

  void done (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
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
  virtual int parse_args (int argc, char* argv[]);
};


int
Supplier_Client::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:e:d:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
  {
    case 'd':
      {
        const char* order = get_opts.optarg;
        if (ACE_OS::strcmp (order, "fifo") == 0)
        {
          order_policy = CosNotification::FifoOrder;
        }
        else if (ACE_OS::strcmp (order, "priority") == 0)
        {
          order_policy = CosNotification::PriorityOrder;
        }
        else if (ACE_OS::strcmp (order, "deadline") == 0)
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
        "-o <iorfile> -e <# of events> "
        "-d <fifo|priority|deadline>"
        "\n",
        argv [0]),
        -1);
  }

  // Indicates sucessful parsing of the command line
  return 0;
}


static CosNotifyChannelAdmin::SupplierAdmin_ptr
create_supplieradmin (CosNotifyChannelAdmin::EventChannel_ptr ec
                      ACE_ENV_ARG_DECL)
{
  CosNotifyChannelAdmin::AdminID adminid = 0;
  CosNotifyChannelAdmin::SupplierAdmin_var admin =
    ec->new_for_suppliers (CosNotifyChannelAdmin::AND_OP,
    adminid
    ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (0);

  return CosNotifyChannelAdmin::SupplierAdmin::_duplicate (admin.in ());
}


static void
SendEvent (int id ACE_ENV_ARG_DECL_NOT_USED)
{
  CosNotification::StructuredEvent event;

  event.header.fixed_header.event_type.domain_name = CORBA::string_dup ("a");
  event.header.fixed_header.event_type.type_name = CORBA::string_dup ("b");
  event.header.fixed_header.event_name = CORBA::string_dup ("test");

  event.header.variable_header.length (3);
  event.header.variable_header[0].name = CORBA::string_dup ("id");
  event.header.variable_header[0].value <<= (CORBA::Long) id;
  event.header.variable_header[1].name =
    CORBA::string_dup (CosNotification::Priority);
  event.header.variable_header[1].value <<= (CORBA::Short) id;
  event.header.variable_header[2].name =
    CORBA::string_dup (CosNotification::Timeout);
  event.header.variable_header[2].value <<= (TimeBase::TimeT) (num_events - id);

  ACE_TRY_NEW_ENV
  {
    supplier_1->send_event (event ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCH (CORBA::Exception, e)
  {
    ACE_PRINT_EXCEPTION (e, "Error: ");
  }
  ACE_ENDTRY;
}

static void
create_suppliers (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                  PortableServer::POA_ptr poa
                  ACE_ENV_ARG_DECL)
{
  // startup the supplier
  ACE_NEW_THROW_EX (supplier_1,
    TAO_Notify_Tests_StructuredPushSupplier (),
    CORBA::NO_MEMORY ());

  supplier_1->init (poa ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  supplier_1->connect (admin ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

int main (int argc, char* argv[])
{
  ACE_Auto_Ptr< sig_i > sig_impl;
  ACE_TRY_NEW_ENV
  {
    Supplier_Client client;
    int status = client.init (argc, argv ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_UNUSED_ARG(status);
    ACE_ASSERT(status == 0);

    CosNotifyChannelAdmin::EventChannel_var ec =
      client.create_event_channel ("MyEventChannel", 0 ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    CosNotification::QoSProperties qos (1);
    qos.length (1);
    qos[0].name = CORBA::string_dup (CosNotification::OrderPolicy);
    qos[0].value <<= order_policy;
    ec->set_qos (qos ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    CORBA::ORB_ptr orb = client.orb ();

    sig_impl.reset( new sig_i( orb ) );
    sig_var sig = sig_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    CORBA::String_var ior =
      orb->object_to_string (sig.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (ior_output_file != 0)
    {
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
        "Cannot open output file for "
        "writing IOR: %s",
        ior_output_file),
        1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);
    }

    CosNotifyChannelAdmin::SupplierAdmin_var admin =
      create_supplieradmin (ec.in () ACE_ENV_ARG_PARAMETER);
    ACE_ASSERT(!CORBA::is_nil (admin.in ()));
    create_suppliers (admin.in (), client.root_poa () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    sig_impl->wait_for_startup();

    ACE_DEBUG((LM_DEBUG, "1 supplier sending %d events...\n", num_events));
    for (int i = 0; i < num_events; ++i)
    {
      ACE_DEBUG((LM_DEBUG, "+"));
      SendEvent (i ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
    ACE_DEBUG((LM_DEBUG, "\nSupplier sent %d events.\n", num_events));

    sig_impl->wait_for_completion();

    ACE_OS::unlink (ior_output_file);

    supplier_1->disconnect(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    ec->destroy(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    return 0;
  }
  ACE_CATCH (CORBA::Exception, e)
  {
    ACE_PRINT_EXCEPTION (e, "Error: ");
  }
  ACE_ENDTRY;

  return 1;
}
