#include "tao/CORBANAME_Parser.h"
#include "tao/CORBALOC_Parser.h"
#include "tao/IIOP_Factory.h"

#include "ace/Service_Config.h"
#include "ace/Intrusive_Auto_Ptr.h"
#include "ace/Dynamic_Service.h"

#include "Service_Configuration_Per_ORB.h"

/// Verifies that services registered with separate repositories remain
/// visible only through the gestalt where each service was registered.
int
testSeparation (int, ACE_TCHAR *[])
{
  ACE_TRACE ("testSeparation");

  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> const one {
    new ACE_Service_Gestalt_Test (10)
  };
  if (one->process_directive (ace_svc_desc_TAO_CORBANAME_Parser) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Failed to register CORBANAME_Parser in one\n")),
                      -1);

  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> const two {
    new ACE_Service_Gestalt_Test (10)
  };
  if (two->process_directive (ace_svc_desc_TAO_CORBALOC_Parser) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Failed to register CORBALOC_Parser in two\n")),
                      -1);

  ACE_TCHAR const * const iiop_factory = ACE_TEXT ("IIOP_Factory");

  TAO_Protocol_Factory const * const p10 =
    ACE_Dynamic_Service<TAO_Protocol_Factory>::instance (one.get (), iiop_factory);
  if (p10 != nullptr)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("Not expected %s locally in one\n"),
                       iiop_factory),
                      -1);

  TAO_Protocol_Factory const * const p11 =
    ACE_Dynamic_Service<TAO_Protocol_Factory>::instance (two.get (), iiop_factory);
  if (p11 != nullptr)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("Not expected %s locally in two\n"),
                       iiop_factory),
                      -1);

  ACE_TCHAR const * const corbaname_parser = ACE_TEXT ("CORBANAME_Parser");

  ACE_Service_Object const * const p20 =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (one.get (), corbaname_parser);
  if (p20 == nullptr)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("Expected %s locally, in one\n"),
                       corbaname_parser),
                      -1);

  ACE_Service_Object const * const p31 =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (two.get (), corbaname_parser);
  if (p31 != nullptr)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("Not expected %s locally, in two\n"),
                       corbaname_parser),
                      -1);

  ACE_TCHAR const * const corbaloc_parser = ACE_TEXT ("CORBALOC_Parser");

  ACE_Service_Object const * const p21 =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (one.get (), corbaloc_parser);
  if (p21 != nullptr)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("Not expected %s locally, in one\n"),
                       corbaloc_parser),
                      -1);

  ACE_Service_Object const * const p30 =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (two.get (), corbaloc_parser);
  if (p30 == nullptr)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("Expected %s locally, in two\n"),
                       corbaloc_parser),
                      -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return testSeparation (argc, argv);
}
