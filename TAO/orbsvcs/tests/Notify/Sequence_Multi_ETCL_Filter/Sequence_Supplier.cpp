// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"

#include "tao/ORB_Core.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/TimeBaseC.h"
#include "Notify_SequencePushSupplier.h"
#include "goS.h"

#include "Notify_Test_Client.h"

#include "ace/OS_NS_unistd.h"

#include <memory>

// ******************************************************************
// Data Section
// ******************************************************************
constexpr int events_per_batch { 16 };

struct Supplier_Options
{
  int num_batches { 6 };
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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:e:d"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
  {
    case 'e':
      this->options_.num_batches = ACE_OS::atoi (get_opts.optarg);
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
  CosNotifyChannelAdmin::AdminID adminid {};
  CosNotifyChannelAdmin::SupplierAdmin_var admin =
    ec->new_for_suppliers (CosNotifyChannelAdmin::AND_OP,
    adminid);


  return CosNotifyChannelAdmin::SupplierAdmin::_duplicate (admin.in ());
}


static void
SendEvents (TAO_Notify_Tests_SequencePushSupplier& supplier,
            int id,
            int num_batches)
{
  static char const* const types[] = { "good", "bad", "ugly" };
  CosNotification::EventBatch events {};

  CosNotification::StructuredEvent event {};

  event.header.fixed_header.event_type.domain_name =
    CORBA::string_dup ("Orbix 2000 Demos");
  event.header.fixed_header.event_type.type_name =
    CORBA::string_dup ("Sequence Notification Push Demo Event");

  event.header.fixed_header.event_name = CORBA::string_dup ("test event");

  event.header.variable_header.length (2);
  event.header.variable_header[0].name =
    CORBA::string_dup (CosNotification::Priority);
  event.header.variable_header[1].name =
    CORBA::string_dup (CosNotification::Timeout);

  event.filterable_data.length (3);
  event.filterable_data[0].name = CORBA::string_dup ("objectId");
  event.filterable_data[1].name = CORBA::string_dup ("type");
  event.filterable_data[2].name = CORBA::string_dup ("enum");

  events.length (events_per_batch);

  for (int z = 0; z < events_per_batch; ++z)
  {
    event.header.variable_header[0].value <<=
      static_cast<CORBA::Short> (id > num_batches / 2 ? -id : id);

    event.filterable_data[0].value <<= static_cast<CORBA::Long> (z);
    event.filterable_data[1].value <<= types[2 - (id % 3)];
    event.filterable_data[2].value <<=
      static_cast<CORBA::Long> (id > num_batches / 2 ? -id : id);

    events[z] = event;
  }

  try
  {
    supplier.send_events (events);
  }
  catch (CORBA::Exception const& e)
  {
    e._tao_print_exception ("Error: Supplier exception: ");
  }
}

static TAO_Notify_Tests_SequencePushSupplier*
create_suppliers (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                  PortableServer::POA_ptr poa)
{
  TAO_Notify_Tests_SequencePushSupplier* supplier {};
  ACE_NEW_THROW_EX (supplier,
                    TAO_Notify_Tests_SequencePushSupplier (),
                    CORBA::NO_MEMORY ());

  supplier->init (poa);

  supplier->connect (admin);
  return supplier;
}


// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  sig_i* sig_impl {};
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
                             ACE_TEXT ("Cannot open output file %s for writing IOR: %s"),
                             client.options ().ior_output_file,
                             ior.in ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      CosNotifyChannelAdmin::SupplierAdmin_var admin =
        create_supplieradmin (ec.in ());
      if (!CORBA::is_nil (admin.in ()))
      {
        TAO_Notify_Tests_SequencePushSupplier* supplier =
          create_suppliers (admin.in (), client.root_poa ());

        sig_impl->wait_for_startup ();

        ACE_DEBUG ((LM_DEBUG, "1 supplier sending %d batches of %d events...\n",
                    client.options ().num_batches, events_per_batch));
        for (int i = 0; i < client.options ().num_batches; ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "+"));
          SendEvents (*supplier, i, client.options ().num_batches);
        }
        ACE_DEBUG ((LM_DEBUG, "\nSupplier waiting for consumer...\n"));

        sig_impl->wait_for_completion ();

        ACE_DEBUG ((LM_DEBUG, "\nSupplier done.\n"));

        ACE_OS::unlink (client.options ().ior_output_file);

        supplier->disconnect ();
        ec->destroy ();
      }
    }
  }
  catch (CORBA::Exception const& e)
  {
    e._tao_print_exception ("Error: Supplier exception: ");
    status = 1;
  }

  return status;
}
