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
static const char* ready = "ready.txt";
static unsigned int expected = 2000;
static Notify_Structured_Push_Consumer* consumer_1 = 0;

class Consumer_Client : public Notify_Test_Client
{
public:
  virtual int parse_args (int argc, ACE_TCHAR *argv[]);
};


int
Consumer_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:e:"));
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

        default:
          ACE_ERROR_RETURN ((LM_ERROR,
            "usage:  %s "
            "-k <ior> "
            "-e <expected events> "
            "\n",
            argv [0]),
            -1);
      }
  // Indicates sucessful parsing of the command line
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

      FILE* ready_file = ACE_OS::fopen (ready, "w");

      if (ready_file == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open ready file for writing\n"),
                            1);
        }

      ACE_OS::fprintf (ready_file, "ready\n");
      ACE_OS::fclose (ready_file);

      client.ORB_run ();

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
