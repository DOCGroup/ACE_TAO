// $Id$

#ifndef RDD_HANDLER_C
#define RDD_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Any_Handler.h"
#include "RDD_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    RDD_Handler::RDD_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    RDD_Handler::RDD_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    RDD_Handler::~RDD_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the deployment plan and populate it
    void RDD_Handler::process_ResourceDeploymentDescription
    (::Deployment::ResourceDeploymentDescription &rdd)
    {
      // This is bogus and should be replaced later.
      ACE_DECLARE_NEW_CORBA_ENV;

      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("requirementName")))
            {
              // Fetch the text node which contains the "requirementName"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_requirementName (text->getNodeValue(), rdd);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceName")))
            {
              // Fetch the text node which contains the "resourceName"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_resourceName (text->getNodeValue(), rdd);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceValue")))
            {
              // delegate the populating process
              // @@should avoid this copy - Andy @@
              // Arvind: Resolved
              Any_Handler::process_Any (this->iter_, rdd.resourceValue);
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle requirement name attribute
    void RDD_Handler::process_requirementName (const XMLCh* name,
                                               ::Deployment::ResourceDeploymentDescription &rdd)
    {
      if (name)
        {
          rdd.requirementName = XMLString::transcode (name);
        }
    }

    /// handle resource name attribute
    void RDD_Handler::process_resourceName (const XMLCh* name,
                                            ::Deployment::ResourceDeploymentDescription &rdd)
    {
      if (name)
        {
          rdd.resourceName = XMLString::transcode (name);
        }
    }


  }
}

#endif /* RDD_HANDLER_C */
