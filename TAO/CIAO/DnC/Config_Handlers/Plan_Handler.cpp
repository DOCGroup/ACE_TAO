// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "Plan_Handler.h"
#include "CompIntrDesc_Handler.h"
#include "Property_Handler.h"
#include "SP_Handler.h"
#include "Requirement_Handler.h"
#include "Any_Handler.h"

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
          else if (node_name == XStr (ACE_TEXT ("UUID")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_uuid (text->getNodeValue(), plan);
            }
          else if (node_name == XStr (ACE_TEXT ("label")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_label (text->getNodeValue(), plan);
            }
          else if (node_name == XStr (ACE_TEXT ("realizes")))
            {
              this->process_ccd_element (node, this->doc_,
                                         this->iter_,
                                         plan);
            }
          else if (node_name == XStr (ACE_TEXT ("implementation")))
            {
              this->process_mdd_element (node, this->doc_,
                                         this->iter_,
                                         plan);
            }
          else if (node_name == XStr (ACE_TEXT ("instance")))
            {
              this->process_idd_element (node, this->doc_,
                                         this->iter_,
                                         plan);
            }
          else if (node_name == XStr (ACE_TEXT ("connection")))
            {
              this->process_pcd_element (node, this->doc_,
                                         this->iter_,
                                         plan);
            }
          else if (node_name == XStr (ACE_TEXT ("externalProperty")))
            {
              this->process_ppm_element (node, this->doc_,
                                         this->iter_,
                                         plan);
            }
          else if (node_name == XStr (ACE_TEXT ("artifact")))
            {
              this->process_add_element (node, this->doc_,
                                         this->iter_,
                                         plan);
            }
          else if (node_name == XStr (ACE_TEXT ("infoProperty")))
            {
              this->process_property_element (node, this->doc_,
                                              this->iter_,
                                              plan);
            }
          else if (node_name == XStr (ACE_TEXT ("dependsOn")))
            {
              this->process_id_element (node, this->doc_,
                                             this->iter_,
                                             plan);
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }

      return;
    }

    /// handle uuid attribute
    void Plan_Handler::process_uuid 
         (const XMLCh* uuid,
          Deployment::DeploymentPlan& plan)
    {
      if (uuid)
        {
          CORBA::String_var value (XMLString::transcode (uuid));
          plan.UUID = value.in ();
        }
    }

    /// handle label attribute
    void Plan_Handler::process_label 
       (const XMLCh* label,
        Deployment::DeploymentPlan& plan)
    {
      if (label)
        {
          CORBA::String_var value (XMLString::transcode (label));
          plan.label = value.in ();
        }
    }

    void Plan_Handler::process_id_element
       (DOMNode* node, DOMDocument*,
        DOMNodeIterator* iter,
        Deployment::DeploymentPlan& plan)
    {
      CORBA::ULong i (plan.dependsOn.length ());
      plan.dependsOn.length (i + 1);
      node = iter->nextNode();
      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
      CORBA::String_var value (XMLString::transcode 
           (text->getNodeValue()));
      plan.dependsOn[i].requiredType = value.in ();
    }

    void Plan_Handler::process_idd_element
       (DOMNode* node, DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::DeploymentPlan& plan)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();
          CORBA::ULong i (plan.instance.length ());
          plan.instance.length (i + 1);
          if (length == 1)
            {
              this->process_idd (doc, iter,
                                 plan.instance[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_idd
                   (named_node_map, doc, iter, i,
                    plan.instance[i]);
            }
        }
    }

    void Plan_Handler::process_pcd_element
       (DOMNode* node, DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::DeploymentPlan& plan)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();
          CORBA::ULong i (plan.connection.length ());
          plan.connection.length (i + 1);
          if (length == 1)
            {
              this->process_pcd (doc, iter,
                                 plan.connection[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_pcd
                   (named_node_map, doc, iter, i,
                    plan.connection[i]);
            }
        }
    }

    void Plan_Handler::process_ppm_element
       (DOMNode* node, DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::DeploymentPlan& plan)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();
          CORBA::ULong i (plan.externalProperty.length ());
          plan.externalProperty.length (i + 1);
          if (length == 1)
            {
              this->process_ppm (doc, iter,
                                 plan.externalProperty[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_ppm
                   (named_node_map, doc, iter, i,
                    plan.externalProperty[i]);
            }
        }
    }

    void Plan_Handler::process_mdd_element
       (DOMNode* node, DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::DeploymentPlan& plan)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();
          CORBA::ULong i (plan.implementation.length ());
          plan.implementation.length (i + 1);
          if (length == 1)
            {
              this->process_mdd (doc, iter,
                                 plan.implementation[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_mdd
                   (named_node_map, doc, iter, i,
                    plan.implementation[i]);
            }
        }
    }

    void Plan_Handler::process_add_element
       (DOMNode* node, DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::DeploymentPlan& plan)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();
          CORBA::ULong i (plan.artifact.length ());
          plan.artifact.length (i + 1);
          if (length == 1)
            {
              this->process_add (doc, iter,
                                 plan.artifact[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_add
                   (named_node_map, doc, iter, i,
                    plan.artifact[i]);
            }
        }

    }

    void Plan_Handler::process_ccd_element
       (DOMNode* node, DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::DeploymentPlan& plan)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();
          if (length == 1)
            {
              this->process_ccd (doc, iter,
                                 plan.realizes);
            }
          else if (length > 1)
            {
              this->process_attributes_for_ccd 
                   (named_node_map, doc, iter,
                    plan.realizes);
            }
        }
    }

    void Plan_Handler::process_property_element
       (DOMNode* node, DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::DeploymentPlan& plan)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();
          CORBA::ULong i (plan.infoProperty.length ());
          plan.infoProperty.length (i + 1);
          if (length == 1)
            {
              Property_Handler::process_Property
                 (iter, plan.infoProperty[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_property
                (named_node_map, doc,
                 iter, i, plan.infoProperty[i]);
            }
        }
    }

    void Plan_Handler::process_rdd 
       (DOMDocument*,
        DOMNodeIterator* iter,
        Deployment::ResourceDeploymentDescription& rdd)
    {
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode ())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("requirementName")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_rdd_req_name (text->getNodeValue(), rdd);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceName")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_rdd_res_name (text->getNodeValue(), rdd);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceValue")))
            {
              Any_Handler::process_Any (iter,
                                        rdd.resourceValue);
            }
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

    void Plan_Handler::process_attributes_for_irdd
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::InstanceResourceDeploymentDescription &irdd)
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
              this->process_irdd (doc, iter,
                                  irdd);
              id_map_.bind (aceattrnodevalue, value);
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
              this->process_irdd (href_doc, href_iter,
                                  irdd);
            }
        }
      return;
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
          else if (node_name == XStr (ACE_TEXT ("implementation")))
            {
              CORBA::ULong implementation_length =
                idd.implementation.length ();
              idd.implementation.length (implementation_length + 1);
              idd.implementation[implementation_length] = 0;
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  this->process_refs (named_node_map);
                }
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
       (DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::MonolithicDeploymentDescription& mdd)
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
              this->process_mdd_name (text->getNodeValue(), mdd);
            }
          else if (node_name == XStr (ACE_TEXT ("source")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_mdd_source (text->getNodeValue(), mdd);
            }
          else if (node_name == XStr (ACE_TEXT ("deployRequirement")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i 
                    (mdd.deployRequirement.length ());
                  mdd.deployRequirement.length (i + 1);
                  if (length == 1)
                    {
                      Requirement_Handler::process_Requirement 
                         (iter,
                          mdd.deployRequirement[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_deploy_requirement
                       (named_node_map, doc,
                        iter, i, mdd.deployRequirement[i]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("execParameter")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i 
                    (mdd.execParameter.length ());
                  mdd.execParameter.length (i + 1);
                  if (length == 1)
                    {
                      Property_Handler::process_Property 
                         (iter,
                          mdd.execParameter[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_property
                       (named_node_map, doc,
                        iter, i, mdd.execParameter[i]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("artifact")))
            {
              CORBA::ULong artifact_length =
                mdd.artifact.length ();
              mdd.artifact.length (artifact_length + 1);
              mdd.artifact[artifact_length] = 0;
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  this->process_refs (named_node_map);
                }
            }
          else
            {
              iter->previousNode();
              return;
            }
        }
    }

    void Plan_Handler::process_refs (DOMNamedNodeMap* named_node_map)
    {
      int length = named_node_map->getLength ();

      for (int j = 0; j < length; j++)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename (attribute_node->getNodeName ());
          ACE_TString aceattrnodevalue = XMLString::transcode
             (attribute_node->getNodeValue ());
          if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
            {
              this->index_ = this->index_ + 1;
              idref_map_.bind (this->index_, aceattrnodevalue);
            }
        }
    }

    void Plan_Handler::process_attributes_for_deployed_resource
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::ResourceDeploymentDescription& plan_res)
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
              this->process_rdd (doc, iter,
                                 plan_res);
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
              this->process_rdd (href_doc, href_iter,
                                 plan_res);
            }
        }

      return;
    }

    void Plan_Handler::process_attributes_for_deploy_requirement
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::Requirement& plan_req)
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
              Requirement_Handler::process_Requirement 
                  (iter,
                   plan_req);
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
              Requirement_Handler::process_Requirement 
                  (href_iter,
                   plan_req);
            }
        }

      return;
    }

    void Plan_Handler::process_attributes_for_add
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::ArtifactDeploymentDescription& plan_add)
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
              this->process_add (doc, iter,
                                 plan_add);
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
              this->process_add (href_doc, href_iter,
                                 plan_add);
            }
        }

      return;
    }

    void Plan_Handler::process_attributes_for_mdd
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::MonolithicDeploymentDescription& plan_mdd)
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
              this->process_mdd (doc, iter,
                                 plan_mdd);
              id_map_.bind (aceattrnodevalue, value);
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
              this->process_mdd (href_doc, href_iter,
                                 plan_mdd);
            }
        }

      return;
    }

    void Plan_Handler::process_ccd 
       (DOMDocument*,
        DOMNodeIterator* iter,
        Deployment::ComponentInterfaceDescription& cid)
    {
      CompIntrDesc_Handler handler (iter, false);
      handler.process_ComponentInterfaceDescription (cid);
    }

    void Plan_Handler::process_attributes_for_property
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::Property& plan_property)
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
              Property_Handler::process_Property (iter,
                                                  plan_property);
              id_map_.bind (aceattrnodevalue, value);
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
              Property_Handler::process_Property (href_iter,
                                                  plan_property);
            }
        }

      return;
    }

    void Plan_Handler::process_attributes_for_ccd
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::ComponentInterfaceDescription& cid)
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
              this->process_ccd (doc,
                                 iter,
                                 cid);
              id_map_.bind (aceattrnodevalue, 0);
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
              this->process_ccd (href_doc,
                                 href_iter,
                                 cid);
            }
        }

      return;
    }

    DOMDocument* Plan_Handler::create_document (const char *url)
    {

      xercesc::XMLPlatformUtils::Initialize();
      static const XMLCh gLS[] = { xercesc::chLatin_L,
                                   xercesc::chLatin_S,
                                   xercesc::chNull };

      DOMImplementation* impl
        = DOMImplementationRegistry::getDOMImplementation(gLS);

      DOMBuilder* parser =
        ((DOMImplementationLS*)impl)->createDOMBuilder
              (DOMImplementationLS::MODE_SYNCHRONOUS, 0);

      // Discard comment nodes in the document
      parser->setFeature (XMLUni::fgDOMComments, false);

      // Disable datatype normalization. The XML 1.0 attribute value
      // normalization always occurs though.
      parser->setFeature (XMLUni::fgDOMDatatypeNormalization, true);

      // Do not create EntityReference nodes in the DOM tree. No
      // EntityReference nodes will be created, only the nodes
      // corresponding to their fully expanded sustitution text will be
      // created.
      parser->setFeature (XMLUni::fgDOMEntities, false);

      // Perform Namespace processing.
      parser->setFeature (XMLUni::fgDOMNamespaces, true);

      // Perform Validation
      parser->setFeature (XMLUni::fgDOMValidation, true);

      // Do not include ignorable whitespace in the DOM tree.
      parser->setFeature (XMLUni::fgDOMWhitespaceInElementContent, false);

      // Enable the parser's schema support.
      parser->setFeature (XMLUni::fgXercesSchema, true);

      // Enable full schema constraint checking, including checking which
      // may be time-consuming or memory intensive. Currently, particle
      // unique attribution constraint checking and particle derivation
      // restriction checking are controlled by this option.
      parser->setFeature (XMLUni::fgXercesSchemaFullChecking, true);

      // The parser will treat validation error as fatal and will exit.
      parser->setFeature (XMLUni::fgXercesValidationErrorAsFatal, true);


      DOMDocument* doc = parser->parseURI (url);
      ACE_TString root_node_name;
      root_node_name = XMLString::transcode
          (doc->getDocumentElement ()->getNodeName ());

      return doc;
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
          if (node_name == XStr (ACE_TEXT ("propertyName")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_pspr_prop_name (text->getNodeValue(), pspr);
            }
          else if (node_name == XStr (ACE_TEXT ("instance")))
            {
              CORBA::ULong instance_length =
                pspr.instance.length ();
              pspr.instance.length (instance_length + 1);
              pspr.instance[instance_length] = 0;
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  this->process_refs (named_node_map);
                }
            }
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
          if (node_name == XStr (ACE_TEXT ("portName")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_pspe_port_name (text->getNodeValue(), pspe);
            }
          else if (node_name == XStr (ACE_TEXT ("provider")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_pspe_provider (text->getNodeValue(), pspe);
            }
          else if (node_name == XStr (ACE_TEXT ("instance")))
            {
              CORBA::ULong instance_length =
                pspe.instance.length ();
              pspe.instance.length (instance_length + 1);
              pspe.instance[instance_length] = 0;
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  this->process_refs (named_node_map);
                }
            }
          else
            {
              iter->previousNode();
              return;
            }
        }
    }

    void Plan_Handler::process_pspr_prop_name
      (const XMLCh* name,
       Deployment::PlanSubcomponentPropertyReference& pspr)
    {
      if (name)
        {
          pspr.propertyName = XMLString::transcode (name);
        }
    }

    void Plan_Handler::process_pspe_port_name
      (const XMLCh* name,
       Deployment::PlanSubcomponentPortEndpoint& pspe)
    {
      if (name)
        {
          pspe.portName = XMLString::transcode (name);
        }
    }

    void Plan_Handler::process_pspe_provider
      (const XMLCh* value,
       Deployment::PlanSubcomponentPortEndpoint& pspe)
    {
      if (value)
        {
          CORBA::String_var temp = XMLString::transcode (value);
          pspe.provider = ACE_static_cast (CORBA::Boolean,
                                           ACE_OS::strtol (temp.in (), 0, 10));
        }
    }

    void Plan_Handler::process_ppm 
       (DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::PlanPropertyMapping& ppm)
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
              this->process_ppm_name (text->getNodeValue(), ppm);
            }
          else if (node_name == XStr (ACE_TEXT ("source")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_ppm_source (text->getNodeValue(), ppm);
            }
          else if (node_name == XStr (ACE_TEXT ("externalName")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_ppm_ext_name (text->getNodeValue(), ppm);
            }
          else if (node_name == XStr (ACE_TEXT ("delegatesTo")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i
                    (ppm.delegatesTo.length ());
                  ppm.delegatesTo.length (i + 1);
                  if (length == 1)
                    {
                      this->process_pspr (doc, iter,
                                          ppm.delegatesTo[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_pspr
                       (named_node_map, doc,
                        iter, i, ppm.delegatesTo[i]);
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

    void Plan_Handler::process_attributes_for_pspe
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::PlanSubcomponentPortEndpoint& pspe)
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
              this->process_pspe (doc, iter,
                                  pspe);
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
              this->process_pspe (href_doc, href_iter,
                                  pspe);
            }
        }
      return;
    }

    void Plan_Handler::process_attributes_for_pspr
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::PlanSubcomponentPropertyReference& pspr)
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
              this->process_pspr (doc, iter,
                                  pspr);
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
              this->process_pspr (href_doc, href_iter,
                                  pspr);
            }
        }
      return;
    }

    void Plan_Handler::process_ppm_name
       (const XMLCh* name,
        Deployment::PlanPropertyMapping& ppm)
    {
      if (name)
        {
          ppm.name = XMLString::transcode (name);
        }
    }

    void Plan_Handler::process_ppm_ext_name
       (const XMLCh* ext_name,
        Deployment::PlanPropertyMapping& ppm)
    {
      if (ext_name)
        {
          ppm.externalName = XMLString::transcode (ext_name);
        }
    }

    void Plan_Handler::process_ppm_source
       (const XMLCh* source,
        Deployment::PlanPropertyMapping& ppm)
    {
      if (source)
        {
          CORBA::ULong i (ppm.source.length ());
          ppm.source.length (i + 1);

          ppm.source[i] = XMLString::transcode (source);
        }
    }

    void Plan_Handler::process_pcd_source
       (const XMLCh* source,
        Deployment::PlanConnectionDescription& pcd)
    {
      if (source)
        {
          CORBA::ULong i (pcd.source.length ());
          pcd.source.length (i + 1);

          pcd.source[i] = XMLString::transcode (source);
        }
    }

    void Plan_Handler::process_pcd_name
       (const XMLCh* name,
        Deployment::PlanConnectionDescription& pcd)
    {
      if (name)
        {
          pcd.name = XMLString::transcode (name);
        }
    }

    void Plan_Handler::process_attributes_for_ppm
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::PlanPropertyMapping& plan_ppm)
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
              this->process_ppm (doc, iter,
                                 plan_ppm);
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
              this->process_ppm (href_doc, href_iter,
                                 plan_ppm);
            }
        }
      return;
    }

    void Plan_Handler::process_attributes_for_crdd
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::ConnectionResourceDeploymentDescription& crdd)
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
              this->process_crdd (doc, iter,
                                 crdd);
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
              this->process_crdd (href_doc, href_iter,
                                 crdd);
            }
        }
      return;
    }

    void Plan_Handler::process_crdd 
       (DOMDocument*,
        DOMNodeIterator* iter,
        Deployment::ConnectionResourceDeploymentDescription& crdd)
    {
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode ())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("targetName")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_crdd_target_name (text->getNodeValue(), crdd);
            }
          else if (node_name == XStr (ACE_TEXT ("requirementName")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_crdd_req_name (text->getNodeValue(), crdd);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceName")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_crdd_res_name (text->getNodeValue(), crdd);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceValue")))
            {
              Any_Handler::process_Any (iter,
                                        crdd.resourceValue);
            }
          else
            {
              iter->previousNode();
              return;
            }
        }  
    }

    void Plan_Handler::process_crdd_res_name
       (const XMLCh* name,
        Deployment::ConnectionResourceDeploymentDescription& crdd)
    {
      if (name)
        {
          crdd.resourceName = XMLString::transcode (name);
        }
    }

    void Plan_Handler::process_crdd_target_name
       (const XMLCh* name,
        Deployment::ConnectionResourceDeploymentDescription& crdd)
    {
      if (name)
        {
          crdd.targetName = XMLString::transcode (name);
        }
    }

    void Plan_Handler::process_crdd_req_name
       (const XMLCh* name,
        Deployment::ConnectionResourceDeploymentDescription& crdd)
    {
      if (name)
        {
          crdd.requirementName = XMLString::transcode (name);
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
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_pcd_name (text->getNodeValue(), pcd);
            }
          else if (node_name == XStr (ACE_TEXT ("source")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_pcd_source (text->getNodeValue(), pcd);
            }
          else if (node_name == XStr (ACE_TEXT ("deployRequirement")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i 
                    (pcd.deployRequirement.length ());
                  pcd.deployRequirement.length (i + 1);
                  if (length == 1)
                    {
                      Requirement_Handler::process_Requirement 
                         (iter,
                          pcd.deployRequirement[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_deploy_requirement
                       (named_node_map, doc,
                        iter, i, pcd.deployRequirement[i]);
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

  }
}
