// $Id$

// ============================================================================
//
// = FILENAME
//    Event_Sup.cpp
//
// = DESCRIPTION
//   Event Supplier for the flight simulator
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
#include "ace/OS.h"
#include "ace/Timeprobe.h"
#include "tao/corba.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "Event_Sup.h"
#include "NavWeapC.h"

const int SOURCE_ID = 1001;

static const char usage [] = 
"[-? |\n"
"            [-O[RBport] ORB port number]"
"            [-m <count> of messages to send [100]"
"            [-f name of schedler input data file]]";

static u_int total_messages = 100;

static char *input_file_name = 0;

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

  ACE_DEBUG ((LM_DEBUG,
              "Internal Demo Consumer received push.\n"));
}

void
Demo_Supplier::Internal_Demo_Consumer::disconnect_push_consumer (CORBA::Environment &) 
{ 
}

// -------------------- Internal Demo Supplier -----------------------------

Demo_Supplier::Internal_Demo_Supplier::Internal_Demo_Supplier (Demo_Supplier *impl)
  : impl_ (impl)
{
}

void
Demo_Supplier::Internal_Demo_Supplier::disconnect_push_supplier (CORBA::Environment &) 
{ 
}

// ----------------------------------------------------------------------------

Demo_Supplier::Demo_Supplier (u_int supplier_id)
  : supplier_id_ (supplier_id),
    internal_demo_consumer_ (new Internal_Demo_Consumer (this)),
    internal_demo_supplier_ (new Internal_Demo_Supplier (this)) 
{
    // this is neccessary because they are not initialized by the comiler
   navigation_.roll = 0;
   navigation_.pitch = 0;
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

      // Get a Scheduling server.

      RtecScheduler::Scheduler_ptr scheduler_ =
        ACE_Scheduler_Factory::server ();

      // Generate the Real-time information.

      this->rt_info_ = scheduler_->create (name,
                                           TAO_TRY_ENV);
      scheduler_->set (rt_info_,
                       RtecScheduler::VERY_LOW_CRITICALITY,
                       ORBSVCS_Time::zero,
                       ORBSVCS_Time::zero,
                       ORBSVCS_Time::zero,
                       2500000,
                       RtecScheduler::VERY_LOW_IMPORTANCE,
                       ORBSVCS_Time::zero,
                       1,
                       RtecScheduler::OPERATION,
                       TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Set the publications to report them to the event channel.

      CORBA::Short x = 0;
      RtecEventChannelAdmin::SupplierQOS qos_;
      qos_.publications_.length (2);
      qos_.publications_[0].event_.source_ = SOURCE_ID;
      qos_.publications_[0].event_.type_ = ACE_ES_EVENT_NOTIFICATION;
      qos_.publications_[0].event_.ttl_ = 1;
      qos_.publications_[0].event_.creation_time_ = ORBSVCS_Time::zero; 
      // default values
      qos_.publications_[0].event_.ec_recv_time_ = ORBSVCS_Time::zero;
      qos_.publications_[0].event_.ec_send_time_ = ORBSVCS_Time::zero;
      qos_.publications_[0].event_.data_.x = 0;
      qos_.publications_[0].event_.data_.y = 0;
      qos_.publications_[0].event_.data_.any_value.replace (CORBA::_tc_short,
                                                            &x,
                                                            0,
                                                            TAO_TRY_ENV);
      qos_.publications_[0].dependency_info_.number_of_calls = 1;
      qos_.publications_[0].dependency_info_.rt_info = rt_info_;
      qos_.publications_[1].event_.source_ = SOURCE_ID;
      qos_.publications_[1].event_.type_ = ACE_ES_EVENT_SHUTDOWN;
      qos_.publications_[1].event_.ttl_ = 1;
      qos_.publications_[1].event_.creation_time_ = ORBSVCS_Time::zero; 
      // default values.
      qos_.publications_[1].event_.ec_recv_time_ = ORBSVCS_Time::zero;
      qos_.publications_[1].event_.ec_send_time_ = ORBSVCS_Time::zero;
      qos_.publications_[1].event_.data_.x = 0;
      qos_.publications_[1].event_.data_.y = 0;
      qos_.publications_[1].event_.data_.any_value.replace (CORBA::_tc_short,
                                                            &x,
                                                            0,
                                                            TAO_TRY_ENV);
      qos_.publications_[1].dependency_info_.number_of_calls = 1;
      qos_.publications_[1].dependency_info_.rt_info = rt_info_;

      // = Connect as a supplier.
      this->supplier_admin_ =
        channel_admin_->for_suppliers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->proxy_consumer_ =
        supplier_admin_->obtain_push_consumer (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // In calling _this we get back an object reference and register
      // the servant with the POA.

      RtecEventComm::PushSupplier_var objref =
        this->internal_demo_supplier_->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Connect the supplier to the proxy consumer.
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
  ACE_DEBUG ((LM_DEBUG,
              "Supplier received disconnect from channel.\n"));
}

void
Demo_Supplier::start_generating_events (void)
{
  u_int total_sent_ = 0;

  // Load the scheduling data for the simulation.
  ACE_Unbounded_Queue<Schedule_Viewer_Data *> schedule_data;
  this->load_schedule_data (schedule_data);
  ACE_Unbounded_Queue_Iterator<Schedule_Viewer_Data *>
    schedule_iter (schedule_data);

  if (schedule_iter.done ())
    {
      ACE_ERROR ((LM_ERROR,
                  "Demo_Supplier::start_generating_events: "
                  "there is no scheduling data\n"));
      return;
    }

  TAO_TRY
    {
      do 
        {
          RtecEventComm::Event event;
          event.source_ = SOURCE_ID;
          event.type_ = ACE_ES_EVENT_NOTIFICATION;
          event.ttl_ = 1;
          event.creation_time_ = ORBSVCS_Time::zero;
          event.ec_recv_time_ = ORBSVCS_Time::zero;
          event.ec_send_time_ = ORBSVCS_Time::zero;
          event.data_.x = 0;
          event.data_.y = 0;

          this->insert_event_data (event.data_.any_value,
                                   schedule_iter);

          RtecEventComm::EventSet events;
          events.length (1);
          events[0] = event;

          proxy_consumer_->push (events,
                                 TAO_TRY_ENV);
          TAO_CHECK_ENV;

          if (total_sent_ < 5)
            ACE_DEBUG ((LM_DEBUG,
                        "Pushing event data.\n"));
          else if (total_sent_ == 5)
            ACE_DEBUG ((LM_DEBUG,
                        "Everything is running. Going to be mute.\n"));
        } 
      while (++total_sent_ < total_messages);

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
      ACE_ERROR ((LM_ERROR, "Demo_Supplier::start_generating_events: "
                            " unexpected exception.\n"));
    }
  TAO_ENDTRY;

  // clean up the scheduling data
  Schedule_Viewer_Data **data_temp;
  for (schedule_iter.first ();
       schedule_iter.done () == 0;
       schedule_iter.advance ())
    if (schedule_iter.next (data_temp) && data_temp)
      delete (*data_temp);

  this->shutdown ();
}

void
Demo_Supplier::load_schedule_data 
  (ACE_Unbounded_Queue<Schedule_Viewer_Data *> &schedule_data)
{
  Schedule_Viewer_Data *data = 0;

  if (input_file_name)
    {
      // Open the scheduler data input file and read its contents into
      // a queue.
      FILE *input_file;

      int scan_count = 0;
      input_file = ACE_OS::fopen(input_file_name, "r");

      if (input_file)
        {
          // Get a line at a time from the data file and parse it.
          char input_buf[BUFSIZ];
          while (ACE_OS::fgets (input_buf, BUFSIZ, input_file))
            {
              // Run through leading whitespace.
              char *temp = input_buf;
              while (*temp && isspace (*temp))
                ++temp;

              // If there is anything besides whitespace in the line
              // read, scan its fields into the scheduling data
              // structure.
              if (ACE_OS::strlen (temp) > 0)
                {
                  ACE_NEW (data, Schedule_Viewer_Data);
                  scan_count = sscanf (temp, "%s %lf %lf %lu %lu %lu %lu",
                                       &(data->operation_name), &(data->utilitzation),
                                       &(data->overhead), &(data->arrival_time),
                                       &(data->deadline_time), &(data->completion_time),
                                       &(data->computation_time));
                  if (scan_count != 7)
                    {
                      ACE_ERROR ((LM_ERROR,
                                  "Demo_Supplier::start_generating_events: "
                                  "scanned incorrect number of data elements: %d\n", scan_count));

                      delete data;
                      return;
                    }

                  // Insert the data into the queue.
                  schedule_data.enqueue_tail (data);
                }
            }
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "Demo_Supplier::start_generating_events: "
                      "could not open input file [%s].\n",
                      input_file_name));
          return;
        }
    }
  else
  {
    u_long last_completion = 0;

    // Just create 10 dummy scheduling records and use them.
    for (int i = 0; i < 10; ++i)
    {
      ACE_NEW (data, Schedule_Viewer_Data);

      char *oper_name = 0;
      switch (i % 4)
      {
      case 0:
        oper_name = "high_20";
        break;

      case 1:
        oper_name = "low_20";
        break;

      case 2:
        oper_name = "high_10";
        break;

      case 3:
      default:
        oper_name = "low_10";
        break;
      }

      ACE_OS::strncpy (data->operation_name,
                       oper_name,
                       BUFSIZ-1);


      data->utilitzation = (double)(20.0+ACE_OS::rand() %10);
      data->overhead = (double)(ACE_OS::rand() %20);

      data->arrival_time = ACE_OS::rand() % 200;
      data->computation_time = (ACE_OS::rand() % 100) + 10;

      data->completion_time = last_completion + (ACE_OS::rand() % 100) + 100;
      data->completion_time =
        data->completion_time <  data->arrival_time + data->computation_time
        ? data->arrival_time + data->computation_time
        : data->completion_time;

      last_completion = data->completion_time;

      data->deadline_time = data->completion_time + (ACE_OS::rand() % 200) - 50;

      // insert the data into the queue.
      schedule_data.enqueue_tail (data);
    }
  }
}

