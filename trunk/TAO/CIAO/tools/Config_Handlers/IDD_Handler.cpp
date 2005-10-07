// $Id$

#include "IDD_Handler.h"
#include "IRDD_Handler.h"
#include "MDD_Handler.h"
#include "Property_Handler.h"
#include "Any_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "cdp.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    IDREF_Base<CORBA::ULong> IDD_Handler::IDREF;

    void
    IDD_Handler::instance_deployment_descrs (
        const DeploymentPlan &src,
        Deployment::InstanceDeploymentDescriptions& dest)
      throw (Config_Error)
    {
      DeploymentPlan::instance_const_iterator idd_e =
        src.end_instance ();
      
      CORBA::ULong pos = 0;
      dest.length (src.count_instance ());
      for (DeploymentPlan::instance_const_iterator idd_b =
             src.begin_instance ();
           idd_b != idd_e;
           ++idd_b)
        {
	  IDD_Handler::instance_deployment_descr ((*idd_b),
						  dest[pos], pos);
	  pos++;
        }
    }

    void
    IDD_Handler::instance_deployment_descr (
        const InstanceDeploymentDescription& src,
        Deployment::InstanceDeploymentDescription& dest,
        CORBA::ULong pos)
      throw (Config_Error)
    {
      try
	{
	  dest.name = src.name ().c_str ();
	  dest.node = src.node ().c_str ();

	  if (src.id_p ())
	    {
	      ACE_CString cstr (src.id ().c_str ());
	      IDD_Handler::IDREF.bind_ref (cstr, pos);
	    }
	  else
	    {
	      ACE_DEBUG((LM_ERROR,
			 "(%P|%t) Warning:  IDD %s has no idref \n",
			 src.name ().c_str ()));
	    }

	  // We know there should be only one element
          dest.source.length (1);
	  dest.source [0] =
	    src.source ().c_str ();

	  CORBA::ULong tmp = 0;
	  MDD_Handler::IDREF.find_ref 
	    (ACE_CString (src.implementation ().id ().c_str ()), tmp);
      
	  dest.implementationRef = tmp;

	  InstanceDeploymentDescription::configProperty_const_iterator pend =
	    src.end_configProperty ();
          
          CORBA::ULong pos = 0;
          dest.configProperty.length (src.count_configProperty ());
	  for (InstanceDeploymentDescription::configProperty_const_iterator pstart =
		 src.begin_configProperty ();
	       pstart != pend;
	       ++pstart)
	    {
	      Property_Handler::get_property (*pstart,
					      dest.configProperty[pos++]);
	    }

	  if (src.deployedResource_p ())
	    {
	      dest.deployedResource.length (1);
	      IRDD_Handler::instance_resource_deployment_descr (src.deployedResource (),
							  dest.deployedResource[0]);
	    }

	  if (src.deployedSharedResource_p ())
	    {
	      dest.deployedSharedResource.length (1);
	      IRDD_Handler::instance_resource_deployment_descr (src.deployedSharedResource (),
								dest.deployedSharedResource[0]);
	    }

	}
      catch (Config_Error &ex)
	{
	  ex.name_ = src.name ()  + ":" + ex.name_;
	  throw ex;
	}
      // Done!
    }
 
    InstanceDeploymentDescription
    IDD_Handler::instance_deployment_descr (
	const Deployment::InstanceDeploymentDescription& src)
      throw (Config_Error)
    {
	
	//Get all the string/IDREFs
	XMLSchema::string < ACE_TCHAR > name ((src.name));
	XMLSchema::string < ACE_TCHAR > node ((src.node));
	XMLSchema::string < ACE_TCHAR > source ((src.source[0]));
	ACE_CString temp;
	MDD_Handler::IDREF.find_ref(src.implementationRef, temp);
	XMLSchema::IDREF< ACE_TCHAR > implementation ((temp.c_str()));

	// Instantiate the IDD
	InstanceDeploymentDescription idd (name, node, source, implementation);

	//Get and store the configProperty(s)
	size_t total = src.configProperty.length();
	for(size_t j = 0; j < total; j++)
	{
	    idd.add_configProperty(
		Property_Handler::get_property (
		    src.configProperty[j]));
	}

	//Check if there is a deployedResource, if so store
	if(src.deployedResource.length() != 0)
	    idd.deployedResource(
		IRDD_Handler::instance_resource_deployment_descr(
		    src.deployedResource[0]));

	//Check if there is a deployedSharedResource, if so store it
	if(src.deployedSharedResource.length() != 0)
	    idd. deployedSharedResource(
		IRDD_Handler::instance_resource_deployment_descr(
		    src.deployedSharedResource[0]));

	return idd;
    }	

  }
}
