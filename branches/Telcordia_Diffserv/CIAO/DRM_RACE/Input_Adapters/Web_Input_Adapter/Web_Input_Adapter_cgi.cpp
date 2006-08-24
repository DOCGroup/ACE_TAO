// -*- C++ -*-
//=================================================================
/**
 * @file Web_Input_Adapter_cgi.cpp
 *
 * CGI script to implement Web_Input_Adapter.
 *
 * @author Ed Mulholland <emulholl@atl.lmco.com>
 */
//=================================================================

#include "ace/Env_Value_T.h"
#include "ace/ARGV.h"
#include "ace/Dirent.h"
#include "ace/OS.h"
#include "ciao/Client_init.h"
#include "tools/Config_Handlers/XML_File_Intf.h"
#include "Web_Input_Adapter_InterfaceC.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Web_Input_Adapter_Impl
    {
      // Environment Variables that will be read by the cgi program.
      typedef ACE_Env_Value <const char*> ENV_VALUE;
      ENV_VALUE path_info ("PATH_INFO", "/main");
      ENV_VALUE server_name ("SERVER_NAME", "?");
      ENV_VALUE server_port ("SERVER_PORT", "?");
      ENV_VALUE server_software ("SERVER_SOFTWARE", "?");
      ENV_VALUE web_input_adapter_ior ("WEB_INPUT_ADAPTER_IOR", "?");
      ENV_VALUE plan_deployment_ior ("PLAN_DEPLOYMENT_IOR", "?");
      ENV_VALUE component_control_ior ("COMPONENT_CONTROL_IOR", "?");
      ENV_VALUE show_deployment_plan ("show_deployment_plan", "?");
      ENV_VALUE dynamic_allocation ("dynamic_allocation", "?");
      ENV_VALUE deploy_deployment_plan ("deploy_deployment_plan", "?");
      ENV_VALUE teardown_deployment_plan ("teardown_deployment_plan", "?");

      // HTML page-generating functions.
      int main_page ();
      int race_component_display_page ();
      int cdp_list_page ();
      int deployable_cdp_list_page ();
      int cdp_display_page ();
      int allocation_display_page ();
      int gif_page ();

      // Setup a table that associates URL suffixes with
      // page-generating functions.
      typedef int (*PAGE_FUNC) ();
      struct Page_Func_Struct
      {
        const char * name;
        PAGE_FUNC func;
      };
      Page_Func_Struct page_functions [] =
        {
          {"/main", &main_page},
          {"/race_component_display", &race_component_display_page},
          {"/cdp_list", &cdp_list_page},
          {"/deployable_cdp_list", &deployable_cdp_list_page},
          {"/cdp_display", &cdp_display_page},
          {"/allocation_display", &allocation_display_page},
          {"/gif_page", &gif_page},
          {0, 0}
        };

      // Setup a list of directories that will be searched for
      // Component Deployment Plan (CDP) files.
      struct Deployment_Plan_Dir
      {
        const char * subst_name;
        char * name;
        ~Deployment_Plan_Dir() { delete [] (name); }
      };
      Deployment_Plan_Dir deployment_plan_dirs [] =
        {
          {"${CIAO_ROOT}/RACE/descriptors", 0},
          {"${CIAO_ROOT}/../../../../Phase-II-DRM/DRM/Middleware/ACE_wrappers/TAO/CIAO/RACE/descriptors", 0},
          {0, 0}
        };
      void substitute_braced_environment_variables
      ( const char * subst_name,
        char *& name );

      // Arguments for the CORBA ORB
      ACE_ARGV orb_args ("");
      CORBA::ORB_var orb;

      // Display a table of all the running RACE components.
      void component_table (Component_Tree_Node & tree,
                            Component_Control * control,
                            std::set<std::string> & already_displayed);

      // Structures used in the implementation of the
      // component_table function.
      struct Attribute
      {
        std::string name;
        std::string stringified_value;
        CORBA::TCKind corba_kind;
      };

      typedef std::vector <Attribute> ATTRIBUTE_VEC;

      struct Neighbor
      {
        std::string receptacle_name;
        Components::CCMObject_var component;
        std::string component_name;
        RACE::Component_Tree_Node component_tree_node;
      };
      typedef std::vector <Neighbor> NEIGHBOR_VEC;

      // Helper functions used in the implementation of the
      // component_table function.
      Component_Data &  component_data_ref  (Component_Tree_Node & node);
      Connection_Data & connection_data_ref (Component_Tree_Node & node);

      // Exception thrown by helper functions if an expected union
      // member is not found.
      class UnionExtractionException
      {
        std::string reason_;
      public:
        UnionExtractionException ( const std::string & reason )
          : reason_ (reason) {}
        std::string reason () { return this->reason_; }
      };

      class NullPointerException
      {
        std::string reason_;
      public:
        NullPointerException ( const std::string & reason )
          : reason_ (reason) {}
        std::string reason () { return this->reason_; }
      };
    }
  }
}

