#include "DLL_TAO_Service.h"

ACE_DLL_SERVICE_IMPL (ACE_DLL_TAO_Service, _make_ACE_DLL_TAO_Service)

ACE_DLL_TAO_Service::ACE_DLL_TAO_Service ()
{
  ACE_DLL_TAO_SERVICE_TRACE (ACE_TEXT ("ACE_DLL_TAO_Service::ACE_DLL_TAO_Service ()"));
}

ACE_DLL_TAO_Service::~ACE_DLL_TAO_Service ()
{
  ACE_DLL_TAO_SERVICE_TRACE (ACE_TEXT ("ACE_DLL_TAO_Service::~ACE_DLL_TAO_Service ()"));
}

int ACE_DLL_TAO_Service::init (int argc, ACE_TCHAR * argv [])
{
  ACE_DLL_TAO_SERVICE_TRACE (ACE_TEXT ("ACE_DLL_TAO_Service::init (int, ACE_TCHAR * [])"));

  try
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) %T - %M - initializing the CORBA ORB\n")));
    this->orb_ = CORBA::ORB_init (argc, argv);

    // This call should fail unless '-ORBGestalt CURRENT' is passed as a command-line
    // option during ORB_init (...).
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) %T - %M - resolving the RootPOA\n")));

    CORBA::Object_var obj =
      this->orb_->resolve_initial_references ("RootPOA");
    this->root_poa_ = PortableServer::POA::_narrow (obj.in ());
  }
  catch (const CORBA::Exception & ex)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%t) %T - %M - %s\n"),
                ex._info ().c_str ()));
    return -1;
  }

  return 0;
}

int ACE_DLL_TAO_Service::fini ()
{
  ACE_DLL_TAO_SERVICE_TRACE (ACE_TEXT ("ACE_DLL_TAO_Service::fini ()"));

  try
  {
    this->orb_->shutdown (true);

    this->root_poa_->destroy (1, 1);
    this->orb_->destroy ();
  }
  catch (const CORBA::Exception & ex)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%t) %T - %M - %s\n"),
                ex._info ().c_str ()));
    return -1;
  }

  return 0;
}
