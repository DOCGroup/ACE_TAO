// CmpClient.cpp,v 1.3 2006/01/25 00:10:27 dengg Exp
/**
 * @file CmpClient.cpp
 *
 * @brief This file contains a client of TargetManager.
 *
 * It picks up the TM ior from the Targetmanager.ior file
 * present in the current directory, and makes the calls on the
 * TM to getAllResources and getAvailable resources
 */

#include "Deployment_DataC.h"
#include "TargetManagerImplC.h"
#include "ace/streams.h"
#include "Config_Handlers/DnC_Dump.h"

void write_to_file (::Deployment::Domain domain);

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
    catch(CORBA::NO_IMPLEMENT &)
    {
      ACE_DEBUG((LM_DEBUG ,"Error:TargetManager:CORBA::NO_IMPLEMENT thrown\n"));
    }
    catch(CORBA::Exception &)
    {
      ACE_DEBUG((LM_DEBUG ,"Error:TargetManager:CORBA Generic Exception \n"));
      ACE_DEBUG((LM_DEBUG ,"Error:TargetManager:Exception in TargetManager call\n"));
    }


    // make a call to the commit resources .....


    Deployment::DeploymentPlan plan;

    plan.instance.length (2);

    ::Deployment::InstanceDeploymentDescription instance_;
    instance_.node = CORBA::string_dup ("foil");
    instance_.deployedResource.length (1);
    instance_.deployedResource[0].requirementName =
      CORBA::string_dup ("Processor");
    instance_.deployedResource[0].resourceName =
      CORBA::string_dup ("CPULoad");

    instance_.deployedResource[0].property.length (1);
    instance_.deployedResource[0].property[0].name =
      CORBA::string_dup ("LoadAverage");
    CORBA::Long d = 20;
    instance_.deployedResource[0].property[0].value <<= d;

    plan.instance[0] = instance_;

    instance_.node = CORBA::string_dup ("blade30");
    instance_.deployedResource.length (1);
    instance_.deployedResource[0].requirementName =
      CORBA::string_dup ("Processor");
    instance_.deployedResource[0].resourceName =
      CORBA::string_dup ("CPULoad");

    instance_.deployedResource[0].property.length (1);
    instance_.deployedResource[0].property[0].name =
      CORBA::string_dup ("LoadAverage");
    d = 50;

    instance_.deployedResource[0].property[0].value <<= d;

    plan.instance[1] = instance_;

    bool resource_available = 1;

    try
    {
      targetI->commitResources(plan);
      ACE_DEBUG ((LM_DEBUG , "\n\ncommitResources Returned \n"));
    }
    catch(CORBA::NO_IMPLEMENT &)
    {
      cerr << "Error:TargetManager:CORBA::NO_IMPLEMENT thrown" << endl;
    }
    catch (Deployment::ResourceNotAvailable & e)
    {
      resource_available = 0;
      cout << "TargetManager commitResources ResourceNotAvailable Exception" <<endl;

      ACE_DEBUG ((LM_DEBUG ,
            "ResourceNotAvailable\n name=[%s]\n elementName=[%s]\n resourceName=[%s]\n \
            resourceType= [%s]\n propertyName=[%s]\n",
            e.name.in (),
            e.elementName.in (),
            e.resourceName.in (),
            e.resourceType.in (),
            e.propertyName.in ()));
    }
    catch(CORBA::Exception & ex)
    {
      cout << "Error:TargetManager:commitResources Exception" <<endl;
      cout << "Error:TargetManager:CORBA Generic Exception " << endl;
      cerr << "Error:TargetManager:Exception in TargetManager call" << ex << endl;
    }


    // Make a call to release resources , if resource < 0
    try
    {
      if (resource_available == 0)
      {
        targetI->releaseResources(plan);
        ACE_DEBUG ((LM_DEBUG , "\n\nreleaseResources Returned \n"));
      }
    }
    catch(CORBA::NO_IMPLEMENT &)
    {
      cerr << "Error:TargetManager:CORBA::NO_IMPLEMENT thrown" << endl;
    }
    catch (Deployment::ResourceNotAvailable &)
    {
      cout << "Error:TargetManager releaseResources ResourceNotAvailable Exception" <<endl;
    }
    catch(CORBA::Exception & ex)
    {
      cout << "Error:TargetManager:releaseResources Exception" <<endl;
      cout << "Error:TargetManager:CORBA Generic Exception " << endl;
      cerr << "Error:TargetManager:Exception in TargetManager call" << ex << endl;
    }

    // Here make a call on the TM with update domain and node deletion

    ::Deployment::Domain updated;
    updated.node.length (1);
    updated.node[0].name = CORBA::string_dup (argv[2]);

    ::CORBA::StringSeq elements;
    elements.length (0);

    bool Add = 1;
    Add = ACE_OS::atoi (argv[3]);

    if (Add)
    {
      try
      {
        targetI->updateDomain (elements , updated, ::Deployment::Add);
      }
      catch(CORBA::NO_IMPLEMENT &)
      {
        cerr << "Error:TargetManager:CORBA::NO_IMPLEMENT thrown" << endl;
      }
      catch(CORBA::Exception & ex)
      {
        cout << "Error:TargetManager:CORBA Generic Exception " << endl;
        cerr << "Error:TargetManager:Exception in UpdateDomain call" << ex << endl;
      }
    }
    else
    {
      try
      {
        targetI->updateDomain (elements , updated, ::Deployment::Delete);
      }
      catch(CORBA::NO_IMPLEMENT &)
      {
        cerr << "Error:TargetManager:CORBA::NO_IMPLEMENT thrown" << endl;
      }
      catch(CORBA::Exception & ex)
      {
        cout << "Error:TargetManager:CORBA Generic Exception " << endl;
        cerr << "Error:TargetManager:Exception in UpdateDomain call" << ex << endl;
      }
    }
    // Now make a call of getAvailableResources on the TargetManager ...
    try
    {
      Deployment::Domain_var domainV = targetI->getAvailableResources();

      // here write things to file ...
      write_to_file (domainV.in());

      ACE_DEBUG ((LM_DEBUG , "\n\nGetAvailableResources Returned \n"));
      ::Deployment::DnC_Dump::dump (domainV);
    }
    catch(CORBA::NO_IMPLEMENT &)
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

void write_to_file (::Deployment::Domain domain)
{
  for (size_t i = 0;i < domain.node.length ();i++)
  {
    std::ofstream out (domain.node[i].name.in ());


    // write in the node usage ...
    for (size_t j = 0;j < domain.node[i].resource.length ();j++)
    {

      if (!strcmp (domain.node[i].resource[j].name.in (), "Processor"))
      {
        CORBA::Double node_cpu;
        domain.node[i].resource[j].property[0].value >>= node_cpu;
        out << node_cpu << std::endl;
      }
      if (!strcmp (domain.node[i].resource[j].name.in (), "NA_Monitor"))
      {
        std::string file_name = "NA_";
        file_name += domain.node[i].name.in ();
        std::ofstream na_out (file_name.c_str ());
        CORBA::Double na_node_cpu;
        domain.node[i].resource[j].property[0].value >>= na_node_cpu;
        na_out << na_node_cpu << std::endl;
        na_out.close ();
      }
    }

    out.close ();
  }

}
