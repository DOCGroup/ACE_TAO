//==================================================================
/**
 *  @file  CCD_Handler.cpp
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef CCD_HANDLER_C
#define CCD_HANDLER_C

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
    CCD_Handler::CCD_Handler (DOMDocument* doc, unsigned long filter)
      : // traverse_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc->createNodeIterator (this->root_,
					this->filter_,
					0,
					true)),
        release_ (true)
    {}

    CCD_Handler::CCD_Handler (DOMNodeIterator* iter, bool release)
      : // traverse_ (0),
	root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    CCD_Handler::~CCD_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the package configuration and populate it
    void CCD_Handler::process_ComponentInterfaceDescription
      (::Deployment::ComponentInterfaceDescription &ccd)
    {
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
              this->process_label (text->getNodeValue(), ccd);
            }
          else if (node_name == XStr (ACE_TEXT ("UUID")))
            {
              // Fetch the text node which contains the "UUID"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_UUID (text->getNodeValue(), ccd);
            }
          else if (node_name == XStr (ACE_TEXT ("specificType")))
            {
              // Fetch the text node which contains the "specificType"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_specificType (text->getNodeValue(), ccd);
            }
          else if (node_name == XStr (ACE_TEXT ("supportedType")))
            {
              // Fetch the text node which contains the "supportedType"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_supportedType (text->getNodeValue(), ccd);
            }
          else if (node_name == XStr (ACE_TEXT ("configProperty")))
            {
            }
          else if (node_name == XStr (ACE_TEXT ("infoProperty")))
            {
            }
          else if (node_name == XStr (ACE_TEXT ("property")))
            {
            }
          else if (node_name == XStr (ACE_TEXT ("port")))
            {
              // increase the length of the sequence
              CORBA::ULong i (ccd.port.length ());
              ccd.port.length (i + 1);

	      // fetch the CompPortDesc handler
	      CompPortDesc_Handler compportdesc_handler (this->iter_, false);

              // delegate the populating process
	      compportdesc_handler.process_ComponentPortDescription (ccd.port[i]);
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
    void CCD_Handler::process_label
      (const XMLCh* name, ::Deployment::ComponentInterfaceDescription &ccd)
    {
      if (name)
        {
          ccd.label = XMLString::transcode (name);
        }
    }

    /// handle UUID attribute
    void CCD_Handler::process_UUID
      (const XMLCh* name, ::Deployment::ComponentInterfaceDescription &ccd)
    {
      if (name)
        {
          ccd.UUID = XMLString::transcode (name);
        }
    }

    /// handle specificType attribute
    void CCD_Handler::process_specificType
      (const XMLCh* name, ::Deployment::ComponentInterfaceDescription &ccd)
    {
      if (name)
        {
          ccd.specificType = XMLString::transcode (name);
        }
    }

    /// handle supportedType attribute
    void CCD_Handler::process_supportedType
      (const XMLCh* name, ::Deployment::ComponentInterfaceDescription &ccd)
    {
      if (name)
        {
	  // increase the length of the sequence
	  CORBA::ULong i (ccd.supportedType.length ());
	  ccd.supportedType.length (i + 1);

	  // push back the last item
          ccd.supportedType[i] = XMLString::transcode (name);
        }
    }

  }
}

#endif /* CCD_HANDLER_C */
