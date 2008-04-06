// $Id$

#include "tao/CORBANAME_Parser.h"
#include "tao/CORBALOC_Parser.h"
#include "tao/Protocol_Factory.h"
#include "ace/Dynamic_Service.h"
#include "ace/Intrusive_Auto_Ptr.h"

#include "Service_Configuration_Per_ORB.h"

// @brief All default-constructor-created Service Config instabces referr to the
// The One Global Configuration

int
testReusingGlobals (int , ACE_TCHAR *[])
{
  ACE_TRACE ("testReusingGlobals");

  {
    ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> one (new ACE_Service_Gestalt (10, false));

    // The ACE_Service_Gestalt_Test will teardown all!
    one->process_directive (ace_svc_desc_TAO_CORBANAME_Parser);
    one->process_directive (ace_svc_desc_TAO_CORBALOC_Parser);

    const ACE_TCHAR *svcname = ACE_TEXT ("IIOP_Factory");
    TAO_Protocol_Factory* p1 = ACE_Dynamic_Service<TAO_Protocol_Factory>::instance (one.get (), svcname);

    if (p1 != 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Not expected to find %s locally\n"), svcname));
        return -1;
      }

    svcname = ACE_TEXT ("CORBANAME_Parser");
    ACE_Service_Object* p2 = ACE_Dynamic_Service<ACE_Service_Object>::instance (one.get (), svcname);

    if (p2 == 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Expected to find %s localy\n"), svcname));
        return -1;
      }

    svcname = ACE_TEXT ("CORBALOC_Parser");
    ACE_Service_Object* p3 = ACE_Dynamic_Service<ACE_Service_Object>::instance (one.get (), svcname);

    if (p3 == 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Expected to find %s locally\n"), svcname));
        return -1;
      }
  }


  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> two (new ACE_Service_Gestalt_Test (10));
  //  ACE_Service_Gestalt_Test two; // Use the ACE_Service_Repository::instance ()

  const ACE_TCHAR *svcname = ACE_TEXT ("IIOP_Factory");
  TAO_Protocol_Factory* p1 = ACE_Dynamic_Service<TAO_Protocol_Factory>::instance (two.get (), svcname);

  if (p1 != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Not expected to find %s in the global repo\n"), svcname));
      return -1;
    }

  svcname = ACE_TEXT ("CORBANAME_Parser");
  ACE_Service_Object* p2 = ACE_Dynamic_Service<ACE_Service_Object>::instance (two.get (), svcname);

  if (p2 == 0) // You should be able to find the same stuff here, too``
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Not expected to find %s in the global repo\n"), svcname));
        return -1;
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return testReusingGlobals (argc, argv);
}

