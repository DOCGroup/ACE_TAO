//==================================================================
/**
 *  @file  Plan_Handler.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 */
//=====================================================================
#ifndef PLAN_HANDLER_H
#define PLAN_HANDLER_H
#include /**/ "ace/pre.h"

#include "DeploymentC.h"
#include "Config_Handler_export.h"
#include "ace/SString.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include "tao/Exception.h"
#include "XercesString.h"
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLURL.hpp>
#include <xercesc/util/XMLUri.hpp>

using Config_Handler::XStr;
using xercesc::XMLUni;
using xercesc::XMLUri;
using xercesc::XMLURL;
using xercesc::XMLString;
using xercesc::XMLException;
using xercesc::DOMException;
using xercesc::DOMBuilder;
using xercesc::DOMImplementationRegistry;
using xercesc::DOMImplementationLS;
using xercesc::DOMImplementation;
using xercesc::DOMInputSource;
using xercesc::DOMText;
using xercesc::DOMNamedNodeMap;
using xercesc::DOMLocator;
using xercesc::DOMError;
using xercesc::DOMDocument;
using xercesc::DOMDocumentTraversal;
using xercesc::DOMNodeIterator;
using xercesc::DOMNode;
using xercesc::DOMNodeFilter;
using xercesc::DOMNamedNodeMap;

namespace CIAO
{
  namespace Config_Handler
  {
    /**
     * @class Plan_Handler
     *
     * @brief Handler class for <DeploymentPlan> type
     *
     */

    class Config_Handler_Export Plan_Handler
    {
    public:

      /// constructor
      Plan_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      Plan_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~Plan_Handler(void);

      /// Process the plan type
      void process_plan (Deployment::DeploymentPlan& plan);

      /// Process the label attribute
      void process_label (const XMLCh* label,
                          Deployment::DeploymentPlan& plan);
      /*
      /// process component interface description
      void process_ccd_element (DOMNode* node,
                                DOMDocument* doc,
                                DOMNodeIterator* iter,
                                Deployment::DeploymentPlan& plan);
      */
      /// process artifact deployment description
      void process_add_element (DOMNode* node,
                                DOMDocument* doc,
                                DOMNodeIterator* iter,
                                Deployment::DeploymentPlan& plan);

      /// process PlanConnectionDescription element
      void process_pcd_element (DOMNode* node,
                                DOMDocument* doc,
                                DOMNodeIterator* iter,
                                Deployment::DeploymentPlan& plan);

      /// process PlanPropertyMapping element
      void process_ppm_element (DOMNode* node,
                                DOMDocument* doc,
                                DOMNodeIterator* iter,
                                Deployment::DeploymentPlan& plan);

      /// process monolithic deployment description
      void process_mdd_element (DOMNode* node,
                                DOMDocument* doc,
                                DOMNodeIterator* iter,
                                Deployment::DeploymentPlan& plan);

      /// process instance deployment description
      void process_idd_element (DOMNode* node,
                                DOMDocument* doc,
                                DOMNodeIterator* iter,
                                Deployment::DeploymentPlan& plan);

      /// process infoProperty
      void process_property_element (DOMNode* node,
                                     DOMDocument* doc,
                                     DOMNodeIterator* iter,
                                     Deployment::DeploymentPlan& plan);

      /// process ID element
      void process_id_element (DOMNode* node,
                                    DOMDocument* doc,
                                    DOMNodeIterator* iter,
                                    Deployment::DeploymentPlan& plan);

      /// process cid
      void process_ccd (DOMNodeIterator* iter,
                        Deployment::ComponentInterfaceDescription& cid);

      /// process pspr
      void process_pspr (DOMDocument* doc,
                        DOMNodeIterator* iter,
                        Deployment::PlanSubcomponentPropertyReference& pspr);

      /// process pspe
      void process_pspe (DOMDocument* doc,
                        DOMNodeIterator* iter,
                        Deployment::PlanSubcomponentPortEndpoint& pspe);

      /// process add
      void process_add (DOMDocument* doc,
                        DOMNodeIterator* iter,
                        Deployment::ArtifactDeploymentDescription& add);

      /// process con
      void process_pcd (DOMDocument* doc,
                        DOMNodeIterator* iter,
                        Deployment::PlanConnectionDescription& con);

      /// process external property
      void process_ppm (DOMDocument* doc,
                        DOMNodeIterator* iter,
                        Deployment::PlanPropertyMapping& ppm);

      /// process idd
      void process_idd (DOMDocument* doc,
                        DOMNodeIterator* iter,
                        Deployment::InstanceDeploymentDescription& idd);

