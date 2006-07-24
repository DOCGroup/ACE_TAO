#include "RTEvent_Logging_Service.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "tao/IORTable/IORTable.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_strings.h"

ACE_RCSID (RTEvent_Logging_Service,
           RTEvent_Logging_Service,
           "$Id$")

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  RTEvent_Logging_Service event_service;
  return event_service.run (argc, argv);
}

// ****************************************************************

RTEvent_Logging_Service::RTEvent_Logging_Service (void)
  : sched_impl_ (0),
    // ec_impl_ (0),
    service_name_ (0),
    ior_file_name_ (0),
    pid_file_name_ (0),
    global_scheduler_ (0),
    rtevent_log_factory_name_ ("RTEventLogFactory"),
    child_poa_name_ ("RTEventLog_ChildPOA"),
    factory_servant_ (0)
{
}

RTEvent_Logging_Service::~RTEvent_Logging_Service (void)
{
  //delete this->ec_impl_;
  //this->ec_impl_ = 0;
  //delete this->sched_impl_;
  //this->sched_impl_ = 0;
}

int
RTEvent_Logging_Service::run (int argc, char* argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize ORB.
      this->orb_ =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (this->parse_args (argc, argv) == -1)
        return 1;

      ACE_NEW_RETURN (factory_servant_,
                      TAO_RTEventLogFactory_i (),
                      -1);

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references("RootPOA"
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      this->root_poa_ =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var context =
        CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

//    Alternative way to get NamingContext.
/*      if (naming_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          1);

      CosNaming::NamingContext_var context =
        naming_client_.get_context ();
*/

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

              scheduler = this->sched_impl_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // Register the servant with the Naming Context....
              context->rebind (schedule_name, scheduler.in ()
                                      ACE_ENV_ARG_PARAMETER);

              ACE_TRY_CHECK;
            }
          else
          {
              CORBA::Object_var tmp =
                context->resolve (schedule_name ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              scheduler = RtecScheduler::Scheduler::_narrow (tmp.in ()
                                                             ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }



/*
      TAO_EC_Event_Channel_Attributes attr (root_poa.in (),
                                            root_poa.in ());
      TAO_EC_Event_Channel* ec;
      ACE_NEW_RETURN (ec,
                      TAO_EC_Event_Channel (attr),
                      1);
      this->ec_impl_ = ec;
      ec->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
*/
      // Notice that we activate *this* object with the POA, but we
      // forward all the requests to the underlying EC
      // implementation.
/*
      RtecEventChannelAdmin::EventChannel_var ec =
        this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var str =
        this->orb_->object_to_string (ec.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      ACE_DEBUG ((LM_DEBUG,
                  "The EC IOR is <%s>\n", str.in ()));

      CosNaming::Name channel_name (1);
      channel_name.length (1);
      channel_name[0].id = CORBA::string_dup (this->service_name_);
      naming_context->rebind (channel_name, ec.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
*/

      if (factory_servant_->init (orb_.in (),
                                  root_poa_.in ()
                                 ACE_ENV_ARG_PARAMETER) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to initialize "
                           "the factory. \n"),
                          1);

      // activate the factory in the root poa.
      factory_ = factory_servant_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Give the ownership to the POA.
      factory_servant_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);

      ACE_TRY_CHECK;
      CORBA::String_var
        ior = orb_->object_to_string (factory_.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (true)
        {
          CORBA::Object_var table_object =
          this->orb_->resolve_initial_references ("IORTable"
                                                  ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);

          IORTable::Table_var adapter =
            IORTable::Table::_narrow (table_object.in ());
          ACE_CHECK_RETURN (-1);

          adapter->bind("RTEventLogService", ior.in ());
          ACE_CHECK_RETURN (-1);
        }

      if (this->ior_file_name_ != 0)
        {
          FILE *output_file= ACE_OS::fopen (this->ior_file_name_, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               this->ior_file_name_),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      if (this->pid_file_name_ != 0)
        {
          FILE *pidf = fopen (this->pid_file_name_, "w");
          if (pidf != 0)
            {
              ACE_OS::fprintf (pidf,
                               "%ld\n",
                               static_cast<long> (ACE_OS::getpid ()));
              ACE_OS::fclose (pidf);
            }
        }

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (rtevent_log_factory_name_);
      context->rebind (name,
                              factory_.in ()
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      context->unbind (name ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!CORBA::is_nil (scheduler.in ()))
        {
          context->unbind (schedule_name ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "EC");
    }
  ACE_ENDTRY;


  return 0;
}

int
RTEvent_Logging_Service::parse_args (int argc, char *argv [])
{
  // default values...
  this->service_name_ = "EventService";

  ACE_Get_Opt get_opt (argc, argv, "n:o:p:s:");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          this->service_name_ = get_opt.opt_arg ();
          break;

        case 'o':
          this->ior_file_name_ = get_opt.opt_arg ();
          break;

        case 'p':
          this->pid_file_name_ = get_opt.opt_arg ();
          break;

        case 's':
          // It could be just a flag (i.e. no "global" or "local"
          // argument, but this is consistent with the EC_Multiple
          // test and also allows for a runtime scheduling service.

          if (ACE_OS::strcasecmp (get_opt.opt_arg (), "global") == 0)
            {
              this->global_scheduler_ = 1;
            }
          else if (ACE_OS::strcasecmp (get_opt.opt_arg (), "local") == 0)
            {
              this->global_scheduler_ = 0;
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Unknown scheduling type <%s> "
                          "defaulting to local\n",
                          get_opt.opt_arg ()));
              this->global_scheduler_ = 0;
            }
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "-n service_name "
                      "-o ior_file_name "
                      "-p pid_file_name "
                      "-s <global|local> "
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  return 0;
}
