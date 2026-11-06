// ******************************************************************
// Include Section
// ******************************************************************

#include <memory>

#include "ace/Get_Opt.h"

#include "tao/ORB_Core.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/TimeBaseC.h"
#include "Notify_StructuredPushSupplier.h"
#include "goS.h"

#include "Notify_Test_Client.h"

// ******************************************************************
// Data Section
// ******************************************************************

struct Supplier_Options
{
  int max_events {20};
  ACE_TCHAR const* ior_output_file {ACE_TEXT ("supplier.ior")};
  ACE_TCHAR const* notify2ior {ACE_TEXT ("notify2.ior")};
};

// ******************************************************************
// Subroutine Section
// ******************************************************************
class sig_i : public POA_sig
{
public:
  explicit sig_i (CORBA::ORB_ptr orb)
    : orb_(orb)
    , started_(false)
  {
  }

  void go () override
  {
    started_ = true;
  }

  void done () override
  {
    started_ = false;
  }

  void wait_for_startup ()
  {
    while (!started_)
      {
        ACE_Time_Value tv (0, 100 * 1000); // 100ms
        orb_->run (tv);
      }
  }

  void wait_for_completion ()
  {
    while (started_)
      {
        ACE_Time_Value tv (0, 100 * 1000); // 100ms
        orb_->run (tv);
      }
  }

private:
  sig_i () = delete;
  sig_i (sig_i const&) = delete;
  sig_i (sig_i&&) = delete;
  sig_i& operator= (sig_i const&) = delete;
  sig_i& operator= (sig_i&&) = delete;

  CORBA::ORB_ptr orb_;
  bool started_ {};
};

class Supplier_Client : public Notify_Test_Client
{
public:
  Supplier_Client () = default;
  Supplier_Client (Supplier_Client const&) = delete;
  Supplier_Client (Supplier_Client&&) = delete;
  Supplier_Client& operator= (Supplier_Client const&) = delete;
  Supplier_Client& operator= (Supplier_Client&&) = delete;

  int parse_args (int argc, ACE_TCHAR* argv[]) override;
  Supplier_Options const& options () const;

private:
  Supplier_Options options_ {};
};

int
Supplier_Client::parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:e:d:f:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'e':
        this->options_.max_events = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'o':
        this->options_.ior_output_file = get_opts.optarg;
        break;

      case 'f':
        this->options_.notify2ior = get_opts.optarg;
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s -o <iorfile> -e <# of events> -f <notify ior>\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

Supplier_Options const&
Supplier_Client::options () const
{
  return this->options_;
}

static CosNotifyChannelAdmin::SupplierAdmin_ptr
create_supplieradmin (CosNotifyChannelAdmin::EventChannel_ptr const ec)
{
  CosNotifyChannelAdmin::AdminID adminid {};
  CosNotifyChannelAdmin::SupplierAdmin_var const admin =
    ec->new_for_suppliers (CosNotifyChannelAdmin::AND_OP, adminid);

  return CosNotifyChannelAdmin::SupplierAdmin::_duplicate (admin.in ());
}

static void
send_event (TAO_Notify_Tests_StructuredPushSupplier* supplier, int id)
{
  CosNotification::StructuredEvent event {};

  event.header.fixed_header.event_type.domain_name = CORBA::string_dup ("");
  event.header.fixed_header.event_type.type_name = CORBA::string_dup ("");
  event.header.fixed_header.event_name = CORBA::string_dup ("blocking-test");

  event.filterable_data.length (1);
  event.filterable_data[0].name = CORBA::string_dup ("id");
  event.filterable_data[0].value <<= id;

  try
  {
    supplier->send_event (event);
  }
  catch (CORBA::Exception const& e)
  {
    e._tao_print_exception ("Error: ");
  }
}

static TAO_Notify_Tests_StructuredPushSupplier*
create_suppliers (CosNotifyChannelAdmin::SupplierAdmin_ptr const admin,
                  PortableServer::POA_ptr const poa)
{
  // The POA takes ownership of the servant after activation in connect().
  TAO_Notify_Tests_StructuredPushSupplier* supplier {};
  ACE_NEW_THROW_EX (supplier,
    TAO_Notify_Tests_StructuredPushSupplier (),
    CORBA::NO_MEMORY ());

  supplier->init (poa);

  supplier->connect (admin);
  // The returned servant pointer is borrowed; its reference is held by the POA.
  return supplier;
}

// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int status {};
  std::unique_ptr<sig_i> sig_impl;
  try
    {
      Supplier_Client client;
      status = client.init (argc, argv);

      if (status == 0)
        {
          static char const* const ec_name = "MyEventChannel";
          CosNotifyChannelAdmin::EventChannel_var ec = client.create_event_channel (ec_name, false);

          Supplier_Options const& options = client.options ();
          constexpr int max_wait = 20;
          int count {};
          while (ACE_OS::access (ACE_TEXT_ALWAYS_CHAR (options.notify2ior), R_OK) == -1 && count < max_wait)
            {
              ACE_OS::sleep (1);
              ++count;
            }
          if (count >= max_wait)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "ERROR: Timed out waiting for the second notify service\n"),
                              1);
          ACE_OS::sleep (2);

          // Due to the way that connections are handled, we need to
          // perform the new event channel creation twice to flush out
          // the connection to the first notification service.
          try
            {
              ec = client.create_event_channel (ec_name, false);
            }
          catch (CORBA::TRANSIENT const&)
            {
            }
          catch (CORBA::COMM_FAILURE const&)
            {
            }

          ec = client.create_event_channel (ec_name, false);

          CORBA::ORB_ptr const orb = client.orb ();

          // Activate the signaler with the POA.
          sig_impl = std::make_unique<sig_i> (orb);
          sig_var const sig = sig_impl->_this ();

          CORBA::String_var const ior = orb->object_to_string (sig.in ());

          {
            std::unique_ptr<FILE, int (*) (FILE *)> output_file (
              ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")), ACE_OS::fclose);
            if (!output_file)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("Cannot open output file %s for writing IOR: %C"),
                                 options.ior_output_file,
                                 ior.in ()),
                                1);
            ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
          }

          CosNotifyChannelAdmin::SupplierAdmin_var const admin = create_supplieradmin (ec.in ());
          if (!CORBA::is_nil (admin.in ()))
            {
              TAO_Notify_Tests_StructuredPushSupplier* const supplier =
                create_suppliers (admin.in (), client.root_poa ());

              sig_impl->wait_for_startup ();

              ACE_DEBUG ((LM_DEBUG, "1 supplier sending %d events...\n", options.max_events));
              for (int i = 0; i < options.max_events; ++i)
                {
                  ACE_DEBUG ((LM_DEBUG, "+"));
                  send_event (supplier, i);
                }
              ACE_DEBUG ((LM_DEBUG, "\nSupplier sent %d events.\n", options.max_events));

              sig_impl->wait_for_completion ();

              ACE_OS::unlink (options.ior_output_file);

              supplier->disconnect ();

              ec->destroy ();
            }
        }
    }
  catch (CORBA::Exception const& e)
    {
      e._tao_print_exception ("Error: ");
      status = 1;
    }

  return status;
}
