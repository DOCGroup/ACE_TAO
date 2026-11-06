// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

#include <memory>

#include "tao/ORB_Core.h"

#include "sigS.h"
#include "Notify_StructuredPushSupplier.h"
#include "Notify_Test_Client.h"

// ******************************************************************
// Data Section
// ******************************************************************

constexpr int max_events { 10 };

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

  void wait_for_startup()
  {
    while (! started_) {
      ACE_Time_Value tv(0, 100 * 1000); // 100ms
      orb_->run(tv);
    }
  }

private:
  CORBA::ORB_var orb_;
  bool started_ {};
};


class Supplier_Client : public Notify_Test_Client
{
public:
  int parse_args (int argc, ACE_TCHAR* argv[]) override;

  ACE_TCHAR const* ior_output_file () const
  {
    return this->ior_output_file_;
  }

private:
  ACE_TCHAR const* ior_output_file_ { ACE_TEXT ("supplier.ior") };
};

int
Supplier_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
  {
    case 'o':
      this->ior_output_file_ = get_opts.optarg;
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
    ec->new_for_suppliers (CosNotifyChannelAdmin::AND_OP, adminid);

  return CosNotifyChannelAdmin::SupplierAdmin::_duplicate (admin.in ());
}


static void
SendEvent (TAO_Notify_Tests_StructuredPushSupplier& supplier, int id)
{
  CosNotification::StructuredEvent event {};

  event.header.fixed_header.event_type.domain_name = CORBA::string_dup ("DOC_TAO");
  event.header.fixed_header.event_type.type_name = CORBA::string_dup ("examples");

  event.filterable_data.length (1);
  event.filterable_data[0].name = CORBA::string_dup ("id");
  event.filterable_data[0].value <<= id;

  try
  {
    supplier.send_event (event);
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

  CosNotification::EventTypeSeq added (1);
  CosNotification::EventTypeSeq removed (1);
  added.length (1);
  removed.length (1);
  added[0].domain_name = CORBA::string_dup ("DOC_TAO");
  added[0].type_name = CORBA::string_dup ("examples");

  removed[0].domain_name = CORBA::string_dup ("*");
  removed[0].type_name = CORBA::string_dup ("*");

  supplier->offer_change (added, removed);
  return supplier;
}


// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
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

          sig_i* sig_impl {};
          ACE_NEW_RETURN (sig_impl, sig_i (orb), 1);
          PortableServer::ServantBase_var owner_transfer(sig_impl);

          CORBA::Object_var object =
            orb->resolve_initial_references ("RootPOA");
          PortableServer::POA_var root_poa =
            PortableServer::POA::_narrow (object.in ());

          PortableServer::ObjectId_var id =
            root_poa->activate_object (sig_impl);

          object = root_poa->id_to_reference (id.in ());

          sig_var sig = sig::_narrow (object.in ());

          CORBA::String_var ior = orb->object_to_string (sig.in ());

          // Output the IOR to the <ior_output_file>
          {
            std::unique_ptr<FILE, int (*) (FILE*)> output_file (
              ACE_OS::fopen (client.ior_output_file (), "w"), ACE_OS::fclose);
            if (output_file)
              ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
          }

          CosNotifyChannelAdmin::SupplierAdmin_var admin =
            create_supplieradmin (ec.in ());
          if (!CORBA::is_nil (admin.in ()))
            {
              TAO_Notify_Tests_StructuredPushSupplier* supplier =
                create_suppliers (admin.in (), client.root_poa ());

              sig_impl->wait_for_startup();

              ACE_DEBUG ((LM_DEBUG,
                          "1 supplier sending %d events...\n", max_events));
              for (int i = 0; i < max_events; ++i)
              {
                ACE_DEBUG ((LM_DEBUG, "+"));
                SendEvent (*supplier, i);
              }
              ACE_DEBUG ((LM_DEBUG,
                          "\nSupplier sent %d events.\n", max_events));

              ACE_OS::unlink (client.ior_output_file ());
              supplier->disconnect ();

              ec->destroy ();
            }
        }
    }
  catch (CORBA::Exception const& e)
    {
      e._tao_print_exception ("Supplier Error: ");
      status = 1;
    }

  return status;
}
