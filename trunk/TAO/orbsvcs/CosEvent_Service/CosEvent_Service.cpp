// $Id$

#include "ace/Get_Opt.h"
#include "CosEvent_Service.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

CosEvent_Service::CosEvent_Service (void)
  : service_name ("CosEventService"),
    rt_service_name ("EventService"),
    remote_Rtec_ (0)
{
  // No-Op.
}

CosEvent_Service::~CosEvent_Service (void)
{
  // No-Op.
}

void
CosEvent_Service::init_ORB  (int argc, char *argv [],
                             CORBA::Environment &ACE_TRY_ENV)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                "",
                                ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Object_var poa_object  =
    this->orb_->resolve_initial_references("RootPOA",
                                           ACE_TRY_ENV);
  ACE_CHECK;

  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in (),
                                  ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager (ACE_TRY_ENV);
  ACE_CHECK;

  poa_manager->activate (ACE_TRY_ENV);
  ACE_CHECK;
}

int
CosEvent_Service::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "r:n:e:o:p:l");
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

        case 'l':
          this->remote_Rtec_ = 1;
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
                      "\n\t-n <COS Event Service name>"
                      "\n\t-r <RealTime Event Service name>"
                      "\n\t-l" // creates the RtEC locally.
                      "\n\t-e [\"EventType_1, EventType_2...\"] for ConsumerQOS."
                      "\n\t-o [\"EventSourceID_1, [EventSourceID_2...\"] for ConsumerQOS."
                      "\n\t-p [\"Source, Event\" pairs] for SupplierQOS."
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  return 0;
}

void
CosEvent_Service::startup (int argc, char *argv[],
                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "Starting up the CosEvent Service...\n"));

  // initalize the ORB.
  this->init_ORB (argc, argv,
                  ACE_TRY_ENV);
  ACE_CHECK;

  this->resolve_naming_service (ACE_TRY_ENV);
  ACE_CHECK;

  this->init (this->poa_,
              this->poa_,
              this->eventTypeIds_,
              this->eventSourceIds_,
              this->source_type_pairs_,
              ACE_TRY_ENV);
  ACE_CHECK;

  this->activate (ACE_TRY_ENV);
  ACE_CHECK;

  // Register ourselves with the naming service.
  ACE_ASSERT(!CORBA::is_nil (this->naming_.in ()));

  CORBA::Object_var obj =
    this->poa_->servant_to_reference (this,
                                      ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::String_var str =
    this->orb_->object_to_string (obj.in (), ACE_TRY_ENV);

  ACE_DEBUG ((LM_DEBUG,
              "The CosEC IOR is <%s>\n", str.in ()));

  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (this->service_name);

  this->naming_->rebind (name,
                         obj.in (),
                         ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Registered with the naming service as: %s\n",
              this->service_name));
}

POA_RtecEventChannelAdmin::EventChannel_ptr
CosEvent_Service::create_rtec (CORBA::Environment &ACE_TRY_ENV)
{
  // see if the user wants a local Rtec..
  if (this->remote_Rtec_ == 0)
    return CosEC_ServantBase::create_rtec (ACE_TRY_ENV);
  else
    return 0;
}

void
CosEvent_Service::activate_rtec (CORBA::Environment &ACE_TRY_ENV)
{
  // see if the user wants to use a local Rtec..
  if (this->remote_Rtec_ == 0)
    {
      CosEC_ServantBase::activate_rtec (ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      // Try to locate a remote rtec.
      this->locate_rtec (ACE_TRY_ENV);
      ACE_CHECK;

      // Use the return value to check success.
      if (CORBA::is_nil (this->rtec_.in ()))
        ACE_DEBUG ((LM_DEBUG,
                    "Could not locate a RT EventChannel.Please start one and try again\n"));
      ACE_CHECK;
    }
}

void
CosEvent_Service::deactivate_rtec (CORBA::Environment &ACE_TRY_ENV)
{
  // Check if the local rtec is to be deactivated.
  if (this->remote_Rtec_ == 0)
    {
      CosEC_ServantBase::deactivate_rtec (ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
CosEvent_Service::locate_rtec (CORBA::Environment &ACE_TRY_ENV)
{
  CosNaming::Name ref_name (1);
  ref_name.length (1);
  ref_name[0].id =
    CORBA::string_dup (this->rt_service_name);

  CORBA::Object_var obj =
    this->naming_->resolve (ref_name,
                            ACE_TRY_ENV);
  ACE_CHECK;

  this->rtec_ =
    RtecEventChannelAdmin::EventChannel::_narrow (obj.in (),
                                                  ACE_TRY_ENV);
  ACE_CHECK;
}

void
CosEvent_Service::resolve_naming_service (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService",
                                            ACE_TRY_ENV);
  ACE_CHECK;

  this->naming_ =
    CosNaming::NamingContext::_narrow (naming_obj.in (),
                                       ACE_TRY_ENV);
  ACE_CHECK;
}

int
CosEvent_Service::run (void)
{
  ACE_DEBUG ((LM_DEBUG, "%s: Running the CosEventService\n", __FILE__));
  if (this->orb_->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), 1);

  return 0;
}

void
CosEvent_Service::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  // Deactivate.
  this->deactivate (ACE_TRY_ENV);
  ACE_CHECK;

  // Unbind from the naming service.
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (this->service_name);

  this->naming_->unbind (name,
                         ACE_TRY_ENV);

  // shutdown the ORB.
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown ();
}

int
main (int argc, char *argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  CosEvent_Service service;

  // check command line args.
  if (service.parse_args (argc, argv) == -1)
    return 1;

  ACE_TRY_NEW_ENV
    {
      service.startup (argc,
                       argv,
                       ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (service.run () == -1)
        {
          service.shutdown ();
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Failed to run the CosEventService.\n"),
                            1);
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Failed to start CosEventService");
      return 1;
    }
  ACE_ENDTRY;

  service.shutdown ();

  return 0;
}
