#include "Multiple.h"

int
Multiple::init (int argc, ACE_TCHAR *argv[])
{
  if (init_ORB (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("init_ORB returned error.\n")),
                      -1);

  if (this->parse_args (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("parse error returned error.\n")),
                      -1);

  if (init_CosEC () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("init_CosEC returned error.\n")),
                      -1);
  return 0;
}

int
Multiple::init_ORB (int argc, ACE_TCHAR *argv[])
{
  try
    {
      this->orb_ = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        this->orb_->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          -1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Exception in Multiple::init_ORB\n"));
      return -1;
    }

  return 0;
}

int
Multiple::init_CosEC ()
{
  try
    {
      // Initialization of the naming service.
      if (this->naming_client_.init (this->orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Unable to initialize ")
                           ACE_TEXT ("the TAO_Naming_Client.\n")),
                          -1);

      CosNaming::Name ec_ref_name (1);
      ec_ref_name.length (1);
      ec_ref_name[0].id =
        CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (this->service_name_.c_str ()));

      CORBA::Object_var const ec_obj =
        this->naming_client_->resolve (ec_ref_name);

      // The CORBA::Object_var object is downcast to
      // CosEventChannelAdmin::EventChannel
      // using the <_narrow> method.
      this->cos_ec_ =
        CosEventChannelAdmin::EventChannel::_narrow (ec_obj.in ());
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Exception in Multiple::init_CosEC\n"));
      return -1;
    }

  return 0;
}

int
Multiple::runORB ()
{
  try
    {
      this->orb_->run ();
    }
  catch (CORBA::Exception const &)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("run")), 1);
    }

  return 0;
}

void
Multiple::shutdown ()
{
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown ();
}
