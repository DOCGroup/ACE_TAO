// $Id$

#ifndef COMPPKGDESC_HANDLER_C
#define COMPPKGDESC_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "CompPkgDesc_Handler.h"
#include "Property_Handler.h"
#include "PCI_Handler.h"
#include "CompIntrDesc_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace CIAO
{
  namespace Config_Handler
  {
    CompPkgDesc_Handler::CompPkgDesc_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    CompPkgDesc_Handler::CompPkgDesc_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    CompPkgDesc_Handler::~CompPkgDesc_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the package configuration and populate it
    void CompPkgDesc_Handler::process_ComponentPackageDescription
      (::Deployment::ComponentPackageDescription &comppkgdesc)
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
              this->process_label (text->getNodeValue(), comppkgdesc);
            }
          else if (node_name == XStr (ACE_TEXT ("UUID")))
            {
              // Fetch the text node which contains the "UUID"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_UUID (text->getNodeValue(), comppkgdesc);
            }
          else if (node_name == XStr (ACE_TEXT ("configProperty")))
            {
              // increase the length of the sequence
              CORBA::ULong i (comppkgdesc.configProperty.length ());
              comppkgdesc.configProperty.length (i + 1);

              // delegate the populating process
              Property_Handler::process_Property (this->iter_,
                                                  comppkgdesc.configProperty[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("infoProperty")))
            {
              // increase the length of the sequence
              CORBA::ULong i (comppkgdesc.infoProperty.length ());
              comppkgdesc.infoProperty.length (i + 1);

              // delegate the populating process
              Property_Handler::process_Property (this->iter_,
                                                  comppkgdesc.infoProperty[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("realizes")))
            {
              // fetch the component package reference handler
              CCD_Handler compintrdesc_handler (iter_, false);

              // delegate the populating process
              compintrdesc_handler.process_ComponentInterfaceDescription (comppkgdesc.realizes);
            }
          else if (node_name == XStr (ACE_TEXT ("implementation")))
            {
              // fetch the component package description handler
              PCI_Handler pci_handler (iter_, false);

              // increase the length of the sequence
              CORBA::ULong i (comppkgdesc.implementation.length ());
              comppkgdesc.implementation.length (i + 1);

              // delegate the populating process
              pci_handler.process_PackagedComponentImplementation (comppkgdesc.implementation[i]);
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
    void CompPkgDesc_Handler::process_label
      (const XMLCh* label, ::Deployment::ComponentPackageDescription &comppkgdesc)
    {
      if (label)
        {
          comppkgdesc.label = XMLString::transcode (label);
        }
    }

    /// handle UUID attribute
    void CompPkgDesc_Handler::process_UUID
      (const XMLCh* UUID, ::Deployment::ComponentPackageDescription &comppkgdesc)
    {
      if (UUID)
        {
          comppkgdesc.UUID = XMLString::transcode (UUID);
        }
    }

  }
}

#endif /* COMPPKGDESC_HANDLER_C */
