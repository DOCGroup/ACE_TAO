/**
 * @file
 * @ingroup Plan_Deployment
 * @brief  C++ implementation of the Plan_Deployment facet
 *         provided by Web_Input_Adapter.
 *
 * @copydoc Plan_Deployment
 * $Id$
 */

#include "tools/Config_Handlers/Common.h"
#include "tools/Config_Handlers/XML_File_Intf.h"
#include "tools/Config_Handlers/Package_Handlers/PCD_Handler.h"
#include <map>
#include <vector>
#include <string>

#include "Web_Input_Adapter_exec.h"
#include "Plan_Deployment_exec.h"
#include "web_common/web_common_impl.h"

namespace CIDL_Web_Input_Adapter_Impl = CIAO::RACE::CIDL_Web_Input_Adapter_Impl;
namespace RACE                        = CIAO::RACE;

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Web_Input_Adapter_Impl
    {
      /// Pimpl helper class.
      /**
       *  Fulfills C++ "pimpl" idiom for certain functional aspects of
       *  main class Plan_Deployment_exec_i.
       */
      class Deploy_and_Teardown_Helper
      {
      private:

        /// @brief a std::map of
        /// <tt>{Deployment Plan URI} --> {Web_Deployment valuetype}</tt>
        typedef std::map <std::string, RACE::Web_Deployment_var> DEPLOYMENT_MAP;

        /// Pointer to Web Input Adapter Component Executor
        Web_Input_Adapter_exec_i & component_;

        /// List of all currently deployed plans.
        /**
         *  (actually, a std::map where the key is the deployment plan
         *   URI and the value is an instance of
         *   CIAO::RACE::Web_Deployment)
         */
        DEPLOYMENT_MAP deployment_map_;

      public:
        /// Constructor
        /**
         *  @param[in] component   Reference to the
         *                         Web_Input_Adapter_exec_i that we
         *                         are helping to provide the
         *                         implementation of.
         */
        Deploy_and_Teardown_Helper (Web_Input_Adapter_exec_i & component);

        /// Deploy a deployment plan using DAnCE, without using RACE.
        /**
         *  Deploy the plan using DAnCE.  If deployment is successful,
         *  then add the plan to a list of currently running plans.
         *
         *  @param[in] uri   URI of a deployment plan to be
         *                   deployed.
         */
        void deploy_plan_without_race (const char * uri);

        /// Deploy a deployment plan using RACE and DAnCE.
        /**
         *  Process the plan using RACE.  If processing is
         *  successful, then deploy the plan using DAnCE.  If
         *  deployment is successful, then add the plan to a list of
         *  currently deployed plans.
         *
         *  @param[in] uri   URI of a deployment plan to
         *                   processed (using RACE) and
         *                   deployed (using DAnCE).
         */
        void deploy_plan_with_race (const char * uri);

        /// Tear down a plan.
        /**
         *  If and only if the \p uri names a plan that was previously
         *  deployed (through this interface) and not yet torn down
         *  (or at least not through this interface), an attempt will
         *  be made to tear down the plan presently.
         *
         *  Regardless of whether or not the teardown succeeds, the
         *  plan will be removed from the locally-maintained list of
         *  plans that are currently deployed.
         *
         *  @param[in] uri   URI of a deployment plan to be
         *                   torn down.
         */
        void teardown_plan (const char * uri);

        /// Get URIs of deployed plans.
        /**
         *  @return   A CORBA string sequence, where each string in
         *            sequence is a deployment plan URI.  A given
         *            deployment plan URI will exist in this sequence
         *            if and only if the corresponding plan was
         *            deployed (through this interface) and not yet
         *            torn down (or at  least not through this
         *            interface).
         */
        RACE::Plan_UriSeq * get_deployed_plan_uris ();
      };
    }
  }
}

CIDL_Web_Input_Adapter_Impl::Deploy_and_Teardown_Helper::Deploy_and_Teardown_Helper
(CIDL_Web_Input_Adapter_Impl::Web_Input_Adapter_exec_i & component)
  : component_ (component)
{
  // do nothing
}

void
CIDL_Web_Input_Adapter_Impl::Deploy_and_Teardown_Helper::deploy_plan_without_race
(const char * uri)
{
  ACE_DEBUG ((LM_DEBUG, "Deploy_and_Teardown_Helper::deploy_and_teardown_helper (%s)\n",
              uri));

  std::string std_string_uri (uri);
  std::string package_uri (uri);
  try
  {
    package_uri.replace (package_uri.find (".cdp"), 4, ".pcd");
  } catch (...) {
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) Could not replace \".cdp\" with \".pcd\" in \"%s\"\n",
                uri));
  }

  RACE::Web_Deployment_var new_deployment =
    new OBV_CIAO::RACE::Web_Deployment_impl (uri, package_uri.c_str ());

  if (! CORBA::is_nil (new_deployment->launch_plan ()))
    this->deployment_map_ [std_string_uri] = new_deployment;
}

