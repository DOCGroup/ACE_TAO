// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "Plan_Handler.h"
#include "ID_Handler.h"
#include "CompIntrDesc_Handler.h"
#include "Property_Handler.h"
#include "SP_Handler.h"
#include "Requirement_Handler.h"
#include "Any_Handler.h"
#include "RUK_Handler.h"
#include "CEPE_Handler.h"
#include "ERE_Handler.h"
#include "CPK_Handler.h"
#include "Process_Element.h"
#include "Utils.h"
#include <iostream>
#include "string.h"

BEGIN_DEPLOYMENT_NAMESPACE

/// handle the DeploymentPlan type and populate the IDL structure
void Plan_Handler::process_plan(Deployment::DeploymentPlan& plan)
{
  for (DOMNode* node = this->iter_->nextNode();
       node != 0;
       node = this->iter_->nextNode())
    {
      XStr node_name (node->getNodeName ());


      if (node_name == XStr
          (ACE_TEXT ("Deployment:DeploymentPlan")))
        {
        }
      else if
        (CIAO::Config_Handler::Utils::process_string 
             (this->iter_, node_name, "label", plan.label));
      else if
        (CIAO::Config_Handler::Utils::process_string 
             (this->iter_, node_name, "UUID", plan.UUID));
      else if
        (process_element<Deployment::ComponentInterfaceDescription>
           (this->doc_, this->iter_, node,
            node_name, "realizes", plan.realizes,
            this, &Plan_Handler::process_ccd,
            this->id_map_));
      else if
        (process_sequence_local<Deployment::MonolithicDeploymentDescription>
           (this->doc_, this->iter_, node,
            node_name, "implementation", plan.implementation,
            this, &Plan_Handler::process_mdd));
      else if
        (process_sequence_local<Deployment::InstanceDeploymentDescription>
           (this->doc_, this->iter_, node,
            node_name, "instance", plan.instance,
            this, &Plan_Handler::process_idd));
      else if
        (process_sequence_local<Deployment::PlanConnectionDescription>
           (this->doc_, this->iter_, node,
            node_name, "connection", plan.connection,
            this, &Plan_Handler::process_pcd));
      else if
        (process_sequence_local<Deployment::PlanPropertyMapping>
           (this->doc_, this->iter_, node,
            node_name, "externalProperty", plan.externalProperty,
            this, &Plan_Handler::process_ppm));
      else if
        (process_sequence_common<Deployment::ImplementationDependency>
           (this->doc_, this->iter_, node,
            node_name, "dependsOn", plan.dependsOn,
            &ID_Handler::process_ImplementationDependency, this->id_map_));
      else if
        (process_sequence_local<Deployment::ArtifactDeploymentDescription>
           (this->doc_, this->iter_, node,
            node_name, "artifact", plan.artifact,
            this, &Plan_Handler::process_add));
      else if
        (process_sequence_common<Deployment::Property>
           (this->doc_, this->iter_, node,
            node_name, "infoProperty", plan.infoProperty,
            &Property_Handler::process_Property,
            this->id_map_));
      else
        {
          // ??? How did we get here ???
          ACE_THROW (CORBA::INTERNAL());
        }
    }

  this->update_mdd_refs (plan);
  this->update_idd_refs (plan);
  this->update_pspe_refs (plan);
  this->update_pspr_refs (plan);
  return;
}

void Plan_Handler::process_rdd (DOMNodeIterator* iter,
                                Deployment::
                                ResourceDeploymentDescription& rdd)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr 
                  (ACE_TEXT ("Deployment:ResourceDeploymentDescription")))
        {
        }
      else if
        (CIAO::Config_Handler::Utils::process_string 
              (iter, node_name, "requirementName", 
               rdd.requirementName));
      else if
        (CIAO::Config_Handler::Utils::process_string 
              (iter, node_name, "resourceName", rdd.resourceName));
      else if (node_name == XStr (ACE_TEXT ("resourceValue")))
        {
          Any_Handler::process_Any (iter, rdd.resourceValue);
        }
      else
        {
          iter->previousNode();
          return;
        }
    }
}

