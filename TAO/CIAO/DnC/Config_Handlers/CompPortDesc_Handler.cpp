// $Id$

#ifndef COMPPORTDESC_HANDLER_C
#define COMPPORTDESC_HANDLER_C

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
	      DOMText* text = ACE_reinterpret_cast (DOMText* node);
	      this->process_provider (text->getNodeValue(), compportdesc);
	    }
	  else if (node_name == XStr (ACE_TEXT ("exclusiveProvider")))
	    {
	      /// Fetch the text node which contains the "exclusiveProvider"
	      node = this->iter_->nextNode();
	      DOMText* text = ACE_reinterpret_cast (DOMText* node);
	      this->process_exclusiveProvider (text->getNodeValue(), compportdesc);
	    }
	  else if (node_name == XStr (ACE_TEXT ("exclusiveUser")))
	    {
	      /// Fetch the text node which contains the "exclusiveUser"
	      node = this->iter_->nextNode();
	      DOMText* text = ACE_reinterpret_cast (DOMText* node);
	      this->process_exclusiveUser (text->getNodeValue(), compportdesc);
	    }
	  else if (node_name == XStr (ACE_TEXT ("optional")))
	    {
	      /// Fetch the text node which contains the "optional"
	      node = this->iter_->nextNode();
	      DOMText* text = ACE_reinterpret_cast (DOMText* node);
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
    void process_specificType (const XMLCh* specificType, ::Deployment::ComponentPortDescription &compportdesc)
    {
      if (specificType)
	{
	  compportdesc.specificType = XMLString::transcode (specificType);
	}
    }

    /// handle supportedType attribute
    void process_specificType (const XMLCh* specificType, ::Deployment::ComponentPortDescription &compportdesc)
    {
      if (supportedType)
	{
	  compportdesc.supportedType = XMLString::transcode (supportedType);
	}
    }

    /// handle provider attribute
    void process_provider (const XMLCh* provider, ::Deployment::ComponentPortDescription &compportdesc)
    {
      if (provider)
	{
	  compportdesc.provider = XMLString::transcode (provider);
	}
    }

    /// handle exclusiveProvider attribute
    void process_exclusiveProvider (const XMLCh* exclusiveProvider, ::Deployment::ComponentPortDescription &compportdesc)
    {
      if (exclusiveProvider)
	{
	  compportdesc.exclusiveProvider = XMLString::transcode (exclusiveProvider);
	}
    }

    /// handle exclusiveUser attribute
    void process_exclusiveUser (const XMLCh* name, ::Deployment::ComponentPortDescription &compportdesc)
    {
      if (exclusiveUser)
	{
	  compportdesc.exclusiveUser = XMLString::transcode (exclusiveUser);
	}
    }

    /// handle optional attribute
    void process_optional (const XMLCh* name, ::Deployment::ComponentPortDescription &compportdesc)
    {
      if (optional)
	{
	  compportdesc.optional = XMLString::transcode (optional);
	}
    }

  }
}

#endif /* COMPPORTDESC_HANDLER_C */