namespace Web_Input_Adapter = CIAO::RACE::CIDL_Web_Input_Adapter_Impl;

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Initialize ORB
  int orb_argc = Web_Input_Adapter::orb_args.argc ();
  Web_Input_Adapter::orb =
    CORBA::ORB_init (orb_argc,
                     Web_Input_Adapter::orb_args.argv (),
                     "");

  // Initialize valuetype factories
  CIAO::Client_init (Web_Input_Adapter::orb.inout());

  // Substitute environment variables in the deployment plan dirs.
  for (Web_Input_Adapter::Deployment_Plan_Dir * i =
         & Web_Input_Adapter::deployment_plan_dirs [0];
       i->subst_name != 0;
       ++i)
    Web_Input_Adapter::substitute_braced_environment_variables
      (i->subst_name, i->name);

  // Determine which page function to call.
  for (Web_Input_Adapter::Page_Func_Struct * i = & Web_Input_Adapter::page_functions [0];
       i->name != 0;
       ++i)
  {
    if (ACE_OS::strcmp (CIAO::RACE::CIDL_Web_Input_Adapter_Impl::path_info,
                        i->name) == 0)
      return (i->func) ();
  }

  std::cout << "Status: 404 Not Found\n\n";
  return 0;
}

int Web_Input_Adapter::gif_page ()
{
  // Determine the filename of the test image
  std::string filename;
  {
    const char * relative_path = "${RACE_ROOT}/Input_Adapters/"
      "Web_Input_Adapter/web_content/images/test-image.gif";
    char * temp;
    substitute_braced_environment_variables (relative_path, temp);
    filename = temp;
    delete [] temp;
  }

  char imagebuffer[BUFSIZ];
  char * ptr = & imagebuffer[0];
  ssize_t bytes_read = -1;

  ACE_HANDLE filehandle = ACE_OS::open (filename.c_str (), O_RDONLY);
  if (filehandle == ACE_INVALID_HANDLE) {
    std::cout << "Content-type: text/plain\n\n"
      "Got ACE_INVALID_HANDLE\n";
    return 0;
  } else {
    while (bytes_read != 0) {
      bytes_read = ACE_OS::read (filehandle,
                                 ptr,
                                 BUFSIZ-1 - (ptr - imagebuffer));
      if (bytes_read == -1)
      {
        std::cout << "Content-type: text/plain\n\n"
          "Error in ACE_OS::read\n";
        ACE_OS::close (filehandle);
        return 0;
      }
      else
        ptr += bytes_read;
    }
  }
  ACE_OS::close (filehandle);

  std::cout << "Content-type: image/gif\n\n";
  std::cout.write (imagebuffer, ptr - &imagebuffer[0]);
  return 0;
}

int Web_Input_Adapter::main_page ()
{
  std::cout <<
    "Content-type: text/html\n\n"
    "\n"

    "<html> <head>\n"
    "<title>Resource Allocation and Control Engine (RACE)</title>\n"
    "<link rel=\"stylesheet\" href=\"/web_content/Web_Input_Adapter.css\" type=\"text/css\"/>\n"
    "</head>\n"
    "\n"

    "<body>\n"
    "\n"

    "<center><image src=\"/web_content/images/race_title.png\"></center>\n"
    "<br/>\n"
    "<center><i>RACE Web-based Application to Test, CHange, and Evaluate Resource allocation (RACEWATCHER) </i></center>\n"
    "<center>Hosted on " << server_name << ":" << server_port << " using " << server_software << " </center>\n"
    "<br/>\n"
    "<hr/>\n"
    "<br/>\n"

    "<center>\n"
    "  <table cellpadding=\"10\">\n"
    "    <tr>\n"
    "      <td colspan=\"2\"><h2>Show Active RACE Components</h2></td>\n"
    "      <td>\n"
    "        <a href=\"/web_input_adapter.cgi/race_component_display\"><image src=\"/web_content/images/race_components.png\"/></a>\n"
    "      </td>\n"
    "    </tr>\n"
    "    <tr>\n"
    "      <td colspan=\"3\"><hr/></td>\n"
    "    </tr>\n"
    "    <tr>\n"
    "      <td>\n"
    "        <a href=\"/web_input_adapter.cgi/cdp_list\"><image src=\"/web_content/images/deployment_plan.png\"/></a>\n"
    "      </td>\n"
    "      <td colspan=\"2\"><h2>View Deployment Plans &amp; Allocation Results</h2></td>\n"
    "    </tr>\n"
    "    <tr>\n"
    "      <td colspan=\"3\"><hr/></td>\n"
    "    </tr>\n"
    "    <tr>\n"
    "      <td colspan=\"2\"><h2>Manage Live Deployment Plans</h2></td>\n"
    "      <td>\n"
    "        <a href=\"/web_input_adapter.cgi/deployable_cdp_list\"><image src=\"/web_content/images/five_components.png\"/></a>\n"
    "      </td>\n"
    "    </tr>\n"
    "  </table>\n"
    "</center>\n";

  std::cout << "<hr>\n"
    "\n"

    "<cite>Ed Mulholland</cite> <br>\n"
    "<address>Comments and suggestions for improvement: \n"
    "<a href=\"mailto:emulholl@atl.lmco.com\">emulholl@atl.lmco.com</a></address>\n";

  std::cout <<
    "</body>\n"
    "</html>\n"
    "\n";

  return 0;
}

int Web_Input_Adapter::cdp_display_page ()
{
  std::cout << "Content-type: text/xml\n\n";
  std::ifstream xml_stream (show_deployment_plan);
  std::string xml_line;
  while (std::getline (xml_stream, xml_line))
    std::cout << xml_line;
  return 0;
}

