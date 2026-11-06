#include "Notify_Push_Supplier.h"
#include "goS.h"
#include "Notify_Test_Client.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"

#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_unistd.h"

struct Supplier_Options
{
  ACE_TCHAR const* ior_output_file { ACE_TEXT ("supplier.ior") };
  int event_count { 90 };
  bool use_filter {};
  bool use_or_operator {};
};

constexpr char grammar[] = "TCL";

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
        ACE_Time_Value tv (0, 100 * 1000);
        this->orb_->run (tv);
      }
  }

  void wait_for_completion ()
  {
    while (this->started_)
      {
        ACE_Time_Value tv (0, 100 * 1000);
        this->orb_->run (tv);
      }
  }

private:
  CORBA::ORB_var orb_;
  bool started_ {};
};

class Supplier_Client : public Notify_Test_Client
{
public:
  Supplier_Client () = default;
  int parse_args (int argc, ACE_TCHAR *argv[]) override;
  Supplier_Client (Supplier_Client const&) = delete;
  Supplier_Client (Supplier_Client&&) = delete;
  Supplier_Client& operator= (Supplier_Client const&) = delete;
  Supplier_Client& operator= (Supplier_Client&&) = delete;

  Supplier_Options const& options () const
  {
    return this->options_;
  }

private:
  Supplier_Options options_;
};

int
Supplier_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("e:f:o:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'f':
        this->options_.use_filter = true;
        this->options_.use_or_operator =
          ACE_OS::strcasecmp (ACE_TEXT ("OR"), get_opts.optarg) == 0;
        break;

      case 'e':
        this->options_.event_count = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'o':
        this->options_.ior_output_file = get_opts.opt_arg ();
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s [-f <AND|OR>] [-e num_events] -o <ior_output_file>\n"),
                           argv [0]),
                          -1);
      }

  return 0;
}

static CosNotifyChannelAdmin::SupplierAdmin_ptr
create_supplieradmin (CosNotifyChannelAdmin::EventChannel_ptr ec,
                      Supplier_Options const& options)
{
  CosNotifyChannelAdmin::AdminID admin_id {};
  CosNotifyChannelAdmin::SupplierAdmin_var admin =
    ec->new_for_suppliers (options.use_or_operator
                             ? CosNotifyChannelAdmin::OR_OP
                             : CosNotifyChannelAdmin::AND_OP,
                           admin_id);

  return CosNotifyChannelAdmin::SupplierAdmin::_duplicate (admin.in ());
}

static int
send_event (Notify_Push_Supplier& supplier, int id)
{
  CosNotification::StructuredEvent event;

  event.header.fixed_header.event_type.domain_name = CORBA::string_dup ("TAO Test Suite");
  event.header.fixed_header.event_type.type_name =
    CORBA::string_dup ("Filtered Structured Event Notification Svc test");
  event.header.fixed_header.event_name = CORBA::string_dup ("test");
  event.header.variable_header.length (1);

  event.filterable_data.length (3);
  event.filterable_data[0].name = CORBA::string_dup ("id");
  event.filterable_data[0].value <<= static_cast<CORBA::ULong> (id);
  event.filterable_data[1].name = CORBA::string_dup ("group");
  event.filterable_data[1].value <<= static_cast<CORBA::ULong> (id % 3);
  event.filterable_data[2].name = CORBA::string_dup ("type");
  // Divide by 3 first so that the type and group aren't synchronized.
  event.filterable_data[2].value <<= static_cast<CORBA::ULong> (id / 3 % 3);

  try
    {
      ACE_DEBUG ((LM_DEBUG, "+"));
      supplier.send_event (event);
    }
  catch (CORBA::Exception const& e)
    {
      e._tao_print_exception ("\nError: Supplier: ");
      return -1;
    }

  return 0;
}

static Notify_Push_Supplier*
create_supplier (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                 CosNotifyChannelAdmin::EventChannel_ptr ec,
                 PortableServer::POA_ptr poa,
                 Supplier_Options const& options)
{
  Notify_Push_Supplier* supplier {};
  ACE_NEW_THROW_EX (supplier, Notify_Push_Supplier (), CORBA::NO_MEMORY ());
  supplier->init (poa);
  supplier->_connect (admin, ec, options.use_filter);
  return supplier;
}

static void
add_admin_filter (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                  CosNotifyChannelAdmin::EventChannel_ptr notify_channel)
{
  CosNotifyFilter::FilterFactory_var ffact = notify_channel->default_filter_factory ();
  CosNotifyFilter::Filter_var filter = ffact->create_filter (grammar);
  ACE_ASSERT (!CORBA::is_nil (filter.in ()));

  CosNotifyFilter::ConstraintExpSeq constraint_list (1);
  constraint_list.length (1);
  constraint_list[0].event_types.length (0);
  constraint_list[0].constraint_expr = CORBA::string_dup ("type != 0");
  filter->add_constraints (constraint_list);
  admin->add_filter (filter.in ());
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Supplier_Client client;
      if (client.init (argc, argv) != 0)
        return 1;

      Supplier_Options const& options = client.options ();
      CosNotifyChannelAdmin::EventChannel_var ec =
        client.create_event_channel ("Struct_Multi_Filter", 0);
      ACE_ASSERT (!CORBA::is_nil (ec.in ()));

      CORBA::ORB_ptr const orb = client.orb ();
      sig_i* sig_impl {};
      ACE_NEW_THROW_EX (sig_impl, sig_i (orb), CORBA::NO_MEMORY ());
      PortableServer::ServantBase_var sig_owner (sig_impl);
      sig_var const sig = sig_impl->_this ();

      CosNotifyChannelAdmin::SupplierAdmin_var admin =
        create_supplieradmin (ec.in (), options);
      ACE_ASSERT (!CORBA::is_nil (admin.in ()));

      if (options.use_filter)
        add_admin_filter (admin.in (), ec.in ());

      Notify_Push_Supplier* supplier =
        create_supplier (admin.in (), ec.in (), client.root_poa (), options);

      CORBA::String_var const ior = orb->object_to_string (sig.in ());
      FILE* const output_file = ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w"));
      ACE_ASSERT (output_file != nullptr);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      ACE_DEBUG ((LM_DEBUG, "Supplier ready...\n"));
      sig_impl->wait_for_startup ();

      ACE_DEBUG ((LM_DEBUG, "Supplier sending %d events...\n", options.event_count));
      for (int i {}; i < options.event_count; ++i)
        if (send_event (*supplier, i) != 0)
          return 1;
      ACE_DEBUG ((LM_DEBUG, "\nSupplier sent %d events.\n", options.event_count));

      sig_impl->wait_for_completion ();
      ACE_OS::unlink (options.ior_output_file);
      ec->destroy ();
      return 0;
    }
  catch (CORBA::Exception const& e)
    {
      e._tao_print_exception ("Error: Supplier: ");
    }

  return 1;
}
