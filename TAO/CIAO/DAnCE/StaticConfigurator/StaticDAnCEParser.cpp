// $Id$

#include "NodeApplication_Impl.h"
//#include "Deployment_CoreC.h"
//#include "Deployment_DataC.h"
#include "CIAO/DAnCE/NodeApplicationManager/ImplementationInfo.h"
//#include "CIAO/DAnCE/RepositoryManager/RepositoryManager_Impl.h"
#include "CIAO/DAnCE/RepositoryManager/Update_Plan.h"
#include "CIAO/DAnCE/DomainApplicationManager/DomainApplicationManager_Impl.h"
#include "Config_Handlers/DnC_Dump.h"
#include "orbsvcs/orbsvcs/Naming/Transient_Naming_Context.h"
#include "ace/OS_NS_stdio.h"
#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "tao/IORTable/IORTable.h"
#include "tao/CDR.h"
#include "Config_Handlers/XML_File_Intf.h"


//using namespace std;
using namespace CIAO;

const char * exec_ior = "file://exec_mgr.ior";
const char * node_daemon_ior = 0;
const char * init_file = "deployment.dat";

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
  char* package_url = 0;

  // deployment plan URL
  char* plan_url = 0;

  // Initialize the ORB so that CORBA::Any will work
  //
  CORBA::ORB_var orb =
    CORBA::ORB_init (argc,
                     argv,
                     "");

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("p:d:"));
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

          default:
            usage(argv[0]);
            return -1;
        }
    }

  if (package_url == 0) {
    usage(argv[0]);
    return -1;
  }
  /*
  if (plan_url == 0) {
    usage(argv[0]);
    return -1;
  }
  */
  CIAO::Config_Handlers::XML_File_Intf intf (package_url);
      
  ::Deployment::DeploymentPlan_var plan =
      intf.get_plan ();
  /*
  // call the PackageConfiguration handler to parse the XML descriptor.
  Deployment::PackageConfiguration* pc;

  CIAO::RepositoryManager_Impl *rep_impl = 0;
  ACE_NEW_RETURN (rep_impl,
  CIAO::RepositoryManager_Impl (),
  -1);
  PortableServer::ServantBase_var owner_transfer (rep_impl);

  rep_impl->installPackage ("PC", package_url);
  pc = rep_impl->findPackageByName ("PC");
  //ACE_DEBUG ((LM_DEBUG, "Dumping package configuration===start\n"));
  //Deployment::DnC_Dump::dump (*pc);
  //ACE_DEBUG ((LM_DEBUG, "Dumping package configuration===end\n"));

  CIAO::REF_MAP ref_map;
  CIAO::REF_MAP primary_ref_map;

  // traverse the PackageConfiguration IDL data structure and
  // update the deployment plan IDL data structure.
  CIAO::traverse_package (pc, plan.out(), ref_map, primary_ref_map);
  */
  char out_char_array[100000];
  TAO_OutputCDR out_cdr(out_char_array, sizeof(out_char_array) );

  out_cdr << plan;

  ::Deployment::NodeImplementationInfo node_impl_info;

  //ImplementationInfo impl_info;

  if (!(node_impl_info << plan.in ()))
    {
      if (CIAO::debug_level () > 1)
        ACE_DEBUG ((LM_DEBUG, "Failed to create Node Implementation Info!\n"));

    } 

  Deployment::ComponentImplementationInfos& impl_infos = node_impl_info[0].impl_infos;

  CORBA::ULong i;
  const CORBA::ULong info_len = impl_infos.length ();
  for (i = 0; i < info_len; ++i)
    {
       ACE_DEBUG ((LM_DEBUG, "The info for installation: \n\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n",
       impl_infos[i].component_instance_name.in (),
       impl_infos[i].executor_dll.in (),
       impl_infos[i].executor_entrypt.in (),
       impl_infos[i].servant_dll.in (),
       impl_infos[i].servant_entrypt.in () ));
    }
      

  FILE* fp = fopen ("plan.h", "w");

  for (i=0;i < info_len; ++i)
    {
      ACE_OS::fprintf (fp,
                       "extern \"C\" ::Components::HomeExecutorBase_ptr %s (void);\n",
                       impl_infos[i].executor_entrypt.in ());
      ACE_OS::fprintf (fp,
                       "extern \"C\" ::PortableServer::Servant %s \n",
                       impl_infos[i].servant_entrypt.in ());
      ACE_OS::fprintf (fp,
                       "\t\t(::Components::HomeExecutorBase_ptr p,\n");
      ACE_OS::fprintf (fp,
                       "\t\t::CIAO::Session_Container *c,\n");
      ACE_OS::fprintf (fp,
                       "\t\tconst char*\n");
      ACE_OS::fprintf (fp,
                       "\t\tACE_ENV_ARG_DECL_WITH_DEFAULTS);\n");
    }

  ACE_OS::fprintf (fp, "struct HomeAttributes\n");
  ACE_OS::fprintf (fp, "{\n");
  ACE_OS::fprintf (fp, "  ACE_TString component_instance_name_;\n");
  ACE_OS::fprintf (fp, "  /// Specify the entrypoint to component executor DLL.\n");
  ACE_OS::fprintf (fp, "  ACE_TString executor_entrypt_;\n");
  ACE_OS::fprintf (fp, "  ::CIAO::HomeFactory executor_fptr_;\n");
  ACE_OS::fprintf (fp, "  /// Specify the entrypoint to component servant DLL.\n");
  ACE_OS::fprintf (fp, "  ACE_TString servant_entrypt_;\n");
  ACE_OS::fprintf (fp, "  ::CIAO::ServantFactory servant_fptr_;\n");
  ACE_OS::fprintf (fp, "};\n");
  ACE_OS::fprintf (fp, "//Homes\n");
  ACE_OS::fprintf (fp, "HomeAttributes homes_table[]= \n");
  ACE_OS::fprintf (fp, "{\n");
  for (i=0;i < info_len; ++i)
    {
      ACE_OS::fprintf (fp, "\t{\"%s\", \"%s\", %s, \"%s\", %s}",
                       impl_infos[i].component_instance_name.in (),
                       impl_infos[i].executor_entrypt.in (),
                       impl_infos[i].executor_entrypt.in (),
                       impl_infos[i].servant_entrypt.in (),
                       impl_infos[i].servant_entrypt.in () );

      if (i != info_len-1)
        ACE_OS::fprintf (fp, ",\n");
    }
  ACE_OS::fprintf (fp, "};\n\n");

  ACE_OS::fprintf (fp, "char plan_cdr_buffer[%d] = {", out_cdr.total_length ());
  for (int i=0; i < out_cdr.total_length (); ++i)
    {
      ACE_OS::fprintf(fp, "0x%X", out_char_array[i]);
      if (i<out_cdr.total_length ()) 
        ACE_OS::fprintf(fp, ",\n");
    }
  ACE_OS::fprintf (fp, "};\n\n");
      
  fclose (fp);

  return 0;
}
