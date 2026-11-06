#include "Notify_StructuredPushSupplier.h"
#include "goS.h"
#include "Notify_Test_Client.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/TimeBaseC.h"

#include "tao/ORB_Core.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

#include <memory>

struct Supplier_Options
{
  CORBA::Short order_policy { CosNotification::FifoOrder };
  int num_events { 30 };
  ACE_TCHAR const* ior_output_file { ACE_TEXT ("supplier.ior") };
};

class sig_i : public POA_sig
{
public:
  explicit sig_i (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  sig_i (sig_i const&) = delete;
  sig_i (sig_i&&) = delete;
  sig_i& operator= (sig_i const&) = delete;
  sig_i& operator= (sig_i&&) = delete;

  void go () override
  {
    this->started_ = true;
  }

  void done () override
  {
    this->started_ = false;
  }

  void wait_for_startup ()
  {
    while (!this->started_)
      {
        ACE_Time_Value timeout (0, 100 * 1000);
        this->orb_->run (timeout);
      }
  }

  void wait_for_completion ()
  {
    while (this->started_)
      {
        ACE_Time_Value timeout (0, 100 * 1000);
        this->orb_->run (timeout);
      }
  }

private:
  CORBA::ORB_var orb_ {};
  bool started_ {};
};

class Supplier_Client : public Notify_Test_Client
{
public:
  Supplier_Client () = default;
  int parse_args (int argc, ACE_TCHAR* argv[]) override;

  Supplier_Client (Supplier_Client const&) = delete;
  Supplier_Client (Supplier_Client&&) = delete;
  Supplier_Client& operator= (Supplier_Client const&) = delete;
  Supplier_Client& operator= (Supplier_Client&&) = delete;

  Supplier_Options const& options () const
  {
    return this->options_;
  }

private:
  Supplier_Options options_ {};
};

int
Supplier_Client::parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:e:d:"));
  int option {};

  while ((option = get_opts ()) != -1)
    {
      switch (option)
        {
        case 'd':
          {
            ACE_TCHAR const* const order = get_opts.opt_arg ();
            if (ACE_OS::strcmp (order, ACE_TEXT ("fifo")) == 0)
              this->options_.order_policy = CosNotification::FifoOrder;
            else if (ACE_OS::strcmp (order, ACE_TEXT ("priority")) == 0)
              this->options_.order_policy = CosNotification::PriorityOrder;
            else if (ACE_OS::strcmp (order, ACE_TEXT ("deadline")) == 0)
              {
                this->options_.order_policy = CosNotification::DeadlineOrder;
#if !defined (ACE_HAS_TIMED_MESSAGE_BLOCKS)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("This order policy requires timed message blocks.\n"
                                             "Please define ACE_HAS_TIMED_MESSAGE_BLOCKS in "
                                             "your config.h\n")),
                                  -1);
#endif
              }
            else
              ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Unknown ordering policy\n")), -1);
            break;
          }
        case 'e':
          this->options_.num_events = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 'o':
          this->options_.ior_output_file = get_opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s -o <iorfile> -e <# of events> "
                                       "-d <fifo|priority|deadline>\n"),
                             argv[0]),
                            -1);
        }
    }
  return 0;
}

static CosNotifyChannelAdmin::SupplierAdmin_ptr
create_supplieradmin (CosNotifyChannelAdmin::EventChannel_ptr ec)
{
  CosNotifyChannelAdmin::AdminID adminid {};
  CosNotifyChannelAdmin::SupplierAdmin_var admin =
    ec->new_for_suppliers (CosNotifyChannelAdmin::AND_OP, adminid);
  return CosNotifyChannelAdmin::SupplierAdmin::_duplicate (admin.in ());
}

static void
send_event (TAO_Notify_Tests_StructuredPushSupplier& supplier,
            int id,
            int num_events)
{
  CosNotification::StructuredEvent event {};
  event.header.fixed_header.event_type.domain_name = CORBA::string_dup ("a");
  event.header.fixed_header.event_type.type_name = CORBA::string_dup ("b");
  event.header.fixed_header.event_name = CORBA::string_dup ("test");
  event.header.variable_header.length (3);
  event.header.variable_header[0].name = CORBA::string_dup ("id");
  event.header.variable_header[0].value <<= static_cast<CORBA::Long> (id);
  event.header.variable_header[1].name = CORBA::string_dup (CosNotification::Priority);
  event.header.variable_header[1].value <<= static_cast<CORBA::Short> (id);
  event.header.variable_header[2].name = CORBA::string_dup (CosNotification::Timeout);
  event.header.variable_header[2].value <<= static_cast<TimeBase::TimeT> (num_events - id);

  try
    {
      supplier.send_event (event);
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Error: ");
    }
}

static void
create_suppliers (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                  PortableServer::POA_ptr poa,
                  TAO_Notify_Tests_StructuredPushSupplier*& supplier)
{
  ACE_NEW_THROW_EX (supplier,
                    TAO_Notify_Tests_StructuredPushSupplier (),
                    CORBA::NO_MEMORY ());
  supplier->init (poa);
  supplier->connect (admin);
}

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try
    {
      Supplier_Client client;
      int const status = client.init (argc, argv);
      if (status != 0)
        return status;
      Supplier_Options const& options = client.options ();

      CosNotifyChannelAdmin::EventChannel_var const ec =
        client.create_event_channel ("MyEventChannel", 0);
      CosNotification::QoSProperties qos (1);
      qos.length (1);
      qos[0].name = CORBA::string_dup (CosNotification::OrderPolicy);
      qos[0].value <<= options.order_policy;
      ec->set_qos (qos);

      std::unique_ptr<sig_i> signal = std::make_unique<sig_i> (client.orb ());
      sig_var const sig = signal->_this ();
      CORBA::String_var const ior = client.orb ()->object_to_string (sig.in ());
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot open output file for writing IOR\n")), 1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      CosNotifyChannelAdmin::SupplierAdmin_var const admin = create_supplieradmin (ec.in ());
      ACE_ASSERT (!CORBA::is_nil (admin.in ()));
      TAO_Notify_Tests_StructuredPushSupplier* supplier {};
      // connect() transfers the initial servant reference to the POA; this pointer is borrowed.
      create_suppliers (admin.in (), client.root_poa (), supplier);

      signal->wait_for_startup ();
      ACE_DEBUG ((LM_DEBUG, "1 supplier sending %d events...\n", options.num_events));
      for (int i {}; i < options.num_events; ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "+"));
          send_event (*supplier, i, options.num_events);
        }
      ACE_DEBUG ((LM_DEBUG, "\nSupplier sent %d events.\n", options.num_events));
      signal->wait_for_completion ();
      ACE_OS::unlink (options.ior_output_file);
      supplier->disconnect ();
      ec->destroy ();
      return 0;
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Error: ");
    }
  return 1;
}
