// $Id$

#include "tao/CORBANAME_Parser.h"
#include "tao/CORBALOC_Parser.h"

#include "Service_Configuration_Per_ORB.h"

// @brief The size of a repository is pre-determined and can not be exceeded

int
run_main (int , ACE_TCHAR *[])
{
  ACE_Service_Gestalt one(1); // Room for just one ...
  ACE_ASSERT (0 == one.process_directive (ace_svc_desc_TAO_CORBANAME_Parser));
  ACE_ASSERT (-1 == one.process_directive (ace_svc_desc_TAO_CORBALOC_Parser));
  ACE_ASSERT (ENOSPC == errno);
  ACE_DEBUG ((LM_DEBUG, "%p\n", "\tReporting an expected error: "));
  return 0;
}
