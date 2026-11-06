#include "Notify_Structured_Push_Consumer.h"
#include "goC.h"
#include "Notify_Test_Client.h"

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_strings.h"

struct Consumer_Options
{
  ACE_TCHAR const* ior { ACE_TEXT ("file://supplier.ior") };
  CORBA::Short discard_policy { CosNotification::FifoOrder };
  CORBA::Long max_events_per_consumer { 4 };
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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:e:d:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        this->options_.ior = get_opts.optarg;
        break;

      case 'e':
        this->options_.max_events_per_consumer = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'd':
      {
        ACE_TCHAR const* discard = get_opts.optarg;
        if (ACE_OS::strcasecmp (discard, ACE_TEXT ("fifo")) == 0)
          {
            this->options_.discard_policy = CosNotification::FifoOrder;
          }
        else if (ACE_OS::strcasecmp (discard, ACE_TEXT ("priority")) == 0)
          {
            this->options_.discard_policy = CosNotification::PriorityOrder;
          }
        else if (ACE_OS::strcasecmp (discard, ACE_TEXT ("lifo")) == 0)
          {
            this->options_.discard_policy = CosNotification::LifoOrder;
          }
        else if (ACE_OS::strcasecmp (discard, ACE_TEXT ("deadline")) == 0)
          {
            this->options_.discard_policy = CosNotification::DeadlineOrder;
#if !defined (ACE_HAS_TIMED_MESSAGE_BLOCKS)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("This order policy requires timed message "
                                         "blocks.\nPlease #define "
                                         "ACE_HAS_TIMED_MESSAGE_BLOCKS in your "
                                         "config.h\n")), -1);
#endif
          }
        else
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Unknown discard policy: %s\n"),
                               discard),
                              -1);
          }
        break;
      }
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior> -e <expected events> "
                                     "-d <fifo|priority|lifo|deadline>\n"),
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
  ACE_DEBUG ((LM_DEBUG,
              "Max Events per Consumer = %d...\n",
              options.max_events_per_consumer));

  Notify_Structured_Push_Consumer* consumer {};
  ACE_NEW_THROW_EX (consumer,
                    Notify_Structured_Push_Consumer ("consumer1",
                                                     options.discard_policy,
                                                     options.max_events_per_consumer,
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

      if (status == 0)
        {
          CosNotifyChannelAdmin::EventChannel_var ec =
            client.create_event_channel ("MyEventChannel", 1);

          CORBA::ORB_ptr orb = client.orb ();
          CORBA::Object_var object = orb->string_to_object (client.options ().ior);

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
              Notify_Structured_Push_Consumer* consumer =
                create_consumers (admin.in (), &client, client.options ());
              // Tell the supplier to go
              sig->go ();

              ACE_DEBUG ((LM_DEBUG, "Consumer waiting for events...\n"));

              client.ORB_run ();

              ACE_DEBUG ((LM_DEBUG, "Consumer done.\n"));
              consumer->disconnect ();

              sig->done ();
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