int Web_Input_Adapter::race_component_display_page ()
{
  std::cout <<
    "Content-type: text/html\n\n"
    "\n"

    "<html> <head>\n"
    "<title>Active RACE Components</title>\n"
    "<link rel=\"stylesheet\" href=\"/web_content/Web_Input_Adapter.css\" type=\"text/css\"/>\n"
    "</head>\n"
    "\n"

    "<body>\n"
    "\n"

    "<center><image src=\"/web_content/images/active_race_components.png\"></center>\n"
    "<br>\n"
    "<hr>\n"

    "<p>The capability to visualize and (re)configured the currently\n"
    "deployed set of RACE components is still under construction.  Under\n"
    "the current, initial capability, the following table shows a list of\n"
    "all RACE components that are\n"
    "  currently running and can be reached, directly or indirectly, from\n"
    "  the Web Input Adapter using CCM facet-and-receptacle navigation\n"
    "  operations.  The names and values of attributes have been\n"
    "  dynamically probed with the help of CORBA DII and TAO IFR Service.\n"
    "</p>\n"
    "\n"

    "<p>Here is a crude component table. Sensing is \n"
    "implemented in the Component_Control facet, and display is implemented\n"
    "in the body of the component_table function.</p>\n"

    "<center>\n"
    "  <table border cellpadding = 5>\n"
    "    <tr>\n"
    "      <th align = left ><b>RACE Component</b></th>\n"
    "      <th align = center ><b>...has attributes...</b></th>\n"
    "      <th align = center ><b>...and is connected to...</b></th>\n"
    "    </tr>\n";

  // Display component table
  {
    ACE_TRY {
      // Get object reference to the component that we want as the
      // root of our component tree.  (The component that we want as
      // the root of our component tree is the Web_Input_Adapter.)
      CORBA::Object_var tmp = orb->string_to_object (web_input_adapter_ior);
      Components::CCMObject_var component = Components::CCMObject::_narrow (tmp.in ());

      // Get object reference to the Component_Control facet
      tmp = orb->string_to_object (component_control_ior);
      Component_Control_var control = Component_Control::_narrow (tmp.in ());

      // Initialize the Component_Control and obtain a reference to
      // the root of the tree.
      if (! control->is_initialized ())
        control->init_component_tree (component.in ());

      Component_Tree_Node_var tree = control->get_component_tree ();

      // Generate table
      {
        std::set<std::string> already_displayed;
        component_table (tree.inout(),
                         control.inout(),
                         already_displayed);
      }

    } ACE_CATCHANY {
      std::cout << "<pre>\n";
      std::cout << "CORBA Exception: " << ACE_ANY_EXCEPTION._info () << "\n";
      std::cout << "</pre>\n";
    } ACE_CATCH (UnionExtractionException, uee) {
      std::cout << "<pre>\n";
      std::cout << "UnionExtractionException: " << uee.reason () << "\n";
      std::cout << "</pre>\n";
    } ACE_CATCH (NullPointerException, npe) {
      std::cout << "<pre>\n";
      std::cout << "NullPointerException: " << npe.reason () << "\n";
      std::cout << "</pre>\n";
    } ACE_CATCHALL {
      std::cout << "<pre>\n";
      std::cout << "Non-CORBA Exception\n";
      std::cout << "</pre>\n";
    } ACE_ENDTRY;
  }

  std::cout <<
    "  </table>\n"
    "</center>\n"
    "\n"
    "<p>The next step in this capability is to make component attributes\n"
    "editable on the fly, so that, for instance, the SimpleBinPacker can be\n"
    "dynamically reconfigured to consider a different number of bins.</p>\n"

    "<p>The RACE components will be visualized graphically.  A\n"
    "preview of this can be shown seen "
    "<a href=\"/web_content/race_component_preview.html\">here</a>.</p>\n"

    "</body>\n"
    "</html>\n";

  return 0;
}

