// $Id$

#include "CounterC.h"

// @@ Angelo, please name files consistently, i.e., you haveRTCORBA::
// server.cpp, so this file should be client.cpp not Client.cpp

// @@ Marina Done.

ACE_RCSID(tao, client, "$Id$")

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  
  ACE_TRY
    {
      // ORB Initialization
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      
      CORBA::Object_var object;
      
      // Get the IOR from a file.
      object = orb->string_to_object ("file://ec.ior", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      // @@ Angelo, you always need to check for error conditions in
      // all your methods/invocations.  Below you need to check that
      // return value isn't nil, and use ACE_TRY_ENV to detect exceptions!
      // @@ Marina Sure! DONE.
      Counter_var counter = Counter::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      if (CORBA::is_nil (counter))
        {
          ACE_DEBUG ((LM_DEBUG, 
                      "Unable to convert the IOR to the proper object reference.\n"));
          return 1;
        }
      
      CORBA::ULong i;
      
      for (i = 0; i < 10; i++)
        {
          counter->increment (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      
      // @@ Angelo, make sure to always use ACE_TRY & friends to
      // detect exception conditions, especially on idl method invocations.
      
      // Marina DONE.
      
       ACE_DEBUG ((LM_DEBUG, "The Counter Value is: %d\n", counter->get_count()));
      
       counter->shutdown ();

      // @@ Angelo, use ACE_DEBUG instead of cout.
      // @@ Marina Done.
      
      CORBA::Policy *policy_ptr;
      policy_ptr = counter->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE, 
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      if (policy_ptr == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Unable to get Priority Policy.\n"));
          return 1;
        }
      
      RTCORBA::PriorityModelPolicy *priority_policy = 
        RTCORBA::PriorityModelPolicy::_narrow (policy_ptr);
      
      ACE_DEBUG ((LM_DEBUG, "Priority Model: %d\nCORBA Priority: %d", 
                  priority_policy->priority_model (ACE_TRY_ENV),       
                  priority_policy->server_priority (ACE_TRY_ENV)));
      
      ACE_TRY_CHECK;
      
      
    }
  
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "CORBA Excaption Raised");
      return 1;
    }
  
  ACE_ENDTRY;
  
  return 0;
}