      /// process mdd
      void process_mdd (DOMDocument* doc,
                        DOMNodeIterator* iter,
                        Deployment::MonolithicDeploymentDescription& mdd);

      /// process rdd
      void process_rdd (DOMDocument* doc,
                        DOMNodeIterator* iter,
                        Deployment::ResourceDeploymentDescription& rdd);

      /// process crdd
      void process_crdd (DOMDocument* doc,
                        DOMNodeIterator* iter,
                        Deployment::ConnectionResourceDeploymentDescription& crdd);
      /// process irdd
      void process_irdd (DOMDocument* doc,
                        DOMNodeIterator* iter,
                        Deployment::InstanceResourceDeploymentDescription &irdd);

      /// process the add element with HREF attribute
      void process_attributes_for_add (DOMNamedNodeMap* named_node_map,
                                       DOMDocument* doc,
                                       DOMNodeIterator* iter,
                                       int value,
                                       Deployment::ArtifactDeploymentDescription& add);

      /// process the crdd element with HREF attribute
      void process_attributes_for_crdd (DOMNamedNodeMap* named_node_map,
                                       DOMDocument* doc,
                                       DOMNodeIterator* iter,
                                       int value,
                                       Deployment::ConnectionResourceDeploymentDescription& crdd);

      /// process the irdd element with HREF attribute
      void process_attributes_for_irdd (DOMNamedNodeMap* named_node_map,
                                       DOMDocument* doc,
                                       DOMNodeIterator* iter,
                                       int value,
                                       Deployment::InstanceResourceDeploymentDescription& irdd);

      /// process the con element with HREF attribute
      void process_attributes_for_pcd (DOMNamedNodeMap* named_node_map,
                                       DOMDocument* doc,
                                       DOMNodeIterator* iter,
                                       int value,
                                       Deployment::PlanConnectionDescription& con);

      /// process the ppm element with HREF attribute
      void process_attributes_for_ppm (DOMNamedNodeMap* named_node_map,
                                       DOMDocument* doc,
                                       DOMNodeIterator* iter,
                                       int value,
                                       Deployment::PlanPropertyMapping& ppm);

      /// process the idd element with HREF attribute
      void process_attributes_for_idd (DOMNamedNodeMap* named_node_map,
                                       DOMDocument* doc,
                                       DOMNodeIterator* iter,
                                       int value,
                                       Deployment::InstanceDeploymentDescription& idd);

      /// process the mdd element with HREF attribute
      void process_attributes_for_mdd (DOMNamedNodeMap* named_node_map,
                                       DOMDocument* doc,
                                       DOMNodeIterator* iter,
                                       int value,
                                       Deployment::MonolithicDeploymentDescription& mdd);
      /*
      /// process the cid element with HREF attribute
      void process_attributes_for_ccd (DOMNamedNodeMap* named_node_map,
                                       DOMDocument* doc,
                                       DOMNodeIterator* iter,
                                       Deployment::ComponentInterfaceDescription& cid);
      */

      /// process the cid element with HREF attribute
      void process_attributes_for_property (DOMNamedNodeMap* named_node_map,
                                            DOMDocument* doc,
                                            DOMNodeIterator* iter,
                                            int value,
                                            Deployment::Property& pro);

      /// process the pspr element with HREF attribute
      void process_attributes_for_pspr (DOMNamedNodeMap* named_node_map,
                                            DOMDocument* doc,
                                            DOMNodeIterator* iter,
                                            int value,
                                            Deployment::PlanSubcomponentPropertyReference& pspr);

      /// process the pspe element with HREF attribute
      void process_attributes_for_pspe (DOMNamedNodeMap* named_node_map,
                                            DOMDocument* doc,
                                            DOMNodeIterator* iter,
                                            int value,
                                            Deployment::PlanSubcomponentPortEndpoint& pspe);

      /// process the req element with HREF attribute
      void process_attributes_for_deploy_requirement (DOMNamedNodeMap* named_node_map,
                                            DOMDocument* doc,
                                            DOMNodeIterator* iter,
                                            int value,
                                            Deployment::Requirement& req);

      /// process the res element with HREF attribute
      void process_attributes_for_deployed_resource (DOMNamedNodeMap* named_node_map,
                                            DOMDocument* doc,
                                            DOMNodeIterator* iter,
                                            int value,
                                            Deployment::ResourceDeploymentDescription& rdd);

      /// create a document
      DOMDocument* create_document (const char *url);

      /// process the add name
      void process_add_name (const XMLCh* name,
                             Deployment::ArtifactDeploymentDescription& add);

