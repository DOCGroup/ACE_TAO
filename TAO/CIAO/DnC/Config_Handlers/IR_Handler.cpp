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
        (DOMNodeIterator *iter, ::Deployment::ResourceUsageKind &ruq)
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
     * Class Req_Handler
     */

    Req_Handler::Req_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    Req_Handler::Req_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    Req_Handler::~Req_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the package configuration and populate it
    void Req_Handler::process_Requirement
      (::Deployment::Requirement &req)
    {
      // This is bogus and should be replaced later.
      ACE_DECLARE_NEW_CORBA_ENV;

      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              // Fetch the text node which contains the "label"
	      node = this->iter_->nextNode();
	      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_name (text->getNodeValue(), req);
	    }
	  else if (node_name == XStr (ACE_TEXT ("resourceType")))
            {
	      // TODO: How to implement this?
	      // Sequence of string ???
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle name attribute
    void Req_Handler::process_name
      (const XMLCh* name, ::Deployment::Requirement &req)
    {
      if (name)
        {
          req.name = XMLString::transcode (name);
        }
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
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              // Fetch the text node which contains the "label"
	      node = this->iter_->nextNode();
	      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_name (text->getNodeValue(), ir);
	    }
	  else if (node_name == XStr (ACE_TEXT ("resourceType")))
            {
	      // TODO: How to implement this?
	      // Sequence of string ???
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle resourceUsage attribute
    void IR_Handler::process_resourceUsage
      (const XMLCh* resourceUsage, ::Deployment::ImplementationRequirement &ir)
    {
      if (resourceUsage)
        {
          ir.name = XMLString::transcode (resourceUsage);
        }
    }

    /// handle resourcePort attribute
    void IR_Handler::process_resourcePort
      (const XMLCh* resourcePort, ::Deployment::ImplementationRequirement &ir)
    {
      if (resourcePort)
        {
          ir.name = XMLString::transcode (resourcePort);
        }
    }

    /// handle componentPort attribute
    void IR_Handler::process_componentPort
      (const XMLCh* componentPort, ::Deployment::ImplementationRequirement &ir)
    {
      if (componentPort)
        {
          ir.name = XMLString::transcode (componentPort);
        }
    }

  }
}

#endif /* IR_HANDLER_C */