int Web_Input_Adapter::cdp_list_page ()
{
  std::cout <<
    "Content-type: text/html\n\n"
    "\n"

    "<html> <head>\n"
    "<title>Deployment Plans &amp; Allocation Results</title>\n"
    "<link rel=\"stylesheet\" href=\"/web_content/Web_Input_Adapter.css\" type=\"text/css\"/>\n"
    "<script type=\"text/javascript\" src=\"/web_content/Web_Input_Adapter.js\"/>\n"
    "</head>\n"
    "\n"

    "<body>\n"
    "\n"

    "<center><image src=\"/web_content/images/dp_ar.png\"></center>\n"
    "<br>\n"
    "<hr>\n"

    "<p>Choose a DAnCE flattened deployment plan from the list below, or\n"
    "use the form at the bottom of this page to upload a new flattened\n"
    "deployment plan.  For each plan, you have the option to view either\n"
    "the raw XML or a table showing static allocation of components to\n"
    "nodes.  You can also generate and view the result of dynamic\n"
    "allocation using RACE.</p>\n"

    "<form name=\"deploymentPlanForm\" method=\"get\">\n"
    " <table border=\"1\" cellpadding=\"20\" cellspacing=\"0\" align=\"center\">\n"
    "  <tr>\n"
    "   <td colspan=\"2\" align=\"left\">\n"
    "    <h3 class=\"tableTitle\">Uploaded Deployment Plans</h3>\n"
    "   </td>\n";

  std::vector <std::string> cdp_filenames, cdp_dirnames;
  for (Web_Input_Adapter::Deployment_Plan_Dir * i =
         & Web_Input_Adapter::deployment_plan_dirs [0];
       i->name != 0;
       ++i)
  {
    const char * dirname = i->name;
    ACE_Dirent the_dir (dirname);
    dirent * the_dirent = 0;
    while ( (the_dirent = the_dir.read()))
    {
      const char * filename = the_dirent->d_name;
      int length = ACE_OS::strlen (filename);
      if (length >= 4)
      {
        const char * last_four = & filename [length - 4];
        if (ACE_OS::strcmp (last_four, ".cdp") == 0)
        {
          cdp_filenames.push_back (filename);
          cdp_dirnames.push_back (dirname);
        }
      }
    }
  }

  std::cout <<
    "   <td rowspan=\""
            << cdp_filenames.size () + 1
            << "\" align=\"left\" valign=\"top\">\n"
    "    <img href=\"/spacer.gif\" width=\"70\" height=\"1\" border=\"0\" alt=\"1\"/>\n"
    "   </td>\n"
    "  </tr>\n";

  for (std::vector <std::string> :: iterator
         b (cdp_filenames.begin ()),
         i (cdp_filenames.begin ()),
         e (cdp_filenames.end ()),
         di (cdp_dirnames.begin ());
         i != e;
         ++i, ++di)
  {
    std::cout <<
      "  <tr>\n";

    if (i == b)
      std::cout <<
        "   <td rowspan=\""
                << cdp_filenames.size ()
                << "\" align=\"left\" valign=\"top\">\n"
        "    <img href=\"/spacer.gif\"\n"
        "         width=\"70\"\n"
        "         height=\"1\" border=\"0\" alt=\"1\"/>\n"
        "   </td>\n";

    std::cout <<
      "   <td colspan=\"1\" align=\"left\" valign=\"center\">\n"
      "    <h3 class=\"filename\">"
      "     <input type=\"radio\"\n"
      "            name=\"show_deployment_plan\"\n";

    if (i == b)
      std::cout <<
        "            checked=\"1\"\n";

    std::cout <<
      "            value=\"" << *di << "/" << *i << "\"\n"
      "            >\n"
      "      " << *i << "\n"
      "     </input>\n"
      "    </h3>\n"
      "   </td>\n"
      "  </tr>\n";
  }

  std::cout <<
    "  <tr>\n"
    "   <td colspan=\"3\" align=\"left\">\n"
    "    <h3 class=\"actionTitle\">Actions</h3>\n"
    "   </td>\n"
    "  </tr>\n"
    "  <tr>\n"
    "   <td rowspan=\"3\" align=\"left\" valign=\"top\">\n"
    "    <img href=\"/spacer.gif\"\n"
    "         width=\"1\"\n"
    "         height=\"1\" border=\"0\" alt=\"1\"/>\n"
    "   </td>\n"
    "   <td colspan=\"2\" align=\"left\" valign=\"center\">\n"
    "    <input type=\"button\"\n"
    "           value=\"Show Deployment Plan XML\"\n" <<
    (cdp_filenames.size () == 0 ?
     "           disabled=\"1\"\n" : "") <<
    "           onClick=\"submitPlan('cdp_display', 'n/a')\" />\n"
    "   </td>\n"
    "  </tr>\n"
    "  <tr>\n"
    "   <td colspan=\"2\" align=\"center\" valign=\"center\">\n"
    "    <input type=\"button\"\n"
    "           value=\"Show Static Allocation\"\n" <<
    (cdp_filenames.size () == 0 ?
     "           disabled=\"1\"\n" : "") <<
    "           onClick=\"submitPlan('allocation_display', 'static')\" />\n"
    "   </td>\n"
    "  </tr>\n"
    "  <tr>\n"
    "   <td colspan=\"2\" align=\"right\" valign=\"center\">\n"
    "    <input type=\"button\"\n"
    "           value=\"Generate Dynamic Allocation\"\n" <<
    (cdp_filenames.size () == 0 ?
     "           disabled=\"1\"\n" : "") <<
    "           onClick=\"submitPlan('allocation_display', 'dynamic')\" />\n"
    "   </td>\n"
    "  </tr>\n"
    " </table>\n"
    " <input name=\"dynamic_allocation\" value=\"false\" type=\"hidden\" />\n"
    "</form>\n"

    "</body>\n"
    "</html>\n";

  return 0;
}

