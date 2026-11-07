#include "goC.h"
#include "Notify_Push_Consumer.h"
#include "Notify_Test_Client.h"

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_strings.h"

struct Consumer_Options
{
  ACE_TCHAR const* ior { ACE_TEXT ("file://supplier.ior") };
  int event_count { 90 };
  bool use_filter {};
};

constexpr char grammar[] = "TCL";

class Consumer_Client : public Notify_Test_Client
{
public:
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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:fe:"));
  int x {};

  while ((x = get_opts ()) != -1)
    switch (x)
      {
      case 'k':
        this->options_.ior = get_opts.opt_arg ();
        break;

      case 'e':
        this->options_.event_count = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'f':
        this->options_.use_filter = true;
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -e <num events> -k <ior> [-f]\n"),
                           argv [0]),
                          -1);
      }

  return 0; // successful parse
}

static void
create_consumer (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                 CosNotifyChannelAdmin::EventChannel_ptr ec,
                 Notify_Test_Client& client,
                 Consumer_Options const& options)
{
  Notify_Push_Consumer* consumer {};
  ACE_NEW_THROW_EX (consumer,
                    Notify_Push_Consumer (options.event_count, options.use_filter, client),
                    CORBA::NO_MEMORY ());

  consumer->init (client.root_poa ());

  consumer->_connect (admin, ec);
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    Consumer_Client client;

    int const status = client.init (argc, argv);
    if (status != 0)
      return 1;

    CosNotifyChannelAdmin::EventChannel_var ec =
      client.create_event_channel ("MyEventChannel", 1);

    CosNotifyChannelAdmin::AdminID admin_id {};
    CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin =
      ec->new_for_consumers (CosNotifyChannelAdmin::AND_OP, admin_id);

    ACE_ASSERT (!CORBA::is_nil (consumer_admin.in ()));

    if (client.options ().use_filter)
      {
        CosNotifyFilter::FilterFactory_var ffact =
          ec->default_filter_factory ();

        CosNotifyFilter::Filter_var filter =
          ffact->create_filter (grammar);

        if (CORBA::is_nil (filter.in ()))
          {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT (" (%P|%t) Consumer unable to initialize filter.\n")));
            return 1;
          }

        CosNotifyFilter::ConstraintExpSeq constraint_list (1);
        constraint_list.length (1);

        constraint_list[0].event_types.length (0);
        constraint_list[0].constraint_expr =
          CORBA::string_dup ("exist group and "
                             "exist $.filterable_data(type) and "
                             "$type != 1 and group != 0");

        CosNotifyFilter::ConstraintInfoSeq_var cons_info = filter->add_constraints (constraint_list);

        consumer_admin->add_filter (filter.in ());
      }

    CORBA::ORB_ptr orb = client.orb ();

    CORBA::Object_var object =
      orb->string_to_object (client.options ().ior);

    sig_var sig = sig::_narrow (object.in ());

    ACE_ASSERT (!CORBA::is_nil (sig.in ()));

    create_consumer (consumer_admin.in (), ec.in (), client, client.options ());

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
