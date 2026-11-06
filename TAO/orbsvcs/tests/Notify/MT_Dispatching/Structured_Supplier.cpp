// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"

#include "tao/ORB_Core.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "Notify_StructuredPushSupplier.h"
#include "goS.h"

#include "Notify_Test_Client.h"

#include <array>
#include <memory>

#include "ace/OS_NS_unistd.h"

// ******************************************************************
// Data Section
// ******************************************************************

constexpr unsigned int max_suppliers { 32 };

struct Supplier_Options
{
  unsigned int supplier_count { 1 };
  int event_count { 1 };
  ACE_TCHAR const* ior_output_file { ACE_TEXT ("supplier.ior") };
};

// ******************************************************************
// Subroutine Section
// ******************************************************************
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
        ACE_Time_Value timeout (0, 100 * 1000);
        orb_->run (timeout);
      }
  }

  void wait_for_completion ()
  {
    while (started_)
      {
        ACE_Time_Value timeout (0, 100 * 1000);
        orb_->run (timeout);
      }
  }

private:
  CORBA::ORB_var orb_;
  bool started_ {};
};

class Supplier_Client : public Notify_Test_Client
{
public:
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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:e:fc:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
  {
    case 'c':
      this->options_.supplier_count = ACE_OS::atoi (get_opts.optarg);
      if (this->options_.supplier_count > max_suppliers)
      {
        this->options_.supplier_count = max_suppliers;
      }
      break;

    case 'e':
      this->options_.event_count = ACE_OS::atoi (get_opts.optarg);
      break;

    case 'o':
      this->options_.ior_output_file = get_opts.optarg;
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("usage: %s -o <iorfile> -e <# of events>\n"),
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


using Suppliers = std::array<TAO_Notify_Tests_StructuredPushSupplier*, max_suppliers>;

static void
SendEvent (Suppliers const& suppliers, unsigned int supplier_count)
{
  CosNotification::StructuredEvent event {};

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
  catch (CORBA::Exception const& e)
  {
    e._tao_print_exception ("Error: ");
  }
}

static void
create_suppliers (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                  Notify_Test_Client& client,
                  unsigned int supplier_count,
                  Suppliers& suppliers)
{
  for (unsigned int i = 0; i < supplier_count; i++)
  {
    ACE_NEW_THROW_EX (suppliers[i],
      TAO_Notify_Tests_StructuredPushSupplier (),
      CORBA::NO_MEMORY ());

    suppliers[i]->init (client.root_poa ());

    suppliers[i]->connect (admin);
  }
}

static void
disconnect_suppliers (Suppliers const& suppliers, unsigned int supplier_count)
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
  sig_i* sig_impl {};
  Suppliers suppliers {};
  try
  {
    Supplier_Client client;
    int status = client.init (argc, argv);
    if (status != 0)
      return 1;

    CosNotifyChannelAdmin::EventChannel_var ec =
      client.create_event_channel ("MyEventChannel", 0);

    CORBA::ORB_ptr orb = client.orb ();

    ACE_NEW_RETURN (sig_impl, sig_i (orb), 1);
    PortableServer::ServantBase_var sig_impl_owner (sig_impl);
    sig_var sig = sig_impl->_this ();

    CORBA::String_var ior =
      orb->object_to_string (sig.in ());

    {
      std::unique_ptr<FILE, int (*) (FILE*)> output_file (
        ACE_OS::fopen (client.options ().ior_output_file, "w"), ACE_OS::fclose);
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
    ACE_ASSERT (!CORBA::is_nil (admin.in ()));
    create_suppliers (admin.in (), client, client.options ().supplier_count, suppliers);

    sig_impl->wait_for_startup ();

    ACE_DEBUG ((LM_DEBUG, "%u supplier(s) sending %d events...\n",
                client.options ().supplier_count, client.options ().event_count));
    for (int i = 0; i < client.options ().event_count; ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "+"));
      SendEvent (suppliers, client.options ().supplier_count);
    }
    ACE_DEBUG ((LM_DEBUG, "\nEach Supplier sent %d events.\n",
                client.options ().event_count));

    sig_impl->wait_for_completion ();

    ACE_OS::unlink (client.options ().ior_output_file);

    disconnect_suppliers (suppliers, client.options ().supplier_count);

    ec->destroy ();

    return 0;
  }
  catch (CORBA::Exception const& e)
  {
    e._tao_print_exception ("Error: ");
  }

  return 1;
}
