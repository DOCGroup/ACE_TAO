// $Id$

#ifndef BRIDGE_HANDLER_C
#define BRIDGE_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Node_Handler.h"
#include "IC_handler.h"
#include "SR_handler.h"
#include "Bridge_handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    Bridge_Handler::Bridge_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                         this->filter_,
                                         0,
                                         true)),
        release_ (true)
    {}

    Bridge_Handler::Bridge_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    Bridge_Handler::~Bridge_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the Bridge type and populate the IDL structure
    void Bridge_Handler::process_bridge
    (::Deployment::Bridge &bridge)
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
              // Fetch the text node which contains the "UUID"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_name (text->getNodeValue(), bridge);
            }
          else if (node_name == XStr (ACE_TEXT ("label")))
            {
              // Fetch the text node which contains the "location"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_label (text->getNodeValue(), bridge);
            }
          else if (node_name == XStr (ACE_TEXT ("resource")))
            {
              // increase the length of the sequence
              CORBA::ULong i (bridge.resource.length ());
              bridge.resource.length (i + 1);

              // delegate the populating process
              Resource_Handler::process_resource (this->iter_,
                                                  bridge.resource[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("connect")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_connect_ref (text->getNodeValue(), bridge);
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
    void Bridge_Handler::process_name (const XMLCh* name,
                                       ::Deployment::Bridge &bridge)
    {
      if (name)
        {
          bridge.name = XMLString::transcode (name);
        }
    }

    /// handle label attribute
    void Bridge_Handler::process_label (const XMLCh* label,
                                        ::Deployment::Bridge &bridge)
    {
      if (label)
        {
          bridge.label = XMLString::transcode (label);
        }
    }

    /// handle connect attribute
    void Bridge_Handler::process_connect_ref (const XMLCh* connect_ref,
                                              ::Deployment::Bridge &bridge)
    {
      if (connect_ref)
        {
          CORBA::ULong i (bridge.connectRef.length ());
          bridge.connectRef.length (i + 1);
          bridge.connectRef[i] = strtoul (XMLString::transcode (connect_ref));
        }
    }

  }
}

#endif /* BRIDGE_HANDLER_C */
