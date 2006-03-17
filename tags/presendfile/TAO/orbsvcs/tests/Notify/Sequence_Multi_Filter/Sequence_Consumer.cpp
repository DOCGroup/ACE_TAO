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

static const char* ior = "file://supplier.ior";
static unsigned int low = 10;
static unsigned int high = 20;
static CORBA::Boolean filter = 1;
static const char* constraintString = "Number == 10";

// ******************************************************************
// Subroutine Section
// ******************************************************************

class Consumer_Client : public Notify_Test_Client
{
public:
  virtual int parse_args (int argc, char* argv[]);
};

int
Consumer_Client::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:l:h:d:c:");
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
  // Indicates sucessful parsing of the command line
  return 0;
}


static CosNotifyChannelAdmin::ConsumerAdmin_ptr
create_consumeradmin (CosNotifyChannelAdmin::EventChannel_ptr ec
                      ACE_ENV_ARG_DECL)
{
  CosNotifyChannelAdmin::AdminID adminid = 0;
  CosNotifyChannelAdmin::ConsumerAdmin_var admin =
    ec->new_for_consumers (CosNotifyChannelAdmin::AND_OP,
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
  // startup the first consumer
  Notify_Sequence_Push_Consumer* consumer_1;
  ACE_NEW_THROW_EX (consumer_1,
    Notify_Sequence_Push_Consumer ("consumer1",
    low,
    high,
    *client),
    CORBA::NO_MEMORY ());
  consumer_1->init(client->root_poa () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  consumer_1->_connect (admin ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

// ******************************************************************
// Main Section
// ******************************************************************

int main (int argc, char* argv[])
{
  ACE_TRY_NEW_ENV
  {
    Consumer_Client client;

    int status = client.init (argc, argv ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_ASSERT(status == 0);
    ACE_UNUSED_ARG(status);

    CosNotifyChannelAdmin::EventChannel_var ec =
      client.create_event_channel ("MyEventChannel", 1
      ACE_ENV_ARG_PARAMETER);
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

    if (filter)
    {
      CosNotifyFilter::FilterFactory_var ffact =
        ec->default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosNotifyFilter::Filter_var filter =
        ffact->create_filter ("TCL" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_ASSERT(! CORBA::is_nil(filter.in()));

      CosNotifyFilter::ConstraintExpSeq constraint_list (1);
      constraint_list.length(1);

      constraint_list[0].event_types.length (0);
      constraint_list[0].constraint_expr = CORBA::string_dup(constraintString);

      filter->add_constraints (constraint_list ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      admin->add_filter(filter.in());
    }

    ACE_ASSERT (!CORBA::is_nil (admin.in ()));
    create_consumers(admin.in (), &client ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    // Tell the supplier to go
    sig->go (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    client.ORB_run( ACE_ENV_SINGLE_ARG_PARAMETER );
    ACE_TRY_CHECK;
    ACE_DEBUG((LM_DEBUG, "Consumer done.\n"));

    sig->done (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    return 0;

  }
  ACE_CATCH (CORBA::Exception, e)
  {
    ACE_PRINT_EXCEPTION (e, "Error: Consumer exception: ");
  }
  ACE_ENDTRY;

  return 1;
}
