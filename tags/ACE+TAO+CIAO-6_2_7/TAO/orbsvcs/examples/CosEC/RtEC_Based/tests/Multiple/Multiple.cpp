// $Id$

#include "Multiple.h"

Multiple::Multiple (void)
  : cos_ec_ (CosEventChannelAdmin::EventChannel::_nil ()),
    service_name_ (ACE_TEXT ("CosEventService")),
    orb_ (CORBA::ORB::_nil ())
{
  // No-Op.
}


Multiple::~Multiple (void)
{
  // No-Op.
}

int
Multiple::init (int argc, ACE_TCHAR *argv[])
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
Multiple::init_ORB  (int argc, ACE_TCHAR *argv [])
{
  try
    {
      this->orb_ = CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object  =
        this->orb_->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          -1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception in Multiple::init_ORB\n");
      return -1;
    }

  return 0;
}

int
Multiple::init_CosEC (void)
{
  try
    {
      // Initialization of the naming service.
      if (this->naming_client_.init (this->orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to initialize "
                           "the TAO_Naming_Client.\n"),
                          -1);

      CosNaming::Name ec_ref_name (1);
      ec_ref_name.length (1);
      ec_ref_name[0].id =
        CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(this->service_name_));

      CORBA::Object_var EC_obj =
        this->naming_client_->resolve (ec_ref_name);

      // The CORBA::Object_var object is downcast to
      // CosEventChannelAdmin::EventChannel
      // using the <_narrow> method.
      this->cos_ec_ =
        CosEventChannelAdmin::EventChannel::_narrow (EC_obj.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception in Multiple::init_ORB\n");
      return -1;
    }

  return 0;
}

int
Multiple::runORB (void)
{
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
Multiple::shutdown (void)
{
  if (!this->orb_->_nil ())
    this->orb_->shutdown ();
}
