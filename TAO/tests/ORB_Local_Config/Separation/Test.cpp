// $Id$

#include "tao/CORBANAME_Parser.h"
#include "tao/CORBALOC_Parser.h"
#include "tao/IIOP_Factory.h"

#include "ace/Service_Config.h"
#include "ace/Dynamic_Service.h"

#include "Service_Configuration_Per_ORB.h"

// @brief Services registered with separate repositories must remain separate
// and inaccesible through anyone but the one they were gegistered with

int
testSeparation(int , ACE_TCHAR *[])
{
  ACE_TRACE ("testSeparation");

  ACE_Service_Gestalt_Test one (10);
  one.process_directive (ace_svc_desc_TAO_CORBANAME_Parser);

  ACE_Service_Gestalt_Test two (10);
  two.process_directive (ace_svc_desc_TAO_CORBALOC_Parser);

  const ACE_TCHAR * svcname = "IIOP_Factory";

  TAO_Protocol_Factory* p10 =
    ACE_Dynamic_Service<TAO_Protocol_Factory>::instance (&one, svcname);
  if (p10 != 0)
    ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Not expected %s locally in one\n"), svcname), -1);

  TAO_Protocol_Factory* p11 =
    ACE_Dynamic_Service<TAO_Protocol_Factory>::instance (&two, "IIOP_Factory");
  if (p11 != 0)
    ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Not expected %s locally in two\n"), svcname), -1);

  svcname = "CORBANAME_Parser";

  ACE_Service_Object* p20 =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (&one, svcname);
  if (p20 == 0)
    ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Expected %s locally, in one\n"), svcname), -1);

  ACE_Service_Object* p31 =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (&two, svcname);
  if (p31 != 0)
    ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Not expected %s locally, in two\n"), svcname), -1);

  svcname = "CORBALOC_Parser";

  ACE_Service_Object* p21 =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (&one, svcname);
  if (p21 != 0)
    ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Not expected %s locally, in one\n"), svcname), -1);

  ACE_Service_Object* p30 =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (&two, svcname);
  if (p30 == 0)
    ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Expected %s locally, in two\n"), svcname), -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return testSeparation (argc, argv);
}


