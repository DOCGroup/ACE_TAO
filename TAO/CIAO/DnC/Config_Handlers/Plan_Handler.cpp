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

#include "Process_Basic_Type.h"
#include "Process_Element.h"
#include "Utils.h"

#include <iostream>
#include "string.h"

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    Plan_Handler::Plan_Handler (DOMDocument* doc, unsigned long filter)
      : traverse_ (doc),
        doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (traverse_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true),
        index_ (0)
    {}

    Plan_Handler::Plan_Handler (DOMNodeIterator* iter, bool release)
      : traverse_ (0), doc_ (0), root_ (0), filter_ (0), iter_ (iter),
        release_ (release)
    {}

    Plan_Handler::~Plan_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the DeploymentPlan type and populate the IDL structure
    void Plan_Handler::process_plan
         (Deployment::DeploymentPlan& plan)
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
            (process_string(this->iter_, node_name, "label", plan.label));
          else if
            (process_element<Deployment::ComponentInterfaceDescription>
             (this->doc_, this->iter_, node,
              node_name, "realizes", plan.realizes,
              this, &Plan_Handler::process_ccd,
              this->id_map_));
          else if
            (process_sequence<Deployment::MonolithicDeploymentDescription>
             (this->doc_, this->iter_, node,
              node_name, "implementation", plan.implementation,
              this, &Plan_Handler::process_mdd,
              this->id_map_));
          else if
            (process_sequence<Deployment::InstanceDeploymentDescription>
             (this->doc_, this->iter_, node,
              node_name, "instance", plan.instance,
              this, &Plan_Handler::process_idd,
              this->id_map_));
          else if
            (process_sequence<Deployment::PlanConnectionDescription>
             (this->doc_, this->iter_, node,
              node_name, "connection", plan.connection,
              this, &Plan_Handler::process_pcd,
              this->id_map_));
          else if
            (process_sequence<Deployment::PlanPropertyMapping>
             (this->doc_, this->iter_, node,
              node_name, "externalProperty", plan.externalProperty,
              this, &Plan_Handler::process_ppm,
              this->id_map_));
          else if
            (process_sequence<Deployment::ImplementationDependency>
             (this->doc_, this->iter_, node,
              node_name, "dependsOn", plan.dependsOn,
              &ID_Handler::process_ImplementationDependency, this->id_map_));
          else if
            (process_sequence<Deployment::ArtifactDeploymentDescription>
             (this->doc_, this->iter_, node,
              node_name, "artifact", plan.artifact,
              this, &Plan_Handler::process_add,
              this->id_map_));
          else if
            (process_sequence<Deployment::Property>
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

    void Plan_Handler::process_rdd
       (DOMNodeIterator* iter,
        Deployment::ResourceDeploymentDescription& rdd)
    {
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode ())
        {
          XStr node_name (node->getNodeName());

          if (false);
          else if
            (process_string(iter, node_name, "requirementName", rdd.requirementName));
          else if
            (process_string(iter, node_name, "resourceName", rdd.resourceName));
          else if (node_name == XStr (ACE_TEXT ("resourceValue")))
              Any_Handler::process_Any (iter, rdd.resourceValue);
          else
            {
              iter->previousNode();
              return;
            }
        }
    }

    void Plan_Handler::process_irdd (DOMNodeIterator* iter,
                                     Deployment::InstanceResourceDeploymentDescription &irdd)
    {
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode ())
        {
          XStr node_name (node->getNodeName());

          if (false);
          else if (node_name == "resourceUsage")
            RUK_Handler::process_ResourceUsageKind (iter, irdd.resourceUsage);
          else if
            (process_string (iter, node_name, "requirementName", irdd.requirementName));
          else if
            (process_string (iter, node_name, "resourceName", irdd.resourceName));
          else if (node_name == XStr (ACE_TEXT ("resourceValue")))
              Any_Handler::process_Any (iter, irdd.resourceValue);
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

          if (false);
          else if
            (process_string (iter, node_name, "name", add.name));
          else if
            (process_string_seq (iter, node_name, "location", add.location));
          else if
            (process_string (iter, node_name, "node", add.node));
          else if
            (process_string_seq (iter, node_name, "source", add.source));
          else if
            (process_sequence<Deployment::Property>(node->getOwnerDocument(), iter, node,
                                                    node_name, "execParameter", add.execParameter,
                                                    &Property_Handler::process_Property,
                                                    this->id_map_));
          else if
            (process_sequence<Deployment::Requirement>(node->getOwnerDocument(), iter, node,
                                                       node_name, "deployRequirement", add.deployRequirement,
                                                       &Requirement_Handler::process_Requirement,
                                                       this->id_map_));
          else if
            (process_sequence<Deployment::ResourceDeploymentDescription>
             (node->getOwnerDocument(), iter, node,
              node_name, "deployedResource", add.deployedResource,
              this, &Plan_Handler::process_rdd,
              this->id_map_));
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

          if (false);
          else if
            (process_string (iter, node_name, "name", idd.name));
          else if
            (process_string (iter, node_name, "node", idd.node));
          else if
            (process_string_seq (iter, node_name, "source", idd.source));
          else if
            (process_reference (node, node_name, "implementation",
                                idd.implementationRef, 
                                this->index_, this->idref_map_));
          else if
            (process_sequence<Deployment::Property>(node->getOwnerDocument(), iter, node,
                                                    node_name, "configProperty", idd.configProperty,
                                                    &Property_Handler::process_Property,
                                                    this->id_map_));
          else if
            (process_sequence<Deployment::InstanceResourceDeploymentDescription>
             (node->getOwnerDocument(), iter, node,
              node_name, "deployedResource", idd.deployedResource,
              this, &Plan_Handler::process_irdd,
              this->id_map_));
          else if
            (process_sequence<Deployment::InstanceResourceDeploymentDescription>
             (node->getOwnerDocument(), iter, node,
              node_name, "deployedSharedResource", idd.deployedSharedResource,
              this, &Plan_Handler::process_irdd,
              this->id_map_));
          else
            {
              iter->previousNode();
              return;
            }
        }
    }

    void Plan_Handler::process_mdd
       (DOMNodeIterator* iter,
        Deployment::MonolithicDeploymentDescription& mdd)
    {
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode ())
        {
          XStr node_name (node->getNodeName());

          if (false);
          else if
            (process_string(iter, node_name, "name", mdd.name));
          else if
            (process_string_seq(iter, node_name, "source", mdd.source));
          else if
            (process_reference_seq(node, node_name, "artifact",
                                   mdd.artifactRef,
                                   this->index_, this->idref_map_));
          else if
            (process_sequence<Deployment::Property>(node->getOwnerDocument(), this->iter_, node,
                                                    node_name, "execParameter", mdd.execParameter,
                                                    &Property_Handler::process_Property,
                                                    this->id_map_));
          else if
            (process_sequence<Deployment::Requirement>(node->getOwnerDocument(), this->iter_, node,
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

    void Plan_Handler::process_ccd
       (DOMNodeIterator* iter,
        Deployment::ComponentInterfaceDescription& cid)
    {
      CCD_Handler handler (iter, false);
      handler.process_ComponentInterfaceDescription (cid);
    }

    void Plan_Handler::process_pspr (DOMNodeIterator* iter,
                                     Deployment::PlanSubcomponentPropertyReference& pspr)
    {
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode ())
        {
          XStr node_name (node->getNodeName());

          if (false);
          else if
            (process_string(iter, node_name, "propertyName", pspr.propertyName));
          else if
            (process_reference(node, node_name, "instance", pspr.instanceRef, this->index_, this->idref_map_));
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

          if (false);
          else if
            (process_string(iter, node_name, "portName", pspe.portName));
          else if
            (process_boolean(iter, node_name, "provider", pspe.provider));
          else if (node_name == "kind")
            process_CPK (iter, pspe.kind);
          else if
            (process_reference(node, node_name, "instance",
                               pspe.instanceRef,
                               this->index_, this->idref_map_));
          else
            {
              iter->previousNode();
              return;
            }
        }
    }

    void Plan_Handler::process_CPK (DOMNodeIterator* iter,
                                    Deployment::CCMComponentPortKind& kind)
    {
      XStr facet ("Facet");
      XStr simplex_receptacle ("SimplexReceptacle");
      XStr multiplex_receptacle ("MultiplexReceptacle");
      XStr event_emitter ("EventEmitter");
      XStr event_publisher ("EventPublisher");
      XStr event_consumer ("EventConsumer");

      XStr kind_str = XStr (Utils::parse_string (iter));

      if (kind_str == facet)
        kind = Deployment::Facet;
      if (kind_str == simplex_receptacle)
        kind = Deployment::SimplexReceptacle;
      if (kind_str == multiplex_receptacle)
        kind = Deployment::MultiplexReceptacle;
      if (kind_str == event_emitter)
        kind = Deployment::EventEmitter;
      if (kind_str == event_publisher)
        kind = Deployment::EventPublisher;
      if (kind_str == event_consumer)
        kind = Deployment::EventConsumer;

      // Something wrong here.. Throw exception
      ACE_DEBUG ((LM_DEBUG,
                  "Config_Handler::Plan_Handler::process_CPK illegal <CCMComponentPortKind> value found \n"));
      ACE_THROW (CORBA::INTERNAL ());
    }
    
    void Plan_Handler::process_ppm(DOMNodeIterator* iter,
                                   Deployment::PlanPropertyMapping& ppm)
    {
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode ())
        {
          XStr node_name (node->getNodeName());

          if (false);
          else if
            (process_string(iter, node_name, "name", ppm.name));
          else if
            (process_string_seq(iter, node_name, "source", ppm.source));
          else if
            (process_string(iter, node_name, "externalName", ppm.externalName));
          else if
            (process_sequence<Deployment::PlanSubcomponentPropertyReference>
             (node->getOwnerDocument(), iter, node,
              node_name, "delegatesTo", ppm.delegatesTo,
              this, &Plan_Handler::process_pspr,
              this->id_map_));
          else
            {
              iter->previousNode();
              return;
            }
        }
    }

    void Plan_Handler::process_crdd (DOMNodeIterator* iter,
                                     Deployment::ConnectionResourceDeploymentDescription& crdd)
    {
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode ())
        {
          XStr node_name (node->getNodeName());

          if (false);
          else if
            (process_string(iter, node_name, "targetName", crdd.targetName));
          else if
            (process_string(iter, node_name, "requirementName", crdd.requirementName));
          else if
            (process_string(iter, node_name, "resourceName", crdd.resourceName));
          else if (node_name == XStr (ACE_TEXT ("resourceValue")))
              Any_Handler::process_Any (iter, crdd.resourceValue);
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

          if (false);
          else if
            (process_string(iter, node_name, "name", pcd.name));
          else if
            (process_string_seq(iter, node_name, "source", pcd.source));
          else if
            (process_sequence<Deployment::Requirement>(node->getOwnerDocument(), this->iter_, node,
                                                       node_name, "deployRequirement", pcd.deployRequirement,
                                                       &Requirement_Handler::process_Requirement,
                                                       this->id_map_));
          else if
            (process_sequence<Deployment::ComponentExternalPortEndpoint>
             (node->getOwnerDocument(), iter, node,
              node_name, "externalEndpoint", pcd.externalEndpoint,
              &CEPE_Handler::process_ComponentExternalPortEndpoint,
              this->id_map_));
          else if
            (process_sequence<Deployment::PlanSubcomponentPortEndpoint>
             (node->getOwnerDocument(), iter, node,
              node_name, "internalEndpoint", pcd.internalEndpoint,
              this, &Plan_Handler::process_pspe,
              this->id_map_));
          else if
            (process_sequence<Deployment::ExternalReferenceEndpoint>
             (node->getOwnerDocument(), iter, node,
              node_name, "externalReference", pcd.externalReference,
              &ERE_Handler::process_ExternalReferenceEndpoint,
              this->id_map_));
          else if
            (process_sequence<Deployment::ConnectionResourceDeploymentDescription>
             (node->getOwnerDocument(), iter, node,
              node_name, "deployedResource", pcd.deployedResource,
              this, &Plan_Handler::process_crdd,
              this->id_map_));
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

  }
}
