// $Id$

#include "Multiple.h"

Multiple::Multiple (void)
  : cos_ec_ (CosEventChannelAdmin::EventChannel::_nil ()),
    service_name ("CosEventService"),
    orb_ (CORBA::ORB::_nil ())
{
  // No-Op.
}


Multiple::~Multiple (void)
{
  // No-Op.
}

int
Multiple::init (int argc, char *argv[])
{
  if (init_ORB (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "init_ORB returned error.\n"),
                      -1);

  if (this->parse_args (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "parse error returned error.\n"),
                      -1);

  if (init_CosEC () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "init_CosEC returned error.\n"),
                      -1);
  return 0;
}

int
Multiple::init_ORB  (int argc, char *argv [])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    ""
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object  =
        this->orb_->resolve_initial_references("RootPOA"
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          -1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in Multiple::init_ORB\n");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Multiple::init_CosEC (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
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
        this->naming_client_->resolve (ec_ref_name
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The CORBA::Object_var object is downcast to
      // CosEventChannelAdmin::EventChannel
      // using the <_narrow> method.
      this->cos_ec_ =
        CosEventChannelAdmin::EventChannel::_narrow (EC_obj.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in Multiple::init_ORB\n");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Multiple::runORB (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
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
Multiple::shutdown (void)
{
  if (!this->orb_->_nil ())
    this->orb_->shutdown ();
}
