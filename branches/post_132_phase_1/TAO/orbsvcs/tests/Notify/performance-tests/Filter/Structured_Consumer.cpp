// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/Get_Opt.h"

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "Notify_Structured_Push_Consumer.h"
#include "goC.h"
#include "tao/debug.h"
#include "Notify_Test_Client.h"

// ******************************************************************
// Data Section
// ******************************************************************

static const char* ior = "file://supplier.ior";
static CORBA::Boolean done = 0;
static CORBA::Boolean dummy = 0;
static CORBA::Boolean filter = 0;
static unsigned int consumers = 2;
static unsigned int expected = 1000;

#define GRAMMAR "TCL"

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
        consumers = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'e':
        expected = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'f':
        filter = 1;
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-c <# of consumers> "
                           "-e <expected events> "
                           "-f "
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

  if (filter)
    {
      // Filter Section
      CosNotifyFilter::FilterFactory_var ffact =
          ec->default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      // One Filter
      CosNotifyFilter::Filter_var filter =
          ffact->create_filter (GRAMMAR ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CosNotifyFilter::ConstraintExpSeq constraint_list (1);
      constraint_list.length (1);

      constraint_list[0].event_types.length (0);
      constraint_list[0].constraint_expr =
                                 CORBA::string_dup ("type == 'even'");

      filter->add_constraints (constraint_list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      admin->add_filter (filter.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
      // End One Filter
    }

  return CosNotifyChannelAdmin::ConsumerAdmin::_duplicate (admin.in ());
}


static void
create_consumers (CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
                  Notify_Test_Client* client
                  ACE_ENV_ARG_DECL)
{
  for(unsigned int i = 0; i < consumers; i++)
    {
      // startup the first consumer
      char name[64] = "";
      ACE_OS::sprintf(name, "consumer%u", i);
      Notify_Structured_Push_Consumer* consumer;
      ACE_NEW_THROW_EX (consumer,
                        Notify_Structured_Push_Consumer (
                                         name,
                                         expected,
                                         (i + 1 == consumers ? done : dummy)),
                        CORBA::NO_MEMORY ());

      consumer->init (client->root_poa () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      consumer->connect (admin ACE_ENV_ARG_PARAMETER);
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
          // If we are filtering, we will get half as many as originally
          // expected
          if (filter)
            {
              expected /= 2;
            }

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
              create_consumers(admin.in (), &client ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // Tell the supplier to go
              sig->go (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              ACE_Time_Value now = ACE_OS::gettimeofday ();
              while (!done)
                {
                  if (orb->work_pending ())
                    {
                      orb->perform_work ();
                    }
                }
              ACE_Time_Value then = ACE_OS::gettimeofday ();

              static const unsigned int per = 100;
              ACE_Time_Value difference = then - now;

              if (TAO_debug_level)
                ACE_DEBUG((LM_DEBUG,
                           "Total time: %d seconds\n", difference.sec ()));

              double denominator = per / (double)expected;
              difference *= denominator;

              if (TAO_debug_level)
                ACE_DEBUG((LM_DEBUG,
                           "Average of %ds %dus for %u events\n",
                           difference.sec (), difference.usec (), per));
            }
        }
    }
  ACE_CATCH (CORBA::Exception, e)
    {
      ACE_PRINT_EXCEPTION (e,
                           "Consumer exception: ");
      status = 1;
    }
  ACE_ENDTRY;

  return status;
}
