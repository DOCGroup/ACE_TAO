// -*- C++ -*-
// $Id$

#include "tao/ORB.h"
#include "tao/Object.h"

#include "ace/Service_Config.h"
#include "ace/Dynamic_Service.h"

int
testCurrentORBInitializer (int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  if (orb.in () == 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Unable to create orb\n")), -1);

  CORBA::Object_var obj =
    orb->resolve_initial_references ("TAO::Transport::Current");

  if (obj.in () == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to get the Transport Current\n")), -1);


  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("testCurrentORBInitializer: OK\n")));
  return 0;
}


int
testCurrentLoader (int, char *[])
{

  ACE_Service_Object *factory = 0;

  factory = ACE_Dynamic_Service<ACE_Service_Object>::instance
    (ACE_TEXT ("TAO_Transport_Current_Loader"));

  int done =
#if defined (TAO_AS_STATIC_LIBS)
    true;
#else
  (factory != 0);
#endif /* TAO_AS_STATIC_LIBS */

  if (factory == 0)
    {
      if (done)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Unable to instantiate TAO::Transport::Current_Factory\n")),
                          -1);
      else
        {

#if !defined (TAO_AS_STATIC_LIBS)
          int ret = ACE_Service_Config::process_directive
            (ACE_DYNAMIC_SERVICE_DIRECTIVE("TAO_Transport_Current_Loader",
                                           "TAO_Transport_Current",
                                           "_make_TAO_Transport_Current_Loader",
                                           ""));
          ACE_ASSERT (ret == 0);

          factory = ACE_Dynamic_Service<ACE_Service_Object>::instance
            (ACE_TEXT ("TAO_Transport_Current_Loader"));

#endif /* !TAO_AS_STATIC_LIBS */
        }
    }

  //  ACE_ASSERT (factory != 0);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("testCurrentLoader: OK\n")));
  return 0;

}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  return testCurrentLoader (argc, argv)
    || testCurrentORBInitializer (argc, argv);
}
