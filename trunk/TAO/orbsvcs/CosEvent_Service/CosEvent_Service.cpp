// $Id$

#include "ace/Get_Opt.h"
#include "CosEvent_Service.h"

// @@ Pradeep: somehow the user should be able to specify if the EC is
// to be instantiated locally or if you want to use a remote EC.
// Please check the real-time Event_Service and how they manage
// optionally create a scheduling service.

CosEvent_Service::CosEvent_Service (void)
  : service_name ("CosEventService"),
    rt_service_name ("EventService"),
    schedule_name_ ("ScheduleService"),
    scheduler_ (RtecScheduler::Scheduler::_nil ()),
    ec_impl_ (0,
              ACE_DEFAULT_EVENT_CHANNEL_TYPE,
              &module_factory_),
    rtec_ (RtecEventChannelAdmin::EventChannel::_nil ()),
    cos_ec_ (CosEventChannelAdmin::EventChannel::_nil ()),
    global_scheduler_ (0),
    remote_Rtec_ (1),
    eventTypeIds_ (0),
    eventSourceIds_ (0),
    source_type_pairs_ (0)
{
  // No-Op.
}

CosEvent_Service::~CosEvent_Service (void)
{
  // No-Op.
}

int
CosEvent_Service::init_ORB  (int argc, char *argv [])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    "",
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object  =
        this->orb_->resolve_initial_references("RootPOA",
                                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          -1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);

      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);

      ACE_TRY_CHECK;

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in CosEvent_Service::init_ORB");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
CosEvent_Service::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "r:n:s:e:o:p:l");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          this->service_name = get_opt.optarg;
          break;

        case 'r':
          this->rt_service_name = get_opt.optarg;
          break;

        case 's':
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
              return -1;
            }
          break;

        case 'l':
          this->remote_Rtec_ = 0;
          break;

        case 'e':
          this->eventTypeIds_ =  get_opt.optarg;
          break;

        case 'o':
          this->eventSourceIds_ = get_opt.optarg;
          break;

        case 'p':
          this->source_type_pairs_ = get_opt.optarg;
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      " -n <COS Event Service name>"
                      " -r <RealTime Event Service name>"
                      " -s <global|local>"
                      " -l" // creates the RtEC locally.
                      " -e [\"EventType_1, EventType_2...\"] for ConsumerQOS."
                      " -o [\"EventSourceID_1, [EventSourceID_2...\"] for ConsumerQOS."
                      " -p [\"Source, Event\" pair] for SupplierQOS."
                      " \n",
                      argv[0]));
          return -1;
        }
    }

  return 0;
}

int
CosEvent_Service::init_NamingService (void)
{
  // Initialization of the naming service.
  if (this->naming_client_.init (this->orb_.in ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Unable to initialize "
                       "the TAO_Naming_Client. \n"),
                      -1);
  return 0;
}

