// $Id$

#ifndef RESOURCE_HANDLER_C
#define RESOURCE_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Node_Handler.h"
#include "IC_handler.h"
#include "SR_handler.h"
#include "SP_handler.h"
#include "Resource_handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    Resource_Handler::Resource_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                         this->filter_,
                                         0,
                                         true)),
        release_ (true)
    {}

    Resource_Handler::Resource_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    Resource_Handler::~Resource_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the Resource type and populate the IDL structure
    void Resource_Handler::process_resource
    (::Deployment::Resource &resource)
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
              this->process_name (text->getNodeValue(), resource);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceType")))
            {
              // Fetch the text node which contains the "location"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_resource_type (text->getNodeValue(), resource);
            }
          else if (node_name == XStr (ACE_TEXT ("property")))
            {
              // increase the length of the sequence
              CORBA::ULong i (resource.property.length ());
              resource.property.length (i + 1);

              // delegate the populating process
              SP_Handler::process_SatisfierProperty (this->iter_,
                                                     resource.property[i]);
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
    void Resource_Handler::process_name (const XMLCh* name,
                                       ::Deployment::Resource &resource)
    {
      if (name)
        {
          resource.name = XMLString::transcode (name);
        }
    }

    /// handle resourceType attribute
    void Resource_Handler::process_resource_type (const XMLCh* resource_type,
                                                  ::Deployment::Resource &resource)
    {
      if (resource_type)
        {
          CORBA::ULong i (resource.resourceType.length ());
          resource.resourceType.length (i + 1);
          resource.resourceType[i] = str (XMLString::transcode (resource_type));
        }
    }

  }
}

#endif /* RESOURCE_HANDLER_C */
