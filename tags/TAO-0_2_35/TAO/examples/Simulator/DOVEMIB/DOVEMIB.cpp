// $Id$

// ============================================================================
//
// = FILENAME
//    DOVEMIB.cpp
//
// = DESCRIPTION
//    The MIB listens to the Event Channel, analyses the CORBA::Any
//    and prints the result into a file.
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
#include "DOVEMIB.h"

ACE_RCSID(DOVEMIB, DOVEMIB, "$Id$")

static const char usage [] = 
"[-? |\n"
"            [-O[RBport] ORB port number]"
"            [-m <count> of messages to receive [100]]"
"            [-f filename for the repository [event_repository]]]";

static int events_received_ = 0;
static char *input_file_name = 0;
static int events_to_receive_ = 100;
RtecEventChannelAdmin::EventChannel_var ec;

// ************************************************************

MIB_Consumer::MIB_Consumer (void)
: anyAnalyser_ (input_file_name) {
}

int 
MIB_Consumer::open_consumer (RtecEventChannelAdmin::EventChannel_ptr ec,
                              const char *my_name) {
  TAO_TRY {
    // Get a Scheduler.

    RtecScheduler::Scheduler_ptr server =
    ACE_Scheduler_Factory::server ();

    // Define Real-time information.

    rt_info_ = server->create (my_name, TAO_TRY_ENV);

      server->set (rt_info_,
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

    // Create the event that we're registering for.

    ACE_ConsumerQOS_Factory dependencies;
    dependencies.start_disjunction_group ();
    dependencies.insert_type (ACE_ES_EVENT_NOTIFICATION, rt_info_);
    dependencies.insert_type (ACE_ES_EVENT_SHUTDOWN, rt_info_);

    // The channel administrator is the event channel we got from
    // the invocation of this routine.

    this->channel_admin_ = ec;

    // = Connect as a consumer.

    this->consumer_admin_ = channel_admin_->for_consumers (TAO_TRY_ENV);
    TAO_CHECK_ENV;

    // Obtain a pointer to a push supplier.  "suppliers" is
    // inherited from a base class.

    this->suppliers_ = consumer_admin_->obtain_push_supplier (TAO_TRY_ENV);
    TAO_CHECK_ENV;

    // The _this function returns an object pointer. This is needed
    // because a consumer inherits from a Servant class that is no
    // CORBA::Object.

    RtecEventComm::PushConsumer_var objref = this->_this (TAO_TRY_ENV);
    TAO_CHECK_ENV;

    this->suppliers_->connect_push_consumer (objref.in (),
                                               dependencies.get_ConsumerQOS (),
                                               TAO_TRY_ENV);
    TAO_CHECK_ENV;
  }
  TAO_CATCH (RtecEventChannelAdmin::EventChannel::SUBSCRIPTION_ERROR, se) {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "MIB_Consumer::open: subscribe failed.\n"),
                        -1);
  }
  TAO_CATCHANY {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "MIB_Consumer::open: unexpected exception.\n"),
                        -1);
  }
  TAO_ENDTRY;

  return 0;
}

void
MIB_Consumer::disconnect_push_consumer (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "Consumer received disconnect from channel.\n"));
}


// This method is called when the Event Service pushes a new
// event to us

