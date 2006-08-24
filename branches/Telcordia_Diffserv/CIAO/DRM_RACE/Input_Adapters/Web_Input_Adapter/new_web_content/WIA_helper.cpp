/**
 * Program to perform interactions with RACE for the python CGI script.
 *
 * @author Jonathan Walsh (jwalsh@atl.lmco.com)
 */


#include <iostream>
#include <string>
#include <vector>

#include <tao/ORB.h>

#include "ace/Env_Value_T.h"
#include "ace/ARGV.h"
#include "ace/Dirent.h"
#include "ciao/Client_init.h"

#include "../Web_Input_AdapterC.h"

using namespace std;

char * plan_path;
CORBA::Boolean is_dynamic;

const char * DEPLOY = "deploy";
const char * TEARDOWN = "teardown";
const char * RACE_STOP = "racestop";
const char * RACE_START = "racestart";
const char * DEPLOYED_QUERY = "query";
const char * DEPLOYNR = "deploynorace";

CORBA::ORB_var orb;

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
      //ENV_VALUE dynamic_allocation ("dynamic_allocation", "?");
      ENV_VALUE deploy_deployment_plan ("deploy_deployment_plan", "?");
      ENV_VALUE teardown_deployment_plan ("teardown_deployment_plan", "?");
      
     void deploy_plan() {
	
       CORBA::Object_var tmp = orb->string_to_object (plan_deployment_ior);
       Plan_Deployment_var plan_depment = Plan_Deployment::_narrow (tmp.in ());

       //CORBA::Boolean is_dynamic = ACE_OS::strcmp (dynamic_allocation, "true") == 0;
       cout << "is_dynamic="<<is_dynamic<<endl;
       
       //dynamic_allocation=plan_path;
       plan_depment->deploy_plan(plan_path, is_dynamic);
       
     }
      
      void teardown_plan() {
	
	CORBA::Object_var tmp = orb->string_to_object (plan_deployment_ior);
	Plan_Deployment_var plan_depment = Plan_Deployment::_narrow (tmp.in ());
	
	plan_depment->teardown_plan (plan_path);
	
      }
      
      void usage() {
	cerr << "Usage: WIA_helper action [plan_path]" << endl;
	cerr << "  action = deploy, teardown, racestart, racestop, query" << endl;
	
      }
      
      void get_deployed_plans() {
	RACE::Plan_UriSeq_var deployed_uris;

	CORBA::Object_var tmp = orb->string_to_object(plan_deployment_ior);

	Plan_Deployment_var plan_depment = Plan_Deployment::_narrow(tmp.in());

	deployed_uris = plan_depment->get_deployed_plan_uris ();
	
	CORBA::ULong j (0), l (deployed_uris->length ());
	for (; j < l; ++j) {
	  cout << deployed_uris[j] << endl;
	}
      }
      
  
    }//namespace
  }
}

/**
 * Main method
 *   argv[1] is the action to perform
 *   argv[2] is the path to the deployment plan
 *   
 */

int main( int argc, char **argv ) {
  
  if ( argc != 3 && argc != 2 ) {
    CIAO::RACE::CIDL_Web_Input_Adapter_Impl::usage();
    return -1;
  }
  orb = CORBA::ORB_init (argc, argv,"");

  if ( strcmp( argv[1], RACE_STOP ) == 0 ) {
    
  } else if ( strcmp( argv[1], RACE_START ) == 0 ) {
    
  } else if ( strcmp( argv[1], DEPLOYED_QUERY ) == 0 ) {

    CIAO::RACE::CIDL_Web_Input_Adapter_Impl::get_deployed_plans();

  } else if ( argc == 3 ){

    plan_path = argv[2];

    if ( strcmp( argv[1], DEPLOY ) == 0 ) {
      is_dynamic = 1;
      cout << "setting is_dynamic="<<is_dynamic;
      CIAO::RACE::CIDL_Web_Input_Adapter_Impl::deploy_plan();

    } else if ( strcmp( argv[1], DEPLOYNR ) == 0  ) {
      is_dynamic = 0;
      CIAO::RACE::CIDL_Web_Input_Adapter_Impl::deploy_plan();

    } else if ( strcmp( argv[1], TEARDOWN ) == 0 ) {
      CIAO::RACE::CIDL_Web_Input_Adapter_Impl::teardown_plan();
    }
  } else {
    CIAO::RACE::CIDL_Web_Input_Adapter_Impl::usage();
    return -1;
  }
  
  return 0;
    
}//main
