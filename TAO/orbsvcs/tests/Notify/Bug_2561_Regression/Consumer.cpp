// ******************************************************************
// Include Section
// ******************************************************************

#include "Notify_Push_Consumer.h"
#include "goC.h"
#include "Notify_Test_Client.h"

#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"

// ******************************************************************
// Data Section
// ******************************************************************

struct Consumer_Options
{
  ACE_TCHAR const* ior { ACE_TEXT ("file://supplier.ior") };
  bool wait_for_supplier {};
};

// ******************************************************************
// Subroutine Section
// ******************************************************************

class Consumer_Client : public Notify_Test_Client
{
public:
  Consumer_Client () = default;
  Consumer_Client (Consumer_Client const&) = delete;
  Consumer_Client (Consumer_Client&&) = delete;
  Consumer_Client& operator= (Consumer_Client const&) = delete;
  Consumer_Client& operator= (Consumer_Client&&) = delete;

  int parse_args (int argc, ACE_TCHAR* argv[]) override;
  Consumer_Options const& options () const
  {
    return this->options_;
  }

private:
  Consumer_Options options_;
};

int
Consumer_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter argcon (argc, argv);
  ACE_Get_Opt get_opts (argcon.get_argc (),
                        argcon.get_TCHAR_argv (), ACE_TEXT ("gk:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'g':
        this->options_.wait_for_supplier = true;
        break;
      case 'k':
        this->options_.ior = get_opts.optarg;
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -g -k <ior>\n"),
                           argv[0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

static CosNotifyChannelAdmin::ConsumerAdmin_ptr
create_consumeradmin (CosNotifyChannelAdmin::EventChannel_ptr ec)
{
  CosNotifyChannelAdmin::AdminID adminid {};
  CosNotifyChannelAdmin::ConsumerAdmin_var admin =
    ec->new_for_consumers (CosNotifyChannelAdmin::OR_OP,
                           adminid);

  return CosNotifyChannelAdmin::ConsumerAdmin::_duplicate (admin.in ());
}

static Notify_Sequence_Push_Consumer*
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client& client)
{
  Notify_Sequence_Push_Consumer* consumer {};
  ACE_NEW_THROW_EX (consumer,
                    Notify_Sequence_Push_Consumer (client),
                    CORBA::NO_MEMORY ());
  consumer->init (client.root_poa ());

  consumer->_connect (admin);
  return consumer;
}

// ******************************************************************
// Main Section
// ******************************************************************

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int status {};
  try
    {
      Consumer_Client client;
      ACE_Argv_Type_Converter argcon (argc, argv);
      status = client.init (argcon.get_argc (), argcon.get_TCHAR_argv ());

      if (status == 0)
        {
          CosNotifyChannelAdmin::EventChannel_var ec =
            client.create_event_channel ("MyEventChannel", 1);

          CORBA::Object_var object =
            client.orb ()->string_to_object (client.options ().ior);

          sig_var sig = sig::_narrow (object.in ());

          if (CORBA::is_nil (sig.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("Object reference <%s> is nil\n"),
                                 client.options ().ior),
                                1);
            }

          CosNotifyChannelAdmin::ConsumerAdmin_var admin =
            create_consumeradmin (ec.in ());

          if (!CORBA::is_nil (admin.in ()))
            {
              Notify_Sequence_Push_Consumer* consumer =
                create_consumers (admin.in (), client);
              if (client.options ().wait_for_supplier)
                {
                  // Tell the supplier to go
                  sig->go ();
                }

              ACE_DEBUG ((LM_DEBUG, "Consumer waiting for events...\n"));

              client.ORB_run ();

              ACE_DEBUG ((LM_DEBUG, "Consumer done.\n"));
              consumer->disconnect ();

              if (client.options ().wait_for_supplier)
                {
                  sig->done ();
                }
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
