// $Id$

// ============================================================================
//
// = FILENAME
//    Event_Con.cpp
//
// = DESCRIPTION
//   This demo just tests the basic functionality of the Event Service
//   One Conumer which inherits from the Rtec Consumer.  One Supplier
//   with an internal Rtec Consumer and one internal Rtec Supplier.
//   The internal Supplier is just a demo supplier because the
//   architecture expects an supplier which has inherited from the
//   Rtec Supplier.
//
// = AUTHOR
//    originally
//    David Levine (levine@cs.wustl.edu) and
//    Tim Harrison (harrison@cs.wustl.edu)
//    modified
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include <limits.h>

#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Profile_Timer.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "Event_Con.h"
#include "NavWeapC.h"

ACE_RCSID(Event_Supplier, Event_Con, "$Id$")

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
  ACE_TRY_NEW_ENV
    {
      // Get a Scheduler.

      RtecScheduler::Scheduler_ptr server =
        ACE_Scheduler_Factory::server ();

      // Define Real-time information.

      rt_info_ = server->create (my_name, ACE_TRY_ENV);

      server->set (rt_info_,
                   RtecScheduler::VERY_LOW_CRITICALITY,
                   ORBSVCS_Time::zero (),
                   ORBSVCS_Time::zero (),
                   ORBSVCS_Time::zero (),
                   2500000,
                   RtecScheduler::VERY_LOW_IMPORTANCE,
                   ORBSVCS_Time::zero (),
                   1,
                   RtecScheduler::OPERATION,
                   ACE_TRY_ENV);


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
        channel_admin_->for_consumers (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Obtain a pointer to a push supplier.  "suppliers" is
      // inherited from a base class.

      this->suppliers_ =
        consumer_admin_->obtain_push_supplier (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // The _this function returns an object pointer. This is needed
      // because a consumer inherits from a Servant class that is no
      // CORBA::Object.

      RtecEventComm::PushConsumer_var objref =
        this->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->suppliers_->connect_push_consumer (objref.in (),
                                               dependencies.get_ConsumerQOS (),
                                               ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (RtecEventChannelAdmin::EventChannel::SUBSCRIPTION_ERROR, se)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Demo_Consumer::open: subscribe failed.\n"),
                        -1);
    }
  ACE_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Demo_Consumer::open: unexpected exception.\n"),
                        -1);
    }
  ACE_ENDTRY;

  return 0;
}

void
Demo_Consumer::disconnect_push_consumer (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "Consumer received disconnect from channel.\n"));
}

void
Demo_Consumer::push (const RtecEventComm::EventSet &events,
                     CORBA::Environment &ACE_TRY_ENV)
{

  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }

  cout << "Number of events: " << received++ << endl;

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

          ACE_TRY
            {
              cout << "ID: " << events[i].data.any_value.type()->id(ACE_TRY_ENV) << endl;
              ACE_TRY_CHECK;
              cout << "Name: " << events[i].data.any_value.type()->name(ACE_TRY_ENV) << endl;
              ACE_TRY_CHECK;
              cout << "member_count: " << events[i].data.any_value.type()->member_count(ACE_TRY_ENV) << endl;
              ACE_TRY_CHECK;
              cout << "TCKind: " << events[i].data.any_value.type()->kind(ACE_TRY_ENV) << endl;
              ACE_TRY_CHECK;

              if (_tc_Navigation->equal (events[i].data.any_value.type(), ACE_TRY_ENV))
                {
                  ACE_TRY_CHECK;
                  Navigation *navigation_ = (Navigation*) events[i].data.any_value.value ();

                  cout << "Found a Navigation struct in the any: pos_lat = " << navigation_->position_latitude << endl;
                }
              else if (_tc_Weapons->equal (events[i].data.any_value.type(), ACE_TRY_ENV))
                {
                  ACE_TRY_CHECK;
                  Weapons *weapons_ = (Weapons*) events[i].data.any_value.value ();

                  cout << "Found a Navigation struct in the any: pos_lat = " << weapons_->number_of_weapons << endl;
                }


            }
          ACE_CATCHANY
            {
              ACE_ERROR ((LM_ERROR, "(%t)Error in extracting the Navigation and Weapons data.\n"));
            }
          ACE_ENDTRY;
        }
    }
}

void
Demo_Consumer::shutdown (void)
{
  ACE_TRY_NEW_ENV
    {
      // Disconnect from the push supplier.

      this->suppliers_->disconnect_push_supplier (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "@@ we should shutdown here!!!\n"));
      ACE_TRY_CHECK;

      TAO_ORB_Core_instance ()->orb ()->shutdown ();
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,
                 "(%t) Demo_Consumer::shutdown: unexpected exception.\n"));
    }
  ACE_ENDTRY;
}

// function get_options

static unsigned int
get_options (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "Oc:djm:s:t:?");
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

  if (argc != get_opt.optind)
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
main (int argc, char *argv [])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize ORB.

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "internet", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (),
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_Scheduler_Factory::use_config (naming_context.in ());

      if (get_options (argc, argv))
        ACE_OS::exit (-1);

      // Get the Event Channel.

      CosNaming::Name channel_name (1);
      channel_name.length (1);
      channel_name[0].id = CORBA::string_dup ("EventService");

      CORBA::Object_var ec_obj =
        naming_context->resolve (channel_name,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow (ec_obj.in(), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (ec.ptr() == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Not able to get the Event Service reference.\n"),
                          -1);

      // Create consumer.

      Demo_Consumer *demo_consumer;
      ACE_NEW_RETURN (demo_consumer,
                      Demo_Consumer (),
                      -1);

      if (demo_consumer->open_consumer (ec.ptr (),
                                        "demo_consumer") == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Someone was feeling introverted.\n"),
                          -1);

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Run the ORB

      if (orb->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "CORBA::ORB::run"),
                          -1);
      ACE_TRY_CHECK;

      delete demo_consumer;

      root_poa->destroy (1,
                         1,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "SYS_EX");
    }
  ACE_ENDTRY;

  return 0;
}
