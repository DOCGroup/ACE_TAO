// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Argv_Type_Converter.h"

#include "tao/ORB_Core.h"

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/TimeBaseC.h"
#include "Notify_SequencePushSupplier.h"
#include "goS.h"

#include "Notify_Test_Client.h"


// ******************************************************************
// Data Section
// ******************************************************************

static TAO_Notify_Tests_SequencePushSupplier* supplier_1 = 0;
static const ACE_TCHAR* ior_output_file = ACE_TEXT ("supplier.ior");

// ******************************************************************
// Subroutine Section
// ******************************************************************

// The supplier will not start sending events until the
// go() operation is invoked.
class sig_i : public POA_sig
{
public:
  sig_i (CORBA::ORB_ptr orb)
    : orb_ (orb)
    , started_ (false)
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
    while (! started_)
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
  virtual int parse_args (int argc, ACE_TCHAR* argv[]);
};


int
Supplier_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter argcon (argc, argv);
  ACE_Get_Opt get_opts (argcon.get_argc (),
                        argcon.get_TCHAR_argv (), ACE_TEXT ("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
  {
    case 'o':
      ior_output_file = get_opts.optarg;
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
        "usage: %s "
        "-o <iorfile> "
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
create_suppliers (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                  PortableServer::POA_ptr poa)
{
  ACE_NEW_THROW_EX (supplier_1,
    TAO_Notify_Tests_SequencePushSupplier (),
    CORBA::NO_MEMORY ());

  supplier_1->init (poa);

  supplier_1->connect (admin);
}


// ******************************************************************
// Main Section
// ******************************************************************

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int status = 0;
  ACE_Auto_Ptr<sig_i> sig_impl;
  try
    {
      Supplier_Client client;
      ACE_Argv_Type_Converter argcon (argc, argv);
      status = client.init (argcon.get_argc (), argcon.get_TCHAR_argv ());

      if (status == 0)
        {
          CosNotifyChannelAdmin::EventChannel_var ec =
            client.create_event_channel ("MyEventChannel", 0);

          sig_impl.reset (new sig_i (client.orb ()));
          sig_var sig = sig_impl->_this ();

          // If the ior_output_file exists, output the ior to it
          if (ior_output_file != 0)
            {
              CORBA::String_var ior =
                client.orb ()->object_to_string (sig.in ());

              FILE *output_file= ACE_OS::fopen (ior_output_file, ACE_TEXT ("w"));
              ACE_ASSERT (output_file != 0);
              ACE_OS::fprintf (output_file, "%s", ior.in ());
              ACE_OS::fclose (output_file);
            }

          CosNotifyChannelAdmin::SupplierAdmin_var admin =
            create_supplieradmin (ec.in ());
          ACE_ASSERT (!CORBA::is_nil (admin.in ()));

          create_suppliers (admin.in (), client.root_poa ());

          sig_impl->wait_for_startup ();

          // If this is changed, you should update max_events
          // found in Notify_Push_Consumer.cpp
          int batch_size = 4;
          int num_events = 80 / batch_size;
          ACE_DEBUG ((LM_DEBUG,
                      "1 supplier sending %d batches of %d events...\n",
                      num_events, batch_size));
          CosNotification::EventBatch events;
          events.length(batch_size);
          for (int i = 0; i < num_events; ++i)
            {
              supplier_1->send_events (events);
            }
          ACE_DEBUG ((LM_DEBUG, "\nSupplier sent %d events.\n", num_events));

          sig_impl->wait_for_completion ();

          ACE_OS::unlink (ior_output_file);

          supplier_1->disconnect ();

          ec->destroy ();
        }
    }
  catch (const CORBA::Exception& e)
  {
    e._tao_print_exception ("Error: ");
    status = 1;
  }

  return status;
}
