// $Id$

#include "Event_Service.h"

#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Event/Module_Factory.h"
#include "orbsvcs/Event/Event_Channel.h"

#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"

ACE_RCSID(Event_Service, Event_Service, "$Id$")

int main (int argc, char *argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  Event_Service event_service;
  return event_service.run (argc, argv);
}

// ****************************************************************

Event_Service::Event_Service (void)
  : module_factory_ (0),
    sched_impl_ (0),
    ec_impl_ (0),
    service_name_ (0),
    ior_file_name_ (0),
    pid_file_name_ (0),
    event_service_type_ (ES_OLD_MT),
    global_scheduler_ (0)
{
}

Event_Service::~Event_Service (void)
{
  delete this->ec_impl_;
  this->ec_impl_ = 0;
  delete this->sched_impl_;
  this->sched_impl_ = 0;
  delete this->module_factory_;
  this->module_factory_ = 0;
}

int
Event_Service::run (int argc, char* argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize ORB.
      this->orb_ =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (this->parse_args (argc, argv) == -1)
        return 1;

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA");
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
        this->orb_->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RtecScheduler::Scheduler_var scheduler;
      // This is the name we (potentially) register the Scheduling
      // Service in the Naming Service.
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup ("ScheduleService");


      if (1) // this->event_service_type_ == ES_OLD_REACTIVE
             // || this->event_service_type_ == ES_OLD_MT)
        {
          // We must find the scheduler object reference...

          if (this->global_scheduler_ == 0)
            {
              ACE_NEW_RETURN (this->sched_impl_,
                              ACE_Config_Scheduler,
                              1);

              scheduler = this->sched_impl_->_this (ACE_TRY_ENV);
              ACE_TRY_CHECK;

              // Register the servant with the Naming Context....
              naming_context->rebind (schedule_name, scheduler.in (),
                                      ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          else
            {
              CORBA::Object_var tmp =
                naming_context->resolve (schedule_name, ACE_TRY_ENV);
              ACE_TRY_CHECK;

              scheduler = RtecScheduler::Scheduler::_narrow (tmp.in (),
                                                             ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
        }

      switch (this->event_service_type_)
        {
        case ES_NEW:
          {
            TAO_EC_Event_Channel* ec;
            ACE_NEW_RETURN (ec,
                            TAO_EC_Event_Channel (root_poa.in (),
                                                  root_poa.in ()),
                            1);
            this->ec_impl_ = ec;
            ec->activate (ACE_TRY_ENV);
            ACE_TRY_CHECK;
          }
          break;

        case ES_OLD_REACTIVE:
          {
            ACE_NEW_RETURN (this->module_factory_,
                            TAO_Reactive_Module_Factory,
                            1);
            ACE_NEW_RETURN (this->ec_impl_,
                            ACE_EventChannel (scheduler.in (),
                                              1,
                                              ACE_DEFAULT_EVENT_CHANNEL_TYPE,
                                              this->module_factory_),
                            1);
          }
          break;
        case ES_OLD_MT:
          {
            ACE_NEW_RETURN (this->module_factory_,
                            TAO_Default_Module_Factory,
                            1);

            ACE_NEW_RETURN (this->ec_impl_,
                            ACE_EventChannel (scheduler.in (),
                                              1,
                                              ACE_DEFAULT_EVENT_CHANNEL_TYPE,
                                              this->module_factory_),
                            1);
          }

          break;
        }

      // Notice that we activate *this* object with the POA, but we
      // forward all the requests to the underlying EC
      // implementation.
      RtecEventChannelAdmin::EventChannel_var ec =
        this->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var str =
        this->orb_->object_to_string (ec.in (), ACE_TRY_ENV);

      ACE_DEBUG ((LM_DEBUG,
		  "The EC IOR is <%s>\n", str.in ()));

      CosNaming::Name channel_name (1);
      channel_name.length (1);
      channel_name[0].id = CORBA::string_dup (this->service_name_);
      naming_context->rebind (channel_name, ec.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "%s; running event service\n", __FILE__));
      if (this->orb_->run () == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), 1);

      naming_context->unbind (channel_name, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (!CORBA::is_nil (scheduler.in ()))
	{
	  naming_context->unbind (schedule_name, ACE_TRY_ENV);
	  ACE_TRY_CHECK;
	}

    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("EC");
    }
  ACE_ENDTRY;


  return 0;
}

int
Event_Service::parse_args (int argc, char *argv [])
{
  // default values...
  this->service_name_ = "EventService";

  ACE_Get_Opt get_opt (argc, argv, "n:s:t:");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          this->service_name_ = get_opt.optarg;
          break;

	case 's':
	  // It could be just a flag (i.e. no "global" or "local"
	  // argument, but this is consistent with the EC_Multiple
	  // test and also allows for a runtime scheduling service.

          if (ACE_OS::strcasecmp (get_opt.optarg, "global") == 0)
            {
              this->global_scheduler_ = 1;
            }
          else if (ACE_OS::strcasecmp (get_opt.optarg, "local") == 0)
            {
              this->global_scheduler_ = 0;
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Unknown scheduling type <%s> "
                          "defaulting to local\n",
                          get_opt.optarg));
              this->global_scheduler_ = 0;
            }
          break;

        case 't':
          if (ACE_OS::strcasecmp (get_opt.optarg, "NEW") == 0)
            {
              this->event_service_type_ = ES_NEW;
            }
          else if (ACE_OS::strcasecmp (get_opt.optarg, "OLD_REACTIVE") == 0)
            {
              this->event_service_type_ = ES_OLD_REACTIVE;
            }
          else if (ACE_OS::strcasecmp (get_opt.optarg, "OLD_MT") == 0)
            {
              this->event_service_type_ = ES_OLD_MT;
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Unknown event service type <%s> "
                          "defaulting to REACTIVE\n",
                          get_opt.optarg));
              this->event_service_type_ = ES_NEW;
            }
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "-n service_name "
                      "-s <global|local> "
                      "-t <new|old_reactive|old_mt> "
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  return 0;
}


RtecEventChannelAdmin::ConsumerAdmin_ptr
Event_Service::for_consumers (CORBA::Environment &ACE_TRY_ENV)
{
  return this->ec_impl_->for_consumers (ACE_TRY_ENV);
}

RtecEventChannelAdmin::SupplierAdmin_ptr
Event_Service::for_suppliers (CORBA::Environment &ACE_TRY_ENV)
{
  return this->ec_impl_->for_suppliers (ACE_TRY_ENV);
}

void
Event_Service::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  this->ec_impl_->destroy (ACE_TRY_ENV);
  ACE_CHECK;
  this->orb_->shutdown ();
}

RtecEventChannelAdmin::Observer_Handle
Event_Service::append_observer (RtecEventChannelAdmin::Observer_ptr observer,
                                CORBA::Environment &ACE_TRY_ENV)
{
  return this->ec_impl_->append_observer (observer, ACE_TRY_ENV);
}

void
Event_Service::remove_observer (RtecEventChannelAdmin::Observer_Handle handle,
                                CORBA::Environment &ACE_TRY_ENV)
{
  this->ec_impl_->remove_observer (handle, ACE_TRY_ENV);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Auto_Basic_Ptr<POA_RtecScheduler::Scheduler>;
template class auto_ptr<POA_RtecScheduler::Scheduler>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Basic_Ptr<POA_RtecScheduler::Scheduler>
#pragma instantiate auto_ptr<POA_RtecScheduler::Scheduler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
