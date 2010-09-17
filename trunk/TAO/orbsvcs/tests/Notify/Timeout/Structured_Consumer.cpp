// $Id$

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

static const ACE_TCHAR* ior = ACE_TEXT("file://supplier.ior");
static unsigned int expected = 2;
static Notify_Structured_Push_Consumer* consumer_1 = 0;

class Consumer_Client : public Notify_Test_Client
{
public:
  virtual int parse_args (int argc, ACE_TCHAR* argv[]);
};


int
Consumer_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
        case 'k':
          ior = get_opts.optarg;
          break;

        default:
          ACE_ERROR_RETURN ((LM_ERROR,
            "usage:  %s "
            "-k <ior> "
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
    ec->new_for_consumers (CosNotifyChannelAdmin::OR_OP, adminid);

  return CosNotifyChannelAdmin::ConsumerAdmin::_duplicate (admin.in ());
}


static void
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client* client)
{
  // startup the consumer
  ACE_NEW_THROW_EX (consumer_1,
                    Notify_Structured_Push_Consumer ("consumer1",
                                                     expected,
                                                     *client),
                    CORBA::NO_MEMORY ());
  consumer_1->init (client->root_poa ());
  consumer_1->_connect (admin);
}

// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int status = 0;
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
        orb->string_to_object (ior);

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
  catch (const CORBA::Exception& e)
    {
      e._tao_print_exception ("Consumer Error: ");
      status = 1;
    }

  return status;
}
