//==================================================================
/**
 *  @file  IR_Handler.cpp
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

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
          ir.resourcePort = XMLString::transcode (resourcePort);
        }
    }

    /// handle componentPort attribute
    void IR_Handler::process_componentPort
      (const XMLCh* componentPort, ::Deployment::ImplementationRequirement &ir)
    {
      if (componentPort)
        {
          ir.componentPort = XMLString::transcode (componentPort);
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
