// $Id$

#ifndef CRDD_HANDLER_C
#define CRDD_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Any_Handler.h"
#include "CRDD_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    CRDD_Handler::CRDD_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    CRDD_Handler::CRDD_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    CRDD_Handler::~CRDD_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the connection resource deployment description and populate it
    void CRDD_Handler::process_ConnectionResourceDeploymentDescription
    (::Deployment::ConnectionResourceDeploymentDescription &crdd)
    {
      // This is bogus and should be replaced later.
      ACE_DECLARE_NEW_CORBA_ENV;

      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("targetName")))
            {
              // Fetch the enum node which contains the "resource usage"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_targetName (text->getNodeValue(), crdd);
            }
          else if (node_name == XStr (ACE_TEXT ("requirementName")))
            {
              // Fetch the text node which contains the "requirementName"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_requirementName (text->getNodeValue(), crdd);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceName")))
            {
              // Fetch the text node which contains the "resourceName"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_resourceName (text->getNodeValue(), crdd);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceValue")))
            {
              // delegate the populating process
              // @@should avoid this copy - Andy @@
              // Arvind: Resolved
              Any_Handler::process_Any (this->iter_, crdd.resourceValue);
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle resource usage attribute
    void CRDD_Handler::process_targetName (const XMLCh* name,
                                           ::Deployment::ConnectionResourceDeploymentDescription &crdd)
    {
      if (name)
        {
          crdd.targetName = XMLString::transcode (name);
        }
    }

    /// handle requirement name attribute
    void CRDD_Handler::process_requirementName (const XMLCh* name,
                                               ::Deployment::ConnectionResourceDeploymentDescription &crdd)
    {
      if (name)
        {
          crdd.requirementName = XMLString::transcode (name);
        }
    }

    /// handle resource name attribute
    void CRDD_Handler::process_resourceName (const XMLCh* name,
                                            ::Deployment::ConnectionResourceDeploymentDescription &crdd)
    {
      if (name)
        {
          crdd.resourceName = XMLString::transcode (name);
        }
    }


  }
}

#endif /* CRDD_HANDLER_C */
