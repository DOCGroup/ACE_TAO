// $Id$

#include "CosEC_Multiple.h"

CosEC_Multiple::CosEC_Multiple (void)
  : cos_ec_ (CosEventChannelAdmin::EventChannel::_nil ()),
    service_name ("CosEventService"),
    orb_ (CORBA::ORB::_nil ())
{
  // No-Op.
}


CosEC_Multiple::~CosEC_Multiple (void)
{
  // No-Op.
}

int
CosEC_Multiple::init (int argc, char *argv[])
{
  if (this->parse_args (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "parse error returned error.\n"),
                      -1);

  if (init_ORB (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "init_ORB returned error.\n"),
                      -1);

  if (init_CosEC () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "init_CosEC returned error.\n"),
                      -1);
  return 0;
}

int
CosEC_Multiple::init_ORB  (int argc, char *argv [])
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
      TAO_TRY_ENV.print_exception ("Exception in CosEC_Basic::init_ORB\n");
      return -1;
    }
  TAO_ENDTRY;
}

int
CosEC_Multiple::init_CosEC (void)
{
  TAO_TRY
    {
      // Initialization of the naming service.
      if (this->naming_client_.init (this->orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);

      CosNaming::Name ec_ref_name (1);
      ec_ref_name.length (1);
      ec_ref_name[0].id =
        CORBA::string_dup (this->service_name);

      CORBA::Object_var EC_obj =
        this->naming_client_->resolve (ec_ref_name,
				      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // The CORBA::Object_var object is downcast to
      // CosEventChannelAdmin::EventChannel
      // using the <_narrow> method.
      this->cos_ec_ =
        CosEventChannelAdmin::EventChannel::_narrow (EC_obj.in (),
                                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;

      return 0;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Exception in init_CosEC\n");
      return -1;
    }
  TAO_ENDTRY;
}

int
CosEC_Multiple::runORB (void)
{
  if (this->orb_->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);

  return 0;
}

void
CosEC_Multiple::shutdown (void)
{
  if (this->orb_ != CORBA::ORB::_nil ())
    this->orb_->shutdown ();

  this->orb_ == CORBA::ORB::_nil ();
}
