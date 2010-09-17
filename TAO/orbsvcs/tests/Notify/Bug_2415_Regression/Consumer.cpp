// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "Notify_Push_Consumer.h"
#include "goC.h"

#include "Notify_Test_Client.h"

#define TEST_OP 1

// ******************************************************************
// Data Section
// ******************************************************************

static const ACE_TCHAR *ior = ACE_TEXT ("file://supplier.ior");
static unsigned int expect_fail = 0;
static Notify_Push_Consumer* consumer_1 = 0;

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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:l:h:d:f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;

      case 'f':
        expect_fail = ACE_OS::atoi (get_opts.optarg);
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-f <expect failure> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

static CosNotifyFilter::Filter_ptr
create_proxyFilter (CosNotifyChannelAdmin::EventChannel_ptr ec)
{
  CosNotifyFilter::FilterFactory_var ffact =
    ec->default_filter_factory ();

  CosNotifyFilter::Filter_var filter =
    ffact->create_filter ("EXTENDED_TCL");

  if(!CORBA::is_nil(filter.in()))
    {
    CosNotifyFilter::ConstraintExpSeq constraint_list (1);
    constraint_list.length(1);

    constraint_list[0].event_types.length (0);

    constraint_list[0].constraint_expr =
      CORBA::string_dup ("$.domain_name == 'domain1'");

    filter->add_constraints (constraint_list);
    }
  return filter._retn();
}

static void
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client* client,
                  CosNotifyFilter::Filter_ptr filter)
{
  // startup the consumer
  ACE_NEW_THROW_EX (consumer_1,
                    Notify_Push_Consumer (*client),
                    CORBA::NO_MEMORY ());

  consumer_1->init (client->root_poa ());

  consumer_1->_connect (admin, filter);
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
            ec->default_consumer_admin();

          CosNotifyFilter::Filter_var filter =
            create_proxyFilter (ec.in());

          if (!CORBA::is_nil (admin.in ()))
            {
              create_consumers (admin.in (), &client, filter.in());

              // Tell the supplier to go
              sig->go ();

              ACE_Time_Value tv(10, 0);
              client.ORB_run(tv);
              ACE_DEBUG((LM_DEBUG, "Consumer done.\n"));

              sig->done ();

              ACE_DEBUG((LM_DEBUG, "Expected %d message(s) and received %d message(s).\n",
                expect_fail == 1 ? 0 : 1, consumer_1->messages_received()));

              if (1 == expect_fail)
                status = consumer_1->messages_received() == 0 ? 0 : 1;
              else
                status = consumer_1->messages_received() == 1 ? 0 : 1;

              ACE_DEBUG((LM_DEBUG, "Test status is %s.\n", status == 1 ? "fail" : "pass"));

            }
        }
    }
  catch (const CORBA::Exception& e)
    {
      e._tao_print_exception ("Error: Consumer exception: ");
      status = 1;
    }

  return status;
}
