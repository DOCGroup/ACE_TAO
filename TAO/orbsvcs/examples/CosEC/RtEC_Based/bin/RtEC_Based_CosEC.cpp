// $Id$

#include "RtEC_Based_CosEC.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "ace/Get_Opt.h"

RtEC_Based_CosEC::RtEC_Based_CosEC (void)
  : service_name ("CosEventService"),
    rt_service_name ("EventService"),
    remote_rtec_ (0)
{
  // No-Op.
}

RtEC_Based_CosEC::~RtEC_Based_CosEC (void)
{
  // No-Op.
}

void
RtEC_Based_CosEC::init_ORB  (int& argc, char *argv []
                             TAO_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                ""
                                TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var poa_object  =
    this->orb_->resolve_initial_references("RootPOA"
                                           TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in ()
                                  TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

int
RtEC_Based_CosEC::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "t:n:e:o:p:r");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          this->service_name = get_opt.opt_arg ();
          break;

        case 't':
          this->rt_service_name = get_opt.opt_arg ();
          break;

        case 'r':
          this->remote_rtec_ = 1;
          break;

        case 'e':
          this->eventTypeIds_ =  get_opt.opt_arg ();
          break;

        case 'o':
          this->eventSourceIds_ = get_opt.opt_arg ();
          break;

        case 'p':
          this->source_type_pairs_ = get_opt.opt_arg ();
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "\n\t-n <COS Event Service name>"
                      "\n\t-t <RealTime Event Service name>"
                      "\n\t-r" // creates the RtEC locally.
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
RtEC_Based_CosEC::startup (int argc, char *argv[]
                           TAO_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG,
              "Starting up the CosEvent Service...\n"));

  // initalize the ORB.
  this->init_ORB (argc, argv
                  TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->parse_args (argc, argv) == -1)
    ACE_THROW (CORBA::BAD_PARAM ());

  this->resolve_naming_service (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->init (this->poa_.in (),
              this->poa_.in (),
              this->eventTypeIds_,
              this->eventSourceIds_,
              this->source_type_pairs_
              TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Register ourselves with the naming service.
  ACE_ASSERT(!CORBA::is_nil (this->naming_.in ()));

  CORBA::Object_var obj =
    this->poa_->servant_to_reference (this
                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::String_var str =
    this->orb_->object_to_string (obj.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "The CosEC IOR is <%s>\n", str.in ()));

  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (this->service_name);

  this->naming_->rebind (name,
                         obj.in ()
                         TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Registered with the naming service as: %s\n",
              this->service_name));
}

POA_RtecEventChannelAdmin::EventChannel_ptr
RtEC_Based_CosEC::create_rtec (TAO_ENV_SINGLE_ARG_DECL)
{
  // see if the user wants a local RtEC..
  if (this->remote_rtec_ == 0)
    return CosEC_ServantBase::create_rtec (TAO_ENV_SINGLE_ARG_PARAMETER);
  else
    return 0;
}

void
RtEC_Based_CosEC::activate_rtec (TAO_ENV_SINGLE_ARG_DECL)
{
  // see if the user wants to use a local RtEC..
  if (this->remote_rtec_ == 0)
    {
      CosEC_ServantBase::activate_rtec (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      // Try to locate a remote rtec.
      this->locate_rtec (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // Use the return value to check success.
      if (CORBA::is_nil (this->rtec_.in ()))
        ACE_DEBUG ((LM_DEBUG,
                    "Could not locate a RT EventChannel.Please start one and try again\n"));
    }
}

void
RtEC_Based_CosEC::deactivate_rtec (TAO_ENV_SINGLE_ARG_DECL)
{
  // Check if the local rtec is to be deactivated.
  if (this->remote_rtec_ == 0)
    {
      CosEC_ServantBase::deactivate_rtec (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
RtEC_Based_CosEC::locate_rtec (TAO_ENV_SINGLE_ARG_DECL)
{
  CosNaming::Name ref_name (1);
  ref_name.length (1);
  ref_name[0].id =
    CORBA::string_dup (this->rt_service_name);

  CORBA::Object_var obj =
    this->naming_->resolve (ref_name
                            TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->rtec_ =
    RtecEventChannelAdmin::EventChannel::_narrow (obj.in ()
                                                  TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
RtEC_Based_CosEC::resolve_naming_service (TAO_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService"
                                            TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_THROW (CORBA::UNKNOWN ());

  this->naming_ =
    CosNaming::NamingContext::_narrow (naming_obj.in ()
                                       TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

int
RtEC_Based_CosEC::run (void)
{
  ACE_DEBUG ((LM_DEBUG, "%s: Running the CosEventService\n", __FILE__));
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_->run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), 1);
    }
  ACE_ENDTRY;

  return 0;
}

void
RtEC_Based_CosEC::shutdown (TAO_ENV_SINGLE_ARG_DECL)
{
  // Deactivate.
  this->deactivate (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Unbind from the naming service.
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (this->service_name);

  this->naming_->unbind (name
                         TAO_ENV_ARG_PARAMETER);

  // shutdown the ORB.
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown ();
}

int
main (int argc, char *argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  RtEC_Based_CosEC service;

  ACE_TRY_NEW_ENV
    {
      service.startup (argc,
                       argv
                       TAO_ENV_ARG_PARAMETER);
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
