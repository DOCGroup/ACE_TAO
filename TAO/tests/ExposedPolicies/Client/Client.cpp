// $Id$

#include "CounterC.h"

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  

  ACE_TRY
    {
      // ORB Initialization
      int argc = 1;
      char * argv[] = {"RemoteEventDispatcher"};
      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
      
      // Get Reference to the Naming Service
      CORBA::Object_var object;
      
      object = orb->resolve_initial_references("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      PortableServer::POA_var poa 
        = PortableServer::POA::_narrow(object.in(), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      // Get the POAManager
      PortableServer::POAManager_var poa_manager 
        = poa->the_POAManager(ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      // Activate the POAManager
      poa_manager->activate(ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      object = orb->string_to_object ("file://ec.ior", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      Counter_var counter = Counter::_narrow (object.in());

      CORBA::ULong i;
      
      for (i = 0; i < 10; i++)
        counter->increment();
      
      cout << "The Counter Value is: " << counter->get_count() << endl;
      
      TAO_Stub* stub = counter->_stubobj ();
      TAO_Profile *profile = stub->next_profile ();

      CORBA::PolicyList &policy_list = profile->policies ();

      cout << "The policy list length is: " << policy_list.length () << endl;

      for (i = 0; i < policy_list.length (); i++)
        cout << "The " << i << "th Policy type is: " << policy_list[i]->policy_type () << endl;

    }

  ACE_CATCHANY 
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "CORBA Excaption Raised");
      return 1;
    }
  
  ACE_ENDTRY;

  return 0;
}
