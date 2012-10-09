// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "Notify_Push_Consumer.h"
#include "goC.h"
#include "Notify_Test_Client.h"

#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"

// ******************************************************************
// Data Section
// ******************************************************************

static const ACE_TCHAR* ior = ACE_TEXT ("file://supplier.ior");
static Notify_Sequence_Push_Consumer* consumer_1;
static bool go = false;

// ******************************************************************
// Subroutine Section
// ******************************************************************

class Consumer_Client : public Notify_Test_Client
{
public:
  virtual int parse_args (int argc, ACE_TCHAR* argv[]);
};


int
Consumer_Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter argcon (argc, argv);
  ACE_Get_Opt get_opts (argcon.get_argc (),
                        argcon.get_TCHAR_argv (), ACE_TEXT ("gk:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'g':
        go = true;
        break;
      case 'k':
        ior = get_opts.optarg;
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s "
                           "-g "
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
    ec->new_for_consumers (CosNotifyChannelAdmin::OR_OP,
                           adminid);


  return CosNotifyChannelAdmin::ConsumerAdmin::_duplicate (admin.in ());
}


static void
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client* client)
{
  // startup the consumer
  ACE_NEW_THROW_EX (consumer_1,
                    Notify_Sequence_Push_Consumer (*client),
                    CORBA::NO_MEMORY ());
  consumer_1->init (client->root_poa ());

  consumer_1->_connect (admin);
}

// ******************************************************************
// Main Section
// ******************************************************************

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int status = 0;
  try
    {
      Consumer_Client client;
      ACE_Argv_Type_Converter argcon (argc, argv);
      status = client.init (argcon.get_argc (), argcon.get_TCHAR_argv ());

      if (status == 0)
        {
          CosNotifyChannelAdmin::EventChannel_var ec =
            client.create_event_channel ("MyEventChannel", 1);

          CORBA::ORB_ptr orb = client.orb ();
          CORBA::Object_var object =
                              orb->string_to_object (ior);

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
              if (go)
                {
                  // Tell the supplier to go
                  sig->go ();
                }

              ACE_DEBUG ((LM_DEBUG, "Consumer waiting for events...\n"));

              client.ORB_run ();

              ACE_DEBUG ((LM_DEBUG, "Consumer done.\n"));
              consumer_1->disconnect ();

              if (go)
                {
                  sig->done ();
                }
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
