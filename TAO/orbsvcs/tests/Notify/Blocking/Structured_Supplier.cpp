// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"

#include "tao/ORB_Core.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/TimeBaseC.h"
#include "Notify_StructuredPushSupplier.h"
#include "goS.h"

#include "Notify_Test_Client.h"

#include "ace/OS_NS_unistd.h"

#include <memory>

// ******************************************************************
// Data Section
// ******************************************************************

struct Supplier_Options
{
  int max_events { 20 };
  ACE_TCHAR const* ior_output_file { ACE_TEXT ("supplier.ior") };
};

// ******************************************************************
// Subroutine Section
// ******************************************************************
class sig_i : public POA_sig
{
public:
  explicit sig_i (CORBA::ORB_ptr orb)
    : orb_ (orb)
  {
  }

  sig_i (sig_i const&) = delete;
  sig_i (sig_i&&) = delete;
  sig_i& operator= (sig_i const&) = delete;
  sig_i& operator= (sig_i&&) = delete;

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
  CORBA::ORB_ptr orb_ {};
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

  int parse_args (int argc, ACE_TCHAR *argv[]) override;
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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:e:d"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
  {
    case 'e':
      this->options_.max_events = ACE_OS::atoi (get_opts.optarg);
      break;

    case 'o':
      this->options_.ior_output_file = get_opts.optarg;
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("usage: %s -o <iorfile> -e <# of events> -d\n"),
                         argv[0]),
                        -1);
    }

  // Indicates successful parsing of the command line
  return 0;
}

static CosNotifyChannelAdmin::SupplierAdmin_ptr
create_supplieradmin (CosNotifyChannelAdmin::EventChannel_ptr ec)
{
  CosNotifyChannelAdmin::AdminID adminid {};
  CosNotifyChannelAdmin::SupplierAdmin_var admin =
    ec->new_for_suppliers (CosNotifyChannelAdmin::AND_OP,
                           adminid);

  return CosNotifyChannelAdmin::SupplierAdmin::_duplicate (admin.in ());
}

static void
SendEvent (TAO_Notify_Tests_StructuredPushSupplier* supplier, int id)
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
create_suppliers (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                  PortableServer::POA_ptr poa)
{
  TAO_Notify_Tests_StructuredPushSupplier* supplier {};
  ACE_NEW_THROW_EX (supplier,
                    TAO_Notify_Tests_StructuredPushSupplier (),
                    CORBA::NO_MEMORY ());

  supplier->init (poa);
  supplier->connect (admin);

  // The Root POA owns the servant; this is a borrowed pointer.
  return supplier;
}

// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int status {};
  try
    {
      Supplier_Client client;
      status = client.init (argc, argv);

      if (status == 0)
        {
          CosNotifyChannelAdmin::EventChannel_var ec =
            client.create_event_channel ("MyEventChannel", 0);

          CORBA::ORB_ptr orb = client.orb ();

          // The Root POA takes ownership when it activates the servant.
          sig_i* sig_impl {};
          ACE_NEW_THROW_EX (sig_impl, sig_i (orb), CORBA::NO_MEMORY ());
          PortableServer::ServantBase_var owner_transfer (sig_impl);
          sig_var sig = sig_impl->_this ();

          CORBA::String_var ior = orb->object_to_string (sig.in ());

          {
            std::unique_ptr<FILE, int (*) (FILE *)> output_file (
              ACE_OS::fopen (client.options ().ior_output_file, ACE_TEXT ("w")),
              ACE_OS::fclose);
            if (!output_file)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("Cannot open output file %s for writing IOR: %C"),
                                 client.options ().ior_output_file,
                                 ior.in ()),
                                1);
            ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
          }

          CosNotifyChannelAdmin::SupplierAdmin_var admin =
            create_supplieradmin (ec.in ());
          if (!CORBA::is_nil (admin.in ()))
            {
              TAO_Notify_Tests_StructuredPushSupplier* const supplier =
                create_suppliers (admin.in (), client.root_poa ());

              sig_impl->wait_for_startup ();

              ACE_DEBUG ((LM_DEBUG,
                          "1 supplier sending %d events...\n",
                          client.options ().max_events));
              for (int i {}; i < client.options ().max_events; ++i)
                {
                  ACE_DEBUG ((LM_DEBUG, "+"));
                  SendEvent (supplier, i);
                }
              ACE_DEBUG ((LM_DEBUG,
                          "\nSupplier sent %d events.\n",
                          client.options ().max_events));

              sig_impl->wait_for_completion ();

              ACE_OS::unlink (client.options ().ior_output_file);

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