void
CIDL_Web_Input_Adapter_Impl::Deploy_and_Teardown_Helper::deploy_plan_with_race
(const char * uri)
{
  std::string std_string_uri (uri);
  std::string package_uri (uri);
  try
  {
    package_uri.replace (package_uri.find (".cdp"), 4, ".pcd");
  } catch (...) {
    ACE_ERROR ((LM_ERROR, 
                "(%P|%t) Could not replace \".cdp\" with \".pcd\" in \"%s\"\n",
                uri));
  }

  // Read the deployment plan from the passed-in URL.
  ::Deployment::DeploymentPlan_var plan;
  ::Deployment::PackageConfiguration_var pcd (new ::Deployment::PackageConfiguration);

  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG, "Web_Input_Adapter::Parsing deployment plan: %s\n",
		  uri));
      CIAO::Config_Handlers::XML_File_Intf intf (uri);
      plan = intf.get_plan ();
    }
  ACE_CATCH (CIAO::Config_Handlers::Config_Error, ex)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) Caught exception while parsing"
                  " deployment plan %s: \"%s\" -- %s\n",
                  uri,
                  ex.name_.c_str (),
                  ex.error_.c_str ()));
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Caught exception while parsing"
                  " deployment plan %s: %s\n",
                  uri,
                  ACE_ANY_EXCEPTION._info().fast_rep()));
    }
  ACE_CATCHALL
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Caught exception while parsing"
                  " deployment plan %s.\n",
                  uri));
    }
  ACE_ENDTRY;

  if (plan.ptr () == 0)
  {
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) XML_File_Intf::get_plan(\"%s\") returned null.\n",
                uri));
  }

  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG, "Web_Input_Adapter::Parsing Package Config %s\n",
		  package_uri.c_str ()));
      ::CIAO::Config_Handlers::Packaging::PCD_Handler::package_config
        (package_uri.c_str (), pcd.inout ());
    }
  ACE_CATCH (CIAO::Config_Handlers::Config_Error, ex)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) Caught exception while parsing"
                  " package handler %s: \"%s\" -- %s\n",
                  package_uri.c_str (),
                  ex.name_.c_str (),
                  ex.error_.c_str ()));
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Caught exception while parsing"
                  " package handler %s: %s\n",
                  package_uri.c_str (),
                  ACE_ANY_EXCEPTION._info().fast_rep()));
    }
  ACE_CATCHALL
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Caught exception while parsing"
                  " package handler %s.\n",
                  package_uri.c_str ()));
    }
  ACE_ENDTRY;

  RACE::Web_Deployment_var new_deployment =
    (this->component_.process_plan (plan.inout (),
                                    pcd.inout ()));

  if (new_deployment.ptr ())
  {
    new_deployment->plan_uri (CORBA::string_dup (uri));
    new_deployment->pcd_uri (CORBA::string_dup (package_uri.c_str ()));

    new_deployment->_add_ref (); // Should be able to do this using
                                 // RACE::Web_Deployment::_duplicate,
                                 // when we initialized new_deployment,
                                 // but _duplicate does not exist for
                                 // valuetypes

    if (! CORBA::is_nil (new_deployment->launch_plan ()))
      this->deployment_map_ [std_string_uri] = new_deployment;
  }
}

void
CIDL_Web_Input_Adapter_Impl::Deploy_and_Teardown_Helper::teardown_plan (const char * uri)
{
  std::string std_string_uri (uri);

  DEPLOYMENT_MAP::iterator found = this->deployment_map_.find (std_string_uri);
  if (found == this->deployment_map_.end ())
  {
    ACE_ERROR ((LM_ERROR,
                "Deploy_and_Teardown_Helper::teardown_plan: no deployed plan "
                "has URI %s\n",
                uri));
    return;
  }

  ACE_TRY_NEW_ENV
    {
      found->second->teardown_plan ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P|%t) Caught exception while tearing plan: ");
    }
  ACE_CATCHALL
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Caught exception while tearing plan.\n"));
    }
  ACE_ENDTRY;

  this->deployment_map_.erase (found);
}

RACE::Plan_UriSeq *
CIDL_Web_Input_Adapter_Impl::Deploy_and_Teardown_Helper::get_deployed_plan_uris ()
{
  DEPLOYMENT_MAP::size_type map_size = this->deployment_map_.size ();
  RACE::Plan_UriSeq_var seq (new RACE::Plan_UriSeq(map_size));
  seq->length (map_size);

  CORBA::ULong corba_i = 0;
  for (DEPLOYMENT_MAP::iterator
         i (this->deployment_map_.begin ()),
         e (this->deployment_map_.end ());
       i != e;
       ++i, ++corba_i)
  {
    seq [corba_i] = CORBA::string_dup (i->first.c_str ());
  }

  return seq._retn ();
}

