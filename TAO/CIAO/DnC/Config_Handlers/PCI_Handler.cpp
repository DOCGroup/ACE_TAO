// $Id$

#ifndef PCI_HANDLER_C
#define PCI_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "CompImplDesc_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace CIAO
{
  namespace Config_Handler
  {
    PCI_Handler::PCI_Handler (DOMDocument* doc, unsigned long filter)
      : traverse_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (traverse_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    PCI_Handler::PCI_Handler (DOMNodeIterator* iter, bool release)
      : traverse_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    PCI_Handler::~PCI_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the package configuration and populate it
    void PCI_Handler::process_PackagedComponentImplementation
      (::Deployment::PackagedComponentImplementation &pci)
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
              this->process_name (text->getNodeValue(), pci);
            }
          else if (node_name == XStr (ACE_TEXT ("referencedImplementation")))
            {
              // fetch the component package reference handler
              CompImplDesc_Handler compimpldesc_handler (iter_, false);

              // delegate the populating process
              compimpldesc_handler.process_ComponentImplementationDescription (pci.referencedImplementation);
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
    void PCI_Handler::process_name
      (const XMLCh* name, ::Deployment::PackagedComponentImplementation &pci)
    {
      if (name)
        {
          pci.name = XMLString::transcode (name);
        }
    }

  }
}

#endif /* PCI_HANDLER_C */