void Plan_Handler::process_irdd (DOMNodeIterator* iter,
                                 Deployment::
                                 InstanceResourceDeploymentDescription &irdd)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr 
                (ACE_TEXT ("Deployment:InstanceResourceDeploymentDescription")))
        {
        }
      else if (node_name == XStr ("resourceUsage"))
        {
          RUK_Handler::process_ResourceUsageKind (iter, irdd.resourceUsage);
        }
      else if
        (CIAO::Config_Handler::Utils::process_string 
            (iter, node_name, "requirementName", 
             irdd.requirementName));
      else if
        (CIAO::Config_Handler::Utils::process_string 
             (iter, node_name, "resourceName", irdd.resourceName));
      else if (node_name == XStr (ACE_TEXT ("resourceValue")))
        {
          Any_Handler::process_Any (iter, irdd.resourceValue);
        }
      else
        {
          iter->previousNode();
          return;
        }
    }
}

void Plan_Handler::process_add (DOMNodeIterator* iter,
                                Deployment::ArtifactDeploymentDescription& add)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr 
          (ACE_TEXT ("Deployment:ArtifactDeploymentDescription")))
        {
        }
      else if
        (CIAO::Config_Handler::Utils::process_string 
               (iter, node_name, "name", add.name));
      else if
        (CIAO::Config_Handler::Utils::process_string_seq 
               (iter, node_name, "location", add.location));
      else if
        (CIAO::Config_Handler::Utils::process_string 
               (iter, node_name, "node", add.node));
      else if
        (CIAO::Config_Handler::Utils::process_string_seq 
               (iter, node_name, "source", add.source));
      else if
        (process_sequence_common<Deployment::Property>
           (node->getOwnerDocument(), iter, node,
            node_name, "execParameter", add.execParameter,
            &Property_Handler::process_Property,
            this->id_map_));
      else if
        (process_sequence_common<Deployment::Requirement>
           (node->getOwnerDocument(), iter, node,
            node_name, "deployRequirement", add.deployRequirement,
            &Requirement_Handler::process_Requirement,
            this->id_map_));
      else if
        (process_sequence_local<Deployment::ResourceDeploymentDescription>
           (node->getOwnerDocument(), iter, node,
            node_name, "deployedResource", add.deployedResource,
            this, &Plan_Handler::process_rdd));
      else
        {
          iter->previousNode();
          return;
        }
    }
}

void Plan_Handler::process_idd (DOMNodeIterator* iter,
                                Deployment::InstanceDeploymentDescription& idd)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr 
                    (ACE_TEXT ("Deployment:InstanceDeploymentDescription")))
        {
        }
      else if
        (CIAO::Config_Handler::Utils::process_string 
              (iter, node_name, "name", idd.name));
      else if
        (CIAO::Config_Handler::Utils::process_string 
              (iter, node_name, "node", idd.node));
      else if
        (CIAO::Config_Handler::Utils::process_string_seq 
              (iter, node_name, "source", idd.source));
      else if
        (process_reference (node, node_name, "implementation",
                            idd.implementationRef, 
                            this->index_, this->idref_map_))
        {
          this->index_ = this->index_ + 1;
        }
      else if
        (process_sequence_common<Deployment::Property>
           (node->getOwnerDocument(), iter, node,
            node_name, "configProperty", idd.configProperty,
            &Property_Handler::process_Property,
            this->id_map_));
      else if
        (process_sequence_local<Deployment::
                                InstanceResourceDeploymentDescription>
           (node->getOwnerDocument(), iter, node,
            node_name, "deployedResource", idd.deployedResource,
            this, &Plan_Handler::process_irdd));
      else if
        (process_sequence_local<Deployment::
                                InstanceResourceDeploymentDescription>
           (node->getOwnerDocument(), iter, node,
            node_name, "deployedSharedResource", idd.deployedSharedResource,
            this, &Plan_Handler::process_irdd));
      else
        {
          iter->previousNode();
          return;
        }
    }
}

void Plan_Handler::process_mdd (DOMNodeIterator* iter,
                                Deployment::MonolithicDeploymentDescription&
                                mdd)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr 
                 (ACE_TEXT ("Deployment:MonolithicDeploymentDescription")))
        {
        }
      else if
        (CIAO::Config_Handler::Utils::process_string 
             (iter, node_name, "name", mdd.name));
      else if
        (CIAO::Config_Handler::Utils::process_string_seq 
             (iter, node_name, "source", mdd.source));
      else if
        (process_reference_seq (node, node_name, "artifact",
                                mdd.artifactRef,
                                this->index_, this->idref_map_))
        {
          this->index_ = this->index_ + 1;
        }
      else if
        (process_sequence_common<Deployment::Property>
           (node->getOwnerDocument(), this->iter_, node,
            node_name, "execParameter", mdd.execParameter,
            &Property_Handler::process_Property,
            this->id_map_));
      else if
        (process_sequence_common<Deployment::Requirement>
           (node->getOwnerDocument(), this->iter_, node,
            node_name, "deployRequirement", mdd.deployRequirement,
            &Requirement_Handler::process_Requirement,
            this->id_map_));
      else
        {
          iter->previousNode();
          return;
        }
    }
}

