// $Id$

#include "NodeApplication_Impl.h"
#include "DeploymentC.h"
#include "CIAO/DAnCE/NodeApplicationManager/ImplementationInfo.h"
#include "CIAO/DAnCE/RepositoryManager/RepositoryManager_Impl.h"
#include "CIAO/DAnCE/RepositoryManager/Update_Plan.h"
#include "CIAO/DAnCE/DomainApplicationManager/DomainApplicationManager_Impl.h"
#include "Config_Handlers/DnC_Dump.h"
#include "orbsvcs/orbsvcs/Naming/Transient_Naming_Context.h"
#include "ace/OS_NS_stdio.h"
#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "tao/IORTable/IORTable.h"
#include "tao/CDR.h"
#include "CIAO/DAnCE/Config_Handlers/XercesString.h"
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/parsers/AbstractDOMParser.hpp>
#include "CIAO/DAnCE/Config_Handlers/Config_Handler_export.h"
#include "CIAO/DAnCE/Config_Handlers/Domain_Handler.h"
#include "CIAO/DAnCE/Config_Handlers/PC_Handler.h"
#include "CIAO/DAnCE/Config_Handlers/Plan_Handler.h"
#include "CIAO/DAnCE/Config_Handlers/CompImplDesc_Handler.h"
#include "CIAO/DAnCE/Config_Handlers/DnC_Dump.h"
#include "CIAO/DAnCE/Config_Handlers/Utils.h"
#include "CIAO/DAnCE/Config_Handlers/Config_Error_Handler.h"

using Config_Handler::XStr;
using xercesc::XMLUni;
using xercesc::XMLString;
using xercesc::XMLException;
using xercesc::DOMException;
using xercesc::DOMBuilder;
using xercesc::DOMImplementationRegistry;
using xercesc::DOMImplementationLS;
using xercesc::DOMImplementation;
using xercesc::DOMAttr;
using xercesc::DOMNamedNodeMap;
using xercesc::DOMLocator;
using xercesc::DOMError;
using xercesc::DOMNodeList;
using xercesc::DOMDocument;
using xercesc::DOMDocumentTraversal;
using xercesc::DOMNodeIterator;
using xercesc::DOMNode;
using xercesc::DOMNodeFilter;
using xercesc::XercesDOMParser;
using xercesc::AbstractDOMParser;
using namespace std;
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

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("p:d:k:t:i:"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
          case 'p':
            package_url = get_opt.opt_arg ();
            break;
          case 'i':
            init_file = get_opt.opt_arg ();
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

  try
    {
      xercesc::XMLPlatformUtils::Initialize();
    }

  catch (const XMLException& xml_e)
    {
      char* message = XMLString::transcode (xml_e.getMessage());
      ACE_Auto_Basic_Array_Ptr<char> cleanup_message (message);
      ACE_DEBUG ((LM_DEBUG, "Error during initialization : %s\n", message));
      return 1;
    }

  try
    {
      // get a reference to the parser.
      std::auto_ptr<DOMBuilder> plan_parser (CIAO::Config_Handler::Utils::
                                             create_parser ());
      CIAO::Config_Handler::Config_Error_Handler handler;
      plan_parser->setErrorHandler(&handler);

      std::auto_ptr<DOMBuilder> tpd_parser (CIAO::Config_Handler::Utils::
                                            create_parser ());
      CIAO::Config_Handler::Config_Error_Handler tpd_handler;
      tpd_parser->setErrorHandler(&tpd_handler);

      // use the parser to parse the deployment plan URL and create
      // a DOM document.
      DOMDocument* plan_doc = plan_parser->parseURI (plan_url);
      if (handler.getErrors())
        {
          return 1;
        }

      //DOMDocument* tpd_doc = tpd_parser->parseURI (package_url);

      //ACE_UNUSED_ARG (tpd_doc);

      if (tpd_handler.getErrors())
        {
          return 1;
        }

      if (plan_doc == NULL)
        {
          ACE_DEBUG ((LM_DEBUG, "Null DOM Document obtained, \
                      May be the URL is wrong!!\n"));
          throw CIAO::Null_Dom_Document ();
        }

      // free up DOMBuilder. DOMBuilder also deletes the DOMDocument memory.
      //auto_ptr<DOMBuilder> cleanup_parser (parser);

      // call the Deployment Plan handler to parse the XML descriptor.
      CIAO::Config_Handler::Plan_Handler plan_handler (plan_doc,
                                                  DOMNodeFilter::SHOW_ELEMENT |
                                                  DOMNodeFilter::SHOW_TEXT);
      Deployment::DeploymentPlan plan;
      plan_handler.process_plan (plan);

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
      CIAO::traverse_package (pc, plan, ref_map, primary_ref_map);

      char out_char_array[100000];
      TAO_OutputCDR out_cdr(out_char_array, sizeof(out_char_array) );

      out_cdr << plan;

      Deployment::ImplementationInfos infos;
      //ImplementationInfo impl_info;

      if (!(infos << plan))
        {
          if (CIAO::debug_level () > 1)
            ACE_DEBUG ((LM_DEBUG, "Failed to create Component Implementation Infos!\n"));

        } 

      CORBA::ULong i;
      const CORBA::ULong info_len = infos.length ();
      for (i = 0; i < info_len; ++i)
        {/*
          ACE_DEBUG ((LM_DEBUG, "The info for installation: \n\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n",
                      infos[i].component_instance_name.in (),
                      infos[i].executor_dll.in (),
                      infos[i].executor_entrypt.in (),
                      infos[i].servant_dll.in (),
                      infos[i].servant_entrypt.in () ));
         */
        }
      

      FILE* fp = fopen ("plan.h", "w");

      for (i=0;i < info_len; ++i)
        {
          ACE_OS::fprintf (fp,
                           "extern \"C\" ::Components::HomeExecutorBase_ptr %s (void);\n",
                           infos[i].executor_entrypt.in ());
          ACE_OS::fprintf (fp,
                           "extern \"C\" ::PortableServer::Servant %s \n",
                           infos[i].servant_entrypt.in ());
          ACE_OS::fprintf (fp,
                           "\t\t(::Components::HomeExecutorBase_ptr p,\n");
          ACE_OS::fprintf (fp,
                           "\t\t::CIAO::Session_Container *c\n");
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
                           infos[i].component_instance_name.in (),
                           infos[i].executor_entrypt.in (),
                           infos[i].executor_entrypt.in (),
                           infos[i].servant_entrypt.in (),
                           infos[i].servant_entrypt.in () );

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

    }
  catch (const DOMException& e)
    {
      const unsigned int maxChars = 2047;
      XMLCh errText[maxChars + 1];

      ACE_ERROR ((LM_ERROR, "\nException occured while parsing %s: \
                  \n",plan_url));
      ACE_ERROR ((LM_ERROR, "DOMException code: %d\n ", e.code));
      if (DOMImplementation::loadDOMExceptionMsg (e.code, errText, maxChars))
        {
          char* message = XMLString::transcode (errText);
          ACE_Auto_Basic_Array_Ptr<char> cleanup_message (message);
          ACE_ERROR ((LM_ERROR, "Message is: %s\n", message));
        }
      //ACE_PRINT_EXCEPTION ("Caught DOM Exception: ");
      ACE_ERROR ((LM_ERROR, "Caught DOM exception\n"));
      return -1;
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Caught unknown exception\n"));
      return -1;
    }

  return 0;
}
