/** 
 * @file proc_failure.cpp
 * @author Friedhelm Wolf <fwolf@dre.vanderbilt.edu>
 *  $Id$
 * Triggers a proc_failure () call on the FaultCorrelationManager 
 * interface
 */

#include "Interfaces/FaultCorrelationManagerC.h"

int usage ()
{
  ACE_ERROR ((LM_ERROR, "proc_failure <fcm_url> <object_id> <node_id>\n"));
  return -1;
}

#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
  
  if (argc != 4)
    return usage ();
  
  try
    {
      ACE_DEBUG ((LM_DEBUG, "resolving FCM reference %C\n", argv[1]));

      CORBA::Object_var obj = orb->string_to_object (argv[1]);

      DAnCE::FaultCorrelationManager_var fcm =
        DAnCE::FaultCorrelationManager::_narrow (obj.in ());

      if (CORBA::is_nil (fcm.in ()))
        {
          ACE_DEBUG ((LM_ERROR, "FCM reference is nil.\n"));
          return -1;
        }

      ACE_DEBUG ((LM_DEBUG, 
                  "calling proc_failure (%C, %C)\n",
                  argv[2],
                  argv[3]));

      fcm->proc_failure (argv[2], argv[3]);
    }
  catch (CORBA::Exception &ex)
    {
      ACE_ERROR ((LM_ERROR, "*** Caught CORBA exception: %s\n",
                  ex._info ().c_str ()));
      return -1;
      
    }
  catch (...)
    {
      orb->destroy ();
      ACE_ERROR ((LM_ERROR, "*** Caugn unknown exception\n"));
      return -1;
    }
  return 0;
}