/// Constructor
/**
 *  @param[in] component   Reference to the
 *                         Web_Input_Adapter_exec_i that we
 *                         are helping to provide the
 *                         implementation of.
 */
CIDL_Web_Input_Adapter_Impl::Plan_Deployment_exec_i::Plan_Deployment_exec_i
(Web_Input_Adapter_exec_i & component)
  :
  component_ (component),
  helper_ (new Deploy_and_Teardown_Helper (component))
{
  // do nothing
}

/// Destructor
CIDL_Web_Input_Adapter_Impl::Plan_Deployment_exec_i::~Plan_Deployment_exec_i ()
{
  delete this->helper_;
}

/// Produce a structure populated with component allocation data
/**
 *  Without actually deploying any components, examine a deployment
 *  plan and produce the allocation (of the components in the plan)
 *  across DAnCE user-friendly node names.
 *
 *  @param[in] uri        the URI for a deployment plan for which an
 *                        allocation is to be produced.
 *  @param[in] use_race   \p false to produce a static allocation, or
 *                        \p true to produce a dynamic allocation
 *                        using RACE.
 */
RACE::Component_AllocationSeq *
CIDL_Web_Input_Adapter_Impl::Plan_Deployment_exec_i::allocate_plan (
    const char * uri,
    ::CORBA::Boolean use_race
    ACE_ENV_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  ACE_DEBUG ((LM_DEBUG,
              "Entered Plan_Deployment_exec_i::allocate_plan (uri = %s, use_race = %d)\n",
              uri,
              use_race));

  RACE::Component_AllocationSeq_var seq (new RACE::Component_AllocationSeq (0));
  seq->length (0);

  if (uri == 0)
  {
    ACE_ERROR ((LM_ERROR,
                "Null uri in Plan_Deployment_exec_i::allocate_plan\n"));
    return seq._retn ();
  }

  std::string package_uri (uri);
  try
  {
    package_uri.replace (package_uri.find (".cdp"), 4, ".pcd");
  } catch (...) {
    ACE_ERROR ((LM_ERROR, 
                "(%P|%t) Could not replace \".cdp\" with \".pcd\" in \"%s\"\n",
                uri));
  }

  // Read the deployment plan from the passed-in URI.
  ::Deployment::DeploymentPlan_var plan;
  ::Deployment::PackageConfiguration_var pcd
      (new ::Deployment::PackageConfiguration);

  ACE_TRY_NEW_ENV
    {
      CIAO::Config_Handlers::XML_File_Intf intf (uri);
      plan = intf.get_plan ();
    }
  ACE_CATCH (CIAO::Config_Handlers::Config_Error, ex)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) Caught exception while parsing"
                  " deployment plan %s: \"%s\" -- %s\n",
                  uri,
                  ex.name_.c_str (),
                  ex.error_.c_str ()));
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Caught exception while parsing"
                  " deployment plan %s: %s\n",
                  uri,
                  ACE_ANY_EXCEPTION._info().fast_rep()));
    }
  ACE_CATCHALL
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Caught exception while parsing"
                  " deployment plan %s.\n",
                  uri));
    }
  ACE_ENDTRY;

  if (plan.ptr () == 0)
  {
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) XML_File_Intf::get_plan(\"%s\") returned null.\n",
                uri));
  }

  ACE_TRY_NEW_ENV
    {
      CIAO::Config_Handlers::Packaging::PCD_Handler::package_config
        (package_uri.c_str (), pcd.inout ());
    }
  ACE_CATCH (CIAO::Config_Handlers::Config_Error, ex)
    {
      ACE_ERROR ((LM_ERROR, "(%P|%t) Caught exception while parsing"
                  " package handler %s: \"%s\" -- %s\n",
                  package_uri.c_str (),
                  ex.name_.c_str (),
                  ex.error_.c_str ()));
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Caught exception while parsing"
                  " package handler %s: %s\n",
                  package_uri.c_str (),
                  ACE_ANY_EXCEPTION._info().fast_rep()));
    }
  ACE_CATCHALL
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Caught exception while parsing"
                  " deployment plan %s.\n",
                  package_uri.c_str ()));
    }
  ACE_ENDTRY;

  if (plan.ptr () == 0)
  {
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) XML_File_Intf::get_plan returned null."));
    return seq._retn ();
  }

  // Optionally use RACE to generate a dynamic deployment
  if (use_race)
  {
    this->component_.process_plan (plan.inout (), pcd.inout ());
  }

  // Populate Allocation Sequence
  typedef std::vector < std::string > StringVec;
  typedef std::map < std::string, StringVec > StringVecMap;
  StringVecMap string_vec_map;

  ::Deployment::InstanceDeploymentDescriptions & idds =
      plan->instance;
  for (CORBA::ULong i = 0, l = idds.length ();
       i < l;
       ++i)
  {
    ::Deployment::InstanceDeploymentDescription & idd = idds [i];

    CORBA::String_var node (idd.node);
    CORBA::String_var name (idd.name);

    std::string node_str (node.in ());
    std::string name_str (name.in ());

    string_vec_map [node_str].push_back (name_str);
  }

  seq->length (string_vec_map.size ());

  CORBA::ULong i = 0;
  for (StringVecMap::iterator
         ii (string_vec_map.begin ()),
         ee (string_vec_map.end ());
       ii != ee;
       ++i, ++ii)
  {
    Component_Allocation & alloc = seq [i];
    Component_Instance_NameSeq & instance_name_seq =
      alloc.component_instance_names;
    const std::string & node_name = ii->first;
    StringVec & instance_names = ii->second;

    alloc.node_name = CORBA::string_dup (node_name.c_str ());
    instance_name_seq.length (instance_names.size ());
    for (CORBA::ULong j = 0, l = instance_name_seq.length ();
         j < l;
         ++j)
    {
      std::string & instance_name = instance_names [j];
      instance_name_seq [j] = CORBA::string_dup (instance_name.c_str ());
    }
  }

  // Return populated Allocation Sequence
  return seq._retn ();
}