// This function fills in the random data into the anys transported by
// the event channel.

void
Demo_Supplier::insert_event_data (CORBA::Any &data,
                                  ACE_Unbounded_Queue_Iterator<Schedule_Viewer_Data *> &schedule_iter)
{
  static u_long last_completion = 0;

  TAO_TRY {
    Schedule_Viewer_Data **sched_data;

    if ((schedule_iter.next (sched_data)) && (sched_data) && (*sched_data)) {
      if ((strcmp((*sched_data)->operation_name, "high_20") == 0) ||
           (strcmp((*sched_data)->operation_name, "low_20") == 0)  ||
           (strcmp((*sched_data)->operation_name, "high_1") == 0)  ||
           (strcmp((*sched_data)->operation_name, "low_1") == 0)) {
        navigation_.position_latitude = ACE_OS::rand() % 90;
        navigation_.position_longitude = ACE_OS::rand() % 180;
        navigation_.altitude = ACE_OS::rand() % 100;
        navigation_.heading = ACE_OS::rand() % 180;
        navigation_.roll = (navigation_.roll >= 180) ? -180 : navigation_.roll + 1;
        navigation_.pitch =  (navigation_.pitch >= 90) ? -90 : navigation_.pitch + 1;

        navigation_.utilization =     (*sched_data)->utilitzation;
        navigation_.overhead =         (*sched_data)->overhead;
        navigation_.arrival_time =     (*sched_data)->arrival_time;
        navigation_.deadline_time =    (*sched_data)->deadline_time;
        navigation_.completion_time =  (*sched_data)->completion_time;
        navigation_.computation_time = (*sched_data)->computation_time;

        // because the scheduler data does not supply these values
        navigation_.utilization = (double) (20.0 + ACE_OS::rand() % 10); 
        navigation_.overhead = (double) (ACE_OS::rand() % 10); 

        data.replace (_tc_Navigation, &navigation_, CORBA::B_TRUE, TAO_TRY_ENV);
      }
      else if ((strcmp((*sched_data)->operation_name, "high_10") == 0) ||
               (strcmp((*sched_data)->operation_name, "low_10") == 0)  ||
                (strcmp((*sched_data)->operation_name, "high_5") == 0)  ||
                (strcmp((*sched_data)->operation_name, "low_5") == 0)) {
        weapons_.number_of_weapons = 2;
        weapons_.weapon1_identifier = CORBA::string_alloc (30);
        strcpy (weapons_.weapon1_identifier,"Photon Torpedoes");
        weapons_.weapon1_status =(ACE_OS::rand() % 4) == 0 ? 0 : 1 ;
        weapons_.weapon2_identifier = CORBA::string_alloc (30);
        strcpy (weapons_.weapon2_identifier,"Quantum Torpedoes");
        weapons_.weapon2_status = (ACE_OS::rand() % 4) == 0 ? 0 : 1;
        weapons_.weapon3_identifier = CORBA::string_alloc (1);
        strcpy (weapons_.weapon3_identifier, "");
        weapons_.weapon3_status = 0;
        weapons_.weapon4_identifier = CORBA::string_alloc (1);
        strcpy (weapons_.weapon4_identifier, "");
        weapons_.weapon4_status = 0;
        weapons_.weapon5_identifier = CORBA::string_alloc (1);
        strcpy (weapons_.weapon5_identifier, "");
        weapons_.weapon5_status = 0;	  
        weapons_.utilization =     (*sched_data)->utilitzation;
        weapons_.overhead =         (*sched_data)->overhead;
        weapons_.arrival_time =     (*sched_data)->arrival_time;
        weapons_.deadline_time =    (*sched_data)->deadline_time;
        weapons_.completion_time =  (*sched_data)->completion_time;
        weapons_.computation_time = (*sched_data)->computation_time;	 

        // because the scheduler data does not supply these values
        weapons_.utilization = (double) (20.0 + ACE_OS::rand() % 10); 
        weapons_.overhead = (double) (ACE_OS::rand() % 10); 

        data.replace (_tc_Weapons, &weapons_, CORBA::B_TRUE, TAO_TRY_ENV);
      }
      else {
        ACE_ERROR ((LM_ERROR, "Demo_Supplier::insert_event_data: unrecognized operation name [%s]",
                      (*sched_data)->operation_name));
      }

      TAO_CHECK_ENV;


	if (last_completion > (*sched_data)->completion_time)
	  last_completion = 0;
	
	if ((*sched_data)->completion_time >= last_completion)
	  {
	    ACE_Time_Value pause (0,
                                    (*sched_data)->completion_time - last_completion);
	    ACE_OS::sleep (pause);
	    last_completion = (*sched_data)->completion_time;
	  }
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "Demo_Supplier::insert_event_data: Could Not access scheduling data"));
      
      schedule_iter.advance ();

      if (schedule_iter.done ())
        schedule_iter.first ();
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

// Function get_options.

static u_int
get_options (int argc,
             char *argv [])
{
  // We need the 'O' in get_opt() because we also want to have ORB
  // parameters, they all start with 'O'.
  ACE_Get_Opt get_opt (argc, argv, "O?m:f:");
  int opt;
  int temp;

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
        case 'm':
          temp = ACE_OS::atoi (get_opt.optarg);
          if (temp > 0)
            {
              total_messages = (u_int) temp;
              cout << "Messages to send:" << total_messages << endl;
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%s: count must be > 0",
                               argv[0]),
                              1);
          break;
        case 'f':
          input_file_name = get_opt.optarg;

          if (!input_file_name || ACE_OS::strlen (input_file_name) > 0)
            cout << "Messages to send:" << total_messages << endl;
          else
            {
              input_file_name = 0;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "%s: file name must be specified with -f option",
                                 argv[0]),
                                1);
            }
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%s: unknown arg, -%c\n"
                             "Usage: %s %s\n",
                             argv[0], char(opt),
                             argv[0],
                             usage),
                            1);
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

