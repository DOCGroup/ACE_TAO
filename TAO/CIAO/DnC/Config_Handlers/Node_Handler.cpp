// $Id$

#ifndef NODE_HANDLER_C
#define NODE_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Node_Handler.h"
#include "SR_handler.h"
#include "Resource_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    Node_Handler::Node_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                         this->filter_,
                                         0,
                                         true)),
        release_ (true)
    {}

    Node_Handler::Node_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    Node_Handler::~Node_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the Node type and populate the IDL structure
    void Node_Handler::process_node (::Deployment::Node &dep_node)
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
              this->process_name (text->getNodeValue(), dep_node);
            }
          else if (node_name == XStr (ACE_TEXT ("label")))
            {
              // Fetch the text node which contains the "location"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_label (text->getNodeValue(), dep_node);
            }
          else if (node_name == XStr (ACE_TEXT ("resource")))
            {
              // increase the length of the sequence
              CORBA::ULong i (dep_node.resource.length ());
              dep_node.resource.length (i + 1);

              // delegate the populating process
              Resource_Handler res_handler (this->iter_, false);
              res_handler.process_resource (dep_node.resource[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("connectRef")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_shared_resource_ref (text->getNodeValue(), dep_node);
            }
          else if (node_name == XStr (ACE_TEXT ("connectionRef")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_connection_ref (text->getNodeValue(), dep_node);
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
    void Node_Handler::process_name (const XMLCh* name,
                                   ::Deployment::Node &node)
    {
      if (name)
        {
          node.name = XMLString::transcode (name);
        }
    }

    /// handle label attribute
    void Node_Handler::process_label (const XMLCh* label,
                                    ::Deployment::Node &node)
    {
      if (label)
        {
          node.label = XMLString::transcode (label);
        }
    }

    /// handle connect attribute
    void Node_Handler::process_shared_resource_ref (const XMLCh* shared_ref,
                                                    ::Deployment::Node &node)
    {
      if (shared_ref)
        {
          CORBA::ULong i (node.sharedResourceRef.length ());
          node.sharedResourceRef.length (i + 1);
          CORBA::String_var temp = XMLString::transcode (shared_ref);
          node.sharedResourceRef[i] = ACE_OS::strtol (temp.in (), 0 , 10);
        }
    }

    /// handle connection attribute
    void Node_Handler::process_connection_ref (const XMLCh* connection_ref,
                                             ::Deployment::Node &node)
    {
      if (connection_ref)
        {
          CORBA::ULong i (node.connectionRef.length ());
          node.connectionRef.length (i + 1);
          CORBA::String_var temp = XMLString::transcode (connection_ref);
          node.connectionRef[i] = ACE_OS::strtol (temp.in (), 0 , 10);
        }
    }

  }
}

#endif /* NODE_HANDLER_C */
