//==================================================================
/**
 *  @file  CompPortDesc_Handler.cpp
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
#include "CompPortDesc_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace CIAO
{
  namespace Config_Handler
  {
    CompPortDesc_Handler::CompPortDesc_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    CompPortDesc_Handler::CompPortDesc_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    CompPortDesc_Handler::~CompPortDesc_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the package configuration and populate it
    void CompPortDesc_Handler::process_ComponentPortDescription
      (::Deployment::ComponentPortDescription &compportdesc)
    {
      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              // Fetch the text node which contains the "name"
	            node = this->iter_->nextNode();
	            DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_name (text->getNodeValue(), compportdesc);
	          }
          else if (node_name == XStr (ACE_TEXT ("specificType")))
            {
              // Fetch the text node which contains the "specificType"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_specificType (text->getNodeValue(), compportdesc);
            }
          else if (node_name == XStr (ACE_TEXT ("supportedType")))
            {
              // Fetch the text node which contains the "supportedType"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_supportedType (text->getNodeValue(), compportdesc);
            }
	        else if (node_name == XStr (ACE_TEXT ("provider")))
	          {
	            /// Fetch the text node which contains the "provider"
	            node = this->iter_->nextNode();
	            DOMText* text = ACE_reinterpret_cast (DOMText*, node);
	            this->process_provider (text->getNodeValue(), compportdesc);
	          }
	        else if (node_name == XStr (ACE_TEXT ("exclusiveProvider")))
	          {
	            /// Fetch the text node which contains the "exclusiveProvider"
	            node = this->iter_->nextNode();
	            DOMText* text = ACE_reinterpret_cast (DOMText*, node);
	            this->process_exclusiveProvider (text->getNodeValue(), compportdesc);
	          }
	        else if (node_name == XStr (ACE_TEXT ("exclusiveUser")))
	          {
	            /// Fetch the text node which contains the "exclusiveUser"
	            node = this->iter_->nextNode();
	            DOMText* text = ACE_reinterpret_cast (DOMText*, node);
	            this->process_exclusiveUser (text->getNodeValue(), compportdesc);
	          }
	        else if (node_name == XStr (ACE_TEXT ("optional")))
	          {
	            /// Fetch the text node which contains the "optional"
	            node = this->iter_->nextNode();
	            DOMText* text = ACE_reinterpret_cast (DOMText*, node);
	            this->process_optional (text->getNodeValue(), compportdesc);
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
    void CompPortDesc_Handler::process_name
      (const XMLCh* name, ::Deployment::ComponentPortDescription &compportdesc)
    {
      if (name)
        {
          compportdesc.name = XMLString::transcode (name);
        }
    }

    /// handle specificType attribute
    void CompPortDesc_Handler::process_specificType (const XMLCh* specificType, ::Deployment::ComponentPortDescription &compportdesc)
    {
      if (specificType)
	      {
	        compportdesc.specificType = XMLString::transcode (specificType);
	      }
    }

    /// handle supportedType attribute
    void CompPortDesc_Handler::process_supportedType (const XMLCh* supportedType, ::Deployment::ComponentPortDescription &compportdesc)
    {
      if (supportedType)
	      {
	        // increase the length of the sequence
	        CORBA::ULong i (compportdesc.supportedType.length ());
	        compportdesc.supportedType.length (i + 1);

	        // push back the last item
	        compportdesc.supportedType[i] = XMLString::transcode (supportedType);
	      }
    }

    /// handle provider attribute
    void CompPortDesc_Handler::process_provider (const XMLCh* provider, ::Deployment::ComponentPortDescription &compportdesc)
    {
      XStr true_val ("true");
      XStr true_cap_val ("TRUE");
      XStr value (provider);

      if (value == true_val || value == true_cap_val)
        compportdesc.provider = 1;
      else
        compportdesc.provider = 0;
    }

    /// handle exclusiveProvider attribute
    void CompPortDesc_Handler::process_exclusiveProvider (const XMLCh* exclusiveProvider, ::Deployment::ComponentPortDescription &compportdesc)
    {
      XStr true_val ("true");
      XStr true_cap_val ("TRUE");
      XStr value (exclusiveProvider);

      if (value == true_val || value == true_cap_val)
        compportdesc.exclusiveProvider = 1;
      else
        compportdesc.exclusiveProvider = 0;
    }

    /// handle exclusiveUser attribute
    void CompPortDesc_Handler::process_exclusiveUser (const XMLCh* exclusiveUser, ::Deployment::ComponentPortDescription &compportdesc)
    {
      XStr true_val ("true");
      XStr true_cap_val ("TRUE");
      XStr value (exclusiveUser);

      if (value == true_val || value == true_cap_val)
        compportdesc.exclusiveUser = 1;
      else
        compportdesc.exclusiveUser = 0;
    }

    /// handle optional attribute
    void CompPortDesc_Handler::process_optional (const XMLCh* optional, ::Deployment::ComponentPortDescription &compportdesc)
    {
      XStr true_val ("true");
      XStr true_cap_val ("TRUE");
      XStr value (optional);

      if (value == true_val || value == true_cap_val)
        compportdesc.optional = 1;
      else
        compportdesc.optional = 0;
    }

  }
}