void Plan_Handler::process_ccd (DOMNodeIterator* iter,
                                Deployment::ComponentInterfaceDescription&
                                cid)
{
  CompIntrDesc_Handler handler (iter, false);
  handler.process_ComponentInterfaceDescription (cid);
}

void Plan_Handler::process_pspr (DOMNodeIterator* iter,
                                 Deployment::PlanSubcomponentPropertyReference&                                  pspr)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr 
             (ACE_TEXT ("Deployment:PlanSubcomponentPropertyReference")))
        {
        }
      else if
        (CIAO::Config_Handler::Utils::process_string 
             (iter, node_name, "propertyName", pspr.propertyName));
      else if
        (process_reference (node, node_name, "instance", pspr.instanceRef, 
                            this->index_, this->idref_map_))
        {
          this->index_ = this->index_ + 1;
        }
      else
        {
          iter->previousNode();
          return;
        }
    }
}

void Plan_Handler::process_pspe (DOMNodeIterator* iter,
                                 Deployment::PlanSubcomponentPortEndpoint& pspe)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr 
            (ACE_TEXT ("Deployment:PlanSubcomponentPortEndpoint")))
        {
        }
      else if
        (CIAO::Config_Handler::Utils::process_string 
              (iter, node_name, "portName", pspe.portName));
      else if
        (CIAO::Config_Handler::Utils::process_boolean 
              (iter, node_name, "provider", pspe.provider));
      else if (node_name == XStr (ACE_TEXT ("kind")))
        {
          CPK_Handler::process_CCMComponentPortKind (iter, pspe.kind);
        }
      else if
        (process_reference (node, node_name, "instance",
                            pspe.instanceRef,
                            this->index_, this->idref_map_))
        {
          this->index_ = this->index_ + 1;
        }
      else
        {
          iter->previousNode();
          return;
        }
    }
}
    
void Plan_Handler::process_ppm(DOMNodeIterator* iter,
                               Deployment::PlanPropertyMapping& ppm)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr 
            (ACE_TEXT ("Deployment:PlanPropertyMapping")))
        {
        }
      else if
        (CIAO::Config_Handler::Utils::process_string 
              (iter, node_name, "name", ppm.name));
      else if
        (CIAO::Config_Handler::Utils::process_string_seq 
              (iter, node_name, "source", ppm.source));
      else if
        (CIAO::Config_Handler::Utils::process_string 
              (iter, node_name, "externalName", ppm.externalName));
      else if
        (process_sequence_local<Deployment::PlanSubcomponentPropertyReference>
           (node->getOwnerDocument(), iter, node,
            node_name, "delegatesTo", ppm.delegatesTo,
            this, &Plan_Handler::process_pspr));
      else
        {
          iter->previousNode();
          return;
        }
    }
}

void Plan_Handler::process_crdd (DOMNodeIterator* iter,
                                 Deployment::
                                 ConnectionResourceDeploymentDescription& crdd)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr 
            (ACE_TEXT ("Deployment:ConnectionResourceDeploymentDescription")))
        {
        }
      else if
        (CIAO::Config_Handler::Utils::process_string 
             (iter, node_name, "targetName", crdd.targetName));
      else if
        (CIAO::Config_Handler::Utils::process_string 
             (iter, node_name, "requirementName", 
              crdd.requirementName));
      else if
        (CIAO::Config_Handler::Utils::process_string 
            (iter, node_name, "resourceName", crdd.resourceName));
      else if (node_name == XStr (ACE_TEXT ("resourceValue")))
        {
          Any_Handler::process_Any (iter, crdd.resourceValue);
        }
      else
        {
          iter->previousNode();
          return;
        }
    }
}

