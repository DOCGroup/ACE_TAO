// $Id$

#ifndef DOMAIN_HANDLER_C
#define DOMAIN_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Node_Handler.h"
#include "IC_handler.h"
#include "SR_handler.h"
#include "Bridge_handler.h"
#include "Domain_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    Domain_Handler::Domain_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                         this->filter_,
                                         0,
                                         true)),
        release_ (true)
    {}

    Domain_Handler::Domain_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    Domain_Handler::~Domain_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the Domain type and populate the IDL structure
    void Domain_Handler::process_domain
    (::Deployment::Domain &domain)
    {
      // This is bogus and should be replaced later.
      ACE_DECLARE_NEW_CORBA_ENV;

      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("UUID")))
            {
              // Fetch the text node which contains the "UUID"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_uuid (text->getNodeValue(), domain);
            }
          else if (node_name == XStr (ACE_TEXT ("label")))
            {
              // Fetch the text node which contains the "location"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_label (text->getNodeValue(), domain);
            }
          else if (node_name == XStr (ACE_TEXT ("sharedResource")))
            {
              // increase the length of the sequence
              CORBA::ULong i (domain.sharedResource.length ());
              domain.sharedResource.length (i + 1);

              // delegate the populating process
              SR_Handler::process_sr (this->iter_,
                                      domain.sharedResource[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("node")))
            {
              // increase the length of the sequence
              CORBA::ULong i (domain.node.length ());
              domain.node.length (i + 1);

              // delegate the populating process
              Node_Handler::process_node (this->iter_,
                                          domain.node[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("interconnect")))
            {
              // increase the length of the sequence
              CORBA::ULong i (domain.interconnect.length ());
              domain.interconnect.length (i + 1);

              // delegate the populating process
              IC_Handler::process_ic (this->iter_,
                                      domain.interconnect[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("bridge")))
            {
              // increase the length of the sequence
              CORBA::ULong i (domain.bridge.length ());
              domain.bridge.length (i + 1);

              // delegate the populating process
              Bridge_Handler::process_bridge (this->iter_,
                                              domain.bridge[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("infoProperty")))
            {
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle uuid attribute
    void Domain_Handler::process_uuid (const XMLCh* uuid,
                                       ::Deployment::Domain &domain)
    {
      if (uuid)
        {
          domain.UUID = XMLString::transcode (uuid);
        }
    }

    /// handle label attribute
    void Domain_Handler::process_label (const XMLCh* label,
                                        ::Deployment::Domain &domain)
    {
      if (label)
        {
          domain.label = XMLString::transcode (label);
        }
    }

  }
}

#endif /* DOMAIN_HANDLER_C */
