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

#include "Process_Basic_Type.h"
#include "Process_Element.h"

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
            (process_string(this->iter_, node_name, "requirementName", rdd.requirementName));
          else if
            (process_string(this->iter_, node_name, "resourceName", rdd.resourceName));
          else if (node_name == XStr (ACE_TEXT ("resourceValue")))
              Any_Handler::process_Any (iter, rdd.resourceValue);
          else
            {
              iter->previousNode();
              return;
            }
        }
    }

    void Plan_Handler::process_irdd
       (DOMDocument*,
        DOMNodeIterator* iter,
        Deployment::InstanceResourceDeploymentDescription &irdd)
    {
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode ())
        {
          XStr node_name (node->getNodeName());

          // TODO:
          if (node_name == XStr (ACE_TEXT ("resourceUsage")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_irdd_res_usage (text->getNodeValue(), irdd);
            }
          else if (node_name == XStr (ACE_TEXT ("requirementName")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_irdd_req_name (text->getNodeValue(), irdd);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceName")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_irdd_res_name (text->getNodeValue(), irdd);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceValue")))
            {
              Any_Handler::process_Any (iter,
                                        irdd.resourceValue);
            }
          else
            {
              iter->previousNode();
              return;
            }
        }
    }

    void Plan_Handler::process_add
       (DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::ArtifactDeploymentDescription& add)
    {
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode ())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_add_name (text->getNodeValue(), add);
            }
          else if (node_name == XStr (ACE_TEXT ("location")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_add_location (text->getNodeValue(), add);
            }
          else if (node_name == XStr (ACE_TEXT ("node")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_add_node (text->getNodeValue(), add);
            }
          else if (node_name == XStr (ACE_TEXT ("source")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_add_source (text->getNodeValue(), add);
            }
          else if (node_name == XStr (ACE_TEXT ("execParameter")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i
                    (add.execParameter.length ());
                  add.execParameter.length (i + 1);
                  if (length == 1)
                    {
                      Property_Handler::process_Property
                         (iter,
                          add.execParameter[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_property
                       (named_node_map, doc,
                        iter, i, add.execParameter[i]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("deployRequirement")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i
                    (add.deployRequirement.length ());
                  add.deployRequirement.length (i + 1);
                  if (length == 1)
                    {
                      Requirement_Handler::process_Requirement
                         (iter,
                          add.deployRequirement[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_deploy_requirement
                       (named_node_map, doc,
                        iter, i, add.deployRequirement[i]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("deployedResource")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i
                    (add.deployedResource.length ());
                  add.deployedResource.length (i + 1);
                  if (length == 1)
                    {
                      this->process_rdd (doc, iter,
                                         add.deployedResource[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_deployed_resource
                       (named_node_map, doc,
                        iter, i, add.deployedResource[i]);
                    }
                }
            }
          else
            {
              iter->previousNode();
              return;
            }
        }
    }

    void Plan_Handler::process_rdd_req_name
       (const XMLCh* name,
        Deployment::ResourceDeploymentDescription& rdd)
    {
      if (name)
        {
          rdd.requirementName = XMLString::transcode (name);
        }
    }

    void Plan_Handler::process_irdd_req_name
       (const XMLCh* name,
        Deployment::InstanceResourceDeploymentDescription &irdd)
    {
      if (name)
        {
          irdd.requirementName = XMLString::transcode (name);
        }
    }

    void Plan_Handler::process_irdd_res_name
       (const XMLCh* name,
        Deployment::InstanceResourceDeploymentDescription &irdd)
    {
      if (name)
        {
          irdd.resourceName = XMLString::transcode (name);
        }
    }

    void Plan_Handler::process_irdd_res_usage
       (const XMLCh* val,
        Deployment::InstanceResourceDeploymentDescription &irdd)
    {
      if (val)
        {
          CORBA::String_var temp = XMLString::transcode (val);
          CORBA::Short sval = ACE_static_cast (CORBA::Short,
                                               ACE_OS::strtol
                                                 (temp.in (),
                                                  0, 10));

          irdd.resourceUsage =
            ACE_static_cast (::Deployment::ResourceUsageKind,
                             sval);
        }
    }

    void Plan_Handler::process_rdd_res_name
       (const XMLCh* name,
        Deployment::ResourceDeploymentDescription& rdd)
    {
      if (name)
        {
          rdd.resourceName = XMLString::transcode (name);
        }
    }

    void Plan_Handler::process_add_name
       (const XMLCh* name,
        Deployment::ArtifactDeploymentDescription& add)
    {
      if (name)
        {
          add.name = XMLString::transcode (name);
        }
    }

    void Plan_Handler::process_idd_name
       (const XMLCh* name,
        Deployment::InstanceDeploymentDescription& idd)
    {
      if (name)
        {
          idd.name = XMLString::transcode (name);
        }
    }

    void Plan_Handler::process_idd_node
       (const XMLCh* node,
        Deployment::InstanceDeploymentDescription& idd)
    {
      if (node)
        {
          idd.node = XMLString::transcode (node);
        }
    }

    void Plan_Handler::process_idd_source
       (const XMLCh* source,
        Deployment::InstanceDeploymentDescription& idd)
    {
      if (source)
        {
          CORBA::ULong i (idd.source.length ());
          idd.source.length (i + 1);

          idd.source[i] = XMLString::transcode (source);
        }
    }

    void Plan_Handler::process_add_location
       (const XMLCh* location,
        Deployment::ArtifactDeploymentDescription& add)
    {
      if (location)
        {
          CORBA::ULong i (add.location.length ());
          add.location.length (i + 1);

          add.location[i] = XMLString::transcode (location);
        }
    }

    void Plan_Handler::process_add_node
       (const XMLCh* node,
        Deployment::ArtifactDeploymentDescription& add)
    {
      if (node)
        {
          add.node = XMLString::transcode (node);
        }
    }

    void Plan_Handler::process_add_source
       (const XMLCh* source,
        Deployment::ArtifactDeploymentDescription& add)
    {
      if (source)
        {
          CORBA::ULong i (add.source.length ());
          add.source.length (i + 1);

          add.source[i] = XMLString::transcode (source);
        }
    }

    void Plan_Handler::process_mdd_source
       (const XMLCh* source,
        Deployment::MonolithicDeploymentDescription& mdd)
    {
      if (source)
        {
          CORBA::ULong i (mdd.source.length ());
          mdd.source.length (i + 1);

          mdd.source[i] = XMLString::transcode (source);
        }
    }

    void Plan_Handler::process_mdd_name
       (const XMLCh* name,
        Deployment::MonolithicDeploymentDescription& mdd)
    {
      if (name)
        {
          mdd.name = XMLString::transcode (name);
        }
    }

    void Plan_Handler::process_idd
       (DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::InstanceDeploymentDescription& idd)
    {
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode ())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_idd_name (text->getNodeValue(), idd);
            }
          else if (node_name == XStr (ACE_TEXT ("node")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_idd_node (text->getNodeValue(), idd);
            }
          else if (node_name == XStr (ACE_TEXT ("source")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_idd_source (text->getNodeValue(), idd);
            }
          else if (node_name == XStr (ACE_TEXT ("configProperty")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i
                    (idd.configProperty.length ());
                  idd.configProperty.length (i + 1);
                  if (length == 1)
                    {
                      Property_Handler::process_Property
                         (iter,
                          idd.configProperty[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_property
                       (named_node_map, doc,
                        iter, i, idd.configProperty[i]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("implementationRef")))
            {
/*              CORBA::ULong implementation_length =
                idd.implementationRef.length ();
              idd.implementationRef.length (implementation_length + 1);
              idd.implementationRef[implementation_length] = 0;
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  this->process_refs (named_node_map);
                }
*/
            }
          else if (node_name == XStr (ACE_TEXT ("deployedResource")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i (idd.deployedResource.length ());
                  idd.deployedResource.length (i + 1);
                  if (length == 1)
                    {
                      this->process_irdd (doc, iter,
                                          idd.deployedResource[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_irdd
                       (named_node_map, doc,
                        iter, i, idd.deployedResource[i]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("deployedSharedResource")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i (idd.deployedSharedResource.length ());
                  idd.deployedSharedResource.length (i + 1);
                  if (length == 1)
                    {
                      this->process_irdd (doc, iter,
                                          idd.deployedSharedResource[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_irdd
                       (named_node_map, doc,
                        iter, i, idd.deployedSharedResource[i]);
                    }
                }
            }
          else
            {
              iter->previousNode();
              return;
            }
        }
    }

    void Plan_Handler::process_attributes_for_idd
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::InstanceDeploymentDescription& plan_idd)
    {
      int length = named_node_map->getLength ();

      for (int j = 0; j < length; j++)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename
             (attribute_node->getNodeName ());
          ACE_TString aceattrnodevalue =  XMLString::transcode
             (attribute_node->getNodeValue ());

          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
            {
              id_map_.bind (aceattrnodevalue, value);
              this->process_idd (doc, iter,
                                 plan_idd);
            }
          else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
            {
              this->process_refs (named_node_map);
            }
          else if (strattrnodename == XStr (ACE_TEXT ("href")))
            {
              XMLURL xml_url (aceattrnodevalue.c_str ());
              XMLURL result (aceattrnodevalue.c_str ());
              std::string url_string = aceattrnodevalue.c_str ();
              ACE_TString doc_path =
               XMLString::transcode ( doc->getDocumentURI ());
              result.makeRelativeTo
                 (XMLString::transcode (doc_path.c_str ()));
              ACE_TString final_url =
               XMLString::transcode (result.getURLText ());

              DOMDocument* href_doc;

              if (xml_url.isRelative ())
                {
                  href_doc = this->create_document
                       (final_url.c_str ());
                }
              else
                {
                  href_doc = this->create_document
                       (url_string.c_str ());
                }

              DOMDocumentTraversal* traverse (href_doc);
              DOMNode* root = (href_doc->getDocumentElement ());
              unsigned long filter = DOMNodeFilter::SHOW_ELEMENT |
                                     DOMNodeFilter::SHOW_TEXT;
              DOMNodeIterator* href_iter = traverse->createNodeIterator
                                              (root,
                                               filter,
                                               0,
                                               true);
              href_iter->nextNode ();
              this->process_idd (href_doc, href_iter,
                                 plan_idd);
            }
        }

      return;
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
            (process_reference(node, node_name, "artifact",
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

    void Plan_Handler::process_pspr
       (DOMDocument*,
        DOMNodeIterator* iter,
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
            (process_ulong(iter, node_name, "instance", pspr.instance));
          else
            {
              iter->previousNode();
              return;
            }
        }
    }

    void Plan_Handler::process_pspe
       (DOMDocument*,
        DOMNodeIterator* iter,
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

    void Plan_Handler::process_pcd
       (DOMDocument* doc,
        DOMNodeIterator* iter,
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
            (process_sequence<Deployment::ComponentExternalEndpoint>
             (this->doc_, this->iter_, node,
              node_name, "externalEndpoint", pcd.externalEndpoint,
              this, &Domain_Handler::process_cepe,
              this->id_map_));
          else if
            (process_sequence<Deployment::PlanSubcomponentPortEndpoint>
             (this->doc_, this->iter_, node,
              node_name, "internalEndpoint", pcd.externalEndpoint,
              this, &Domain_Handler::process_cepe,
              this->id_map_));



          else if (node_name == XStr (ACE_TEXT ("deployedResource")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i
                    (pcd.deployedResource.length ());
                  pcd.deployedResource.length (i + 1);
                  if (length == 1)
                    {
                      this->process_crdd (doc, iter,
                                          pcd.deployedResource[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_crdd
                       (named_node_map, doc,
                        iter, i, pcd.deployedResource[i]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("internalEndPoint")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i
                    (pcd.internalEndpoint.length ());
                  pcd.internalEndpoint.length (i + 1);
                  if (length == 1)
                    {
                      this->process_pspe (doc, iter,
                                          pcd.internalEndpoint[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_pspe
                       (named_node_map, doc,
                        iter, i, pcd.internalEndpoint[i]);
                    }
                }
            }
          else
            {
              iter->previousNode();
              return;
            }
        }
    }

    void Plan_Handler::process_attributes_for_pcd
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::PlanConnectionDescription& plan_con)
    {
      int length = named_node_map->getLength ();

      for (int j = 0; j < length; j++)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename
             (attribute_node->getNodeName ());
          ACE_TString aceattrnodevalue =  XMLString::transcode
             (attribute_node->getNodeValue ());

          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
            {
              id_map_.bind (aceattrnodevalue, value);
              this->process_pcd (doc, iter,
                                 plan_con);
            }
          else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
            {
              this->process_refs (named_node_map);
            }
          else if (strattrnodename == XStr (ACE_TEXT ("href")))
            {
              XMLURL xml_url (aceattrnodevalue.c_str ());
              XMLURL result (aceattrnodevalue.c_str ());
              std::string url_string = aceattrnodevalue.c_str ();
              ACE_TString doc_path =
               XMLString::transcode ( doc->getDocumentURI ());
              result.makeRelativeTo
                 (XMLString::transcode (doc_path.c_str ()));
              ACE_TString final_url =
               XMLString::transcode (result.getURLText ());

              DOMDocument* href_doc;

              if (xml_url.isRelative ())
                {
                  href_doc = this->create_document
                       (final_url.c_str ());
                }
              else
                {
                  href_doc = this->create_document
                       (url_string.c_str ());
                }

              DOMDocumentTraversal* traverse (href_doc);
              DOMNode* root = (href_doc->getDocumentElement ());
              unsigned long filter = DOMNodeFilter::SHOW_ELEMENT |
                                     DOMNodeFilter::SHOW_TEXT;
              DOMNodeIterator* href_iter = traverse->createNodeIterator
                                              (root,
                                               filter,
                                               0,
                                               true);
              href_iter->nextNode ();
              this->process_pcd (href_doc, href_iter,
                                 plan_con);
            }
        }
      return;
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
