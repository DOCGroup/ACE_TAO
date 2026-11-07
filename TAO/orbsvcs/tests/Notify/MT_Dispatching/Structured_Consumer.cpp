// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "Notify_Structured_Push_Consumer.h"
#include "goC.h"

#include "Notify_Test_Client.h"

#include <array>

// ******************************************************************
// Data Section
// ******************************************************************

constexpr unsigned int max_consumers { 1024 };

struct Consumer_Options
{
  ACE_TCHAR const* ior { ACE_TEXT ("file://supplier.ior") };
  unsigned int consumer_count { 1 };
  unsigned int expected_events { 1 };
};

// ******************************************************************
// Subroutine Section
// ******************************************************************

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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:e:c:f"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
  {
    case 'k':
      this->options_.ior = get_opts.optarg;
      break;

    case 'c':
      this->options_.consumer_count = ACE_OS::atoi (get_opts.optarg);
      if (this->options_.consumer_count > max_consumers)
        this->options_.consumer_count = max_consumers;
      break;

    case 'e':
      this->options_.expected_events = ACE_OS::atoi (get_opts.optarg);
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("usage: %s -k <ior> -c <# of consumers> "
                                   "-e <expected events>\n"),
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


using Consumers = std::array<Notify_Structured_Push_Consumer*, max_consumers>;

static void
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client& client,
                  Consumer_Options const& options,
                  Consumers& consumers)
{
  for (unsigned int i = 0; i < options.consumer_count; i++)
  {
    ACE_NEW_THROW_EX (consumers[i],
      Notify_Structured_Push_Consumer (options.expected_events, client),
      CORBA::NO_MEMORY ());

    consumers[i]->init (client.root_poa ());

    consumers[i]->connect (admin);
  }
}

static void
disconnect_consumers (Consumers const& consumers, unsigned int consumer_count)
{
  for (unsigned int i = 0; i < consumer_count; i++)
  {
    consumers[i]->disconnect ();
  }
}

// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int status {};
  Consumers consumers {};
  try
  {
    Consumer_Client client;

    status = client.init (argc, argv);

    if (status == 0)
    {
      CosNotifyChannelAdmin::EventChannel_var ec =
        client.create_event_channel ("MyEventChannel", 1);

      CORBA::Object_var object =
        client.orb ()->string_to_object (client.options ().ior);

      sig_var sig = sig::_narrow (object.in ());

      if (CORBA::is_nil (sig.in ()))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
          ACE_TEXT ("Object reference <%s> is nil\n"),
          client.options ().ior),
          1);
      }

      CosNotifyChannelAdmin::ConsumerAdmin_var admin =
        create_consumeradmin (ec.in ());

      if (!CORBA::is_nil (admin.in ()))
      {
        create_consumers (admin.in (), client, client.options (), consumers);

        // Tell the supplier to go
        sig->go ();

        client.ORB_run ();

        disconnect_consumers (consumers, client.options ().consumer_count);

        ACE_DEBUG ((LM_DEBUG, "Consumer done.\n"));
        sig->done ();
      }
    }
  }
  catch (CORBA::Exception const& e)
  {
    e._tao_print_exception ("Error: ");
    status = 1;
  }

  return status;
}
