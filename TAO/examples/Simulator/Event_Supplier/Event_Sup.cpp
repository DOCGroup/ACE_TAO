// $Id$
//
// ============================================================================
//
// 
// = FILENAME
//    Event_Sup.cpp
//
// = AUTHOR
//    originally
//    David Levine (levine@cs.wustl.edu) and
//    Tim Harrison (harrison@cs.wustl.edu)
//    modified
//    Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//   Event Supplier for the flight simulator
//
// ============================================================================


#include <limits.h>


#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Profile_Timer.h"
#include "ace/OS.h"

#include "tao/Timeprobe.h"
#include "tao/corba.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "Event_Sup.h"
#include "NavWeapC.h"

#define SOURCE_ID    1001

static const char usage [] = "[-? |\n"
"            [-m <count> of messages to send [100]]]";

unsigned int total_messages = 100;



// ------------ Internal_Demo_Consumer ---------------------------------



Demo_Supplier::Internal_Demo_Consumer::Internal_Demo_Consumer (Demo_Supplier *impl)
  : impl_ (impl)
{
}


void 
Demo_Supplier::Internal_Demo_Consumer::push (const RtecEventComm::EventSet &events,
			       CORBA::Environment &env)
{ 
  this->impl_->push (events, env); 
  ACE_DEBUG ((LM_DEBUG, "Internal Demo Consumer received push.\n"));
}

void 
Demo_Supplier::Internal_Demo_Consumer::disconnect_push_consumer (CORBA::Environment &) { }


// -------------------- Internal Demo Supplier -----------------------------

Demo_Supplier::Internal_Demo_Supplier::Internal_Demo_Supplier (Demo_Supplier *impl)
  : impl_ (impl)
{
}

void 
Demo_Supplier::Internal_Demo_Supplier::disconnect_push_supplier (CORBA::Environment &) { }



// ----------------------------------------------------------------------------


Demo_Supplier::Demo_Supplier (unsigned int supplier_id)
  : supplier_id_ (supplier_id),
    internal_demo_consumer_ (new Internal_Demo_Consumer (this)),
    internal_demo_supplier_ (new Internal_Demo_Supplier (this))
{
}

Demo_Supplier::~Demo_Supplier (void)
{
  delete internal_demo_consumer_;
  delete internal_demo_supplier_;
}