int Web_Input_Adapter::deployable_cdp_list_page ()
{
  std::cout <<
    "Content-type: text/html\n\n"
    "\n"

    "<html> <head>\n"
    "<title>Manage Live Deployment Plans</title>\n"
    "<link rel=\"stylesheet\" href=\"/web_content/Web_Input_Adapter.css\" type=\"text/css\"/>\n"
    "<script type=\"text/javascript\" src=\"/web_content/Web_Input_Adapter.js\"/>\n"
    "</head>\n"
    "\n"

    "<body>\n"
    "\n"

    "<center><image src=\"/web_content/images/manage_live_deployment_plans.png\"></center>\n"
    "<br>\n"
    "<hr>\n";

  RACE::Plan_UriSeq_var deployed_uris;
  ACE_TRY  {
    CORBA::Object_var tmp = orb->string_to_object (plan_deployment_ior);
    Plan_Deployment_var plan_depment = Plan_Deployment::_narrow (tmp.in ());


    /*************************************************/
    /*                                               */
    /*      Deploy or teardown plan on request       */
    /*                                               */
    /*************************************************/

    if (ACE_OS::strcmp (deploy_deployment_plan, "?") != 0)
    {
      CORBA::Boolean is_dynamic =
        ACE_OS::strcmp (dynamic_allocation, "true") == 0;

      plan_depment->deploy_plan (deploy_deployment_plan, is_dynamic);
    }

    if (ACE_OS::strcmp (teardown_deployment_plan, "?") != 0)
    {
      plan_depment->teardown_plan (teardown_deployment_plan);
    }

    /***************************************/
    /*                                     */
    /*      Call up deployed_uris          */
    /*                                     */
    /***************************************/
    deployed_uris = plan_depment->get_deployed_plan_uris ();

  } ACE_CATCHANY {
    std::cout << "<pre>\n";
    std::cout << "CORBA Exception: " << ACE_ANY_EXCEPTION._info () << "\n";
    std::cout << "</pre>\n";
  } ACE_CATCHALL {
    std::cout << "<pre>\n";
    std::cout << "Non-CORBA Exception\n";
    std::cout << "</pre>\n";
  } ACE_ENDTRY;

  std::cout <<
    "<p>DAnCE flattened deployment plans appear in two lists below: a\n"
    "list of currently deployed plans and a list of not-yet-deployed\n"
    "plans.  Plans from the not-yet-deployed list may be deployed through\n"
    "DAnCE, with or without pre-processing through RACE.</p>\n"

    "<form name=\"deploymentPlanForm\" method=\"get\">\n";

  /***************************************************************/
  /*                                                             */
  /*  Separate deployed and non-deployed CDP filenames/dirnames  */
  /*                                                             */
  /***************************************************************/

  std::vector <std::string> deployed_cdp_filenames, deployed_cdp_dirnames;
  std::vector <std::string> undeployed_cdp_filenames, undeployed_cdp_dirnames;

  for (Web_Input_Adapter::Deployment_Plan_Dir * i =
         & Web_Input_Adapter::deployment_plan_dirs [0];
       i->name != 0;
       ++i)
  {
    const char * dirname = i->name;
    ACE_Dirent the_dir (dirname);
    dirent * the_dirent = 0;
    while ( (the_dirent = the_dir.read()))
    {
      const char * filename = the_dirent->d_name;
      int length = ACE_OS::strlen (filename);
      if (length >= 4)
      {
        const char * last_four = & filename [length - 4];
        if (ACE_OS::strcmp (last_four, ".cdp") == 0)
        {
          CORBA::ULong j (0), l (deployed_uris->length ());
          for (; j < l; ++j)
          {
            std::string complete_filename =
              std::string () +
              dirname +
              "/" +
              filename;

            if (ACE_OS::strcmp (deployed_uris [j], complete_filename.c_str ()) == 0)
            {
              deployed_cdp_filenames.push_back (filename);
              deployed_cdp_dirnames.push_back (dirname);
              break;
            }
          }
          if (j == l)
          {
            undeployed_cdp_filenames.push_back (filename);
            undeployed_cdp_dirnames.push_back (dirname);
          }
        }
      }
    }
  }

  /*********************************************/
  /*                                           */
  /*  Print deployed CDP filenames in a table  */
  /*                                           */
  /*********************************************/

  std::cout <<
    " <table border=\"1\" cellpadding=\"20\" cellspacing=\"0\" align=\"center\">\n"
    "  <tr>\n"
    "   <td colspan=\"3\" align=\"left\">\n"
    "    <h3 class=\"tableTitle\"><b>Currently</b> Deployed Plans</h3>\n"
    "   </td>\n"
    "   <td rowspan=\""
            << deployed_cdp_filenames.size () + 1
            << "\" align=\"left\" valign=\"top\">\n";

  if (deployed_cdp_filenames.size () == 0)
    std::cout << "    <h3 class=\"filename\">(none)</h3>\n";
  else
    std::cout <<
      "    <img href=\"/spacer.gif\" width=\"70\" height=\"1\" border=\"0\" alt=\"1\"/>\n";

  std::cout <<
    "   </td>\n"
    "  </tr>\n";

  for (std::vector <std::string> :: iterator
         b (deployed_cdp_filenames.begin ()),
         i (deployed_cdp_filenames.begin ()),
         e (deployed_cdp_filenames.end ()),
         di (deployed_cdp_dirnames.begin ());
         i != e;
         ++i, ++di)
  {
    std::cout <<
      "  <tr>\n";

    if (i == b)
      std::cout <<
        "   <td rowspan=\""
                << deployed_cdp_filenames.size ()
                << "\" align=\"left\" valign=\"top\">\n"
        "    <img href=\"/spacer.gif\"\n"
        "         width=\"70\"\n"
        "         height=\"1\" border=\"0\" alt=\"1\"/>\n"
        "   </td>\n";

    std::cout <<
      "   <td colspan=\"1\" align=\"left\" valign=\"center\">\n"
      "    <h3 class=\"filename\">"
      "     " << *i << "\n"
      "    </h3>\n"
      "   </td>\n"
      "   <td colspan=\"1\" align=\"left\" valign=\"center\">\n"
      "    <input type=\"button\"\n"
      "           class=\"smallButton\"\n"
      "           onClick=\"tearDownPlan('" << *di << "/" << *i << "')\"\n"
      "           value=\"TEAR DOWN\"\n"
      "           />\n"
      "   </td>\n"
      "  </tr>\n";
  }

  std::cout <<
    " </table>\n"
    " <br/>\n"
    " <br/>\n";

  /*************************************************/
  /*                                               */
  /*  Print non-deployed CDP filenames in a table  */
  /*                                               */
  /*************************************************/

  std::cout <<
    " <table border=\"1\" cellpadding=\"20\" cellspacing=\"0\" align=\"center\">\n"
    "  <tr>\n"
    "   <td colspan=\"4\" align=\"left\">\n"
    "    <h3 class=\"tableTitle\">Not-Yet-Deployed Plans</h3>\n"
    "   </td>\n"
    "   <td rowspan=\""
            << undeployed_cdp_filenames.size () + 1
            << "\" align=\"left\" valign=\"top\">\n";

  if (undeployed_cdp_filenames.size () == 0)
    std::cout << "    <h3 class=\"filename\">(none)</h3>\n";
  else
    std::cout <<
      "    <img href=\"/spacer.gif\" width=\"70\" height=\"1\" border=\"0\" alt=\"1\"/>\n";

  std::cout <<
    "   </td>\n"
    "  </tr>\n";

  for (std::vector <std::string> :: iterator
         b (undeployed_cdp_filenames.begin ()),
         i (undeployed_cdp_filenames.begin ()),
         e (undeployed_cdp_filenames.end ()),
         di (undeployed_cdp_dirnames.begin ());
         i != e;
         ++i, ++di)
  {
    std::cout <<
      "  <tr>\n";

    if (i == b)
      std::cout <<
        "   <td rowspan=\""
                << undeployed_cdp_filenames.size ()
                << "\" align=\"left\" valign=\"top\">\n"
        "    <img href=\"/spacer.gif\"\n"
        "         width=\"70\"\n"
        "         height=\"1\" border=\"0\" alt=\"1\"/>\n"
        "   </td>\n";

    std::cout <<
      "   <td colspan=\"1\" align=\"left\" valign=\"center\">\n"
      "    <h3 class=\"filename\">"
      "     " << *i << "\n"
      "    </h3>\n"
      "   </td>\n"
      "   <td colspan=\"1\" align=\"left\" valign=\"center\">\n"
      "    <input type=\"button\"\n"
      "           class=\"smallButton\"\n"
      "           onClick=\"deployPlan('static',\n"
      "                                '" << *di << "/" << *i << "')\"\n"
      "           value=\"DEPLOY through\nDAnCE\"\n"
      "           />\n"
      "   </td>\n"
      "   <td colspan=\"1\" align=\"left\" valign=\"center\">\n"
      "    <input type=\"button\"\n"
      "           class=\"smallButton\"\n"
      "           onClick=\"deployPlan('dynamic',\n"
      "                                '" << *di << "/" << *i << "')\"\n"
      "           value=\"DEPLOY through\nRACE and DAnCE\"\n"
      "           />\n"
      "   </td>\n"
      "  </tr>\n";
  }

  std::cout <<
    " </table>\n"
    " <input name=\"dynamic_allocation\" value=\"false\" type=\"hidden\" />\n"
    " <input name=\"deploy_deployment_plan\" value=\"?\" type=\"hidden\" />\n"
    " <input name=\"teardown_deployment_plan\" value=\"?\" type=\"hidden\" />\n"
    "</form>\n"

    "</body>\n"
    "</html>\n";

  return 0;
}

