
//=============================================================================
/**
 *  @file    Event_Con.cpp
 *
 *  $Id$
 *
 * This demo just tests the basic functionality of the Event Service
 * One Conumer which inherits from the Rtec Consumer.  One Supplier
 * with an internal Rtec Consumer and one internal Rtec Supplier.
 * The internal Supplier is just a demo supplier because the
 * architecture expects an supplier which has inherited from the
 * Rtec Supplier.
 *
 *
 *  @author originally David Levine (levine@cs.wustl.edu) and Tim Harrison (harrison@cs.wustl.edu) modified Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================


#include "Event_Con.h"
#include "NavWeapC.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"

#include "tao/ORB_Core.h"
#include "tao/AnyTypeCode/TypeCode.h"

#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Profile_Timer.h"
#include "ace/streams.h"

#include "ace/os_include/os_limits.h"

static const char usage [] =
"[-? |\n"
"            [-c <consumers> [4]]\n"
"            [-d directly connect all consumers/suppliers\n"
"            [-j to collect jitter statistics]\n"
"            [-m <count> of messages to send [10]]\n"
"            [-s <suppliers>, [1]]\n"
"            [-t <timeout interval>, msec [250]]]";

static int received = 0;

// ************************************************************

Demo_Consumer::Demo_Consumer (void)
{
}

int
Demo_Consumer::open_consumer (RtecEventChannelAdmin::EventChannel_ptr ec,
                              const char *my_name)
{
  try
    {
      // Get a Scheduler.

      RtecScheduler::Scheduler_ptr server =
        ACE_Scheduler_Factory::server ();

      // Define Real-time information.
      rt_info_ = server->create (my_name);

      server->set (rt_info_,
                   RtecScheduler::VERY_LOW_CRITICALITY,
                   ORBSVCS_Time::zero (),
                   ORBSVCS_Time::zero (),
                   ORBSVCS_Time::zero (),
                   2500000,
                   RtecScheduler::VERY_LOW_IMPORTANCE,
                   ORBSVCS_Time::zero (),
                   1,
                   RtecScheduler::OPERATION);


      // Create the event that we're registering for.

      ACE_ConsumerQOS_Factory dependencies;
      dependencies.start_disjunction_group ();
      dependencies.insert_type (ACE_ES_EVENT_NOTIFICATION, rt_info_);
      dependencies.insert_type (ACE_ES_EVENT_SHUTDOWN, rt_info_);

      // The channel administrator is the event channel we got from
      // the invocation of this routine.

      this->channel_admin_ = ec;

      // = Connect as a consumer.

      this->consumer_admin_ =
        channel_admin_->for_consumers ();

      // Obtain a pointer to a push supplier.  "suppliers" is
      // inherited from a base class.

      this->suppliers_ =
        consumer_admin_->obtain_push_supplier ();

      // The _this function returns an object pointer. This is needed
      // because a consumer inherits from a Servant class that is no
      // CORBA::Object.

      RtecEventComm::PushConsumer_var objref =
        this->_this ();

      this->suppliers_->connect_push_consumer (objref.in (),
                                               dependencies.get_ConsumerQOS ());
    }
  catch (const RtecEventChannelAdmin::EventChannel::SUBSCRIPTION_ERROR& )
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Demo_Consumer::open: subscribe failed.\n"),
                        -1);
    }
  catch (const CORBA::Exception&)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Demo_Consumer::open: unexpected exception.\n"),
                        -1);
    }
  return 0;
}

void
Demo_Consumer::disconnect_push_consumer (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Consumer received disconnect from channel.\n"));
}

void
Demo_Consumer::push (const RtecEventComm::EventSet &events)
{

  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }

  ACE_DEBUG ((LM_DEBUG, "Number of events: %d\n", received));
  ++received;

  for (CORBA::ULong i = 0; i < events.length (); ++i)
    {
      if (events[i].header.type == ACE_ES_EVENT_SHUTDOWN)
        {
          ACE_DEBUG ((LM_DEBUG, "Demo Consumer: received shutdown event\n"));
          this->shutdown ();
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Demo Consumer: received ACE_ES_EVENT_NOTIFICATION event.\n"));

          try
            {
              // Use a temporary int to avoid overload ambiguities with
              // the enum.
              int kind = events[i].data.any_value.type()->kind ();

              ACE_DEBUG ((LM_DEBUG, "ID: %s\n", events[i].data.any_value.type()->id()));
              ACE_DEBUG ((LM_DEBUG, "Name: %s\n", events[i].data.any_value.type()->name()));
              ACE_DEBUG ((LM_DEBUG, "member_count: %u\n", events[i].data.any_value.type()->member_count()));
              ACE_DEBUG ((LM_DEBUG, "TCKind: %d\n", kind));

              CORBA::Boolean ret =
                _tc_Navigation->equal (events[i].data.any_value.type());

              if (ret)
                {
                  const Navigation *navigation_ = 0;
                  events[i].data.any_value >>= navigation_;
                  ACE_DEBUG ((LM_DEBUG, "Found a Navigation struct in the any: pos_lat = %d\n", navigation_->position_latitude));
                }
              else
                {
                  ret = _tc_Weapons->equal (events[i].data.any_value.type());

                  if (ret)
                    {
                      const Weapons *weapons_ = 0;
                      events[i].data.any_value >>= weapons_;
                      ACE_DEBUG ((LM_DEBUG, "Found a Weapons struct in the any: nr_of_weapons = %u\n", weapons_->number_of_weapons));
                    }
                }
            }
          catch (const CORBA::Exception&)
            {
              ACE_ERROR ((LM_ERROR, "(%t)Error in extracting the Navigation and Weapons data.\n"));
            }
        }
    }
}

void
Demo_Consumer::shutdown (void)
{
  try
    {
      // Disconnect from the push supplier.

      this->suppliers_->disconnect_push_supplier ();

      ACE_DEBUG ((LM_DEBUG, "@@ we should shutdown here!!!\n"));

      TAO_ORB_Core_instance ()->orb ()->shutdown ();
    }
  catch (const CORBA::Exception&)
    {
      ACE_ERROR ((LM_ERROR,
                 "(%t) Demo_Consumer::shutdown: unexpected exception.\n"));
    }
}

// function get_options

static unsigned int
get_options (int argc, ACE_TCHAR *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("Oc:djm:s:t:?"));
  int opt;

  while ((opt = get_opt ()) != EOF)
  {
    switch (opt)
      {
      case '?':
        ACE_DEBUG ((LM_DEBUG,
                    "Usage: %s %s\n",
                    argv[0], usage));
        ACE_OS::exit (0);
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s: unknown arg, -%c\n"
                           "Usage: %s %s\n",
                           argv[0], char(opt),
                           argv[0], usage), 1);
      }
  }

  if (argc != get_opt.opt_ind ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s: too many arguments\n"
                       "Usage: %s %s\n",
                       argv[0],
                       argv[0],
                       usage),
                      1);
  return 0;
}

// function main.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize ORB.

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "internet");

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      ACE_Scheduler_Factory::use_config (naming_context.in ());

      if (get_options (argc, argv))
        ACE_OS::exit (-1);

      // Get the Event Channel.

      CosNaming::Name channel_name (1);
      channel_name.length (1);
      channel_name[0].id = CORBA::string_dup ("EventService");

      CORBA::Object_var ec_obj =
        naming_context->resolve (channel_name);

      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow (ec_obj.in());

      if (CORBA::is_nil (ec.in()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Not able to get the Event Service reference.\n"),
                          -1);

      // Create consumer.

      Demo_Consumer *demo_consumer = 0;
      ACE_NEW_RETURN (demo_consumer,
                      Demo_Consumer (),
                      -1);

      if (demo_consumer->open_consumer (ec.in (),
                                        "demo_consumer") == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Someone was feeling introverted.\n"),
                          -1);

      poa_manager->activate ();

      // Run the ORB

      orb->run ();

      delete demo_consumer;

      root_poa->destroy (1,
                         1);

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("SYS_EX");
    }

  return 0;
}
