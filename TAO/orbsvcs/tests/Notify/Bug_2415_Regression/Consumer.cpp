// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdlib.h"

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "Notify_Push_Consumer.h"
#include "goC.h"

#include "Notify_Test_Client.h"

// ******************************************************************
// Data Section
// ******************************************************************

struct Consumer_Options
{
  ACE_TCHAR const* ior { ACE_TEXT ("file://supplier.ior") };
  unsigned int expect_fail {};
};

// ******************************************************************
// Subroutine Section
// ******************************************************************

class Consumer_Client : public Notify_Test_Client
{
public:
  Consumer_Client () = default;
  Consumer_Client (Consumer_Client const&) = delete;
  Consumer_Client (Consumer_Client&&) = delete;
  Consumer_Client& operator= (Consumer_Client const&) = delete;
  Consumer_Client& operator= (Consumer_Client&&) = delete;

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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:l:h:d:f:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        this->options_.ior = get_opts.optarg;
        break;

      case 'f':
        this->options_.expect_fail = ACE_OS::atoi (get_opts.optarg);
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior> -f <expect failure>\n"),
                           argv[0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

static CosNotifyFilter::Filter_ptr
create_proxyFilter (CosNotifyChannelAdmin::EventChannel_ptr ec)
{
  CosNotifyFilter::FilterFactory_var ffact =
    ec->default_filter_factory ();

  CosNotifyFilter::Filter_var filter =
    ffact->create_filter ("EXTENDED_TCL");

  if (!CORBA::is_nil (filter.in ()))
    {
      CosNotifyFilter::ConstraintExpSeq constraint_list (1);
      constraint_list.length (1);

      constraint_list[0].event_types.length (0);

      constraint_list[0].constraint_expr =
        CORBA::string_dup ("$.domain_name == 'domain1'");

      CosNotifyFilter::ConstraintInfoSeq_var cons_info = filter->add_constraints (constraint_list);
    }
  return filter._retn();
}

static Notify_Push_Consumer*
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client& client,
                  CosNotifyFilter::Filter_ptr filter)
{
  Notify_Push_Consumer* consumer {};
  ACE_NEW_THROW_EX (consumer,
                    Notify_Push_Consumer (client),
                    CORBA::NO_MEMORY ());

  consumer->init (client.root_poa ());

  consumer->_connect (admin, filter);
  return consumer;
}

// ******************************************************************
// Main Section
// ******************************************************************

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int status {};
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
            ec->default_consumer_admin ();

          CosNotifyFilter::Filter_var filter =
            create_proxyFilter (ec.in ());

          if (!CORBA::is_nil (admin.in ()))
            {
              Notify_Push_Consumer* consumer =
                create_consumers (admin.in (), client, filter.in ());

              // Tell the supplier to go
              sig->go ();

              ACE_Time_Value tv (10, 0);
              client.ORB_run (tv);
              ACE_DEBUG ((LM_DEBUG, "Consumer done.\n"));

              sig->done ();

              ACE_DEBUG ((LM_DEBUG,
                          "Expected %d message(s) and received %d message(s).\n",
                          client.options ().expect_fail == 1 ? 0 : 1,
                          consumer->messages_received ()));

              if (client.options ().expect_fail == 1)
                status = consumer->messages_received () == 0 ? 0 : 1;
              else
                status = consumer->messages_received () == 1 ? 0 : 1;

              ACE_DEBUG ((LM_DEBUG,
                          "Test status is %s.\n",
                          status == 1 ? "fail" : "pass"));
            }
        }
    }
  catch (CORBA::Exception const& e)
    {
      e._tao_print_exception ("Error: Consumer exception: ");
      status = 1;
    }

  return status;
}