int
CosEvent_Service::get_Rtec_viaNamingService (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CosNaming::Name rt_ref_name (1); // name of the RT Event service.
      rt_ref_name.length (1);
      rt_ref_name[0].id =
      CORBA::string_dup (this->rt_service_name);

      CORBA::Object_var rtEC_obj =
       this->naming_client_->resolve (rt_ref_name,
				      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // The CORBA::Object_var object is downcast to
      // RtecEventChannelAdmin::EventChannel
      // using the <_narrow> method.
      this->rtec_ =
        RtecEventChannelAdmin::EventChannel::_narrow (rtEC_obj.in (),
                                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return 0;
  }
  ACE_CATCHANY
   {
     ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                          "Exception in CosEvent_Service::get_Rtec_viaNamingService\n");
     return -1;
   }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
CosEvent_Service::start_Scheduler (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (this->global_scheduler_ == 0)
        {
          this->scheduler_ =
            this->scheduler_impl_._this (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          CORBA::String_var str =
            this->orb_->object_to_string (this->scheduler_.in (),
                                          ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG,
                      "CosEvent_Service: The (local) scheduler IOR is <%s>\n",
                      str.in ()));

          if (ACE_Scheduler_Factory::server (this->scheduler_.in ()) == -1)
            return -1;
        }
      else // get the global scheduler
        {
          CosNaming::Name schedule_name (1);
          schedule_name.length (1);
          schedule_name[0].id = CORBA::string_dup (this->schedule_name_);

          CORBA::Object_var sched_obj =
            this->naming_client_->resolve (schedule_name,
                                           ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // The CORBA::Object_var object is downcast to
          // RtecScheduler::Scheduler using the <_narrow> method.
          this->scheduler_ =
            RtecScheduler::Scheduler::_narrow (sched_obj.in (),
                                               ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (ACE_Scheduler_Factory::server (this->scheduler_.in ()) == -1)
            return -1;
        }

      ACE_Scheduler_Factory::use_config (this->naming_client_.get_context());
      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in CosEvent_Service::start_Scheduler\n");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
int
CosEvent_Service::create_local_RtecService (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->rtec_ = this->ec_impl_._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var str = this->orb_->object_to_string (this->rtec_.in (),
                                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "CosEvent_Service: The RTEC IOR is <%s>\n",
                  str.in ()));

      this->ec_impl_.activate ();

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in CosEvent_Service::create_local_RtecService\n");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

void
CosEvent_Service::init_SupplierQOS (RtecScheduler::handle_t supp_handle)
{
  // @@ Pradeep: It is very important that you make the type of
  // events generated by the CosEC an option.
  // I know this is not very well documented, but the type should
  // be >= ACE_ES_EVENT_UNDEFINED = 16
  // Something else: please make the EventSourceID for the
  // supplier also an option...

  char c = ' '; // space
  char *tok = 0;

 // if nothing was specified on the command line use defaults..
  if (this->source_type_pairs_ == 0)
    this->supplier_qos_.insert (1,
                                ACE_ES_EVENT_ANY,
                                supp_handle,
                                1);
  else // parse the event types..
    {
      tok = ACE_OS::strtok (this->source_type_pairs_, &c);
      if (tok == 0) // error
        {
          ACE_DEBUG ((LM_DEBUG, "error parsing source,event pairs for SupplierQOS, defaulting to source id = 1, eventid = ACE_ES_EVENT_ANY"));

          this->supplier_qos_.insert (1,
                                      ACE_ES_EVENT_ANY,
                                      supp_handle,
                                      1);
        }
      else
        // we just use 1 source-type pair in the event channel.
        // so scan for the 1st pair only.
        {
          int source_val = 0, type_val = 0;
          source_val = ACE_OS::atoi (tok);

          tok = ACE_OS::strtok (0, &c);

          if (tok != 0)
            type_val = ACE_OS::atoi (tok);

          ACE_DEBUG ((LM_DEBUG, "supplier_qos::insert (%d, %d)\n",
                      source_val, type_val));

          // Setup the QOS params..
          this->supplier_qos_.insert (source_val,
                                      type_val,
                                      supp_handle,
                                      1);
        }
    }
}

void
CosEvent_Service::init_ConsumerQOS (RtecScheduler::handle_t cons_handle)
{
  // @@ Pradeep: ditto here, make the set of sources (and/or type)
  // a parameter, and make sure the user can specify multiple of
  // them (just call insert_source() or insert_type() in the
  // parse_args routine).

  char c = ' '; // space
  char *tok = 0;

  this->consumer_qos_.start_disjunction_group ();

  // insert the event ids first..

  // if nothing was specified on the command line use defaults..
  if (this->eventTypeIds_ == 0)
    {
      //this->consumer_qos_.insert_type (ACE_ES_EVENT_ANY, // default
      //                             cons_handle);
      // @@ if i uncomment this line then the Rtec displays the message
      // "Consumer tried to register for allevents!  This is not implemented."
      // whenever a consumer tries to register with it.
    }
  else // parse the event types..
    {
      tok = ACE_OS::strtok ( this->eventTypeIds_, &c);
      if (tok == 0) // error
        {
          ACE_DEBUG ((LM_DEBUG, "error parsing eventIds for ConsumerQOS, defaulting to 1"));
          this->consumer_qos_.insert_type (ACE_ES_EVENT_ANY,
                                           cons_handle);
        }
      else
        do
          {
            int type_val = ACE_OS::atoi (tok);
            ACE_DEBUG ((LM_DEBUG, "consumer_qos::insert_type (%d)\n",
                        type_val));
            this->consumer_qos_.insert_type (type_val,
                                             cons_handle);
            tok = ACE_OS::strtok (0, &c);
          }
        while (tok != 0);
    }

  // repeat for source ids..

  // if nothing was specified on the command line use defaults..
  if (this->eventSourceIds_ == 0)
    this->consumer_qos_.insert_source (1, // default = 1
                                       cons_handle);
  else // parse the event types..
    {
      tok = ACE_OS::strtok (this->eventSourceIds_, &c);
      if (tok == 0) // error
        {
          ACE_DEBUG ((LM_DEBUG, "error parsing sourceIds for ConsumerQOS, defaulting to 1"));
          this->consumer_qos_.insert_source (1, // default = 1
                                             cons_handle);
        }
      else
        do
          {
            int source_val = ACE_OS::atoi (tok);
            ACE_DEBUG ((LM_DEBUG, "consumer_qos::insert_source (%d)\n",
                        source_val));
            this->consumer_qos_.insert_type (source_val,
                                             cons_handle);
            tok = ACE_OS::strtok (0, &c);
          }
        while (tok != 0);
    }
}

int
CosEvent_Service::create_CosEC (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      RtecScheduler::handle_t supp_handle =
        this->scheduler_->create ("supplier",
                                  ACE_TRY_ENV);

      ACE_TRY_CHECK;

      this->init_SupplierQOS (supp_handle);

      RtecScheduler::handle_t cons_handle =
        this->scheduler_->create ("consumer",
                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->init_ConsumerQOS (cons_handle);

      const RtecEventChannelAdmin::ConsumerQOS &consumerqos =
        this->consumer_qos_.get_ConsumerQOS ();

      const RtecEventChannelAdmin::SupplierQOS &supplierqos =
        this->supplier_qos_.get_SupplierQOS ();

      if (this->ec_i_.init (consumerqos,
                            supplierqos,
                            this->rtec_,
                            ACE_TRY_ENV) != 0)
        return -1;

      ACE_TRY_CHECK;

      this->cos_ec_ = this->ec_i_._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var str = this->orb_->object_to_string (this->cos_ec_,
                                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "CosEvent_Service: The COSEC IOR is <%s>\n",
                  str.in ()));
      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in CosEvent_Service::create_CosEC");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
CosEvent_Service::register_CosEC (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
   {
      // Name the object.
      CosNaming::Name ec_obj_name (1);
      ec_obj_name.length (1);
      ec_obj_name[0].id =
	CORBA::string_dup (this->service_name);

      // Now, attach the object name to the context.
      this->naming_client_->rebind (ec_obj_name,
                                    this->cos_ec_,
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;
      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in CosEvent_Service::register_CosEC.\n");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
CosEvent_Service::startup (int argc, char *argv[])
{
  // initalize the ORB.
  if (this->init_ORB (argc, argv) == -1)
    return -1;

  // The ORB is initialized before <parse_args> because the ACE_Channel
  // <destroy> method assumes an initialized ORB.

  // check command line args.
  if (this->parse_args (argc, argv) == -1)
    return -1;

  // initialize the Naming Client.
  if (this->init_NamingService () == -1)
    return -1;

  // see if the user wants a local Rtec..
  if (this->remote_Rtec_ == 0 && this->create_local_RtecService () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Failed to create a local RtEC.\n"),
                        -1);
    }
  else
    // get hold of the Rtec Service via the Naming Service.
    if (this->get_Rtec_viaNamingService () == -1)
      {
        ACE_DEBUG ((LM_ERROR, "Could not locate the Real Time Event service <%s> via the Naming Service, trying to create a local copy..\n", this->rt_service_name));

        // Rtec was not active.. try creating a local copy.
        if (this->create_local_RtecService () == -1)
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Failed to create a local RtEC.\n"),
                            -1);
      }

  // start the scheduler
  if (this->start_Scheduler ())
    {
      // scheduler startup failed..cleanup
      this->shutdown ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) Failed to start the scheduler\n"),
                        -1);
    }
  // now try to create the COS EC.
  if (this->create_CosEC () == -1)
    {
      // CosEC creation failed.. cleanup
      this->shutdown ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Failed to create CosEC.\n"),
                        -1);
    }

  // finally, register it with the naming service.
  if (this->register_CosEC () == -1)
    {
      // CosEC registration failed.. cleanup
      this->shutdown ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         " (%P|%t) Failed to register the CosEC with the naming service.\n"),
                        -1);
    }

  return 0;
}

int
CosEvent_Service::run (void)
{
  ACE_DEBUG ((LM_DEBUG, "%s; running the Cos event service\n", __FILE__));
  if (this->orb_->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), 1);

  return 0;
}

int
CosEvent_Service::shutdown (void)
{
  ACE_DECLARE_NEW_CORBA_ENV
  ACE_TRY
    {
      if (!this->cos_ec_->_nil ())
        {
          // unbind the cosEC from the naming service.
          CosNaming::Name ec_obj_name (1);
          ec_obj_name.length (1);
          ec_obj_name[0].id =
            CORBA::string_dup (this->service_name);
          this->naming_client_->unbind (ec_obj_name,
                                        ACE_TRY_ENV);
          ACE_TRY_CHECK;

          this->cos_ec_->destroy (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      this->ec_impl_.shutdown ();

      // shutdown the ORB.
      if (!this->orb_->_nil ())
        this->orb_->shutdown ();

      return 0;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in shutdown.\n");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
main (int argc, char *argv[])
{
  CosEvent_Service service;

  if (service.startup (argc, argv) == -1)
    {
      service.shutdown ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Failed to setup the Cos Event Channel.\n"),
                        1);
    }

  if (service.run () == -1)
    {
      service.shutdown ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Failed to run the Cos Event Channel.\n"),
                        1);
    }

  service.shutdown ();
}
