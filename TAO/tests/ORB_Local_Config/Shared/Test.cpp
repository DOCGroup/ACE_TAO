#include "tao/CORBANAME_Parser.h"
#include "tao/CORBALOC_Parser.h"
#include "tao/Protocol_Factory.h"
#include "ace/Dynamic_Service.h"
#include "ace/Intrusive_Auto_Ptr.h"

#include "Service_Configuration_Per_ORB.h"

// @brief All default-constructor-created Service Config instabces referr to the
// The One Global Configuration

int
testReusingGlobals (int, ACE_TCHAR *[])
{
  ACE_TRACE ("testReusingGlobals");

  {
    ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> const one (
      new ACE_Service_Gestalt (10, false));

    // The ACE_Service_Gestalt_Test will teardown all!
    one->process_directive (ace_svc_desc_TAO_CORBANAME_Parser);
    one->process_directive (ace_svc_desc_TAO_CORBALOC_Parser);

    ACE_TCHAR const * const iiop_factory = ACE_TEXT ("IIOP_Factory");
    TAO_Protocol_Factory * const p1 =
      ACE_Dynamic_Service<TAO_Protocol_Factory>::instance (one.get (), iiop_factory);

    if (p1 != nullptr)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Not expected to find %s locally\n"),
                    iiop_factory));
        return -1;
      }

    ACE_TCHAR const * const corbaname_parser = ACE_TEXT ("CORBANAME_Parser");
    ACE_Service_Object * const p2 =
      ACE_Dynamic_Service<ACE_Service_Object>::instance (one.get (), corbaname_parser);

    if (p2 == nullptr)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Expected to find %s locally\n"),
                    corbaname_parser));
        return -1;
      }

    ACE_TCHAR const * const corbaloc_parser = ACE_TEXT ("CORBALOC_Parser");
    ACE_Service_Object * const p3 =
      ACE_Dynamic_Service<ACE_Service_Object>::instance (one.get (), corbaloc_parser);

    if (p3 == nullptr)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Expected to find %s locally\n"),
                    corbaloc_parser));
        return -1;
      }
  }

  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> const two (
    new ACE_Service_Gestalt_Test (10));
  //  ACE_Service_Gestalt_Test two; // Use the ACE_Service_Repository::instance ()

  ACE_TCHAR const * const iiop_factory = ACE_TEXT ("IIOP_Factory");
  TAO_Protocol_Factory * const p1 =
    ACE_Dynamic_Service<TAO_Protocol_Factory>::instance (two.get (), iiop_factory);

  if (p1 != nullptr)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Not expected to find %s in the global repo\n"),
                  iiop_factory));
      return -1;
    }

  ACE_TCHAR const * const corbaname_parser = ACE_TEXT ("CORBANAME_Parser");
  ACE_Service_Object * const p2 =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (two.get (), corbaname_parser);

  if (p2 == nullptr) // You should be able to find the same stuff here, too
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Not expected to find %s in the global repo\n"),
                  corbaname_parser));
      return -1;
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return testReusingGlobals (argc, argv);
}