int Web_Input_Adapter::allocation_display_page ()
{
  // Derive a nickname for the deployment plan.
  std::string deployment_plan_nickname;
  {
    CORBA::String_var deployment_plan_copy =
      CORBA::string_dup (show_deployment_plan);
    char * last_slash = ACE_OS::strrchr (deployment_plan_copy, '/');
    if (last_slash == 0 || *(++last_slash) == '\0')
      deployment_plan_nickname = show_deployment_plan;
    else
      deployment_plan_nickname = last_slash;
  }

  CORBA::Boolean is_dynamic =
    ACE_OS::strcmp (dynamic_allocation, "true") == 0;

  std::string static_or_dynamic = is_dynamic ?
    "Dynamic" : "Static";

  std::cout <<
    "Content-type: text/html\n\n"
    "\n"

    "<html> <head>\n"
    "<title>" << static_or_dynamic << " Allocation for " <<
    deployment_plan_nickname <<
    "</title>\n"
    "<link rel=\"stylesheet\" href=\"/web_content/Web_Input_Adapter.css\" type=\"text/css\"/>\n"
    "<script type=\"text/javascript\" src=\"/web_content/Web_Input_Adapter.js\"/>\n"
    "</head>\n"
    "\n"

    "<body>\n"
    "\n"
    "<center>\n"
    "  <h2 class=\"allocationTitle\">\n"
    "    " << static_or_dynamic << "  Allocation for " <<
    deployment_plan_nickname << "\n"
    "  </h2>\n"
    "</center>\n"
    "<hr>\n";

  // Call up static allocation
  Component_AllocationSeq_var allocations;
  ACE_TRY  {
    CORBA::Object_var tmp = orb->string_to_object (plan_deployment_ior);
    Plan_Deployment_var plan_depment = Plan_Deployment::_narrow (tmp.in ());
    allocations = plan_depment->allocate_plan (show_deployment_plan,
                                               is_dynamic);
  } ACE_CATCHANY {
    std::cout << "<pre>\n";
    std::cout << "CORBA Exception: " << ACE_ANY_EXCEPTION._info () << "\n";
    std::cout << "</pre>\n";
  } ACE_CATCHALL {
    std::cout << "<pre>\n";
    std::cout << "Non-CORBA Exception\n";
    std::cout << "</pre>\n";
  } ACE_ENDTRY;

  // Display allocation in very simple form.
  std::cout << "<pre>\n";
  for (CORBA::ULong i = 0, l = allocations->length ();
       i < l;
       ++i)
  {
    Component_Allocation & allocation = allocations [i];
    std::cout << allocation.node_name << "\n";

    Component_Instance_NameSeq & instance_names =
      allocation.component_instance_names;
    for (CORBA::ULong i = 0, l = instance_names.length ();
         i < l;
         ++i)
    {
      std::cout << "\t" << instance_names [i] << "\n";
    }
  }
  std::cout <<
    "</pre>\n"
    "</body>\n"
    "</html>\n";


  return 0;
}