int
Demo_Supplier::open_supplier (RtecEventChannelAdmin::EventChannel_ptr ec,
				 const char *name)
{

  TAO_TRY
    {
      this->channel_admin_ = ec;

      // Get a Scheduling  server

      RtecScheduler::Scheduler_ptr scheduler_ = ACE_Scheduler_Factory::server ();


      // Generate the Real-time information

      this->rt_info_ = scheduler_->create (name, TAO_TRY_ENV);
      
      scheduler_->set (rt_info_, 
		       ORBSVCS_Time::zero, 
		       ORBSVCS_Time::zero, 
		       ORBSVCS_Time::zero, 
		       2500000,
		       RtecScheduler::VERY_LOW,
		       ORBSVCS_Time::zero, 
		       1,
		       TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Set the publications to report them to the event channel

      CORBA::Short x = 0;
      RtecEventChannelAdmin::SupplierQOS qos_;
      qos_.publications_.length (2);
      qos_.publications_[0].event_.source_ = SOURCE_ID;
      qos_.publications_[0].event_.type_ = ACE_ES_EVENT_NOTIFICATION;
      qos_.publications_[0].event_.ttl_ = 1;
      qos_.publications_[0].event_.creation_time_ = ORBSVCS_Time::zero; // default values
      qos_.publications_[0].event_.ec_recv_time_ = ORBSVCS_Time::zero;
      qos_.publications_[0].event_.ec_send_time_ = ORBSVCS_Time::zero;
      qos_.publications_[0].event_.data_.x = 0;
      qos_.publications_[0].event_.data_.y = 0;
      qos_.publications_[0].event_.data_.any_value.replace (CORBA::_tc_short, &x, 0, TAO_TRY_ENV); 
      qos_.publications_[0].dependency_info_.number_of_calls = 1;
      qos_.publications_[0].dependency_info_.rt_info = rt_info_;
      qos_.publications_[1].event_.source_ = SOURCE_ID;
      qos_.publications_[1].event_.type_ = ACE_ES_EVENT_SHUTDOWN;
      qos_.publications_[1].event_.ttl_ = 1;
      qos_.publications_[1].event_.creation_time_ = ORBSVCS_Time::zero; //default values
      qos_.publications_[1].event_.ec_recv_time_ = ORBSVCS_Time::zero;
      qos_.publications_[1].event_.ec_send_time_ = ORBSVCS_Time::zero;
      qos_.publications_[1].event_.data_.x = 0;
      qos_.publications_[1].event_.data_.y = 0;
      qos_.publications_[1].event_.data_.any_value.replace (CORBA::_tc_short, &x, 0, TAO_TRY_ENV);
      qos_.publications_[1].dependency_info_.number_of_calls = 1;
      qos_.publications_[1].dependency_info_.rt_info = rt_info_;


      // = Connect as a supplier.
      this->supplier_admin_ =
	channel_admin_->for_suppliers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

          this->proxy_consumer_ = supplier_admin_->obtain_push_consumer (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // In calling _this we get back an object reference and register the servant with 
      // the POA
      RtecEventComm::PushSupplier_var objref = this->internal_demo_supplier_->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Connect the supplier to the proxy consumer
      this->proxy_consumer_->connect_push_supplier (objref.in (),
						    qos_,
						    TAO_TRY_ENV);
      TAO_CHECK_ENV;

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Demo_Supplier::open");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}



void
Demo_Supplier::disconnect_push_supplier (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG, "Supplier received disconnect from channel.\n"));
}




void
Demo_Supplier::start_generating_events (void)
{
  ACE_Time_Value pause (0, 500000);
  unsigned int total_sent_ = 0;

  TAO_TRY
    {
      do {
	      RtecEventComm::Event event1, event2;
	      event1.source_ = SOURCE_ID;
	      event1.type_ = ACE_ES_EVENT_NOTIFICATION;
	      event1.ttl_ = 1;
	      event1.creation_time_ = ORBSVCS_Time::zero;
	      event1.ec_recv_time_ = ORBSVCS_Time::zero;
	      event1.ec_send_time_ = ORBSVCS_Time::zero;
	      event1.data_.x = 0;
	      event1.data_.y = 0;
	      event2.source_ = SOURCE_ID;
	      event2.type_ = ACE_ES_EVENT_NOTIFICATION;
	      event2.ttl_ = 1;
	      event2.creation_time_ = ORBSVCS_Time::zero;
	      event2.ec_recv_time_ = ORBSVCS_Time::zero;
	      event2.ec_send_time_ = ORBSVCS_Time::zero;
	      event2.data_.x = 0;
	      event2.data_.y = 0;	      // Send one time navigation data and one time weapons data

	      this->insert_event_data (event1.data_.any_value, event2.data_.any_value);
	      
	      RtecEventComm::EventSet events;
	      events.length (1);
	      
	      if ((total_sent_%2) == 0)
		events[0] = event1;
	      else
		events[0] = event2;
	      
	      //events[1] = event2; // two events are not supported right now

	      proxy_consumer_->push (events, TAO_TRY_ENV);
	      TAO_CHECK_ENV;
	      
	      ACE_DEBUG ((LM_DEBUG, "Pushing event data.\n"));

	      ACE_OS::sleep (pause);

      } while (++total_sent_ < total_messages);

 
      // Sending a shutdown event -- not wanted right now
      /*  RtecEventComm::Event event;
      event.source_ = SOURCE_ID;
      event.type_ = ACE_ES_EVENT_SHUTDOWN;
      event.ttl_ = 1;
      event.creation_time_ = ORBSVCS_Time::zero;
      event.ec_recv_time_ = ORBSVCS_Time::zero;
      event.ec_send_time_ = ORBSVCS_Time::zero;
      event.data_.x = 0;
      event.data_.y = 0;

      RtecEventComm::EventSet events;
      events.length (1);
      events[0] = event;
      TAO_CHECK_ENV;

      proxy_consumer_->push (events, TAO_TRY_ENV);
      TAO_CHECK_ENV; */

    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, 
                         "Latency_Supplier::generate_events:"
                         " unexpected exception.\n"));
    }
  TAO_ENDTRY;
  this->shutdown (); 
}


// This function fills in the random data into the anys transported by 
// the event channel
void
Demo_Supplier::insert_event_data (CORBA::Any &data1, CORBA::Any &data2)
{
  TAO_TRY
    {
      navigation_.position_latitude = ACE_OS::rand() % 90;
      navigation_.position_longitude = ACE_OS::rand() % 180;
      navigation_.altitude = ACE_OS::rand() % 100;
      navigation_.heading = ACE_OS::rand() % 180;
      navigation_.roll = (navigation_.roll == 180) ? -180 : navigation_.roll + 1;
      navigation_.pitch =  (navigation_.pitch == 90) ? -90 : navigation_.pitch + 1;
      navigation_.utilitzation = ((double)(ACE_OS::rand() %50))/50.0;
      navigation_.overhead = ((double)(ACE_OS::rand() %50))/50.0;
      navigation_.arrival_time = ACE_OS::rand();
      navigation_.deadline_time = navigation_.arrival_time + (ACE_OS::rand() % 1000);
      navigation_.dispatch_time = navigation_.arrival_time + (ACE_OS::rand() % 1000);


	
      weapons_.number_of_weapons = 2;
      weapons_.weapon1.identifier = CORBA::string_alloc (30);
      strcpy (weapons_.weapon1.identifier,"Photon Torpedoes");
      weapons_.weapon1.status =(ACE_OS::rand() % 4) == 0 ? 0 : 1 ;
      weapons_.weapon2.identifier = CORBA::string_alloc (30);
      strcpy (weapons_.weapon2.identifier,"Quantum Torpedoes");
      weapons_.weapon2.status = (ACE_OS::rand() % 4) == 0 ? 0 : 1;
      weapons_.weapon3.identifier = 0;
      weapons_.weapon2.status = 0;
      weapons_.weapon4.identifier = 0;
      weapons_.weapon3.status = 0; 
      weapons_.weapon5.identifier = 0;	    
      weapons_.weapon4.status = 0;

      weapons_.utilitzation = ((double)(ACE_OS::rand() %50))/50.0;
      weapons_.overhead = ((double)(ACE_OS::rand() %50))/50.0;
      weapons_.arrival_time = ACE_OS::rand();
      weapons_.deadline_time = weapons_.arrival_time + (ACE_OS::rand() % 1000);
      weapons_.dispatch_time = weapons_.arrival_time + (ACE_OS::rand() % 1000);

      data1.replace (_tc_Navigation, &navigation_, CORBA::B_TRUE, TAO_TRY_ENV);
      data2.replace (_tc_Weapons, &weapons_, CORBA::B_TRUE, TAO_TRY_ENV);
    } 
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "(%t)Error in Demo_Supplier::insert_event_data.\n"));
    }
  TAO_ENDTRY;
}

