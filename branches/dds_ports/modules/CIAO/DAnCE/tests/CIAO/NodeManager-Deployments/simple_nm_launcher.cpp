/** 
 * @file simple_nm_launcher.cpp
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *  $Id$
 * Launches then immediately tears down a plan. 
 */

#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"
#include "tao/ORB.h"
#include "DAnCE/Logger/Logger_Service.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "Deployment/Deployment_NodeApplicationC.h"
#include "Deployment/Deployment_NodeManagerC.h"
#include "Deployment/Deployment_DeploymentPlanC.h"
#include "tools/Config_Handlers/XML_File_Intf.h"

int usage ()
{
  ACE_ERROR ((LM_ERROR, "simple_nm_launcher <nm_url> <plan>\n"));
  return -1;
}

#include <iostream>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  DANCE_DISABLE_TRACE ();
  
  auto_ptr<DAnCE::Logger_Service> logger;
  
  DAnCE::Logger_Service
    * dlf = ACE_Dynamic_Service<DAnCE::Logger_Service>::instance ("DAnCE_Logger_Backend_Factory");
  
  if (!dlf)
    dlf = new DAnCE::Logger_Service;
  
  logger.reset (dlf);
  logger->init (argc, argv);
  
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
  
  if (argc != 3)
    return usage ();
  
  try
    {
      ACE_DEBUG ((LM_DEBUG, "*** simple_nm_launcher: parsing XML\n"));
      // Parse plan
      CIAO::Config_Handlers::XML_File_Intf xml (argv[2]);
      xml.add_search_path ("CIAO_ROOT", "/docs/schema/");
      
      auto_ptr< ::Deployment::DeploymentPlan> plan (xml.release_plan ());
      
      if (plan.get () == 0) 
        { 
          ACE_ERROR ((LM_ERROR, "*** error parsing XML document\n")); 
          throw 1; 
        }
      

      ACE_DEBUG ((LM_DEBUG, "*** simple_nm_launcher: resoling node manager reference.\n"));
      CORBA::Object_var obj = orb->string_to_object (argv[1]);
      Deployment::NodeManager_var nm = Deployment::NodeManager::_narrow (obj.in ());
      
      if (CORBA::is_nil (nm.in ()))
        {
          ACE_ERROR ((LM_ERROR, "*** simple_nm_launcher: NodeManager reference is nil."));
          throw 1;
        }

      ACE_DEBUG ((LM_DEBUG, "*** simple_nm_launcher: calling prepareplan.\n"));
      Deployment::NodeApplicationManager_var nam = nm->preparePlan (*plan,
                                                                    Deployment::ResourceCommitmentManager::_nil ());
      
      ACE_DEBUG ((LM_DEBUG, "*** simple_nm_launcher: calling startLaunch\n"));
      Deployment::Connections_var conns;
      Deployment::Properties props;
      Deployment::Application_var app = nam->startLaunch (props, conns.out ());
      Deployment::NodeApplication_var na = Deployment::NodeApplication::_narrow (app.in ());
      
      ACE_DEBUG ((LM_DEBUG, "*** simple_nm_launcher: calling finishLaunch\n"));
      na->finishLaunch (conns.in (), false);
      
      ACE_DEBUG ((LM_DEBUG, "*** simple_nm_launcher: calling start\n"));
      na->start ();
      
      ACE_DEBUG ((LM_DEBUG, "*** simple_nm_launcher: start finished, sleeping 5 seconds.\n"));
      ACE_OS::sleep (5);
      ACE_DEBUG ((LM_DEBUG, "*** simple_nm_launcher: waking up from sleep, calling destroyApplication\n"));
      
      nam->destroyApplication (na.in ());
      
      ACE_DEBUG ((LM_DEBUG, "*** simple_nm_launcher: calling destroyManager\n"));
      
      nm->destroyManager (nam.in ());
      
      ACE_DEBUG ((LM_DEBUG, "*** simple_nm_launcher: destroyManager completed.\n"));
      
      orb->destroy ();
    }
  catch (Deployment::StopError &ex)
    {
      ACE_ERROR ((LM_ERROR, "*** Caught StopError exception with name %s and reason %s\n",
                  ex.name.in (), ex.reason.in ()));
      return -1;
    }
  catch (Deployment::StartError &ex)
    {
      ACE_ERROR ((LM_ERROR, "*** Caught StartError exception with name %s and reason %s\n",
                  ex.name.in (), ex.reason.in ()));
      return -1;
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