void Web_Input_Adapter::substitute_braced_environment_variables
( const char * subst_name,
  char *& name)
{
  char buffer [BUFSIZ];
  char * bufptr = & buffer [0];
  *bufptr = '\0';
  int remaining = BUFSIZ - 1;

  const char * ptr = subst_name;
  const char * end = & (subst_name [ACE_OS::strlen (subst_name)]);
  int in_braces = 0;

  while (ptr < end && remaining > 0)
  {
    const char * next = 0;
    if (in_braces)
    {
      next = ACE_OS::strchr (ptr, '}');
      if (next == 0)
        break; // no further changes to buffer
      else
      {
        char env_name_pattern [BUFSIZ];
        char env_name_buffer  [BUFSIZ];
        if (ACE_OS::snprintf (env_name_pattern,
                              BUFSIZ,
                              "%%.%ds",
                              next - ptr) <= BUFSIZ -1)
        {
          ACE_OS::snprintf (env_name_buffer,
                            BUFSIZ,
                            env_name_pattern,
                            ptr);
          const char * env_value = ACE_OS::getenv (env_name_buffer);
          if (env_value)
            ACE_OS::snprintf (bufptr, remaining, "%s", env_value);
        }
      }
      ptr = & next [1];
      in_braces = 0;
    }
    else /** ! in_braces **/
    {
      next = ACE_OS::strstr (ptr, "${");
      if (next == 0)
        next = end;
      else
        in_braces = 1;
      int source_length = next - ptr;
      int copy_length   = (source_length < remaining ?
                           source_length :
                           remaining);
      ACE_OS::strncpy (bufptr, ptr, copy_length);
      bufptr [copy_length] = '\0';

      if (next == end)
        ptr = end;
      else
        ptr = & next [2];
    }

    int new_chars_length = ACE_OS::strlen (bufptr);
    bufptr = & bufptr [new_chars_length];
    remaining -= new_chars_length;
  }

  name = new char [ACE_OS::strlen (buffer) + 1];
  ACE_OS::strcpy (name, buffer);
}

