// $Id$

#ifndef IC_HANDLER_C
#define IC_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Node_Handler.h"
#include "IC_Handler.h"
#include "SR_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    IC_Handler::IC_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                         this->filter_,
                                         0,
                                         true)),
        release_ (true)
    {}

    IC_Handler::IC_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    IC_Handler::~IC_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the Interconnect type and populate the IDL structure
    void IC_Handler::process_ic (::Deployment::Interconnect &ic)
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
              this->process_name (text->getNodeValue(), ic);
            }
          else if (node_name == XStr (ACE_TEXT ("label")))
            {
              // Fetch the text node which contains the "location"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_label (text->getNodeValue(), ic);
            }
          else if (node_name == XStr (ACE_TEXT ("resource")))
            {
              // increase the length of the sequence
              CORBA::ULong i (ic.resource.length ());
              ic.resource.length (i + 1);

              // delegate the populating process
              Resource_Handler resource_handler (this->iter_, false);
              resource_handler.process_resource (ic.resource[i]);
            }
          else if (node_name == XStr (ACE_TEXT ("connect")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_connect_ref (text->getNodeValue(), ic);
            }
          else if (node_name == XStr (ACE_TEXT ("connection")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_connection_ref (text->getNodeValue(), ic);
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
    void IC_Handler::process_name (const XMLCh* name,
                                   ::Deployment::Interconnect &ic)
    {
      if (name)
        {
          ic.name = XMLString::transcode (name);
        }
    }

    /// handle label attribute
    void IC_Handler::process_label (const XMLCh* label,
                                    ::Deployment::Interconnect &ic)
    {
      if (label)
        {
          ic.label = XMLString::transcode (label);
        }
    }

    /// handle connect attribute
    void IC_Handler::process_connect_ref (const XMLCh* connect_ref,
                                          ::Deployment::Interconnect &ic)
    {
      if (connect_ref)
        {
          CORBA::ULong i (ic.connectRef.length ());
          ic.connectRef.length (i + 1);
          ic.connectRef[i] = strtoul (XMLString::transcode (connect_ref), 0, 10);
        }
    }

    /// handle connection attribute
    void IC_Handler::process_connection_ref (const XMLCh* connection_ref,
                                             ::Deployment::Interconnect &ic)
    {
      if (connection_ref)
        {
          CORBA::ULong i (ic.connectionRef.length ());
          ic.connectionRef.length (i + 1);
          ic.connectionRef[i] = strtoul (XMLString::transcode (connection_ref), 0, 10);
        }
    }

  }
}

#endif /* IC_HANDLER_C */