      /// process the pspr property name
      void process_pspr_prop_name (const XMLCh* name,
                             Deployment::PlanSubcomponentPropertyReference& pspr);

      /// process the pspe port name
      void process_pspe_port_name (const XMLCh* name,
                             Deployment::PlanSubcomponentPortEndpoint& pspe);

      /// process the pspe provider
      void process_pspe_provider (const XMLCh* value,
                             Deployment::PlanSubcomponentPortEndpoint& pspe);

      /// process the idd name
      void process_idd_name (const XMLCh* name,
                             Deployment::InstanceDeploymentDescription& idd);

      /// process the ppm  name
      void process_ppm_name (const XMLCh* name,
                             Deployment::PlanPropertyMapping& ppm);

      /// process the ppm external name
      void process_ppm_ext_name (const XMLCh* name,
                             Deployment::PlanPropertyMapping& ppm);

      /// process the idd node
      void process_idd_node (const XMLCh* node,
                             Deployment::InstanceDeploymentDescription& idd);

      /// process the idd source
      void process_idd_source (const XMLCh* source,
                             Deployment::InstanceDeploymentDescription& idd);

      /// process the add location
      void process_add_location (const XMLCh* location,
                                 Deployment::ArtifactDeploymentDescription& add);

      /// process the add source
      void process_add_source (const XMLCh* source,
                             Deployment::ArtifactDeploymentDescription& add);

      /// process the add node
      void process_add_node (const XMLCh* node,
                             Deployment::ArtifactDeploymentDescription& add);

      /// process the mdd source
      void process_mdd_source (const XMLCh* source,
                             Deployment::MonolithicDeploymentDescription& mdd);

      /// process the ppm source
      void process_ppm_source (const XMLCh* source,
                             Deployment::PlanPropertyMapping& ppm);

      /// process the pcd source
      void process_pcd_source (const XMLCh* source,
                             Deployment::PlanConnectionDescription& pcd);

      /// process the pcd name
      void process_pcd_name (const XMLCh* name,
                             Deployment::PlanConnectionDescription& pcd);

      /// process the add node
      void process_mdd_name (const XMLCh* name,
                             Deployment::MonolithicDeploymentDescription& mdd);

      /// process the rdd req name
      void process_rdd_req_name (const XMLCh* name,
                             Deployment::ResourceDeploymentDescription& rdd);

      /// process the rdd res name
      void process_rdd_res_name (const XMLCh* name,
                             Deployment::ResourceDeploymentDescription& rdd);

      /// process the irdd res name
      void process_irdd_res_name (const XMLCh* name,
                             Deployment::InstanceResourceDeploymentDescription &irdd);

      /// process the crdd res name
      void process_crdd_res_name (const XMLCh* name,
                             Deployment::ConnectionResourceDeploymentDescription& crdd);

      /// process the crdd req name
      void process_crdd_req_name (const XMLCh* name,
                             Deployment::ConnectionResourceDeploymentDescription& crdd);

      /// process the crdd target name
      void process_crdd_target_name (const XMLCh* name,
                             Deployment::ConnectionResourceDeploymentDescription& crdd);

      /// process the irdd req name
      void process_irdd_req_name (const XMLCh* name,
                             Deployment::InstanceResourceDeploymentDescription &irdd);

      /// process the irdd res usage
      void process_irdd_res_usage (const XMLCh* name,
                             Deployment::InstanceResourceDeploymentDescription &irdd);

      /// process references
      void process_refs (DOMNamedNodeMap* named_node_map);

      /// update mdd refs
      void update_mdd_refs (Deployment::DeploymentPlan& plan);

      /// update idd refs
      void update_idd_refs (Deployment::DeploymentPlan& plan);

      /// update pspe refs
      void update_pspe_refs (Deployment::DeploymentPlan& plan);

      /// update pspr refs
      void update_pspr_refs (Deployment::DeploymentPlan& plan);

    private:

      typedef ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex> REF_MAP;
      typedef ACE_Hash_Map_Iterator<ACE_TString, int, ACE_Null_Mutex> REF_ITER;
      typedef ACE_Hash_Map_Manager<int, ACE_TString, ACE_Null_Mutex> IDREF_MAP;

      DOMDocumentTraversal* traverse_;

      DOMDocument* doc_;

      DOMNode* root_;

      unsigned long filter_;

      DOMNodeIterator* iter_;

      bool release_;
      int index_;

      REF_MAP id_map_;
      IDREF_MAP idref_map_;

    };
  };
};

#include /**/ "ace/post.h"

#endif /* PLAN_HANDLER_H */
