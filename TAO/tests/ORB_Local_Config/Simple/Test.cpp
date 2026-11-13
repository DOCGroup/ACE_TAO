#include "tao/CORBANAME_Parser.h"
#include "tao/CORBALOC_Parser.h"
#include "tao/Protocol_Factory.h"
#include "ace/Dynamic_Service.h"
#include "ace/Intrusive_Auto_Ptr.h"

#include "Service_Configuration_Per_ORB.h"

/// Loads static parser services into a local gestalt and verifies their
/// visibility through that gestalt.
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> const one {
    new ACE_Service_Gestalt ()
  };

  if (one->process_directive (ace_svc_desc_TAO_CORBANAME_Parser) != 0
      || one->process_directive (ace_svc_desc_TAO_CORBALOC_Parser) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Failed to register parser services\n")),
                      -1);

  TAO_Protocol_Factory const * const p1 =
    ACE_Dynamic_Service<TAO_Protocol_Factory>::instance (one.get (), "IIOP_Factory");

  if (p1 != nullptr)
    ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT ("Unexpected to find IIOP_Factory\n")), -1);

  ACE_Service_Object const * const p2 =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (one.get (), "CORBANAME_Parser");

  if (p2 == nullptr)
    ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT ("Expected to find CORBANAME_Parser\n")), -1);

  ACE_Service_Object const * const p3 =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (one.get (), "CORBALOC_Parser");

  if (p3 == nullptr)
    ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT ("Expected to find CORBALOC_Parser\n")), -1);

  return 0;
}
