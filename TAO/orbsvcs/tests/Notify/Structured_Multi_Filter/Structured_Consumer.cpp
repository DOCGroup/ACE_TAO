#include "goC.h"
#include "Notify_Push_Consumer.h"
#include "Notify_Test_Client.h"

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"

#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_strings.h"

struct Consumer_Options
{
  ACE_TCHAR const* ior { ACE_TEXT ("file://messenger.ior") };
  int event_count { 90 };
  NS_FilterType consumer_filter { NS_FilterType::none };
  NS_FilterType supplier_filter { NS_FilterType::none };
};

constexpr char grammar[] = "TCL";

class Consumer_Client : public Notify_Test_Client
{
public:
  Consumer_Client () = default;
  int parse_args (int argc, ACE_TCHAR *argv[]) override;
  Consumer_Client (Consumer_Client const&) = delete;
  Consumer_Client (Consumer_Client&&) = delete;
  Consumer_Client& operator= (Consumer_Client const&) = delete;
  Consumer_Client& operator= (Consumer_Client&&) = delete;

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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:f:s:e:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        this->options_.ior = get_opts.opt_arg ();
        break;

      case 'e':
        this->options_.event_count = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'f':
        this->options_.consumer_filter =
          ACE_OS::strcasecmp (ACE_TEXT ("OR"), get_opts.optarg) == 0
            ? NS_FilterType::or_op : NS_FilterType::and_op;
        break;

      case 's':
        this->options_.supplier_filter =
          ACE_OS::strcasecmp (ACE_TEXT ("OR"), get_opts.optarg) == 0
            ? NS_FilterType::or_op : NS_FilterType::and_op;
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior> [-s <AND|OR>] -e <num events> "
                                     "[-f <AND|OR>]\n"),
                           argv [0]),
                          -1);
      }

  return 0;
}

static void
create_consumer (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                 CosNotifyChannelAdmin::EventChannel_ptr ec,
                 Notify_Test_Client& client,
                 Consumer_Options const& options)
{
  Notify_Push_Consumer* consumer {};
  ACE_NEW_THROW_EX (consumer,
                    Notify_Push_Consumer (options.event_count, options.consumer_filter,
                                          options.supplier_filter, client),
                    CORBA::NO_MEMORY ());

  consumer->init (client.root_poa ());
  consumer->_connect (admin, ec);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Consumer_Client client;
      if (client.init (argc, argv) != 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: Unable to init consumer.\n")));
          return 1;
        }

      Consumer_Options const& options = client.options ();
      CosNotifyChannelAdmin::EventChannel_var ec =
        client.create_event_channel ("Struct_Multi_Filter", 1);

      CosNotifyChannelAdmin::AdminID admin_id {};
      CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin =
        ec->new_for_consumers (options.consumer_filter == NS_FilterType::or_op
                                 ? CosNotifyChannelAdmin::OR_OP
                                 : CosNotifyChannelAdmin::AND_OP,
                               admin_id);
      ACE_ASSERT (!CORBA::is_nil (consumer_admin.in ()));

      if (options.consumer_filter != NS_FilterType::none)
        {
          CosNotifyFilter::FilterFactory_var ffact = ec->default_filter_factory ();
          CosNotifyFilter::Filter_var filter = ffact->create_filter (grammar);
          if (CORBA::is_nil (filter.in ()))
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT (" (%P|%t) Consumer unable to initialize filter.\n")));
              return 1;
            }

          CosNotifyFilter::ConstraintExpSeq constraint_list (1);
          constraint_list.length (1);
          constraint_list[0].event_types.length (0);
          constraint_list[0].constraint_expr = CORBA::string_dup ("type != 1");
          filter->add_constraints (constraint_list);
          consumer_admin->add_filter (filter.in ());
        }

      CORBA::Object_var const object = client.orb ()->string_to_object (options.ior);
      sig_var const sig = sig::_narrow (object.in ());
      ACE_ASSERT (!CORBA::is_nil (sig.in ()));

      create_consumer (consumer_admin.in (), ec.in (), client, options);
      ACE_DEBUG ((LM_DEBUG, "\nConsumer waiting for events...\n"));
      sig->go ();
      client.ORB_run ();
      ACE_DEBUG ((LM_DEBUG, "\nConsumer done.\n"));
      sig->done ();
      return 0;
    }
  catch (CORBA::Exception const& e)
    {
      e._tao_print_exception ("\nError: Consumer:");
    }

  return 1;
}
