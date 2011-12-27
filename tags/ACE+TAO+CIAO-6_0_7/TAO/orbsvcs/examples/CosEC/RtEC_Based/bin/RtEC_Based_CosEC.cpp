// $Id$

#include "RtEC_Based_CosEC.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "ace/Get_Opt.h"

RtEC_Based_CosEC::RtEC_Based_CosEC (void)
  : service_name (ACE_TEXT("CosEventService")),
    rt_service_name (ACE_TEXT("EventService")),
    remote_rtec_ (0)
{
  // No-Op.
}

RtEC_Based_CosEC::~RtEC_Based_CosEC (void)
{
  // No-Op.
}

void
RtEC_Based_CosEC::init_ORB  (int& argc, ACE_TCHAR *argv [])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  CORBA::Object_var poa_object  =
    this->orb_->resolve_initial_references("RootPOA");

  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager ();

  poa_manager->activate ();
}

int
RtEC_Based_CosEC::parse_args (int argc, ACE_TCHAR *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("t:n:e:o:p:r"));
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
          this->eventTypeIds_ = get_opt.opt_arg ();
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
RtEC_Based_CosEC::startup (int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ((LM_DEBUG,
              "Starting up the CosEvent Service...\n"));

  // initalize the ORB.
  this->init_ORB (argc, argv);

  if (this->parse_args (argc, argv) == -1)
    throw CORBA::BAD_PARAM ();

  this->resolve_naming_service ();

  this->init (this->poa_.in (),
              this->poa_.in (),
              this->eventTypeIds_,
              this->eventSourceIds_,
              this->source_type_pairs_);

  this->activate ();

  // Register ourselves with the naming service.
  ACE_ASSERT(!CORBA::is_nil (this->naming_.in ()));

  CORBA::Object_var obj =
    this->poa_->servant_to_reference (this);

  CORBA::String_var str =
    this->orb_->object_to_string (obj.in ());

  ACE_DEBUG ((LM_DEBUG,
              "The CosEC IOR is <%s>\n", str.in ()));

  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(this->service_name));

  this->naming_->rebind (name,
                         obj.in ());

  ACE_DEBUG ((LM_DEBUG,
              "Registered with the naming service as: %s\n",
              this->service_name));
}

POA_RtecEventChannelAdmin::EventChannel_ptr
RtEC_Based_CosEC::create_rtec (void)
{
  // see if the user wants a local RtEC..
  if (this->remote_rtec_ == 0)
    return CosEC_ServantBase::create_rtec ();
  else
    return 0;
}

void
RtEC_Based_CosEC::activate_rtec (void)
{
  // see if the user wants to use a local RtEC..
  if (this->remote_rtec_ == 0)
    {
      CosEC_ServantBase::activate_rtec ();
    }
  else
    {
      // Try to locate a remote rtec.
      this->locate_rtec ();

      // Use the return value to check success.
      if (CORBA::is_nil (this->rtec_.in ()))
        ACE_DEBUG ((LM_DEBUG,
                    "Could not locate a RT EventChannel.Please start one and try again\n"));
    }
}

void
RtEC_Based_CosEC::deactivate_rtec (void)
{
  // Check if the local rtec is to be deactivated.
  if (this->remote_rtec_ == 0)
    {
      CosEC_ServantBase::deactivate_rtec ();
    }
}

void
RtEC_Based_CosEC::locate_rtec (void)
{
  CosNaming::Name ref_name (1);
  ref_name.length (1);
  ref_name[0].id =
    CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(this->rt_service_name));

  CORBA::Object_var obj =
    this->naming_->resolve (ref_name);

  this->rtec_ =
    RtecEventChannelAdmin::EventChannel::_narrow (obj.in ());
}

void
RtEC_Based_CosEC::resolve_naming_service (void)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService");

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    throw CORBA::UNKNOWN ();

  this->naming_ =
    CosNaming::NamingContext::_narrow (naming_obj.in ());
}

int
RtEC_Based_CosEC::run (void)
{
  ACE_DEBUG ((LM_DEBUG, "%s: Running the CosEventService\n", __FILE__));
  try
    {
      this->orb_->run ();
    }
  catch (const CORBA::Exception&)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), 1);
    }

  return 0;
}

void
RtEC_Based_CosEC::shutdown (void)
{
  // Deactivate.
  this->deactivate ();

  // Unbind from the naming service.
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(this->service_name));

  this->naming_->unbind (name);

  // shutdown the ORB.
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown ();
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  RtEC_Based_CosEC service;

  try
    {
      service.startup (argc,
                       argv);

      if (service.run () == -1)
        {
          service.shutdown ();
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Failed to run the CosEventService.\n"),
                            1);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Failed to start CosEventService");
      return 1;
    }

  service.shutdown ();

  return 0;
}
