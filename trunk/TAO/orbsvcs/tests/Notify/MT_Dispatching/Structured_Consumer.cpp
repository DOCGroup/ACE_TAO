// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "Notify_Structured_Push_Consumer.h"
#include "goC.h"
#include "ace/OS_NS_sys_time.h"

#include "Notify_Test_Client.h"

// ******************************************************************
// Data Section
// ******************************************************************

static Notify_Structured_Push_Consumer* consumers[1024] = {0};
static const char* ior = "file://supplier.ior";
static unsigned int consumer_count = 1;
static unsigned int expected = 1;

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
  ACE_Get_Opt get_opts (argc, argv, "k:e:c:f");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
  {
    case 'k':
      ior = get_opts.optarg;
      break;

    case 'c':
      consumer_count = ACE_OS::atoi (get_opts.optarg);
      break;

    case 'e':
      expected = ACE_OS::atoi (get_opts.optarg);
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
        "usage:  %s "
        "-k <ior> "
        "-c <# of consumers> "
        "-e <expected events> "
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
  for (unsigned int i = 0; i < consumer_count; i++)
  {
    // startup the consumers
    char name[64] = "";
    ACE_OS::sprintf (name, "consumer%u", i);
    ACE_NEW_THROW_EX (consumers[ i ],
      Notify_Structured_Push_Consumer (
      name,
      expected,
      *client),
      CORBA::NO_MEMORY ());

    consumers[ i ]->init (client->root_poa () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    consumers[ i ]->connect (admin ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }
}

static void
disconnect_consumers (ACE_ENV_SINGLE_ARG_DECL)
{
  for (unsigned int i = 0; i < consumer_count; i++)
  {
    consumers[ i ]->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }
}

// ******************************************************************
// Main Section
// ******************************************************************

int main (int argc, char* argv[])
{
  int status = 0;
  ACE_TRY_NEW_ENV
  {
    Consumer_Client client;

    status = client.init (argc, argv ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (status == 0)
    {
      CosNotifyChannelAdmin::EventChannel_var ec =
        client.create_event_channel ("MyEventChannel", 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        client.orb ()->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      sig_var sig = sig::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (sig.in ()))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
          "Object reference <%s> is nil\n",
          ior),
          1);
      }

      CosNotifyChannelAdmin::ConsumerAdmin_var admin =
        create_consumeradmin (ec.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!CORBA::is_nil (admin.in ()))
      {
        create_consumers (admin.in (), &client ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        // Tell the supplier to go
        sig->go (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        client.ORB_run( ACE_ENV_SINGLE_ARG_PARAMETER );
        ACE_TRY_CHECK;

        disconnect_consumers(ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        ACE_DEBUG ((LM_DEBUG, "Consumer done.\n"));
        sig->done (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    }
  }
  ACE_CATCH (CORBA::Exception, e)
  {
    ACE_PRINT_EXCEPTION (e, "Error: ");
    status = 1;
  }
  ACE_ENDTRY;

  return status;
}
