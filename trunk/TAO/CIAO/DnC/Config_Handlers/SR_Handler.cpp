// $Id$

#ifndef SR_HANDLER_C
#define SR_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Node_Handler.h"
#include "IC_handler.h"
#include "SR_handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    SR_Handler::SR_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                         this->filter_,
                                         0,
                                         true)),
        release_ (true)
    {}

    SR_Handler::SR_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    SR_Handler::~SR_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the SharedResource type and populate the IDL structure
    void SR_Handler::process_sr (::Deployment::SharedResource &sr)
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
              this->process_name (text->getNodeValue(), sr);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceType")))
            {
              // Fetch the text node which contains the "location"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_resource_type (text->getNodeValue(), sr);
            }
          else if (node_name == XStr (ACE_TEXT ("node")))
            {
              // Fetch the text node which contains the "location"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_node_ref (text->getNodeValue(), sr);
            }
          else if (node_name == XStr (ACE_TEXT ("property")))
            {
              // increase the length of the sequence
              CORBA::ULong i (sr.property.length ());
              resource.sr.length (i + 1);

              // delegate the populating process
              SP_Handler::process_SatisfierProperty (this->iter_,
                                                     sr.property[i]);
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
    void SR_Handler::process_name (const XMLCh* name,
                                   ::Deployment::SharedResource &sr)
    {
      if (name)
        {
          sr.name = XMLString::transcode (name);
        }
    }

    /// handle resourceType attribute
    void SR_Handler::process_resource_type (const XMLCh* resource_type,
                                            ::Deployment::SharedResource &sr)
    {
      if (resource_type)
        {
          CORBA::ULong i (sr.resourceType.length ());
          sr.resourceType.length (i + 1);
          sr.resourceType[i] = str (XMLString::transcode (resource_type));
        }
    }

    /// handle node attribute
    void SR_Handler::process_node_ref (const XMLCh* node_ref,
                                       ::Deployment::SharedResource &sr)
    {
      if (node_ref)
        {
          CORBA::ULong i (sr.nodeRef.length ());
          sr.nodeRef.length (i + 1);
          sr.nodeRef[i] = strtoul (XMLString::transcode (node_ref));
        }
    }

  }
}

#endif /* SR_HANDLER_C */
