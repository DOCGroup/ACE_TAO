//$Id$
#ifndef CIAO_IMPLEMENTATION_INFO_CPP
#define CIAO_IMPLEMENTATION_INFO_CPP

#include "ImplementationInfo.h"
#include "DAnCE/Deployment/CIAO_ServerResourcesC.h"
#include "ace/SString.h"

CIAO::NodeImplementationInfoHandler::
NodeImplementationInfoHandler (const ::Deployment::DeploymentPlan & plan,
                               const Deployment::ComponentPlans & shared_components) :
  plan_ (plan),
  node_info_ (0),
  containers_info_map_ (plan, shared_components)
{
  ACE_NEW (node_info_, ::Deployment::NodeImplementationInfo);
  this->populate_server_resource_def ();
  this->populate_container_impl_infos ();
}

Deployment::NodeImplementationInfo *
CIAO::NodeImplementationInfoHandler::node_impl_info (void) const
{
  Deployment::NodeImplementationInfo_var retv;
  ACE_NEW_RETURN (retv,
                  Deployment::NodeImplementationInfo (this->node_info_.in ()),
                  0);
  return retv._retn ();
}

void
CIAO::NodeImplementationInfoHandler::populate_server_resource_def (void)
{
  CORBA::ULong const instance_len = plan_.instance.length ();
  const char * target_resource_id = 0;

  // Iterate over the instance list to find whether any server resource
  // has been specified
  // TODO: We shoud do some sanity check here, since all the component
  // instance in this NodeApplication should have the same "server_resource_def"
  // defined. Since currently we ignored this sanity check, then will allow
  // users to specify some self-conflicting configuration in the descriptor.

  // NW: this stuff is broken because it assumes all component
  // instances in a node will use the same ServerResource file.  This
  // is not the case.  Component instances using the same
  // ServerResource file will be deployed in the same NA.  However,
  // those using different ServerResource files should be deployed to
  // different NAs.  Something to be fixed later.
  for (CORBA::ULong i = 0; i < instance_len; ++i)
    {
      if (this->plan_.instance[i].deployedResource.length () != 0)
        {
          target_resource_id =
            this->plan_.instance[i].deployedResource[0].resourceName.in ();

          // Some component instance has server resource usage defined, so we
          // set the <nodeapp_config> field of the NodeImplementationInfo struct.
          for (CORBA::ULong j = 0; j < this->plan_.infoProperty.length (); ++j)
            {
              CIAO::DAnCE::ServerResource *server_resource_def = 0;
              this->plan_.infoProperty[j].value >>= server_resource_def;

              if (ACE_OS::strcmp ((*server_resource_def).Id,
                                  target_resource_id) == 0)
                {
                  // Found the target server resource def, and store it.
                  this->node_info_->nodeapp_config.length (1);

                  this->node_info_->nodeapp_config[0].name =
                    CORBA::string_dup ("CIAOServerResources");

                  this->node_info_->nodeapp_config[0].value <<=
                    *server_resource_def;
                  break;
                }
            }
        }
    }
}

void
CIAO::NodeImplementationInfoHandler::populate_container_impl_infos (void)
{
  CORBA::ULong const curr_len =
    this->containers_info_map_.containers_info ()->length ();

  ACE_UNUSED_ARG (curr_len);

  // assignment operation
  this->node_info_->impl_infos =
    *(this->containers_info_map_.containers_info ());
}

#endif /* CIAO_IMPLEMENTATION_INFO_CPP */