void
MIB_Consumer::push (const RtecEventComm::EventSet &events,
                        CORBA::Environment &)
{

  if (events.length () == 0) {
    ACE_DEBUG ((LM_DEBUG, "no events\n"));
    return;
  }

  // count the number of events
  events_received_++;

  // if we have enough we quit
  if (events_received_ > events_to_receive_) {
      this->shutdown ();
  }

  // Several events could be sent at once
  for (CORBA::ULong i = 0; i < events.length (); ++i) {

    if (events[i].header.type == ACE_ES_EVENT_SHUTDOWN) {

      ACE_DEBUG ((LM_DEBUG, "MIB Consumer: received shutdown event\n"));
      this->shutdown ();
    }
    else {

      if (events_received_ == 1) {
        ACE_DEBUG ((LM_DEBUG, "MIB Consumer: received an event, going to be mute.\n"));
      }
 
      TAO_TRY
      {
        // print the time stamps 
        ACE_hrtime_t creation;
        ORBSVCS_Time::TimeT_to_hrtime (creation,
                                       events[i].header.creation_time);

        ACE_hrtime_t ec_recv;
        ORBSVCS_Time::TimeT_to_hrtime (ec_recv,
                                       events[i].header.ec_recv_time);

        ACE_hrtime_t ec_send;
        ORBSVCS_Time::TimeT_to_hrtime (ec_send,
                                       events[i].header.ec_send_time);

        anyAnalyser_.printTimeStamp (creation, ec_recv, ec_send);

        if (events[i].data.any_value.any_owns_data ())
        { 
          void * void_ptr = ACE_OS::malloc (events[i].data.any_value.type()->size(TAO_TRY_ENV));

          TAO_InputCDR stream ((ACE_Message_Block *)events[i].data.any_value.value ());
          if (stream.decode (events[i].data.any_value.type(), void_ptr, 0, TAO_TRY_ENV)
              != CORBA::TypeCode::TRAVERSE_CONTINUE)
          {
            cout << "MIB_Consumer::push: "
                 << "Something went wrong when decoding the Message Block (Any value)!"
                 << endl;
            // something went wrong
            ACE_OS::free(void_ptr);
            return;
          }
	        TAO_CHECK_ENV;

          // invoke the AnyAnalyser
          anyAnalyser_.printAny (events[i].data.any_value.type(), void_ptr);               
          ACE_OS::free(void_ptr);
        }
        else
        {                          
          // invoke the AnyAnalyser
          anyAnalyser_.printAny (events[i].data.any_value.type(), events[i].data.any_value.value());                         
        }
          
      }
      TAO_CATCHANY {
        ACE_ERROR ((LM_ERROR, "(%t)Error in extracting the Navigation and Weapons data.\n"));
      }
      TAO_ENDTRY;
    }
  }
}

void
MIB_Consumer::shutdown (void)
{
  TAO_TRY
    {
      // cause the AnyAnalyser to close the file 
      anyAnalyser_.close();

      // Disconnect from the push supplier.
      this->suppliers_->disconnect_push_supplier (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "@@ we should shutdown here!!!\n"));
      TAO_CHECK_ENV;

      ec = 0;

      TAO_ORB_Core_instance ()->orb ()->shutdown ();
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,
                 "(%t) MIB_Consumer::shutdown: unexpected exception.\n"));
    }
  TAO_ENDTRY;
}

// function get_options

static unsigned int
get_options (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "O?f:m:");
  int opt;
  unsigned int temp;

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

      case 'f':
        input_file_name = get_opt.optarg;

        if (!input_file_name || ACE_OS::strlen (input_file_name) == 0) {            
          input_file_name = 0;
          ACE_ERROR_RETURN ((LM_ERROR,
                              "%s: file name must be specified with -f option",
                               argv[0]),
                               1);
        }
        break;

      case 'm':
        temp = (unsigned int) ACE_OS::atoi (get_opt.optarg);
        if (temp > 0) {
          events_to_receive_ = temp;
          ACE_DEBUG ((LM_DEBUG,"Messages to receive: %d\n", events_to_receive_));
        }
        else {
          ACE_ERROR_RETURN ((LM_ERROR, "%s: count must be > 0", argv[0]), 1);
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
  TAO_TRY
    {
      // Initialize ORB.

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "internet", TAO_TRY_ENV);
      TAO_CHECK_ENV;

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

      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_Scheduler_Factory::use_config (naming_context.in ());

      if (get_options (argc, argv))
        ACE_OS::exit (-1);

      // Get the Event Channel.

      CosNaming::Name channel_name (1);
      channel_name.length (1);
      channel_name[0].id = CORBA::string_dup ("EventService");

      CORBA::Object_var ec_obj =
        naming_context->resolve (channel_name,
                                 TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ec =
        RtecEventChannelAdmin::EventChannel::_narrow (ec_obj.in(), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (ec.ptr() == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Not able to get the Event Service reference.\n"),
                          -1);

      // Create consumer.

      MIB_Consumer *mIB_Consumer;
      ACE_NEW_RETURN (mIB_Consumer,
                      MIB_Consumer (),
                      -1);

      if (mIB_Consumer->open_consumer (ec.ptr (),
                                        "MIB_Consumer") == -1)
        ACE_ERROR_RETURN ((LM_ERROR, 
                           "Someone was feeling introverted.\n"),
                          -1);

      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Run the ORB

      if (orb->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "CORBA::ORB::run"),
                          -1);
      TAO_CHECK_ENV;

      ec = 0;

      root_poa->destroy (1,
                         1,
                         TAO_TRY_ENV);
      TAO_CHECK_ENV;

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;

  return 0;
}
