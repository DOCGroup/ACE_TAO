//==================================================================
/**
 *  @file  CompIntrDesc_Handler.cpp
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef COMPINTRDESC_HANDLER_C
#define COMPINTRDESC_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "PCI_Handler.h"
#include "CompIntrDesc_Handler.h"
#include "CompPropDesc_Handler.h"
#include "CompPortDesc_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace CIAO
{
  namespace Config_Handler
  {
    CompIntrDesc_Handler::CompIntrDesc_Handler (DOMDocument* doc, unsigned long filter)
      : // traverse_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc->createNodeIterator (this->root_,
					this->filter_,
					0,
					true)),
        release_ (true)
    {}

    CompIntrDesc_Handler::CompIntrDesc_Handler (DOMNodeIterator* iter, bool release)
      : // traverse_ (0),
	root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    CompIntrDesc_Handler::~CompIntrDesc_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the package configuration and populate it
    void CompIntrDesc_Handler::process_ComponentInterfaceDescription
      (::Deployment::ComponentInterfaceDescription &compintrdesc)
    {
      // This is bogus and should be replaced later.
      ACE_DECLARE_NEW_CORBA_ENV;

      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("label")))
            {
              // Fetch the text node which contains the "label"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_label (text->getNodeValue(), compintrdesc);
            }
          else if (node_name == XStr (ACE_TEXT ("UUID")))
            {
              // Fetch the text node which contains the "UUID"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_UUID (text->getNodeValue(), compintrdesc);
            }
          else if (node_name == XStr (ACE_TEXT ("specificType")))
            {
              // Fetch the text node which contains the "specificType"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_specificType (text->getNodeValue(), compintrdesc);
            }
          else if (node_name == XStr (ACE_TEXT ("supportedType")))
            {
              // Fetch the text node which contains the "supportedType"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_supportedType (text->getNodeValue(), compintrdesc);
            }
          else if (node_name == XStr (ACE_TEXT ("configProperty")))
            {
              // increase the length of the sequence
              CORBA::ULong i (compintrdesc.configProperty.length ());
              compintrdesc.configProperty.length (i + 1);

              // delegate the populating process
              Property_Handler::process_Property (this->iter_,
                                                  compintrdesc.configProperty[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("infoProperty")))
            {
              // increase the length of the sequence
              CORBA::ULong i (compintrdesc.infoProperty.length ());
              compintrdesc.infoProperty.length (i + 1);

              // delegate the populating process
              Property_Handler::process_Property (this->iter_,
                                                  compintrdesc.infoProperty[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("property")))
            {
              // increase the length of the sequence
              CORBA::ULong i (compintrdesc.property.length ());
              compintrdesc.property.length (i + 1);

	      // fetch the CompPropDesc handler
	      CompPropDesc_Handler comppropdesc_handler (this->iter_, false);

              // delegate the populating process
	      comppropdesc_handler.process_ComponentPropertyDescription (compintrdesc.property[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("port")))
            {
              // increase the length of the sequence
              CORBA::ULong i (compintrdesc.port.length ());
              compintrdesc.port.length (i + 1);

	      // fetch the CompPortDesc handler
	      CompPortDesc_Handler compportdesc_handler (this->iter_, false);

              // delegate the populating process
	      compportdesc_handler.process_ComponentPortDescription (compintrdesc.port[i]);
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle label attribute
    void CompIntrDesc_Handler::process_label
      (const XMLCh* name, ::Deployment::ComponentInterfaceDescription &compintrdesc)
    {
      if (name)
        {
          compintrdesc.label = XMLString::transcode (name);
        }
    }

    /// handle UUID attribute
    void CompIntrDesc_Handler::process_UUID
      (const XMLCh* name, ::Deployment::ComponentInterfaceDescription &compintrdesc)
    {
      if (name)
        {
          compintrdesc.UUID = XMLString::transcode (name);
        }
    }

    /// handle specificType attribute
    void CompIntrDesc_Handler::process_specificType
      (const XMLCh* name, ::Deployment::ComponentInterfaceDescription &compintrdesc)
    {
      if (name)
        {
          compintrdesc.specificType = XMLString::transcode (name);
        }
    }

    /// handle supportedType attribute
    void CompIntrDesc_Handler::process_supportedType
      (const XMLCh* name, ::Deployment::ComponentInterfaceDescription &compintrdesc)
    {
      if (name)
        {
          compintrdesc.supportedType = XMLString::transcode (name);
        }
    }

  }
}

#endif /* COMPINTRDESC_HANDLER_C */