/**
 *  @brief Deploy a deployment plan, using DAnCE, optionally preceeded by
 *         dynamic allocation using RACE.
 *
 *  If \p use_race is \p false, then deploye the plan using DAnCE.
 *  If deployment is successful, then add the plan to a list of
 *  currently deployed plans.
 *
 *  If \p use_race is \p true, then processed the plan using RACE.  If
 *  processing is successful, then deploy the plan using DAnCE.  If
 *  deployment is successful, then add the plan to a list of currently
 *  deployed plans.
 *
 *  @param[in] uri        the URI of a deployment plan to be deployed.
 *  @param[in] use_race   \p false to deploy a static allocation, or
 *                        \p true to deploy a dynamic allocation
 *                        produced by RACE.
 */
void CIDL_Web_Input_Adapter_Impl::Plan_Deployment_exec_i::deploy_plan (
    const char * uri,
    ::CORBA::Boolean use_race
    ACE_ENV_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  ACE_DEBUG ((LM_DEBUG,
              "Entered Plan_Deployment_exec_i::deploy_plan (uri = %s, use_race = %d)\n",
              uri,
              use_race));

  if (uri == 0)
  {
    ACE_ERROR ((LM_ERROR,
                "Null uri in Plan_Deployment_exec_i::deploy_plan\n"));
    return;
  }

  if (use_race)
    this->helper_->deploy_plan_with_race (uri);
  else
    this->helper_->deploy_plan_without_race (uri);
}

/// Tear down a plan.
/**
 *  If and only if the \p uri names a plan that was previously
 *  deployed (through this interface) and not yet torn down
 *  (or at least not through this interface), an attempt will
 *  be made to tear down the plan presently.
 *
 *  Regardless of whether or not the teardown succeeds, the
 *  plan will be removed from the locally-maintained list of
 *  plans that are currently deployed.
 *
 *  @param[in] uri   URI of a deployment plan to be
 *                   torn down.
 */
void CIDL_Web_Input_Adapter_Impl::Plan_Deployment_exec_i::teardown_plan (
    const char * uri
    ACE_ENV_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  ACE_DEBUG ((LM_DEBUG,
              "Entered Plan_Deployment_exec_i::teardown_plan (uri = %s)\n",
              uri));

  if (uri == 0)
  {
    ACE_ERROR ((LM_ERROR,
                "Null uri in Plan_Deployment_exec_i::teardown_plan\n"));
    return;
  }

  this->helper_->teardown_plan (uri);
}

/// Get URIs of deployed plans.
/**
 *  @return   A CORBA string sequence, where each string in
 *            sequence is a deployment plan URI.  A given
 *            deployment plan URI will exist in this sequence
 *            if and only if the corresponding plan was
 *            deployed (through this interface) and not yet
 *            torn down (or at  least not through this
 *            interface).
 */
::CIAO::RACE::Plan_UriSeq *
CIDL_Web_Input_Adapter_Impl::Plan_Deployment_exec_i::get_deployed_plan_uris (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((
    ::CORBA::SystemException
  ))
{
  return this->helper_->get_deployed_plan_uris ();
}