// function main

int
main (int argc, char *argv [])
{
  TAO_TRY
    {
      // Initialize ORB.

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "internet",
                         TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Connect to the RootPOA.
      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Get the Naming Service object reference.
      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable get the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Tell the ScheduleService to use the same naming service.
      ACE_Scheduler_Factory::use_config (naming_context.in ());

      if (get_options (argc, argv))
        ACE_OS::exit (-1);

      // Get the Event Channel
      // @@ If you specify an invalid name, the Name Service crashes

      CosNaming::Name channel_name (1);
      channel_name.length (1);
      channel_name[0].id = CORBA::string_dup ("EventService");

      CORBA::Object_var ec_obj =
        naming_context->resolve (channel_name, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow (ec_obj.in(),
                                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Create the demo supplier.
      Demo_Supplier *demo_supplier;
      ACE_NEW_RETURN (demo_supplier,
                      Demo_Supplier (SOURCE_ID),
                      -1);

      if (demo_supplier->open_supplier (ec.ptr (),
                                        "demo_supplier") == -1)
        ACE_ERROR_RETURN ((LM_ERROR, 
                           "Supplier open failed.\n"),
                          -1);

      // Register the internal demo consumer for timeout events.
      demo_supplier->start_generating_events ();

      ACE_DEBUG ((LM_DEBUG,
                  "Supplier is registered for timeout events.\n"));

      // The POA Manager has to be activated before starting the ORB
      // event loop.
      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Run the ORB.
      if (orb->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "CORBA::ORB::run"),
                          -1);
      TAO_CHECK_ENV;

      delete demo_supplier;
      TAO_CHECK_ENV;

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;

  return 0;
}