void Web_Input_Adapter::component_table (CIAO::RACE::Component_Tree_Node & tree,
                                         CIAO::RACE::Component_Control * control,
                                         std::set<std::string> & already_displayed)
{
  Component_Data & component_data =
    Web_Input_Adapter::component_data_ref (tree);

  Components::CCMObject * root = component_data.the_component.inout ();
  if (CORBA::is_nil (root))
    throw NullPointerException ("root of tree");

  // Get the name of the component
  if (component_data.component_name.in () == 0)
    throw NullPointerException ("component_data.component_name");
  CORBA::String_var name;
  name = component_data.component_name;

  // Don't display the same component twice
  {
    std::string std_name (name.in ());
    if (already_displayed.find (std_name) != already_displayed.end ())
      return;
    else
      already_displayed.insert (std_name);
  }

  // Begin table row
  std::cout << "<tr>\n";

  // Display the name of the component
  std::cout << "<td>" << name.in () << "</td>\n";

  // Get the attributes of the component
  ATTRIBUTE_VEC attr_vec;
  {
    {
      std::ofstream out ("/tmp/get_component_tree.log", std::ios_base::app | std::ios_base::out);
      out << "About to call control->get_component_attributes ("
          << name.in ()
          << ")...\n";
    }
    RACE::AttbuteSeq_var seq = control->get_component_attributes (name.in ());
    {
      std::ofstream out ("/tmp/get_component_tree.log", std::ios_base::app | std::ios_base::out);
      out << "Returned from call control->get_component_attributes ("
          << name.in ()
          << ")...\n";
    }

    char buffer [BUFSIZ];
    for (CORBA::ULong i = 0, l = seq->length (); i < l; ++i)
    {
      Web_Input_Adapter::Attribute __attr;
      {
        RACE::Attbute &      _attr = seq[i];
        CORBA::Any &         ret   = _attr.value;
        CORBA::TypeCode_var  _type = ret.type ();

        if (_attr.name.in () == 0)
          throw NullPointerException ("_attr.name");
        __attr.name       = _attr.name.in ();
        __attr.corba_kind = _type->kind ();

        ACE_OS::snprintf (buffer, BUFSIZ, "_get_%s", __attr.name.c_str ());

        switch (__attr.corba_kind)
        {
        case CORBA::tk_ulong:
          {
            CORBA::ULong extracted;
            ret >>= extracted;
            ACE_OS::snprintf (buffer, BUFSIZ, "%d", extracted);
            break;
          }
        case CORBA::tk_long:
          {
            CORBA::Long extracted;
            ret >>= extracted;
            ACE_OS::snprintf (buffer, BUFSIZ, "%d", extracted);
            break;
          }
        case CORBA::tk_string:
          {
            const char * extracted;
            ret >>= extracted;
            ACE_OS::snprintf (buffer, BUFSIZ, "%s", extracted);
            break;
          }
        default:
          {
            if (_type->equivalent (CORBA::_tc_ulong)) {
              CORBA::ULong extracted;
              ret >>= extracted;
              ACE_OS::snprintf (buffer, BUFSIZ, "%d", extracted);
              break;
            }
              
            if (_type->equivalent (CORBA::_tc_long)) {
              CORBA::Long extracted;
              ret >>= extracted;
              ACE_OS::snprintf (buffer, BUFSIZ, "%d", extracted);
              break;
            }
              
            else if (_type->equivalent (CORBA::_tc_string)) {
              const char * extracted;
              ret >>= extracted;
              ACE_OS::snprintf (buffer, BUFSIZ, "%s", extracted);
              break;
            }
              
            else ACE_OS::snprintf (buffer, BUFSIZ, "");
          }
        }
        __attr.stringified_value = buffer;
      }
      attr_vec.push_back (__attr);
    }
  }

  // Display the attributes of the component
  if (attr_vec.size ())
  {
    std::cout << "<td><table cellpadding = 5>\n";
    for (ATTRIBUTE_VEC::iterator i (attr_vec.begin()), e (attr_vec.end());
         i != e;
         ++i)
    {
      std::cout <<
        "<tr><td><tt>" << i->name << "</tt></td>"
        "<td align = right>" << i->stringified_value << "</td>"
        "<!-- TCKind = " << i->corba_kind << " --></tr>\n";
    }
    std::cout << "</table></td>\n";
  }
  else
  {
    std::cout << "<td>&nbsp;</td>\n";
  }

  // Get the neighbors of the component
  NEIGHBOR_VEC neighbor_vec;
  {
    RACE::Component_Tree & receptacles = tree.children;

    for (CORBA::ULong i = 0, l = receptacles.length (); i < l ; ++i)
    {
      RACE::Component_Tree_Node & receptacle = receptacles [i];
      RACE::Connection_Data & receptacle_data =
        Web_Input_Adapter::connection_data_ref (receptacle);

      if (receptacle_data.receptacle_name.in () == 0)
        throw NullPointerException ("receptacle_data.receptacle_name");
      CORBA::String_var receptacle_name =
        CORBA::string_dup (receptacle_data.receptacle_name.in ());
      RACE::Component_Tree & connectees = receptacle.children;

      for (int j = 0, l = connectees.length (); j < l; ++j)
      {
        RACE::Component_Tree_Node & connectee = connectees [j];
        RACE::Component_Data & connectee_data =
          Web_Input_Adapter::component_data_ref (connectee);

        if (connectee_data.the_component.in () == 0)
          throw NullPointerException ("connectee_data.the_component");
        Components::CCMObject_var connectee_component =
          connectee_data.the_component;

        if (connectee_data.component_name.in () == 0)
          throw NullPointerException ("connectee_data.component_name");
        CORBA::String_var connectee_name =
          CORBA::string_dup (connectee_data.component_name.in ());

        Neighbor __neighbor;
        __neighbor.receptacle_name =     receptacle_name.in ();
        __neighbor.component =           connectee_component;
        __neighbor.component_name =      connectee_name.in ();
        __neighbor.component_tree_node = connectee;
        neighbor_vec.push_back (__neighbor);
      }
    }
  }

  // Display the neighbors of the component
  if (neighbor_vec.size ())
  {
    std::cout << "<td><table cellpadding = 5>\n";
    for (NEIGHBOR_VEC::iterator
           i (neighbor_vec.begin()),
           e (neighbor_vec.end());
         i != e;
         ++i)
    {
      std::cout <<
        "<tr><td> " << i->component_name << " via "
        "<tt>" << i->receptacle_name << "</tt> receptacle</td></tr>\n";
    }
    std::cout << "</table></td>\n";
  }
  else
  {
    std::cout << "<td>&nbsp;</td>\n";
  }

  // End the table row
  std::cout << "</tr>\n";

  // Recursively display neighbors in rows of their own
  for (NEIGHBOR_VEC::iterator
         i (neighbor_vec.begin()),
         e (neighbor_vec.end());
       i != e;
       ++i)
  {
    component_table (i->component_tree_node, control, already_displayed);
  }
}

::CIAO::RACE::Component_Data &
Web_Input_Adapter::component_data_ref (CIAO::RACE::Component_Tree_Node & node)
{
  Component_Tree_Node_Union & node_data = node.node_data;
  if (node_data._d () != COMPONENT_NODE)
    throw UnionExtractionException ("COMPONENT_NODE not found");
  return node_data.the_component_data ();
}

::CIAO::RACE::Connection_Data &
Web_Input_Adapter::connection_data_ref (CIAO::RACE::Component_Tree_Node & node)
{
  Component_Tree_Node_Union & node_data = node.node_data;
  if (node_data._d () != CONNECTION_NODE)
    throw UnionExtractionException ("CONNECTION_NODE not found");
  return node_data.the_connection_data ();
}
