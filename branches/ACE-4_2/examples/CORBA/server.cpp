#include "ace/Service_Config.h"
// $Id$

#include "ace/CORBA_Handler.h"
#include "Test_i.h"

#if defined (ACE_HAS_ORBIX)

#if defined (ACE_HAS_MT_ORBIX)
typedef ACE_MT_CORBA_Handler CORBA_HANDLER;
#else
typedef ACE_ST_CORBA_Handler CORBA_HANDLER;
#endif /* ACE_HAS_MT_ORBIX */

int 
main (int argc, char *argv[])
{
  ACE_Service_Config daemon;

  char pwd[BUFSIZ];
  char app[BUFSIZ];

  ACE_OS::getcwd (pwd, sizeof pwd);
  ACE_OS::sprintf (app, "%s/%s", pwd, argv[0]);

  if (CORBA_HANDLER::instance ()->activate_service (Test_IMPL, 0, app) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "Could not activate services for supplier\n"), -1);

  TIE_Test (Test_i) test (new Test_i);
  
  for (;;)
    if (CORBA_HANDLER::instance ()->reactor ()->handle_events () == -1)
      break;

  return 0;
}
#endif /* ACE_HAS_ORBIX */
