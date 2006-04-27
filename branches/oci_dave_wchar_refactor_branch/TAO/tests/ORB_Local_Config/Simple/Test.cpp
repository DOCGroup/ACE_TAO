// $Id$

#include "tao/CORBANAME_Parser.h"
#include "tao/CORBALOC_Parser.h"
#include "tao/Protocol_Factory.h"
#include "ace/Dynamic_Service.h"

#include "Service_Configuration_Per_ORB.h"


// @brief Dynamic services loading

int
run_main (int , ACE_TCHAR *[])
{
  ACE_Service_Gestalt_Test one; // Use the ACE_Service_Repository::instance ()

  one.process_directive (ace_svc_desc_TAO_CORBANAME_Parser);
  one.process_directive (ace_svc_desc_TAO_CORBALOC_Parser);

  TAO_Protocol_Factory* p1 =
    ACE_Dynamic_Service<TAO_Protocol_Factory>::instance (&one, "IIOP_Factory");

  ACE_ASSERT ((p1 == 0));

  TAO_CORBANAME_Parser* p2 =
    ACE_Dynamic_Service<TAO_CORBANAME_Parser>::instance (&one, "CORBANAME_Parser");

  ACE_ASSERT ((p2 != 0));

  TAO_CORBALOC_Parser* p3 =
    ACE_Dynamic_Service<TAO_CORBALOC_Parser>::instance (&one, "CORBALOC_Parser");

  ACE_ASSERT ((p3 != 0));

  return 0;
}