void 
Demo_Supplier::push (const RtecEventComm::EventSet &events,
			       CORBA::Environment &env)
{ 

}

void
Demo_Supplier::shutdown (void)
{

  TAO_TRY
    {
      // @@ Had problems with the next command, the application got stuck in it
      // channel_admin_->destroy (TAO_TRY_ENV);
      // TAO_CHECK_ENV;
      
      TAO_ORB_Core_instance ()->orb ()->shutdown ();
      TAO_CHECK_ENV;
     
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "(%t) Demo_Supplier::shutdown:"
                  " unexpected exception.\n"));
      TAO_TRY_ENV.print_exception ("Demo_Supplier::shutdown");
    }
  TAO_ENDTRY;
}




// function get_options

static unsigned int
get_options (int argc, char *argv [])
{
  // We need the 'O' in get_opt() because we also want to have ORB parameters, they
  // all start with 'O'
  ACE_Get_Opt get_opt (argc, argv, "O?m:");
  int opt;
  int temp;

  while ((opt = get_opt ()) != EOF)
  {
    switch (opt) {
    case '?':
      ACE_DEBUG ((LM_DEBUG,
		  "Usage: %s %s\n",
		  argv[0], usage));
      ACE_OS::exit (0);
      break;
    case 'm':
      if ((temp = ACE_OS::atoi (get_opt.optarg)) > 0)
        {
          total_messages = (u_int) temp;
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
			     "%s: count must be > 0",
			     argv[0]), 1);
        }
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
    {
      ACE_ERROR_RETURN ((LM_ERROR,
			 "%s: too many arguments\n"
			 "Usage: %s %s\n",
			 argv[0], argv[0], usage), 1);
    }

  return 0;
}



// function main

int
main (int argc, char *argv [])
{

  TAO_TRY
    {
      // Initialize ORB.

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "internet", TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Connect to the RootPOA
      CORBA::Object_var poa_object = orb->resolve_initial_references("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     " (%P|%t) Unable to initialize the POA.\n"),
			    1);
	}
	  
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (poa_object.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Get the Naming Service object reference
      CORBA::Object_var naming_obj = orb->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     " (%P|%t) Unable get the Naming Service.\n"),
			    1);
	}

      CosNaming::NamingContext_var naming_context = 
	CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;


      // Tell the ScheduleService to use the same naming service
      ACE_Scheduler_Factory::use_config (naming_context.in ());
      
      if (get_options (argc, argv))
	ACE_OS::exit (-1);

      // Get the Event Channel
      // @@ If you specify an invalid name, the Name Service crashes

      CosNaming::Name channel_name (1);
      channel_name.length (1);
      channel_name[0].id = CORBA::string_dup ("EventService");
      
      CORBA::Object_var ec_obj = naming_context->resolve (channel_name, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventChannelAdmin::EventChannel_var ec =
	RtecEventChannelAdmin::EventChannel::_narrow (ec_obj.in(), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Create the demo supplier
      Demo_Supplier *demo_supplier;
      ACE_NEW_RETURN (demo_supplier, Demo_Supplier (SOURCE_ID), -1);

      if (demo_supplier->open_supplier (ec.ptr (), "demo_supplier") == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "Supplier open failed.\n"), -1);
      

      // Register the internal demo consumer for timeout events
      demo_supplier->start_generating_events ();
      
      ACE_DEBUG ((LM_DEBUG, "Supplier is registered for timeout events.\n"));

      // The POA Manager has to be activated before starting the ORB event loop
      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Run the ORB

      if (orb->run () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CORBA::ORB::run"), -1);
      TAO_CHECK_ENV;


      delete demo_supplier;
      TAO_CHECK_ENV;

    }
  TAO_CATCH (RtecEventComm::Disconnected, d)
    {
      ACE_ERROR ((LM_ERROR, "(%t) Main:  Demo_Supplier::push: disconnected.\n"));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;


  return 0;
}
