// $Id$
/**
 * @file CmpClient.cpp
 *
 * @brief This file contains a client of TargetManager.
 *
 * It picks up the TM ior from the Targetmanager.ior file
 * present in the current directory, and makes the calls on the
 * TM to getAllResources and getAvailable resources
 */

#include "TargetManagerC.h"
#include "ace/streams.h"
#include "Config_Handlers/DnC_Dump.h"


int main (int argc, char* argv[])
{
 try {
      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var orb =
          CORBA::ORB_init (argc, argv,
                       "" /* the ORB name, it can be anything! */);

      // There must be at least two arguments, the first is the factory
      // name, the rest are the names of the stock symbols we want to
      // get quotes for.
      if (argc < 2) {
        cerr << "Usage: " << argv[0]
           << " Factory_IOR ..." << endl;
      return 1;
    }

    // Use the first argument to create the factory object reference,
    // in real applications we use the naming service, but let's do
    // the easy part first!
    CORBA::Object_var factory_object =
      orb->string_to_object (argv[1]);

    // Now downcast the object reference to the appropriate type
    CIAO::TargetManagerImpl_var targetCmp =
      CIAO::TargetManagerImpl::_narrow (factory_object.in ());

    // Now get the facet reference from the target Manager Component
    ACE_DEBUG((LM_DEBUG, "Making a Call to provide_targetMgr ()\n"));
    Deployment::TargetManager_ptr targetI = targetCmp->provide_targetMgr ();

    // Now make calls on the Target Manager facet

    try
    {
      Deployment::Domain_var domainV = targetI->getAllResources ();
      ACE_DEBUG ((LM_DEBUG , "\n\nGetAllResources Returned \n"));
      ::Deployment::DnC_Dump::dump (domainV);
    }
    catch(CORBA::NO_IMPLEMENT & ex)
    {
      ACE_DEBUG((LM_DEBUG ,"Error:TargetManager:CORBA::NO_IMPLEMENT thrown\n"));
    }
    catch(CORBA::Exception & ex)
    {
      ACE_DEBUG((LM_DEBUG ,"Error:TargetManager:CORBA Generic Exception \n"));
      ACE_DEBUG((LM_DEBUG ,"Error:TargetManager:Exception in TargetManager call\n"));
    }

    // Now make a call of getAvailableResources on the TargetManager ...
    try
    {
      Deployment::Domain_var domainV = targetI->getAvailableResources();
      ACE_DEBUG ((LM_DEBUG , "\n\nGetAvailableResources Returned \n"));
      ::Deployment::DnC_Dump::dump (domainV);
    }
    catch(CORBA::NO_IMPLEMENT & ex)
    {
      cerr << "Error:TargetManager:CORBA::NO_IMPLEMENT thrown" << endl;
    }
    catch(CORBA::Exception & ex)
    {
      cout << "Error:TargetManager:CORBA Generic Exception " << endl;
      cerr << "Error:TargetManager:Exception in TargetManager call" << ex << endl;
    }

    // Finally destroy the ORB
    orb->destroy ();
  }
  catch (CORBA::Exception & ex) {
    cerr << "Error:TargetManager:CORBA exception raised!" << ex << endl;
  }
  return 0;
}
