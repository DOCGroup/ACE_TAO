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
static const ACE_TCHAR *ior = ACE_TEXT ("file://supplier.ior");
static unsigned int consumer_count = 1;
static unsigned int expected = 1;

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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:e:c:f"));
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

    consumers[ i ]->init (client->root_poa ());

    consumers[ i ]->connect (admin);
  }
}

static void
disconnect_consumers (void)
{
  for (unsigned int i = 0; i < consumer_count; i++)
  {
    consumers[ i ]->disconnect ();
  }
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

    if (status == 0)
    {
      CosNotifyChannelAdmin::EventChannel_var ec =
        client.create_event_channel ("MyEventChannel", 1);

      CORBA::Object_var object =
        client.orb ()->string_to_object (ior);

      sig_var sig = sig::_narrow (object.in ());

      if (CORBA::is_nil (sig.in ()))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
          "Object reference <%s> is nil\n",
          ior),
          1);
      }

      CosNotifyChannelAdmin::ConsumerAdmin_var admin =
        create_consumeradmin (ec.in ());

      if (!CORBA::is_nil (admin.in ()))
      {
        create_consumers (admin.in (), &client);

        // Tell the supplier to go
        sig->go ();

        client.ORB_run( );

        disconnect_consumers();

        ACE_DEBUG ((LM_DEBUG, "Consumer done.\n"));
        sig->done ();
      }
    }
  }
  catch (const CORBA::Exception& e)
  {
    e._tao_print_exception ("Error: ");
    status = 1;
  }

  return status;
}
