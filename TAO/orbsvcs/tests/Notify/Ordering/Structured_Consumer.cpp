#include "Notify_Structured_Push_Consumer.h"
#include "goC.h"
#include "Notify_Test_Client.h"

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

#include "ace/Get_Opt.h"

struct Consumer_Options
{
  ACE_TCHAR const* ior { ACE_TEXT ("file://supplier.ior") };
  CORBA::Short order_policy { CosNotification::FifoOrder };
  int expected { 30 };
  bool use_ordering {};
};

class Consumer_Client : public Notify_Test_Client
{
public:
  Consumer_Client () = default;
  int parse_args (int argc, ACE_TCHAR* argv[]) override;

  Consumer_Client (Consumer_Client const&) = delete;
  Consumer_Client (Consumer_Client&&) = delete;
  Consumer_Client& operator= (Consumer_Client const&) = delete;
  Consumer_Client& operator= (Consumer_Client&&) = delete;

  Consumer_Options const& options () const
  {
    return this->options_;
  }

private:
  Consumer_Options options_ {};
};

int
Consumer_Client::parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("ok:e:d:"));
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
        case 'o':
          this->options_.use_ordering = true;
          break;
        case 'd':
          {
            ACE_TCHAR const* const order = get_opts.opt_arg ();
            if (ACE_OS::strcmp (order, ACE_TEXT ("fifo")) == 0)
              this->options_.order_policy = CosNotification::FifoOrder;
            else if (ACE_OS::strcmp (order, ACE_TEXT ("priority")) == 0)
              this->options_.order_policy = CosNotification::PriorityOrder;
            else if (ACE_OS::strcmp (order, ACE_TEXT ("deadline")) == 0)
              {
                this->options_.order_policy = CosNotification::DeadlineOrder;
#if !defined (ACE_HAS_TIMED_MESSAGE_BLOCKS)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("This order policy requires timed message blocks.\n"
                                             "Please define ACE_HAS_TIMED_MESSAGE_BLOCKS in "
                                             "your config.h\n")),
                                  -1);
#endif
              }
            else
              ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Unknown ordering policy\n")), -1);
            break;
          }
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s -k <ior> -e <expected events> "
                                       "-d <fifo|priority|deadline>\n"),
                             argv[0]),
                            -1);
        }
    }
  return 0;
}

static CosNotifyChannelAdmin::ConsumerAdmin_ptr
create_consumeradmin (CosNotifyChannelAdmin::EventChannel_ptr ec)
{
  CosNotifyChannelAdmin::AdminID adminid {};
  CosNotifyChannelAdmin::ConsumerAdmin_var admin =
    ec->new_for_consumers (CosNotifyChannelAdmin::OR_OP, adminid);
  return CosNotifyChannelAdmin::ConsumerAdmin::_duplicate (admin.in ());
}

static void
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client& client,
                  Consumer_Options const& options,
                  Notify_Structured_Push_Consumer*& consumer)
{
  // connect() transfers the initial servant reference to the POA; this pointer is borrowed.
  ACE_NEW_THROW_EX (consumer,
                    Notify_Structured_Push_Consumer ("consumer1",
                                                     options.order_policy,
                                                     options.use_ordering,
                                                     options.expected,
                                                     client),
                    CORBA::NO_MEMORY ());
  consumer->init (client.root_poa ());
  consumer->_connect (admin);
}

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try
    {
      Consumer_Client client;
      int const status = client.init (argc, argv);
      if (status != 0)
        return status;

      CosNotifyChannelAdmin::EventChannel_var const ec =
        client.create_event_channel ("MyEventChannel", 1);
      CORBA::Object_var const object = client.orb ()->string_to_object (client.options ().ior);
      sig_var const sig = sig::_narrow (object.in ());
      ACE_ASSERT (!CORBA::is_nil (sig.in ()));

      CosNotifyChannelAdmin::ConsumerAdmin_var const admin = create_consumeradmin (ec.in ());
      ACE_ASSERT (!CORBA::is_nil (admin.in ()));

      Notify_Structured_Push_Consumer* consumer {};
      create_consumers (admin.in (), client, client.options (), consumer);
      sig->go ();
      client.ORB_run ();

      ACE_DEBUG ((LM_DEBUG, "Consumer done.\n"));
      consumer->disconnect ();
      sig->done ();
      return 0;
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Error: ");
    }
  return 1;
}
