//==================================================================
/**
 *  @file  IR_Handler.cpp
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef IR_HANDLER_C
#define IR_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "IAD_Handler.h"
#include "IR_Handler.h"
#include "Utils.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace CIAO
{
  namespace Config_Handler
  {
    /*
     * Class ResourceUsageKind_Handler
     */

     void ResourceUsageKind_Handler::process_ResourceUsageKind
        (DOMNodeIterator *iter, ::Deployment::ResourceUsageKind &kind)
     {
       // -- ResourceUsageKind enum
       XStr none                    ("None");
       XStr instance_uses_resource  ("InstanceUsesResource");
       XStr resource_uses_instance  ("ResourceUsesInstance");
       XStr port_uses_resource      ("PortUsesResource");
       XStr resource_uses_port      ("ResourceUsesPort");
       
       XStr kind_str = XStr (Utils::parse_string (iter));
       if (kind_str == none)
	 kind = Deployment::None;
       else if (kind_str = instance_uses_resource)
	 kind = Deployment::InstanceUsesResource;
       else if (kind_str == resource_uses_instance)
	 kind = Deployment::ResourceUsesInstance;
       else if (kind_str == port_uses_resource)
	 kind = Deployment::PortUsesResource;
       else if (kind_str == resource_uses_port)
	 kind = Deployment::ResourceUsesPort;
       
       // Something wrong here.. Throw exception
       ACE_DEBUG ((LM_DEBUG,
		   "Config_Handler::ResourceUsageKind_Handler::process_ResourceUsageKind \
               illegal <ResourceUsageKind> value found \n"));
       ACE_THROW (CORBA::INTERNAL ());
     }
 
    /*
     * Class IR_Handler
     */

    IR_Handler::IR_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    IR_Handler::IR_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    IR_Handler::~IR_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the package configuration and populate it
    void IR_Handler::process_ImplementationRequirement
      (::Deployment::ImplementationRequirement &ir)
    {
      // This is bogus and should be replaced later.
      ACE_DECLARE_NEW_CORBA_ENV;

      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
	  /*
	   *  CAUTION: These name and resource types are derived from the Requirement class,
	   *  The parser implementation duplicated here.
	   */
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              // Fetch the text node which contains the "label"
	      node = this->iter_->nextNode();
	      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_name (text->getNodeValue(), ir);
	    }
	  else if (node_name == XStr (ACE_TEXT ("resourceType")))
            {
              // Fetch the text node which contains the "resourceType"
	      node = this->iter_->nextNode();
	      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_resourceType (text->getNodeValue(), ir);
            }
	  /////////////////////////////////////////////////////////
	  else if (node_name == XStr (ACE_TEXT ("resourceUsage")))
            {
	      // increase the length of the sequence
	      CORBA::ULong i (ir.resourceUsage.length ());
	      ir.resourceUsage.length (i + 1);

	      // delegate the populating process
	      ResourceUsageKind_Handler::process_ResourceUsageKind (this->iter_,
								    ir.resourceUsage[i]);
            }
	  else if (node_name == XStr (ACE_TEXT ("resourcePort")))
            {
              // Fetch the text node which contains the "resourcePort"
	      node = this->iter_->nextNode();
	      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_resourcePort (text->getNodeValue(), ir);
            }
	  else if (node_name == XStr (ACE_TEXT ("componentPort")))
            {
              // Fetch the text node which contains the "componentPort"
	      node = this->iter_->nextNode();
	      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_componentPort (text->getNodeValue(), ir);
            }

          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle resourcePort attribute
    void IR_Handler::process_resourcePort
      (const XMLCh* resourcePort, ::Deployment::ImplementationRequirement &ir)
    {
      if (resourcePort)
        {
	  // increase the length of the sequence
	  CORBA::ULong i (ir.resourcePort.length ());
	  ir.resourcePort.length (i + 1);

          ir.resourcePort[i] = XMLString::transcode (resourcePort);
        }
    }

    /// handle componentPort attribute
    void IR_Handler::process_componentPort
      (const XMLCh* componentPort, ::Deployment::ImplementationRequirement &ir)
    {
      if (componentPort)
        {
	  // increase the length of the sequence
	  CORBA::ULong i (ir.componentPort.length ());
	  ir.componentPort.length (i + 1);

          ir.componentPort[i] = XMLString::transcode (componentPort);
        }
    }

    /*
     * Derived from Requirement
     */

    /// handle name attribute
    void IR_Handler::process_name
      (const XMLCh* name, ::Deployment::ImplementationRequirement &ir)
    {
      if (name)
        {
          ir.name = XMLString::transcode (name);
        }
    }

    /// handle resourceType attribute
    void IR_Handler::process_resourceType
      (const XMLCh* name, ::Deployment::ImplementationRequirement &ir)
    {
      if (name)
        {
          ir.resourceType = XMLString::transcode (name);
        }
    }

  }
}

#endif /* IR_HANDLER_C */
