// $Id$

#ifndef COMPPKGDESC_HANDLER_C
#define COMPPKGDESC_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "PCI_Handler.h"
#include "CID_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace CIAO
{
  namespace Config_Handler
  {
    CompPkgDesc_Handler::CompPkgDesc_Handler (DOMDocument* doc, unsigned long filter)
      : traverse_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (traverse_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    CompPkgDesc_Handler::CompPkgDesc_Handler (DOMNodeIterator* iter, bool release)
      : traverse_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
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
              CID_Handler cid_handler (iter_, false);

              // delegate the populating process
              cid_handler.process_ComponentInterfaceDescription (comppkgdesc.realizes);
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
      (const XMLCh* name, ::Deployment::ComponentPackageDescription &comppkgdesc)
    {
      if (name)
        {
          comppkgdesc.label = XMLString::transcode (name);
        }
    }

    /// handle UUID attribute
    void CompPkgDesc_Handler::process_UUID
      (const XMLCh* name, ::Deployment::ComponentPackageDescription &comppkgdesc)
    {
      if (name)
        {
          comppkgdesc.UUID = XMLString::transcode (name);
        }
    }

  }
}

#endif /* COMPPKGDESC_HANDLER_C */
