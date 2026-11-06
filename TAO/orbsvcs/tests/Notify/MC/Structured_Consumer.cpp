// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"

#include <memory>

#include "Notify_Structured_Push_Consumer.h"
#include "MonitorTestInterfaceC.h"

#include "Notify_Test_Client.h"

struct Consumer_Options
{
  ACE_TCHAR const* ior { ACE_TEXT ("file://test_monitor.ior") };
  ACE_TCHAR const* ready_output_file { ACE_TEXT ("ready.txt") };
  unsigned int expected { 2000 };
  unsigned int delay_period { 5 };
  unsigned int delay_count {};
};

class Consumer_Client : public Notify_Test_Client
{
public:
  int parse_args (int argc, ACE_TCHAR* argv[]) override;

  Consumer_Options const& options () const
  {
    return this->options_;
  }

private:
  Consumer_Options options_ {};
};

int
Consumer_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:e:p:d:o:"));
  int option {};

  while ((option = get_opts ()) != -1)
    {
      switch (option)
        {
        case 'k':
          this->options_.ior = get_opts.opt_arg ();
          break;
        case 'e':
          this->options_.expected = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 'p':
          this->options_.delay_period = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 'd':
          this->options_.delay_count = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        case 'o':
          this->options_.ready_output_file = get_opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s -k <ior> -e <expected events> "
                                       "-d <delay every n seconds> -p <delay seconds> "
                                       "-o <readyfile>\n"),
                             argv[0]),
                            -1);
        }
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
                  Notify_Test_Client* client,
                  Consumer_Options const& options,
                  Notify_Structured_Push_Consumer*& consumer)
{
  ACE_NEW_THROW_EX (consumer,
                    Notify_Structured_Push_Consumer ("consumer1",
                                                     options.expected,
                                                     *client),
                    CORBA::NO_MEMORY ());
  consumer->set_delay_parameters (options.delay_count, options.delay_period);
  consumer->init (client->root_poa ());
  consumer->_connect (admin);
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

      CosNotifyChannelAdmin::EventChannel_var const ec =
        client.create_event_channel ("MyEventChannel", 0);

#ifdef TEST_QOS_MAX_QUEUE_LENGTH
      ACE_OS::printf ("%s: setting max queue length to 1000\n", argv[0]);
      CosNotification::AdminProperties properties(1);
      properties.length(1);
      properties[0].name = CORBA::string_dup (CosNotification::MaxQueueLength);
      properties[0].value <<= 1000;
      ec->set_admin(properties);
#endif //TEST_QOS_MAX_QUEUE_LENGTH

      Consumer_Options const& options = client.options ();
      CORBA::ORB_ptr const orb = client.orb ();
      CORBA::Object_var const object = orb->string_to_object (options.ior);

      MonitorTestInterface_var sig =
        MonitorTestInterface::_narrow (object.in ());

      if (CORBA::is_nil (sig.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error: Narrow failed.\n"),
                            1);
        }

      CosNotifyChannelAdmin::ConsumerAdmin_var const admin =
        create_consumeradmin (ec.in ());

      if (CORBA::is_nil (admin.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error: nil ConsumerAdmin.\n"),
                            1);
        }

      Notify_Structured_Push_Consumer* consumer {};
      create_consumers (admin.in (), &client, options, consumer);

      sig->running (MonitorTestInterface::Consumer);

      ACE_DEBUG ((LM_DEBUG, "\nConsumer waiting for events...\n"));

      {
        std::unique_ptr<FILE, int (*) (FILE *)> ready_file (
          ACE_OS::fopen (options.ready_output_file, ACE_TEXT ("w")), ACE_OS::fclose);
        if (!ready_file)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open ready file for writing\n"),
                              1);
          }

        ACE_OS::fprintf (ready_file.get (), ACE_TEXT ("%s"), ACE_TEXT ("ready\n"));
      }

      client.ORB_run ();
#ifdef PAUSE_ON_EXIT
      _cputs( "All events received. Still connected.\n");
      _cputs( "Hit a key to exit consumer: ");
      _getch();
#endif // PAUSE_ON_EXIT
      ACE_DEBUG ((LM_DEBUG, "Consumer done.\n"));
      consumer->disconnect ();

      ec->destroy ();

      sig->finished (MonitorTestInterface::Consumer);
    }
  catch (CORBA::Exception const& e)
    {
      e._tao_print_exception ("Consumer Error: ");
      status = 1;
    }

  return status;
}
