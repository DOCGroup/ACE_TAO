// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

#include "tao/ORB_Core.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/TimeBaseC.h"

#include "goS.h"
#include "Notify_StructuredPushSupplier.h"
#include "Notify_Test_Client.h"


// ******************************************************************
// Data Section
// ******************************************************************

static TAO_Notify_Tests_StructuredPushSupplier* supplier_1 = 0;
static int max_events = 10;
static const ACE_TCHAR *ior_output_file = ACE_TEXT ("supplier.ior");

// ******************************************************************
// Subroutine Section
// ******************************************************************
class sig_i : public POA_sig
{
public:
  sig_i (CORBA::ORB_ptr orb)
    : orb_ (orb),
      started_ (false)
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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:e:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
  {
    case 'e':
      max_events = ACE_OS::atoi (get_opts.optarg);
      break;

    case 'o':
      ior_output_file = get_opts.optarg;
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
        "usage:  %s "
        "-o <iorfile> -e <# of events> -d"
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
SendEvent (int id)
{
  CosNotification::StructuredEvent event;

  event.header.fixed_header.event_type.domain_name = CORBA::string_dup ("DOC_TAO");
  event.header.fixed_header.event_type.type_name = CORBA::string_dup ("examples");

  event.filterable_data.length (1);
  event.filterable_data[0].name = CORBA::string_dup ("id");
  event.filterable_data[0].value <<= id;

  try
  {
    supplier_1->send_event (event);
  }
  catch (const CORBA::Exception& e)
  {
    e._tao_print_exception ("Error: ");
  }
}

static void
create_suppliers (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                  PortableServer::POA_ptr poa)
{
  // startup the supplier
  ACE_NEW_THROW_EX (supplier_1,
    TAO_Notify_Tests_StructuredPushSupplier (),
    CORBA::NO_MEMORY ());

  supplier_1->init (poa);
  supplier_1->connect (admin);

  CosNotification::EventTypeSeq added (1);
  CosNotification::EventTypeSeq removed (1);
  added.length (1);
  removed.length (1);
  added[0].domain_name = CORBA::string_dup ("DOC_TAO");
  added[0].type_name = CORBA::string_dup ("examples");

  removed[0].domain_name = CORBA::string_dup ("*");
  removed[0].type_name = CORBA::string_dup ("*");

  supplier_1->offer_change (added, removed);
}


// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int status = 0;
  ACE_Auto_Ptr< sig_i > sig_impl;
  try
    {
      Supplier_Client client;
      status = client.init (argc, argv);

      if (status == 0)
        {
          CosNotifyChannelAdmin::EventChannel_var ec =
            client.create_event_channel ("MyEventChannel", 0);

          CORBA::ORB_ptr orb = client.orb ();

          // Activate the signaler with the POA
          sig_impl.reset ( new sig_i ( orb ) );
          sig_var sig = sig_impl->_this ();

          CORBA::String_var ior =
            orb->object_to_string (sig.in ());

          // If the ior_output_file exists, output the ior to it
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
          if (!CORBA::is_nil (admin.in ()))
            {
              create_suppliers (admin.in (), client.root_poa ());

              sig_impl->wait_for_startup ();

              ACE_DEBUG ((LM_DEBUG,
                          "1 supplier sending %d events...\n", max_events));
              for (int i = 0; i < max_events; ++i)
              {
                ACE_DEBUG ((LM_DEBUG, "+"));
                SendEvent (i);
              }
              ACE_DEBUG ((LM_DEBUG,
                          "\nSupplier sent %d events.\n", max_events));

              sig_impl->wait_for_completion ();

              ACE_OS::unlink (ior_output_file);

              supplier_1->disconnect ();

              ec->destroy ();
            }
        }
    }
  catch (const CORBA::Exception& e)
    {
      e._tao_print_exception ("Error: ");
      status = 1;
    }

  return status;
}
