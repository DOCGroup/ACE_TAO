//==================================================================
/**
 *  @file  IAD_Handler.cpp
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef IAD_HANDLER_C
#define IAD_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "PCI_Handler.h"
#include "CompIntrDesc_Handler.h"
#include "NIA_Handler.h"
#include "IAD_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace CIAO
{
  namespace Config_Handler
  {
    IAD_Handler::IAD_Handler (DOMDocument* doc, unsigned long filter)
      : root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    IAD_Handler::IAD_Handler (DOMNodeIterator* iter, bool release)
      : root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    IAD_Handler::~IAD_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the package configuration and populate it
    void IAD_Handler::process_ImplementationArtifactDescription
      (::Deployment::ImplementationArtifactDescription &iad)
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
              this->process_label (text->getNodeValue(), iad);
            }
          else if (node_name == XStr (ACE_TEXT ("UUID")))
            {
              // Fetch the text node which contains the "UUID"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_UUID (text->getNodeValue(), iad);
            }
          else if (node_name == XStr (ACE_TEXT ("location")))
            {
              // Fetch the text node which contains the "location"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_location (text->getNodeValue(), iad);
            }
          else if (node_name == XStr (ACE_TEXT ("execParameter")))
            {
              // increase the length of the sequence
              CORBA::ULong i (iad.execParameter.length ());
              iad.execParameter.length (i + 1);

              // delegate the populating process
              Property_Handler::process_Property (this->iter_,
                                                  iad.execParameter[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("infoProperty")))
            {
              // increase the length of the sequence
              CORBA::ULong i (iad.infoProperty.length ());
              iad.infoProperty.length (i + 1);

              // delegate the populating process
              Property_Handler::process_Property (this->iter_,
                                                  iad.infoProperty[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("deployRequirement")))
            {
              // increase the length of the sequence
              CORBA::ULong i (iad.deployRequirement.length ());
              iad.deployRequirement.length (i + 1);

              // delegate the populating process
              Requirement_Handler::process_Requirement (this->iter_,
							iad.deployRequirement[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("dependsOn")))
            {
              // increase the length of the sequence
              CORBA::ULong i (iad.dependsOn.length ());
              iad.dependsOn.length (i + 1);

	      // fetch the NIA handler
	      NIA_Handler nia_handler (this->iter_, false);

              // delegate the populating process
	      nia_handler.process_NamedImplementationArtifact (iad.dependsOn[i]);
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
    void IAD_Handler::process_label
      (const XMLCh* name, ::Deployment::ImplementationArtifactDescription &iad)
    {
      if (name)
        {
          iad.label = XMLString::transcode (name);
        }
    }

    /// handle UUID attribute
    void IAD_Handler::process_UUID
      (const XMLCh* name, ::Deployment::ImplementationArtifactDescription &iad)
    {
      if (name)
        {
          iad.UUID = XMLString::transcode (name);
        }
    }

    /// handle location attribute
    void IAD_Handler::process_location
      (const XMLCh* name, ::Deployment::ImplementationArtifactDescription &iad)
    {
      if (name)
        {
          iad.location = XMLString::transcode (name);
        }
    }

  }  // namespace Config_Handler
}  // namespace CIAO

#endif /* IAD_HANDLER_C */
