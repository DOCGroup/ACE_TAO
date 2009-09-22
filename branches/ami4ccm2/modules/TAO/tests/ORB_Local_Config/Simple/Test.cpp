// $Id$

#include "tao/CORBANAME_Parser.h"
#include "tao/CORBALOC_Parser.h"
#include "tao/Protocol_Factory.h"
#include "ace/Dynamic_Service.h"
#include "ace/Intrusive_Auto_Ptr.h"

#include "Service_Configuration_Per_ORB.h"


// @brief Dynamic services loading

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> one (new ACE_Service_Gestalt());

  one->process_directive (ace_svc_desc_TAO_CORBANAME_Parser);
  one->process_directive (ace_svc_desc_TAO_CORBALOC_Parser);

  TAO_Protocol_Factory* p1 = ACE_Dynamic_Service<TAO_Protocol_Factory>::instance (one.get (), "IIOP_Factory");

  if (p1 != 0)
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Unexpected to find IIOP_Factory\n")), -1);

  ACE_Service_Object* p2 = ACE_Dynamic_Service<ACE_Service_Object>::instance (one.get (), "CORBANAME_Parser");

  if (p2 == 0)
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Expected to find CORBANAME_Parser\n")), -1);

  ACE_Service_Object* p3 = ACE_Dynamic_Service<ACE_Service_Object>::instance (one.get (), "CORBALOC_Parser");

  if (p3 == 0)
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Expected to find CORBALOC_Parser\n")), -1);

  return 0;
}