void Plan_Handler::process_pcd (DOMNodeIterator* iter,
                                Deployment::PlanConnectionDescription& pcd)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr 
             (ACE_TEXT ("Deployment:PlanConnectionDescription")))
        {
        }
      else if
        (CIAO::Config_Handler::Utils::process_string 
              (iter, node_name, "name", pcd.name));
      else if
        (CIAO::Config_Handler::Utils::process_string_seq 
              (iter, node_name, "source", pcd.source));
      else if
        (process_sequence_common<Deployment::Requirement>
           (node->getOwnerDocument(), this->iter_, node,
            node_name, "deployRequirement", pcd.deployRequirement,
            &Requirement_Handler::process_Requirement,
            this->id_map_));
      else if
        (process_sequence_common<Deployment::ComponentExternalPortEndpoint>
           (node->getOwnerDocument(), iter, node,
            node_name, "externalEndpoint", pcd.externalEndpoint,
            &CEPE_Handler::process_ComponentExternalPortEndpoint,
            this->id_map_));
      else if
        (process_sequence_local<Deployment::PlanSubcomponentPortEndpoint>
           (node->getOwnerDocument(), iter, node,
            node_name, "internalEndpoint", pcd.internalEndpoint,
            this, &Plan_Handler::process_pspe));
      else if
        (process_sequence_common<Deployment::ExternalReferenceEndpoint>
           (node->getOwnerDocument(), iter, node,
            node_name, "externalReference", pcd.externalReference,
            &ERE_Handler::process_ExternalReferenceEndpoint,
            this->id_map_));
      else if
        (process_sequence_local<Deployment::
                                ConnectionResourceDeploymentDescription>
           (node->getOwnerDocument(), iter, node,
            node_name, "deployedResource", pcd.deployedResource,
            this, &Plan_Handler::process_crdd));
      else
        {
          iter->previousNode();
          return;
        }
    }
}

void Plan_Handler::update_mdd_refs (Deployment::DeploymentPlan& plan)
{
  CORBA::ULong x;
  CORBA::ULong y;
  int ref_value;
  int orig_value;
  ACE_TString ref_name;

  for (x = 0; x < plan.implementation.length (); ++x)
    {
      for (y = 0; y < plan.implementation[x].artifactRef.length (); ++y)
        {
          ref_value = plan.implementation[x].artifactRef[y];
          //ACE_DEBUG ((LM_DEBUG, "ref_value in ADD is %d \n", ref_value));
          if (idref_map_.find (ref_value, ref_name) == 0)
            {
              if (id_map_.find (ref_name, orig_value) == 0)
                {
                  plan.implementation[x].artifactRef[y] = orig_value;
                }
            }
        }
    }
}

void Plan_Handler::update_idd_refs (Deployment::DeploymentPlan& plan)
{
  CORBA::ULong x;
  int ref_value;
  int orig_value;
  ACE_TString ref_name;

  for (x = 0; x < plan.instance.length (); ++x)
    {
      ref_value = plan.instance[x].implementationRef;
          //ACE_DEBUG ((LM_DEBUG, "ref_value in MDD is %d \n", ref_value));
      if (idref_map_.find (ref_value, ref_name) == 0)
        {
          if (id_map_.find (ref_name, orig_value) == 0)
            {
              plan.instance[x].implementationRef = orig_value;
            }
        }
    }
}

void Plan_Handler::update_pspe_refs (Deployment::DeploymentPlan& plan)
{
  CORBA::ULong x;
  CORBA::ULong y;
  int ref_value;
  int orig_value;
  ACE_TString ref_name;

  for (x = 0; x < plan.connection.length (); ++x)
    {
      for (y = 0; y < plan.connection[x].internalEndpoint.length (); ++y)
        {
          ref_value = plan.connection[x].internalEndpoint[y].
            instanceRef;
          //ACE_DEBUG ((LM_DEBUG, "ref_value in IDD is %d \n", ref_value));
          if (idref_map_.find (ref_value, ref_name) == 0)
            {
              if (id_map_.find (ref_name, orig_value) == 0)
                {
                  plan.connection[x].internalEndpoint[y].
                    instanceRef = orig_value;
                }
            }
        }
    }
}

void Plan_Handler::update_pspr_refs (Deployment::DeploymentPlan& plan)
{
  CORBA::ULong x;
  CORBA::ULong y;
  int ref_value;
  int orig_value;
  ACE_TString ref_name;

  for (x = 0; x < plan.externalProperty.length (); ++x)
    {
      for (y = 0; y < plan.externalProperty[x].delegatesTo.length (); ++y)
        {
          ref_value = plan.externalProperty[x].delegatesTo[y].
            instanceRef;
          if (idref_map_.find (ref_value, ref_name) == 0)
            {
              if (id_map_.find (ref_name, orig_value) == 0)
                {
                  plan.externalProperty[x].delegatesTo[y].
                    instanceRef = orig_value;
                }
            }
        }
    }
}

END_DEPLOYMENT_NAMESPACE
