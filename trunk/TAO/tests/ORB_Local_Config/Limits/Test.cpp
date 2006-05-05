// $Id$

#include "tao/CORBANAME_Parser.h"
#include "tao/CORBALOC_Parser.h"

#include "Service_Configuration_Per_ORB.h"

// @brief The size of a repository is pre-determined and can not be exceeded

int
testLimits (int , ACE_TCHAR *[])
{
  ACE_Service_Gestalt one(1); // Room for just one ...
  if (0 != one.process_directive (ace_svc_desc_TAO_CORBANAME_Parser))
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Unable to register the first service\n")), -1);

  if (-1 != one.process_directive (ace_svc_desc_TAO_CORBALOC_Parser))
      ACE_ERROR_RETURN ((LM_DEBUG, ACE_TEXT("Unexped to be able to add more\n")), -1);

  if (ENOSPC != errno)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT("Expected error code %d (ENOSPC), but got %d\n"),
                       ENOSPC,
                       errno),
                      -1);

  ACE_DEBUG ((LM_DEBUG, "%p\n", "\tAttempt to overfill returned: "));
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  return testLimits (argc, argv);
}
