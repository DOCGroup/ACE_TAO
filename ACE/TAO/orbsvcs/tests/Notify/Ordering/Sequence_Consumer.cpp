// $Id$
#include "Notify_Sequence_Push_Consumer.h"
#include "goC.h"

#include "Notify_Test_Client.h"

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_stat.h"

static const char* ior = "file://supplier.ior";
static CORBA::Short order_policy = CosNotification::FifoOrder;
static int expected = 30; // Must match the amount sent by supplier
static Notify_Sequence_Push_Consumer* consumer_1;
static bool use_ordering = false;

class Consumer_Client : public Notify_Test_Client
{
public:
  virtual int parse_args (int argc, char* argv[]);
};


int
Consumer_Client::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "ok:e:d:");
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

    case 'o':
      use_ordering = true;
      break;

    case 'd':
      {
        const char* order = get_opts.optarg;
        if (ACE_OS::strcmp (order, "fifo") == 0)
        {
          order_policy = CosNotification::FifoOrder;
        }
        else if (ACE_OS::strcmp (order, "priority") == 0)
        {
          order_policy = CosNotification::PriorityOrder;
        }
        else if (ACE_OS::strcmp (order, "deadline") == 0)
        {
          order_policy = CosNotification::DeadlineOrder;
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
            "Unknown ordering policy: %s\n",
            order_policy),
            -1);
        }
        break;
      }
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
        "usage:  %s "
        "-k <ior> "
        "-e <expected events> "
        "-d <fifo|priority|deadline> "
        "\n",
        argv [0]),
        -1);
  }
  // Indicates sucessful parsing of the command line
  return 0;
}


static CosNotifyChannelAdmin::ConsumerAdmin_ptr
create_consumeradmin (CosNotifyChannelAdmin::EventChannel_ptr ec
                      ACE_ENV_ARG_DECL)
{
  CosNotifyChannelAdmin::AdminID adminid = 0;
  CosNotifyChannelAdmin::ConsumerAdmin_var admin =
    ec->new_for_consumers (CosNotifyChannelAdmin::OR_OP,
    adminid
    ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (0);

  return CosNotifyChannelAdmin::ConsumerAdmin::_duplicate (admin.in ());
}


static void
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client* client
                  ACE_ENV_ARG_DECL)
{
  ACE_NEW_THROW_EX (consumer_1,
    Notify_Sequence_Push_Consumer ("consumer1",
    order_policy,
    use_ordering,
    expected,
    *client),
    CORBA::NO_MEMORY ());

  consumer_1->init (client->root_poa () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  consumer_1->_connect (admin ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

int main (int argc, char* argv[])
{
  ACE_TRY_NEW_ENV
  {
    Consumer_Client client;

    int status = client.init (argc, argv ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_UNUSED_ARG(status);
    ACE_ASSERT(status == 0);

    CosNotifyChannelAdmin::EventChannel_var ec =
      client.create_event_channel ("MyEventChannel", 1 ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    CORBA::ORB_ptr orb = client.orb ();
    CORBA::Object_var object =
      orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    sig_var sig = sig::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    ACE_ASSERT(! CORBA::is_nil (sig.in ()));

    CosNotifyChannelAdmin::ConsumerAdmin_var admin =
      create_consumeradmin (ec.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    ACE_ASSERT(!CORBA::is_nil (admin.in ()));
    create_consumers (admin.in (), &client ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    sig->go (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    client.ORB_run( ACE_ENV_SINGLE_ARG_PARAMETER );
    ACE_TRY_CHECK;

    ACE_DEBUG((LM_DEBUG, "Consumer done.\n"));
    consumer_1->disconnect(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    sig->done (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    return 0;
  }
  ACE_CATCH (CORBA::Exception, e)
  {
    ACE_PRINT_EXCEPTION (e, "Error: ");
  }
  ACE_ENDTRY;

  return 1;
}
