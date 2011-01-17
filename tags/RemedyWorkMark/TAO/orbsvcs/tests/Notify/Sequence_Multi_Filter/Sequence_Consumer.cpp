// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "Notify_Sequence_Push_Consumer.h"
#include "goC.h"

#include "Notify_Test_Client.h"

// ******************************************************************
// Data Section
// ******************************************************************

static const ACE_TCHAR *ior = ACE_TEXT ("file://supplier.ior");
static unsigned int low = 10;
static unsigned int high = 20;
static CORBA::Boolean filter = 1;
static const ACE_TCHAR *constraintString = ACE_TEXT ("Number == 10");

// ******************************************************************
// Subroutine Section
// ******************************************************************

class Consumer_Client : public Notify_Test_Client
{
public:
  virtual int parse_args (int argc, ACE_TCHAR *argv[]);
};

int
Consumer_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:l:h:d:c:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
  {
    case 'k':
      ior = get_opts.optarg;
      break;

    case 'l':
      low = ACE_OS::atoi (get_opts.optarg);
      break;

    case 'h':
      high = ACE_OS::atoi (get_opts.optarg);
      break;

    case 'c':
      constraintString = get_opts.optarg;
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
        "usage:  %s "
        "-k <ior> "
        "-l <low expected events> "
        "-h <high expected events> "
        "-c <constraint string> "
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
    ec->new_for_consumers (CosNotifyChannelAdmin::AND_OP,
    adminid);


  return CosNotifyChannelAdmin::ConsumerAdmin::_duplicate (admin.in ());
}


static void
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client* client)
{
  // startup the first consumer
  Notify_Sequence_Push_Consumer* consumer_1;
  ACE_NEW_THROW_EX (consumer_1,
    Notify_Sequence_Push_Consumer ("consumer1",
    low,
    high,
    *client),
    CORBA::NO_MEMORY ());
  consumer_1->init(client->root_poa ());

  consumer_1->_connect (admin);
}

// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    Consumer_Client client;

    int status = client.init (argc, argv);
    ACE_ASSERT(status == 0);
    ACE_UNUSED_ARG(status);

    CosNotifyChannelAdmin::EventChannel_var ec =
      client.create_event_channel ("MyEventChannel", 1);

    CORBA::ORB_ptr orb = client.orb ();
    CORBA::Object_var object =
      orb->string_to_object (ior);

    sig_var sig = sig::_narrow (object.in ());

    ACE_ASSERT(! CORBA::is_nil (sig.in ()));

    CosNotifyChannelAdmin::ConsumerAdmin_var admin =
      create_consumeradmin (ec.in ());

    if (filter)
    {
      CosNotifyFilter::FilterFactory_var ffact =
        ec->default_filter_factory ();

      CosNotifyFilter::Filter_var filter =
        ffact->create_filter ("TCL");

      ACE_ASSERT(! CORBA::is_nil(filter.in()));

      CosNotifyFilter::ConstraintExpSeq constraint_list (1);
      constraint_list.length(1);

      constraint_list[0].event_types.length (0);
      constraint_list[0].constraint_expr = CORBA::string_dup(ACE_TEXT_ALWAYS_CHAR (constraintString));

      filter->add_constraints (constraint_list);

      admin->add_filter(filter.in());
    }

    ACE_ASSERT (!CORBA::is_nil (admin.in ()));
    create_consumers(admin.in (), &client);

    // Tell the supplier to go
    sig->go ();

    client.ORB_run( );
    ACE_DEBUG((LM_DEBUG, "Consumer done.\n"));

    sig->done ();

    return 0;

  }
  catch (const CORBA::Exception& e)
  {
    e._tao_print_exception ("Error: Consumer exception: ");
  }

  return 1;
}
