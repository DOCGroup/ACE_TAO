// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "controlC.h"
#include "Notify_Push_Consumer.h"
#include "Notify_Test_Client.h"

// ******************************************************************
// Data Section
// ******************************************************************

static CORBA::Boolean filter = 0;
static CORBA::Short num_events = 0;
static CORBA::Short num_consumers = 0;

#define GRAMMAR "ETCL"

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
  ACE_Get_Opt get_opts (argc, argv, "fn:s:c:");
  int x;

  while ((x = get_opts ()) != -1)
    switch (x)
      {
      case 'f':
        filter = 1;
        break;

      case 'n':
        num_events = atoi (get_opts.optarg);
        break;

      case 'c':
        num_consumers = atoi (get_opts.optarg);
        break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "[-f] -n <num events> -c <num consumers> "
                           "-ORBInitRef <Naming Service Location>"
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0; // successful parse
}

static void
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client* client
                  ACE_ENV_ARG_DECL)
{
  Notify_Push_Consumer* consumer = 0;
  ACE_NEW_THROW_EX (consumer,
                    Notify_Push_Consumer ("Consumer"),
                    CORBA::NO_MEMORY ());

  consumer->init (client->root_poa () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  consumer->connect (admin ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

// ******************************************************************
// Main Section
// ******************************************************************

int main (int argc, char * argv[])
{
  int status = 0;

  ACE_TRY_NEW_ENV;
  {
    Consumer_Client client;

    status = client.init (argc, argv ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (status == 0)
      {
        CosNotifyChannelAdmin::EventChannel_var ec =
          client.create_event_channel ("NotifyEventChannelFactory",
                                       1
                                       ACE_ENV_ARG_PARAMETER);


        CosNotifyChannelAdmin::AdminID adminid = 0;
        CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin =
          ec->new_for_consumers (CosNotifyChannelAdmin::AND_OP,
                                 adminid
                                 ACE_ENV_ARG_PARAMETER);

        ACE_TRY_CHECK;

        // Filter Section

        if (filter)
          {
            CosNotifyFilter::FilterFactory_var ffact =
              ec->default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;

            CosNotifyFilter::Filter_var filter =
              ffact->create_filter (GRAMMAR ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            if (CORBA::is_nil (filter.in ()))
              {
                ACE_ERROR ((LM_ERROR,
                            " (%P|%t) Consumer unable to initialize filter.\n"));
                return 1;
              }

            CosNotifyFilter::ConstraintExpSeq constraint_list (1);
            constraint_list.length (1);

            constraint_list[0].event_types.length (0);
            constraint_list[0].constraint_expr = CORBA::string_dup ("Number == 100");

            filter->add_constraints (constraint_list ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            // apply filter
            consumer_admin->add_filter (filter.in ());
          }

        CosNotification::EventTypeSeq added (1);
        CosNotification::EventTypeSeq removed (0);

        added.length (1);

        removed.length (0);

        added[0].domain_name = CORBA::string_dup ("*");
        added[0].type_name = CORBA::string_dup ("*");

        consumer_admin->subscription_change (added, removed ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        // End Filter Section

        // start server
        CosNaming::Name ctrl_name;
        ctrl_name.length (1);
        ctrl_name[0].id = CORBA::string_dup ("Supplier");

        CosNaming::NamingContext_ptr naming_context = client.naming_context ();
        CORBA::Object_var ctrl_obj = naming_context->resolve (ctrl_name
                                                              ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (CORBA::is_nil (ctrl_obj.in ()))
          {
            ACE_ERROR ((LM_ERROR,
                        "(%P|%T) Consumer unable to find "
                        "Supplier Naming Context.\n"));
            return 1;
          }

        supplier_ctrl_var supplier = supplier_ctrl::_narrow (ctrl_obj.in ());

        if (!CORBA::is_nil (consumer_admin.in ()))
          {
            for ( int ctr = 0; ctr < num_consumers; ctr ++)
              create_consumers (consumer_admin.in (), &client ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

            // Start Supplier
            supplier->ctrl (1, num_events);

            // wait for Supplier
            ACE_OS::sleep (5);

            CORBA::ORB_ptr orb = client.orb ();
            while (!client.done ())
              {
                if (orb->work_pending ())
                  {
                    orb->perform_work ();

                    if (filter)
                      {
                        if ((num_events / 3 * num_consumers) ==
                            Notify_Push_Consumer::get_count ())
                          client.done () = 1;
                      }
                    else
                      {
                        if (num_events * num_consumers == Notify_Push_Consumer::get_count ())
                          client.done () = 1;
                      }

                    ACE_DEBUG ((LM_DEBUG,
                                "Consumer received event #%d\n",
                                Notify_Push_Consumer::get_count ()));
                  }
              }

            // Are there any more events
            num_events = Notify_Push_Consumer::get_count ();

            ACE_OS::sleep (5);
            if (orb->work_pending ())
              orb->perform_work ();

            if (num_events != Notify_Push_Consumer::get_count ())
              status = 1;

            // cleanup
            ec->destroy ();
            orb->destroy ();
          }
      }
  }

  ACE_CATCH (CORBA::Exception, e)
    {
      ACE_PRINT_EXCEPTION (e, "Consumer Exception:");
      status =  1;
    }
  ACE_ENDTRY;

  return status;
}

