// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"

#include "Notify_Structured_Push_Consumer.h"
#include "MonitorTestInterfaceC.h"

#include "Notify_Test_Client.h"

// ******************************************************************
// Data Section
// ******************************************************************

static const ACE_TCHAR *ior = ACE_TEXT ("file://test_monitor.ior");
static const ACE_TCHAR *ready_output_file = ACE_TEXT ("ready.txt");
static unsigned int expected = 2000;
static unsigned int delay_period = 5;
static unsigned int delay_count = 0;
static Notify_Structured_Push_Consumer* consumer_1 = 0;

class Consumer_Client : public Notify_Test_Client
{
public:
  virtual int parse_args (int argc, ACE_TCHAR *argv[]);
};


int
Consumer_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:e:p:d:o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
        case 'k':
          ior = get_opts.optarg;
          break;

        case 'e':
          expected = ACE_OS::atoi (get_opts.optarg);
          break;

        case 'p':
          delay_period = ACE_OS::atoi (get_opts.optarg);
          break;

        case 'd':
          delay_count = ACE_OS::atoi (get_opts.optarg);
          break;

        case 'o':
          ready_output_file = get_opts.optarg;
          break;

        default:
          ACE_ERROR_RETURN ((LM_ERROR,
            "usage:  %s "
            "-k <ior> "
            "-e <expected events> "
            "-d <delay every 'n' seconds> "
            "-p <how many seconds to delay> "
            "-o <readyfile> -e <# of events> "
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
    ec->new_for_consumers (CosNotifyChannelAdmin::OR_OP, adminid);

  return CosNotifyChannelAdmin::ConsumerAdmin::_duplicate (admin.in ());
}


static void
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client* client)
{
  // startup the consumer
  ACE_NEW_THROW_EX (consumer_1,
                    Notify_Structured_Push_Consumer ("consumer1",
                                                     expected,
                                                     *client),
                    CORBA::NO_MEMORY ());
  consumer_1->set_delay_parameters (delay_count, delay_period);
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

      if (status != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error: Client init failed.\n"),
                            1);
        }

      CosNotifyChannelAdmin::EventChannel_var ec =
        client.create_event_channel ("MyEventChannel", 0);

#ifdef TEST_QOS_MAX_QUEUE_LENGTH
      ACE_OS::printf ("%s: setting max queue length to 1000\n", argv[0]);
      CosNotification::AdminProperties properties(1);
      properties.length(1);
      properties[0].name = CORBA::string_dup (CosNotification::MaxQueueLength);
      properties[0].value <<= 1000;
      ec->set_admin(properties);
#endif //TEST_QOS_MAX_QUEUE_LENGTH

      CORBA::ORB_ptr orb = client.orb ();
      CORBA::Object_var object =
        orb->string_to_object (ior);

      MonitorTestInterface_var sig =
        MonitorTestInterface::_narrow (object.in ());

      if (CORBA::is_nil (sig.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error: Narrow failed.\n"),
                            1);
        }

      CosNotifyChannelAdmin::ConsumerAdmin_var admin =
        create_consumeradmin (ec.in ());

      if (CORBA::is_nil (admin.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error: nil ConsumerAdmin.\n"),
                            1);
        }

      create_consumers (admin.in (), &client);

      sig->running (MonitorTestInterface::Consumer);

      ACE_DEBUG ((LM_DEBUG, "\nConsumer waiting for events...\n"));

      FILE* ready_file = ACE_OS::fopen (ACE_TEXT_ALWAYS_CHAR(ready_output_file), "w");

      if (ready_file == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open ready file for writing\n"),
                            1);
        }

      ACE_OS::fprintf (ready_file, "ready\n");
      ACE_OS::fclose (ready_file);

      client.ORB_run ();
#ifdef PAUSE_ON_EXIT
      _cputs( "All events received. Still connected.\n");
      _cputs( "Hit a key to exit consumer: " );
      _getch();
#endif // PAUSE_ON_EXIT
      ACE_DEBUG ((LM_DEBUG, "Consumer done.\n"));
      consumer_1->disconnect ();

      ec->destroy ();

      sig->finished (MonitorTestInterface::Consumer);
    }
  catch (const CORBA::Exception& e)
    {
      e._tao_print_exception ("Consumer Error: ");
      status = 1;
    }

  return status;
}
