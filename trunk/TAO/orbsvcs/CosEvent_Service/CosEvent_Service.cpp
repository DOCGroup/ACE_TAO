// $Id$

#include "ace/Get_Opt.h"
#include "CosEvent_Service.h"

CosEvent_Service::CosEvent_Service (void)
  : service_name ("CosEventService"),
    rt_service_name ("EventService"),
    ec_impl_ (0,
              ACE_DEFAULT_EVENT_CHANNEL_TYPE,
              &module_factory_),
    rtec_ ( RtecEventChannelAdmin::EventChannel::_nil ()),
    scheduler_ (RtecScheduler::Scheduler::_nil ()),
    cos_ec_ (CosEventChannelAdmin::EventChannel::_nil ())
{
}

CosEvent_Service::~CosEvent_Service (void)
{
  // No-Op.
}

int
CosEvent_Service::init_ORB  (int argc, char *argv [])
{
  TAO_TRY
    {
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    "",
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      CORBA::Object_var poa_object  =
        this->orb_->resolve_initial_references("RootPOA",
                                               TAO_TRY_ENV);
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          -1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      return 0;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception in CosEvent_Service::init_ORB");
      return -1;
    }
  TAO_ENDTRY;
}

int
CosEvent_Service::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "r:n:");
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

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "-n COS Event Service name"
                      "-r RealTime Event Service name"
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  return 0;
}

int
CosEvent_Service::init_NamingService (void)
{
  TAO_TRY
  {
    // Initialization of the naming service.
    if (this->naming_client_.init (this->orb_.in ()) != 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                        -1);
    return 0;
  }
 TAO_CATCHANY
   {
     TAO_TRY_ENV.print_exception ("Exception in CosEvent_Service::get_Rtec_viaNamingService\n");
     return -1;
   }
 TAO_ENDTRY;
}

int
CosEvent_Service::get_Rtec_viaNamingService (void)
{
  TAO_TRY
    {
      CosNaming::Name rt_ref_name (1); // name of the RT Event service.
      rt_ref_name.length (1);
      rt_ref_name[0].id =
      CORBA::string_dup (this->rt_service_name);

      CORBA::Object_var rtEC_obj =
       this->naming_client_->resolve (rt_ref_name,
				      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // The CORBA::Object_var object is downcast to
      // RtecEventChannelAdmin::EventChannel
      // using the <_narrow> method.
      this->rtec_ =
        RtecEventChannelAdmin::EventChannel::_narrow (rtEC_obj.in (),
                                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      return 0;
  }
 TAO_CATCHANY
   {
     TAO_TRY_ENV.print_exception ("Exception in CosEvent_Service::get_Rtec_viaNamingService\n");
     return -1;
   }
 TAO_ENDTRY;
}

int
CosEvent_Service::start_Scheduler (void)
{
  TAO_TRY
    {
      this->scheduler_ =
        this->scheduler_impl_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      CORBA::String_var str =
        this->orb_->object_to_string (this->scheduler_.in (),
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      ACE_DEBUG ((LM_DEBUG,
                  "CosEvent_Service: The (local) scheduler IOR is <%s>\n",
                  str.in ()));

      if (ACE_Scheduler_Factory::server (this->scheduler_.in ()) == -1)
        return -1;

      return 0;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception in CosEvent_Service::start_Scheduler\n");
      return -1;
    }
  TAO_ENDTRY;
}
int
CosEvent_Service::create_local_RtecService (void)
{
 TAO_TRY
    {
      this->rtec_ = this->ec_impl_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

       CORBA::String_var str = this->orb_->object_to_string (this->rtec_.in (),
                                          TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      ACE_DEBUG ((LM_DEBUG,
                  "CosEvent_Service: The RTEC IOR is <%s>\n",
                  str.in ()));

      this->ec_impl_.activate ();

      return 0;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception in CosEvent_Service::create_local_RtecService\n");
      return -1;
    }
  TAO_ENDTRY;
}

int
CosEvent_Service::create_CosEC (void)
{
TAO_TRY
    {
      // Setup the QOS params..
      this->supplier_qos_.insert (1,
                                  ACE_ES_EVENT_ANY, //@@this should be a param.
                                  this->scheduler_->create ("supplier",
                                                            TAO_TRY_ENV),
                                  1);

      this->consumer_qos_.start_disjunction_group ();
      this->consumer_qos_.insert_source (1,
                                         this->scheduler_->create ("consumer",
                                                                   TAO_TRY_ENV));
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      const RtecEventChannelAdmin::ConsumerQOS &consumerqos =
        this->consumer_qos_.get_ConsumerQOS ();

      const RtecEventChannelAdmin::SupplierQOS &supplierqos =
        this->supplier_qos_.get_SupplierQOS ();

      if (this->ec_i_.init (consumerqos,
                            supplierqos,
                            this->rtec_,
                            TAO_TRY_ENV) != 0)
        return -1;

      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      this->cos_ec_ = this->ec_i_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV_RETURN (TAO_TRY_ENV, -1);

      return 0;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception in CosEvent_Service::create_CosEC");
      return -1;
    }
  TAO_ENDTRY;
}

int
CosEvent_Service::register_CosEC (void)
{
 TAO_TRY
    {
      // Name the object.
      CosNaming::Name ec_obj_name (1);
      ec_obj_name.length (1);
      ec_obj_name[0].id =
	CORBA::string_dup (this->service_name);

      // Now, attach the object name to the context.
      this->naming_client_->bind (ec_obj_name,
				  this->cos_ec_,
				  TAO_TRY_ENV);
      TAO_CHECK_ENV;
      return 0;
    }
 TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("CosEvent_Service::register_CosEC.\n");
      return -1;
    }
  TAO_ENDTRY;
}

int
CosEvent_Service::startup (int argc, char *argv[])
{
  // check command line args.
  if (this->parse_args (argc, argv) == -1)
    return -1;

  // initalize the ORB.
  if (this->init_ORB (argc, argv) == -1)
    return -1;

  // initialize the Naming Client.
  if (this->init_NamingService () == -1)
    return -1;

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
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) Failed to start the scheduler\n"),
                      -1);

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
      // CosEC creation failed.. cleanup
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
 TAO_TRY
    {
      // unbind the cosEC from the naming service.
      CosNaming::Name ec_obj_name (1);
      ec_obj_name.length (1);
      ec_obj_name[0].id =
	CORBA::string_dup (this->service_name);
      this->naming_client_->unbind (ec_obj_name,
				    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->cos_ec_ != CosEventChannelAdmin::EventChannel::_nil ())
        this->cos_ec_->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->ec_impl_.shutdown ();
      return 0;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception in shutdown.\n");
      return -1;
    }
  TAO_ENDTRY;
}

int
main (int argc, char *argv[])
{
  CosEvent_Service service;

  if (service.startup (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Failed to setup the Cos Event Channel.\n"),
                      -1);
  if (service.run () == -1)
    ACE_ERROR((LM_ERROR,
                       "Failed to run the Cos Event Channel.\n"));
  service.shutdown ();
}
