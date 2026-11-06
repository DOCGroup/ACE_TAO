// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

#include "sigC.h"
#include "Notify_Structured_Push_Consumer.h"
#include "Notify_Test_Client.h"

// ******************************************************************
// Data Section
// ******************************************************************

constexpr unsigned int expected_events { 2 };

class Consumer_Client : public Notify_Test_Client
{
public:
  int parse_args (int argc, ACE_TCHAR* argv[]) override;

  ACE_TCHAR const* ior () const
  {
    return this->ior_;
  }

private:
  ACE_TCHAR const* ior_ { ACE_TEXT ("file://supplier.ior") };
};


int
Consumer_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
        case 'k':
          this->ior_ = get_opts.optarg;
          break;

        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s -k <ior>\n"),
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
    ec->new_for_consumers (CosNotifyChannelAdmin::OR_OP, adminid);

  return CosNotifyChannelAdmin::ConsumerAdmin::_duplicate (admin.in ());
}


static void
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client* client)
{
  // startup the consumer
  Notify_Structured_Push_Consumer* consumer {};
  ACE_NEW_THROW_EX (consumer,
                    Notify_Structured_Push_Consumer ("consumer1",
                                                     expected_events,
                                                     *client),
                    CORBA::NO_MEMORY ());
  consumer->init (client->root_poa ());
  consumer->_connect (admin);
}

// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int status {};
  try
    {
      Consumer_Client client;

      status = client.init (argc, argv);
      if (status != 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Error: Client init failed.\n"),1);

      CosNotifyChannelAdmin::EventChannel_var ec =
        client.create_event_channel ("MyEventChannel", 1);

      CORBA::ORB_ptr orb = client.orb ();
      CORBA::Object_var object =
        orb->string_to_object (client.ior ());

      sig_var sig = sig::_narrow (object.in ());

      CosNotifyChannelAdmin::ConsumerAdmin_var admin =
        create_consumeradmin (ec.in ());

      if (CORBA::is_nil (admin.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Error: nil ConsumerAdmin.\n"),1);

      create_consumers (admin.in (), &client);

      ACE_DEBUG ((LM_DEBUG, "\nConsumer waiting for events...\n"));

      sig->go ();
      ACE_Time_Value tv (5);
      orb->run (tv);

      ACE_DEBUG ((LM_DEBUG, "Consumer done.\n"));
    }
  catch (CORBA::Exception const& e)
    {
      e._tao_print_exception ("Consumer Error: ");
      status = 1;
    }

  return status;
}
