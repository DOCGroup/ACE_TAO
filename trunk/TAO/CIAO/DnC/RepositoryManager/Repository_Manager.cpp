// $Id$

#include "RepositoryManager_Impl.h"
#include "DeploymentC.h"
#include "ace/Hash_Map_Manager.h"
#include "ExecutionManager/ExecutionManagerC.h"
#include "NodeManager/NodeDaemonC.h"
#include "ace/OS_NS_stdio.h"
#include "ace/streams.h"

const char * exec_ior = "file://exec_mgr.ior";
const char * node_daemon_ior = 0;

class Null_Dom_Document { };
// exception thrown when we have a null dom document.

typedef ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex> REF_MAP;
typedef ACE_Hash_Map_Manager<int, int, ACE_Null_Mutex> ART_REF_MAP;

void traverse_instance (Deployment::SubcomponentInstantiationDescription
                        &instance,
			Deployment::DeploymentPlan &plan, int l,
                        REF_MAP &ref_map, REF_MAP &primary_ref_map);

void traverse_interface (Deployment::SubcomponentInstantiationDescription
                         &instance,
                         Deployment::PlanSubcomponentPortEndpoint
                         &pspe);

static void
usage (const ACE_TCHAR* program)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Usage: %s -p <URI> -d <URI> -k <EX_IOR> \
                         -t <NODE_MANAGER_IOR> \n")
              ACE_TEXT (" <URI>: URI identifying the package\n"),
              program));
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // top level package URL
  ACE_TCHAR* package_url = 0;

  // deployment plan URL
  ACE_TCHAR* plan_url = 0;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("p:d:k:t"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
          case 'p':
            package_url = get_opt.opt_arg ();
            break;
          case 'd':
            plan_url = get_opt.opt_arg ();
            break;
          case 'k':
            exec_ior = get_opt.opt_arg ();
            break;
          case 't':
            node_daemon_ior = get_opt.opt_arg ();
            break;
          default:
            usage(argv[0]);
            return -1;
        }
    }

  if (package_url == 0) {
    usage(argv[0]);
    return -1;
  }

  if (plan_url == 0) {
    usage(argv[0]);
    return -1;
  }

  // Initialize the ORB so that CORBA::Any will work
  //
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "");

  try
    {
      // get a reference to the parser.
      DOMBuilder* parser = CIAO::Config_Handler::Utils::
                           create_parser ();

      // use the parser to parse the deployment plan URL and create
      // a DOM document.
      DOMDocument* dup_doc = parser->parseURI (plan_url);

      if (dup_doc == NULL)
        {
          ACE_DEBUG ((LM_DEBUG, "Null DOM Document obtained, \
                      May be the URL is wrong!!\n"));
          throw Null_Dom_Document ();
        }

      // free up DOMBuilder. DOMBuilder also deletes the DOMDocument memory.
      auto_ptr<DOMBuilder> cleanup_parser (parser);

      // call the Deployment Plan handler to parse the XML descriptor.
      CIAO::Config_Handler::Plan_Handler plan_handler (dup_doc,
                                                  DOMNodeFilter::SHOW_ELEMENT |
                                                  DOMNodeFilter::SHOW_TEXT);
      Deployment::DeploymentPlan plan;
      plan_handler.process_plan (plan);

      // call the PackageConfiguration handler to parse the XML descriptor.
      Deployment::PackageConfiguration* pc;
      CIAO::RepositoryManager_Impl rep_impl;
      rep_impl.installPackage ("PC", package_url);
      pc = rep_impl.findPackageByName ("PC");

      REF_MAP ref_map;
      REF_MAP primary_ref_map;

      for (CORBA::ULong x = 0; x < pc->basePackage.length (); ++x)
        {
          for (CORBA::ULong y = 0;
               y < pc->basePackage[x].implementation.length (); ++y)
            {
              Deployment::ComponentImplementationDescription cid =
                pc->basePackage[x].implementation[y].referencedImplementation;
              for (CORBA::ULong z = 0; z < cid.assemblyImpl.length (); ++z)
                {
                  for (CORBA::ULong k = 0;
                       k < cid.assemblyImpl[z].instance.length (); ++k)
                    {
                      const char* in_name =
                        cid.assemblyImpl[z].instance[k].name;
                      for (CORBA::ULong l = 0; l < plan.instance.length (); ++l)
                        {
                          const char* plan_name = plan.instance[l].name;
                          if (strcmp (plan_name, in_name) == 0)
                            {
                              traverse_instance (cid.assemblyImpl[z].
                                                 instance[k],
                                                 plan, l, ref_map,
                                                 primary_ref_map);
                            }
                        }
                    }
                  for (CORBA::ULong m = 0;
                       m < cid.assemblyImpl[z].connection.length (); ++m)
                    {
                      CORBA::ULong con_length (plan.connection.length ());
                      plan.connection.length (con_length + 1);
                      for (CORBA::ULong n = 0;
                           n < cid.assemblyImpl[z].connection[m].
                           internalEndpoint.length (); ++n)
                        {
                          CORBA::ULong iep_len (plan.connection[con_length].
                                                internalEndpoint.length ());
                          plan.connection[con_length].internalEndpoint
                                       .length (iep_len + 1);
                          plan.connection[con_length].internalEndpoint
                              [iep_len].portName = cid.assemblyImpl[z].
                                 connection[m].internalEndpoint[n].portName;
                          CORBA::ULong ins_ref = cid.assemblyImpl[z].
                            connection[m].internalEndpoint[n].instanceRef;
                          const char* ins_name =
                            cid.assemblyImpl[z].instance[ins_ref].name;
                          for (CORBA::ULong w = 0; w < plan.instance.length ();
                               ++w)
                            {
                              const char* pl_name = plan.instance[w].name;
                              if (strcmp (pl_name, ins_name) == 0)
                                {
                                  plan.connection[con_length].internalEndpoint
                                    [iep_len].instanceRef = w;
                                  break;
                                }
                            }
                          traverse_interface (cid.assemblyImpl[z]
                                              .instance[ins_ref],
                                              plan.connection[con_length].
                                              internalEndpoint[iep_len]);
                        }
                    }
                }
            }
        }

      Deployment::DnC_Dump::dump (plan);


      //Deployment::DnC_Dump::dump (*pc);

      // Pass the parsed plan to the Execution Manager to start the
      // Deployment Process.

      //Deployment::DnC_Dump::dump (*pc);


      // Pass the parsed plan to the Execution Manager to start the
      // Deployment Process.


      CORBA::Object_var obj = orb->string_to_object (exec_ior
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //CIAO::ExecutionManagerDaemon_var exec_mgr =
      //  CIAO::ExecutionManagerDaemon::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      Deployment::ExecutionManager_var exec_mgr =
        Deployment::ExecutionManager::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (exec_mgr.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Executor: nil Execution Manager reference, \
                       narrow failed\n"));
          return 1;

        }

      CIAO::NodeDaemon_var node_mgr;
      if (node_daemon_ior != 0)
      {
        CORBA::Object_var node_mgr_obj = orb->string_to_object
                                                 (node_daemon_ior
                                                  ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        node_mgr =
          CIAO::NodeDaemon::_narrow (node_mgr_obj.in ()
                                     ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (CORBA::is_nil (node_mgr.in ()))
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Executor: nil Node Manager reference, \
                        narrow failed\n"));
            return 1;
          }
      }

      ACE_DEBUG ((LM_DEBUG, "Executor: Obtained Execution Manager ref \n"));
      Deployment::DomainApplicationManager_var dapp_mgr =
        exec_mgr->preparePlan (plan, 1);

      if (CORBA::is_nil (dapp_mgr.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Executor:preparePlan call failed:nil \
                       DomainApplicationManager reference\n"));
          return 1;
        }
      ACE_DEBUG ((LM_DEBUG,
                  "Executor: Obtained DomainApplication Manager ref \n"));

      // Create a dummy set of properties and start the
      // Launching of applications
      Deployment::Properties_var properties;
      ACE_NEW_RETURN (properties,
                      Deployment::Properties,
                      1);
      ACE_DEBUG ((LM_DEBUG, "Executor: start Launch application....."));
      // Start the Application immediately
      int start = 1;
      dapp_mgr->startLaunch (properties.in (), start);
      ACE_DEBUG ((LM_DEBUG, "[success]\n"));

      ACE_DEBUG ((LM_DEBUG, "Executor: finish Launch application....."));
      // Call finish Launch to complete the connections
      dapp_mgr->finishLaunch (start);
      ACE_DEBUG ((LM_DEBUG, "[success]\n"));

      ACE_DEBUG ((LM_DEBUG, "Executor: Application Deployed successfully \n"));
      ACE_DEBUG ((LM_DEBUG, "Press <Enter> to tear down application \n"));

      char dummy [256];
      std::cin.getline (dummy, 256);
      // Tear down the assembly

      ACE_DEBUG ((LM_DEBUG, "Executor: destroy the application....."));
      dapp_mgr->destroyApplication ();
      ACE_DEBUG ((LM_DEBUG, "[success]\n"));

      ACE_DEBUG ((LM_DEBUG, "Executor: destroy the manager....."));
      exec_mgr->destroyManager (dapp_mgr.in ());
      //exec_mgr->shutdown ();

      ACE_DEBUG ((LM_DEBUG, "[success]\n"));

      if (node_daemon_ior != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "shutting down node manager \n"));
          node_mgr->shutdown (); // shut down the node manager.
        }

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  catch (CORBA::Exception& ex)
    {
      ACE_PRINT_EXCEPTION (ex, "Caught CORBA Exception: ");
	while (true);
      return -1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Caught unknown exception\n"));
	while (true);
      return -1;
    }

  return 0;
}

void traverse_interface (Deployment::SubcomponentInstantiationDescription
                         &instance,
                         Deployment::PlanSubcomponentPortEndpoint
                         &pspe)
{
  for (CORBA::ULong m = 0; m < instance.package.length (); ++m)
    {
      Deployment::ComponentPackageDescription
	package = instance.package[m];

      Deployment::ComponentInterfaceDescription
        cid = package.realizes;

      for (CORBA::ULong n = 0; n < cid.port.length (); ++n)
        {
          const char* main_port_name = cid.port[n].name;
          const char* port_name = pspe.portName;
          if (strcmp (main_port_name, port_name) == 0)
            {
              pspe.kind = cid.port[n].kind;
            }
        }
    }
}

void traverse_instance (Deployment::SubcomponentInstantiationDescription
                        &instance,
			Deployment::DeploymentPlan &plan, int l,
                        REF_MAP &ref_map, REF_MAP &primary_ref_map)
{
  ART_REF_MAP art_ref_map;

  for (CORBA::ULong m = 0; m < instance.package.length (); ++m)
    {
      Deployment::ComponentPackageDescription
	package = instance.package[m];

      for (CORBA::ULong n = 0; n < package.implementation.length (); ++n)
	{
	  Deployment::PackagedComponentImplementation
	    impl = instance.package[m].implementation[n];
	  CORBA::ULong impl_length (plan.implementation.length ());
	  plan.implementation.length (impl_length + 1);
	  plan.implementation[impl_length].name = plan.instance[l].name;
	  plan.instance[l].implementationRef = impl_length;

	  for (CORBA::ULong p = 0;
               p < impl.referencedImplementation.monolithicImpl.length (); ++p)
	    {
	      Deployment::MonolithicImplementationDescription
		mid = impl.referencedImplementation.monolithicImpl[p];

	      for (CORBA::ULong q = 0; q < mid.primaryArtifact.length (); ++q)
		{
		  Deployment::ImplementationArtifactDescription
		    pack_iad = mid.primaryArtifact[q].referencedArtifact;

		  CORBA::ULong art_length (plan.artifact.length ());
		  plan.artifact.length (art_length + 1);
		  plan.artifact[art_length].name =
                       mid.primaryArtifact[q].name;
		  plan.artifact[art_length].node = plan.instance[l].node;
                  ACE_TString artifact_name =
                     (const char*)plan.artifact[art_length].name;
                  ref_map.bind (artifact_name, art_length);
                  primary_ref_map.bind (artifact_name,
                                        art_length);
		  CORBA::ULong
		    art_ref_len (plan.implementation[impl_length].
                                 artifactRef.length ());
		  plan.implementation[impl_length].artifactRef.length
                        (art_ref_len + 1);
		  plan.implementation[impl_length].artifactRef[art_ref_len]
		    = art_length;
		  for (CORBA::ULong e = 0; e < pack_iad.location.length (); ++e)
		    {
		      CORBA::ULong
			art_loc_len
                          (plan.artifact[art_length].location.length ());
		      plan.artifact[art_length].location.length
                        (art_loc_len + 1);
		      plan.artifact[art_length].location[art_loc_len]
			= pack_iad.location[e];
		    }
		  for (CORBA::ULong f = 0;
                       f < pack_iad.execParameter.length (); ++f)
		    {
		      CORBA::ULong
			art_pro_len
                         (plan.artifact[art_length].execParameter.length ());
		      plan.artifact[art_length].execParameter.length
			(art_pro_len + 1);
		      plan.artifact[art_length].execParameter[art_pro_len]
			= pack_iad.execParameter[f];
		    }
		  for (CORBA::ULong g = 0;
                       g < pack_iad.dependsOn.length (); ++g)
		    {
                      ACE_TString dep_name =
                         (const char*)pack_iad.dependsOn[g].name;
                      int arti_len;

                      if (ref_map.find (dep_name, arti_len) == 0)
                        {
                          if (primary_ref_map.find (dep_name, arti_len) != 0)
                            {
                              if (art_ref_map.find (arti_len, arti_len) != 0)
                                {
		                  CORBA::ULong
		                    new_art_ref_len
                                      (plan.implementation[impl_length].
                                             artifactRef.length ());
		                  plan.implementation[impl_length].
                                     artifactRef.length
                                       (new_art_ref_len + 1);
		                  plan.implementation[impl_length].
                                     artifactRef[new_art_ref_len] =
                                       arti_len;
                                  art_ref_map.bind (arti_len, arti_len);
                                }
                            }
                        }
                      else
                        {
                          Deployment::ImplementationArtifactDescription
                             depends_iad = pack_iad.dependsOn[g].
                                  referencedArtifact;
		          CORBA::ULong new_art_length (plan.artifact.length ());
		          plan.artifact.length (new_art_length + 1);
                          plan.artifact[new_art_length].name =
                            pack_iad.dependsOn[g].name;
		          plan.artifact[new_art_length].node =
                             plan.instance[l].node;
		          for (CORBA::ULong h = 0;
                               h < depends_iad.location.length (); ++h)
		            {
		              CORBA::ULong
			        new_art_loc_len
                                  (plan.artifact[new_art_length].
                                      location.length ());
		              plan.artifact[new_art_length].location.length
                                (new_art_loc_len + 1);
		              plan.artifact[new_art_length].
                                 location[new_art_loc_len]
			           = depends_iad.location[h];
		            }
                          ref_map.bind (
                             (const char*)plan.artifact[new_art_length].name,
                              new_art_length);
		          CORBA::ULong
		            new_art_ref_len (plan.implementation[impl_length].
                                         artifactRef.length ());
		          plan.implementation[impl_length].artifactRef.length
                                (new_art_ref_len + 1);
		          plan.implementation[impl_length].
                                artifactRef[new_art_ref_len] = new_art_length;
                          art_ref_map.bind (new_art_length, new_art_length);
                        }
                    }
		}
	    }
	  for (CORBA::ULong x = 0;
               x < impl.referencedImplementation.configProperty.length (); ++x)
	    {
	      CORBA::ULong
                impl_pro_len
                  (plan.implementation[impl_length].execParameter.length ());
              plan.implementation[impl_length].execParameter.length
			(impl_pro_len + 1);
              plan.implementation[impl_length].execParameter[impl_pro_len]
			= impl.referencedImplementation.configProperty[x];

	      CORBA::ULong
                ins_pro_len
                  (plan.instance[l].configProperty.length ());
              plan.instance[l].configProperty.length
			(ins_pro_len + 1);
              plan.instance[l].configProperty[ins_pro_len]
			= impl.referencedImplementation.configProperty[x];
            }
	}
    }
}
