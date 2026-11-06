// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/TimeBaseC.h"
#include "Notify_Structured_Push_Consumer.h"
#include "goC.h"

#include "Notify_Test_Client.h"

// ******************************************************************
// Data Section
// ******************************************************************

struct Consumer_Options
{
  ACE_TCHAR const* ior { ACE_TEXT ("file://supplier.ior") };
  // TimeT is in 10ths of a microsecond.
  TimeBase::TimeT blocking_timeout { 2 * 1000 * 1000 * 10 };
  // Must match the number sent by supplier.
  unsigned int expected { 20 };
};

class Consumer_Client : public Notify_Test_Client
{
public:
  Consumer_Client () = default;
  Consumer_Client (Consumer_Client const&) = delete;
  Consumer_Client (Consumer_Client&&) = delete;
  Consumer_Client& operator= (Consumer_Client const&) = delete;
  Consumer_Client& operator= (Consumer_Client&&) = delete;

  int parse_args (int argc, ACE_TCHAR *argv[]) override;
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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:e:t:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
  {
    case 'k':
      this->options_.ior = get_opts.optarg;
      break;

    case 'e':
      this->options_.expected = ACE_OS::atoi (get_opts.optarg);
      break;

    case 't':
      this->options_.blocking_timeout = ACE_OS::atoi (get_opts.optarg);
      this->options_.blocking_timeout *= 10 * 1000;
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("usage: %s -k <ior> -e <expected events> "
                                   "-t <relative blocking timeout milliseconds>\n"),
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

static Notify_Structured_Push_Consumer*
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client* client,
                  Consumer_Options const& options)
{
  // The Root POA owns the servant after init and _connect.
  Notify_Structured_Push_Consumer* consumer {};
  ACE_NEW_THROW_EX (consumer,
                    Notify_Structured_Push_Consumer ("consumer1",
                                                     options.blocking_timeout,
                                                     options.expected,
                                                     *client),
                    CORBA::NO_MEMORY ());
  consumer->init (client->root_poa ());

  consumer->_connect (admin);
  return consumer;
}

// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int status {};
  try
    {
      Consumer_Client client;

      status = client.init (argc, argv);
      if (status != 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Error: Client init failed.\n"), 1);

      CosNotifyChannelAdmin::EventChannel_var ec =
        client.create_event_channel ("MyEventChannel", 1);

      CORBA::ORB_ptr orb = client.orb ();
      CORBA::Object_var object =
        orb->string_to_object (client.options ().ior);

      sig_var sig = sig::_narrow (object.in ());

      if (CORBA::is_nil (sig.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Error: Narrow failed.\n"), 1);

      CosNotifyChannelAdmin::ConsumerAdmin_var admin =
        create_consumeradmin (ec.in ());

      if (CORBA::is_nil (admin.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Error: nil ConsumerAdmin.\n"), 1);

      Notify_Structured_Push_Consumer* consumer =
        create_consumers (admin.in (), &client, client.options ());

      sig->go ();

      ACE_DEBUG ((LM_DEBUG, "\nConsumer waiting for events...\n"));

      client.ORB_run ();

      ACE_DEBUG ((LM_DEBUG, "Consumer done.\n"));
      consumer->disconnect ();

      sig->done ();
    }
  catch (CORBA::Exception const& e)
    {
      e._tao_print_exception ("Error: ");
      status = 1;
    }

  return status;
}
