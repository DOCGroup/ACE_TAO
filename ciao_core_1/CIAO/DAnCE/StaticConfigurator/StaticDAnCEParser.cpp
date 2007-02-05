//=============================================================================
/**
 *  @file StaticDAnCEParser.cpp
 *
 *  $Id$
 *
 *  This parser is used in static deployment of CIAO components. This
 *  parser parses a deployment plan and generates a header file plan.h
 *  containing information about the home and servant creation
 *  function entry points. This can be used by a Static_NodeManager to
 *  instantiate instances of homes and servants just like the regular
 *  NodeManager. For more details, see
 *  $CIAO_ROOT/DAnCE/StaticConfigurator/docs/static_dance.html
 *
 *  @author Venkita Subramonian <venkita@cse.wustl.edu>
 */
//=============================================================================

#include "DAnCE/NodeApplicationManager/ImplementationInfo.h"
#include "ace/OS_NS_stdio.h"
#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "tao/IORTable/IORTable.h"
#include "tao/CDR.h"
#include "Config_Handlers/XML_File_Intf.h"

static void
usage (const ACE_TCHAR* program)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Usage: %s -p <package URI>\n"),
              program));
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // top level package URL
  char* package_url = 0;

  // Initialize the ORB so that CORBA::Any will work
  //
  CORBA::ORB_var orb =
    CORBA::ORB_init (argc,
                     argv,
                     "");

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("p:"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
          case 'p':
            package_url = get_opt.opt_arg ();
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

  CIAO::Config_Handlers::XML_File_Intf intf (package_url);

  ::Deployment::DeploymentPlan_var plan =
      intf.get_plan ();

  ::Deployment::ComponentPlans dummy;
  dummy.length (0);

  CIAO::NodeImplementationInfoHandler handler (plan, dummy);

  Deployment::NodeImplementationInfo_var node_impl_info (handler.node_impl_info ());

  if (node_impl_info.ptr() == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                  "DAnCE (%P|%t) StaticParser -"
                  "Failed to create Node Implementation Infos!\n"), 1);

    }

  Deployment::ContainerImplementationInfos& impl_infos = node_impl_info->impl_infos;

  FILE* fp = ACE_OS::fopen ("plan.h", "w");

  CORBA::ULong i,j,num_containers,num_components;
  num_containers = impl_infos.length ();
  for (i = 0; i < num_containers; ++i)
    {
      num_components = impl_infos[i].impl_infos.length ();
      for (j = 0; j < num_components; ++j)
        {
          ACE_DEBUG ((LM_DEBUG, "The info for installation: \n\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n",
                      impl_infos[i].impl_infos[j].component_instance_name.in (),
                      impl_infos[i].impl_infos[j].executor_dll.in (),
                      impl_infos[i].impl_infos[j].executor_entrypt.in (),
                      impl_infos[i].impl_infos[j].servant_dll.in (),
                      impl_infos[i].impl_infos[j].servant_entrypt.in () ));
          ACE_OS::fprintf (fp,
                           "extern \"C\" ::Components::HomeExecutorBase_ptr %s (void);\n",
                           impl_infos[i].impl_infos[j].executor_entrypt.in ());
          ACE_OS::fprintf (fp,
                           "extern \"C\" ::PortableServer::Servant %s \n",
                           impl_infos[i].impl_infos[j].servant_entrypt.in ());
          ACE_OS::fprintf (fp,
                           "\t\t(::Components::HomeExecutorBase_ptr p,\n");
          ACE_OS::fprintf (fp,
                           "\t\t::CIAO::Session_Container *c,\n");
          ACE_OS::fprintf (fp,
                           "\t\tconst char*\n");
          ACE_OS::fprintf (fp,
                           "\t\t);\n");
        }
    }

  ACE_OS::fprintf (fp, "struct HomeAttributes\n");
  ACE_OS::fprintf (fp, "{\n");
  ACE_OS::fprintf (fp, "  char const * component_instance_name_;\n");
  ACE_OS::fprintf (fp, "  /// Specify the entrypoint to component executor DLL.\n");
  ACE_OS::fprintf (fp, "  char const * executor_entrypt_;\n");
  ACE_OS::fprintf (fp, "  ::CIAO::HomeFactory executor_fptr_;\n");
  ACE_OS::fprintf (fp, "  /// Specify the entrypoint to component servant DLL.\n");
  ACE_OS::fprintf (fp, "  char const * servant_entrypt_;\n");
  ACE_OS::fprintf (fp, "  ::CIAO::ServantFactory servant_fptr_;\n");
  ACE_OS::fprintf (fp, "};\n");
  ACE_OS::fprintf (fp, "/// Homes\n");
  ACE_OS::fprintf (fp, "HomeAttributes homes_table[]= \n");
  ACE_OS::fprintf (fp, "{\n");

  num_containers = impl_infos.length ();
  for (i = 0; i < num_containers; ++i)
    {
      num_components = impl_infos[i].impl_infos.length ();
      for (j = 0; j < num_components; ++j)
        {
          ACE_OS::fprintf (fp, "\t{\"%s\", \"%s\", %s, \"%s\", %s}",
                           impl_infos[i].impl_infos[j].component_instance_name.in (),
                           impl_infos[i].impl_infos[j].executor_entrypt.in (),
                           impl_infos[i].impl_infos[j].executor_entrypt.in (),
                           impl_infos[i].impl_infos[j].servant_entrypt.in (),
                           impl_infos[i].impl_infos[j].servant_entrypt.in () );
          if (!(i == num_containers -1 && j == num_components-1))
            ACE_OS::fprintf (fp, ",\n");
        }
    }
  ACE_OS::fprintf (fp, "};\n\n");

  ACE_OS::fclose (fp);

  return 0;
}
