// $Id$

#include "Notify_Structured_Push_Consumer.h"
#include "goC.h"
#include "Notify_Test_Client.h"

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_strings.h"

static const ACE_TCHAR *ior = ACE_TEXT ("file://supplier.ior");
static CORBA::Short discard_policy = CosNotification::FifoOrder;
CORBA::Long max_events_per_consumer = 4;
static Notify_Structured_Push_Consumer* consumer_1;

class Consumer_Client : public Notify_Test_Client
{
public:
  virtual int parse_args (int argc, ACE_TCHAR *argv[]);
};


int
Consumer_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:e:d:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;

      case 'd':
      {
        const ACE_TCHAR *discard = get_opts.optarg;
        if (ACE_OS::strcasecmp (discard, ACE_TEXT ("fifo")) == 0)
          {
            discard_policy = CosNotification::FifoOrder;
          }
        else if (ACE_OS::strcasecmp (discard, ACE_TEXT ("priority")) == 0)
          {
            discard_policy = CosNotification::PriorityOrder;
          }
        else if (ACE_OS::strcasecmp (discard, ACE_TEXT ("lifo")) == 0)
          {
            discard_policy = CosNotification::LifoOrder;
          }
        else if (ACE_OS::strcasecmp (discard, ACE_TEXT ("deadline")) == 0)
          {
            discard_policy = CosNotification::DeadlineOrder;
#if !defined (ACE_HAS_TIMED_MESSAGE_BLOCKS)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "This order policy requires timed message "
                               "blocks.\nPlease #define "
                               "ACE_HAS_TIMED_MESSAGE_BLOCKS in your "
                               "config.h\n"), -1);
#endif
          }
        else
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unknown discard policy: %s\n",
                               discard_policy),
                              -1);
          }
        break;
      }
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-d <fifo|priority|lifo|deadline> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}


static CosNotifyChannelAdmin::ConsumerAdmin_ptr
create_consumeradmin (CosNotifyChannelAdmin::EventChannel_ptr ec)
{
  CosNotifyChannelAdmin::AdminID adminid = 0;
  CosNotifyChannelAdmin::ConsumerAdmin_var admin =
    ec->new_for_consumers (CosNotifyChannelAdmin::OR_OP,
                           adminid);


  return CosNotifyChannelAdmin::ConsumerAdmin::_duplicate (admin.in ());
}


static void
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client* client)
{
  ACE_DEBUG ((LM_DEBUG, "Max Events per Consumer = %d...\n", max_events_per_consumer));

  // startup the consumer
  ACE_NEW_THROW_EX (consumer_1,
                    Notify_Structured_Push_Consumer (
                                          "consumer1",
                                          discard_policy,
                                          max_events_per_consumer,
                                          *client),
                    CORBA::NO_MEMORY ());
  consumer_1->init (client->root_poa ());

  consumer_1->_connect (admin);
}

// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int status = 0;
  try
    {
      Consumer_Client client;

      status = client.init (argc, argv);

      if (status == 0)
        {
          CosNotifyChannelAdmin::EventChannel_var ec =
            client.create_event_channel ("MyEventChannel", 1);

          CORBA::ORB_ptr orb = client.orb ();
          CORBA::Object_var object =
                              orb->string_to_object (ior);

          sig_var sig = sig::_narrow (object.in ());

          if (CORBA::is_nil (sig.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Object reference <%s> is nil\n",
                                 ior),
                                1);
            }

          CosNotifyChannelAdmin::ConsumerAdmin_var admin =
            create_consumeradmin (ec.in ());

          if (!CORBA::is_nil (admin.in ()))
            {
              create_consumers (admin.in (), &client);
              // Tell the supplier to go
              sig->go ();

              ACE_DEBUG((LM_DEBUG, "Consumer waiting for events...\n"));

              client.ORB_run( );

              ACE_DEBUG((LM_DEBUG, "Consumer done.\n"));
              consumer_1->disconnect();

              sig->done ();
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
