// $Id$

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

static const char* ior = "file://supplier.ior";
static int numEvents = 90;
static Notify_Push_Consumer* consumer = 0;
static bool useFilter = false;

static const char* GRAMMAR = "TCL";

class Consumer_Client : public Notify_Test_Client
{
public:
  virtual int parse_args (int argc, ACE_TCHAR *argv[]);
};

int
Consumer_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "fe:");
  int x;

  while ((x = get_opts ()) != -1)
    switch (x)
  {
    case 'e':
      numEvents = ACE_OS::atoi (get_opts.optarg);
      break;

    case 'f':
      useFilter = true;
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
        "usage:  %s -n <num events> [-f]\n", argv [0]), -1);
  }

  return 0; // successful parse
}

static void
create_consumer (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                 CosNotifyChannelAdmin::EventChannel_ptr ec,
                 Notify_Test_Client* client)
{
  ACE_NEW_THROW_EX (consumer,
    Notify_Push_Consumer ("Consumer", numEvents, useFilter, *client),
    CORBA::NO_MEMORY ());

  consumer->init (client->root_poa ());

  consumer->_connect (admin, ec);
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    Consumer_Client client;

    int status = client.init (argc, argv);
    ACE_UNUSED_ARG(status);
    ACE_ASSERT(status == 0);

    CosNotifyChannelAdmin::EventChannel_var ec =
      client.create_event_channel ("MyEventChannel", 1);

    CosNotifyChannelAdmin::AdminID adminid = 0;
    CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin =
      ec->new_for_consumers(CosNotifyChannelAdmin::AND_OP, adminid);

    ACE_ASSERT(! CORBA::is_nil (consumer_admin.in ()));

    if (useFilter)
    {
      CosNotifyFilter::FilterFactory_var ffact =
        ec->default_filter_factory ();

      CosNotifyFilter::Filter_var filter =
        ffact->create_filter (GRAMMAR);

      if (CORBA::is_nil (filter.in ()))
      {
        ACE_ERROR ((LM_ERROR,
          " (%P|%t) Consumer unable to initialize filter.\n"));
        return 1;
      }

      CosNotifyFilter::ConstraintExpSeq constraint_list (1);
      constraint_list.length (1);

      constraint_list[0].event_types.length (0);
      constraint_list[0].constraint_expr =
        CORBA::string_dup ("exist group and "
                           "exist $.filterable_data(type) and "
                           "$type != 1 and group != 0");

      filter->add_constraints (constraint_list);

      consumer_admin->add_filter (filter.in ());
    }

    CORBA::ORB_ptr orb = client.orb ();

    CORBA::Object_var object =
      orb->string_to_object (ior);

    sig_var sig = sig::_narrow (object.in ());

    ACE_ASSERT(! CORBA::is_nil (sig.in ()));

    create_consumer (consumer_admin.in (), ec.in (), &client);

    ACE_DEBUG((LM_DEBUG, "\nConsumer waiting for events...\n"));

    sig->go ();

    client.ORB_run( );

    ACE_DEBUG((LM_DEBUG, "\nConsumer done.\n"));

    sig->done();

    return 0;
  }
  catch (const CORBA::Exception& e)
  {
    e._tao_print_exception ("\nError: Consumer:");
  }

  return 1;
}
